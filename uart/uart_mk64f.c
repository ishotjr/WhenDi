#include "uart_mk64f.h"
#include "atmosphere_platform.h"

#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

const ATMO_UART_DriverInstance_t mk64fUARTDriverInstance = {
	ATMO_MK64F_UART_Init,
	ATMO_MK64F_UART_DeInit,
	ATMO_MK64F_UART_SetConfiguration,
	ATMO_MK64F_UART_GetConfiguration,
	ATMO_MK64F_UART_WriteBlocking,
	ATMO_MK64F_UART_ReadBlocking,
	ATMO_MK64F_UART_FlushRx,
	ATMO_MK64F_UART_FlushTx,
	ATMO_MK64F_UART_RegisterRxAbilityHandle,
	ATMO_MK64F_UART_RegisterRxCbFunc,
};

#define ATMO_MK64F_NUM_UARTS (4)

static ATMO_UART_InternalConfig_t configs[ATMO_MK64F_NUM_UARTS] = {0};
static UART_Type* uartPeripherals[ATMO_MK64F_NUM_UARTS] = {UART0, UART1, UART2, UART3};
static clock_name_t uartClocks[ATMO_MK64F_NUM_UARTS] = {UART0_CLK_SRC, UART1_CLK_SRC, UART2_CLK_SRC, UART3_CLK_SRC};
static IRQn_Type uartIRQNums[ATMO_MK64F_NUM_UARTS] = {UART0_RX_TX_IRQn, UART1_RX_TX_IRQn, UART2_RX_TX_IRQn, UART3_RX_TX_IRQn};

typedef struct {
	uint32_t periphNum;
	char c;
}ATMO_MK64F_UART_Rx_Payload_t;

#define ATMO_MK64F_UART_RX_QUEUE_SIZE 10
static QueueHandle_t rxQueue;
static bool rxQueueInitialized = false;
static TaskHandle_t rxQueueCheckerTaskHandle = NULL;

#ifdef ATMO_STATIC_CORE
#define UART_MAX_BUFFER_LEN (ATMO_STATIC_SIZE)
#else
#define UART_MAX_BUFFER_LEN (256)
#endif

static char uart_buffer[ATMO_MK64F_NUM_UARTS][UART_MAX_BUFFER_LEN];
static unsigned int currentBuffLen[ATMO_MK64F_NUM_UARTS] = {0};

void ATMO_MK64F_UART_RxByteHandler(uint32_t periphNum)
{

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(uartPeripherals[periphNum]))
    {
        uint8_t data = UART_ReadByte(uartPeripherals[periphNum]);

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    	ATMO_MK64F_UART_Rx_Payload_t payload = {periphNum, data};

    	xQueueSendFromISR(rxQueue, &payload, &xHigherPriorityTaskWoken);

        UART_ClearStatusFlags(uartPeripherals[periphNum], kUART_RxDataRegFullFlag | kUART_RxOverrunFlag);

    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void UART0_RX_TX_IRQHandler()
{
    ATMO_MK64F_UART_RxByteHandler(0);
}

void UART1_RX_TX_IRQHandler()
{
	ATMO_MK64F_UART_RxByteHandler(1);
}

void UART2_RX_TX_IRQHandler()
{
	ATMO_MK64F_UART_RxByteHandler(2);
}

void UART3_RX_TX_IRQHandler()
{
	ATMO_MK64F_UART_RxByteHandler(3);
}


ATMO_Status_t ATMO_MK64F_UART_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	ATMO_Status_t status = ATMO_Status_Success;

	for( int i = 0; ( i < ATMO_MK64F_NUM_UARTS ) && ( status == ATMO_Status_Success ); i++ )
	{
		ATMO_DriverInstanceData_t* driverData = (ATMO_DriverInstanceData_t*)ATMO_Malloc( sizeof( ATMO_DriverInstanceData_t ) );
		uint32_t* i2cPeriphNum = (uint32_t*)ATMO_Malloc( sizeof( uint32_t ) );
		*i2cPeriphNum = i;

		driverData->name = "MK64F UART";
		driverData->initialized = false;
		driverData->instanceNumber = *instanceNumber;
		driverData->argument = i2cPeriphNum;
		status = ATMO_UART_AddDriverInstance( &mk64fUARTDriverInstance, driverData, instanceNumber );
	}

	return status;
}

static inline uint32_t ATMO_MK64F_UART_GetPeriph(ATMO_DriverInstanceData_t *instance)
{
	return *((uint32_t*)instance->argument);
}


static bool ATMO_MK64F_UART_ATMOConfigToNxp(uart_config_t *nxpConfig,  ATMO_UART_Peripheral_t *atmo)
{
	nxpConfig->baudRate_Bps = atmo->baudRate;
	nxpConfig->enableRx = true;
	nxpConfig->enableTx = true;

	switch(atmo->parity)
	{
		case ATMO_UART_Parity_None:
		{
			nxpConfig->parityMode = kUART_ParityDisabled;
			break;
		}
		case ATMO_UART_Parity_Even:
		{
			nxpConfig->parityMode = kUART_ParityEven;
			break;
		}
		case ATMO_UART_Parity_Odd:
		{
			nxpConfig->parityMode = kUART_ParityOdd;
			break;
		}
		default:
		{
			return false;
		}
	}

	nxpConfig->txFifoWatermark = 0;
	nxpConfig->rxFifoWatermark = 1;
	nxpConfig->stopBitCount = atmo->numStopBits;

	return true;
}

static inline bool ATMO_MK64F_UART_IsWhiteSpace(char c)
{
	return(c == ' ' || c == '\r' || c == '\n');
}

/**
 * Don't want to be executing callback from the UART RX ISR
 * All received bytes are sent here where they are dispatched
 */
void ATMO_MK64F_UART_RXQueueChecker( void* parameters )
{
	ATMO_MK64F_UART_Rx_Payload_t payload;
	while(xQueueReceive(rxQueue, &payload, portMAX_DELAY))
	{
		ATMO_UART_ProcessRxData(&configs[payload.periphNum], (uint8_t *)&payload.c, 1, (uint8_t *)uart_buffer[payload.periphNum], UART_MAX_BUFFER_LEN, &currentBuffLen[payload.periphNum]);
	}
}

ATMO_UART_Status_t ATMO_MK64F_UART_Init(ATMO_DriverInstanceData_t *instance)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS )
	{
		return ATMO_UART_Status_Invalid;
	}

	if(!rxQueueInitialized)
	{
		rxQueue = xQueueCreate(ATMO_MK64F_UART_RX_QUEUE_SIZE, sizeof(ATMO_MK64F_UART_Rx_Payload_t));

		BaseType_t xReturned = xTaskCreate( ATMO_MK64F_UART_RXQueueChecker, "UART RX Queue Checker", 1024, NULL, configMAX_PRIORITIES - 1, &rxQueueCheckerTaskHandle );

		if( xReturned != pdPASS )
		{
			return ATMO_UART_Status_Fail;
		}

		rxQueueInitialized = true;
	}

	configs[periphNum].abilityHandleRegistered = false;
	configs[periphNum].cb = NULL;
	configs[periphNum].initialized = false;

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_DeInit(ATMO_DriverInstanceData_t *instance)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS )
	{
		return ATMO_UART_Status_Invalid;
	}

    UART_DisableInterrupts(uartPeripherals[periphNum], kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    DisableIRQ(uartIRQNums[periphNum]);
	UART_Deinit(uartPeripherals[periphNum]);

	configs[periphNum].initialized = false;

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_SetConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_UART_Peripheral_t *config)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS )
	{
		return ATMO_UART_Status_Invalid;
	}

	uart_config_t nxpConfig;

	if(!ATMO_MK64F_UART_ATMOConfigToNxp(&nxpConfig, config))
	{
		return ATMO_UART_Status_Fail;
	}

	// Initialize uart
	if( UART_Init(uartPeripherals[periphNum], &nxpConfig, CLOCK_GetFreq(uartClocks[periphNum])) != 0 )
	{
		return ATMO_UART_Status_Fail;
	}

    /* Enable RX interrupt. */
    UART_EnableInterrupts(uartPeripherals[periphNum], kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    NVIC_SetPriority(uartIRQNums[periphNum], 5);
    EnableIRQ(uartIRQNums[periphNum]);

	memcpy(&configs[periphNum].config, config, sizeof(ATMO_UART_Peripheral_t));

	configs[periphNum].initialized = true;

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_GetConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_UART_Peripheral_t *config)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS || !configs[periphNum].initialized)
	{
		return ATMO_UART_Status_Invalid;
	}

	memcpy(config, &configs[periphNum].config, sizeof(ATMO_UART_Peripheral_t));

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_WriteBlocking(ATMO_DriverInstanceData_t *instance, const char *buffer, uint32_t length, uint32_t *numBytesSent, uint16_t timeoutMs)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS || !configs[periphNum].initialized)
	{
		return ATMO_UART_Status_Invalid;
	}

	UART_WriteBlocking(uartPeripherals[periphNum], buffer, length);

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_ReadBlocking(ATMO_DriverInstanceData_t *instance, char *buffer, uint32_t length, uint32_t *numBytesReceived, uint16_t timeoutMs)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS || !configs[periphNum].initialized)
	{
		return ATMO_UART_Status_Invalid;
	}

	// Disable interrupts for a bit
    DisableIRQ(uartIRQNums[periphNum]);
    status_t status = UART_ReadBlocking(uartPeripherals[periphNum], buffer, length);
    EnableIRQ(uartIRQNums[periphNum]);
    if(status != kStatus_Success)
    {
    	if(numBytesReceived != NULL)
    	{
    		*numBytesReceived = 0;
    	}
    	return ATMO_UART_Status_Fail;
    }

    if(numBytesReceived != NULL)
    {
    	*numBytesReceived = length;
    }

    return ATMO_UART_Status_Success;
}


ATMO_UART_Status_t ATMO_MK64F_UART_FlushTx(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_UART_Status_NotSupported;
}

ATMO_UART_Status_t ATMO_MK64F_UART_FlushRx(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_UART_Status_NotSupported;
}

ATMO_UART_Status_t ATMO_MK64F_UART_RegisterRxAbilityHandle(ATMO_DriverInstanceData_t *instance, unsigned int abilityHandle)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS || !configs[periphNum].initialized)
	{
		return ATMO_UART_Status_Invalid;
	}

	configs[periphNum].abilityHandle = abilityHandle;
	configs[periphNum].abilityHandleRegistered = true;

	return ATMO_UART_Status_Success;
}

ATMO_UART_Status_t ATMO_MK64F_UART_RegisterRxCbFunc(ATMO_DriverInstanceData_t *instance, ATMO_Callback_t cbFunc)
{
	uint32_t periphNum = ATMO_MK64F_UART_GetPeriph(instance);

	if(periphNum >= ATMO_MK64F_NUM_UARTS || !configs[periphNum].initialized)
	{
		return ATMO_UART_Status_Invalid;
	}

	configs[periphNum].cb = cbFunc;

	return ATMO_UART_Status_Success;
}

