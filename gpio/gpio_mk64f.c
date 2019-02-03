/**
 ******************************************************************************
 * @file    atmo_gpio.c
 * @author  
 * @version 
 * @date    
 * @brief   Atmosphere API - GPIO source file for MCUXpresso SDK
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "gpio_mk64f.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "TimersManager.h"
#include "pin_mapping.h"
#include "pin_mux.h"
#include "atmosphere_platform.h"

ATMO_GPIO_DriverInstance_t mk64fGPIODriverInstance = {
	ATMO_MK64F_GPIO_Init,
	ATMO_MK64F_GPIO_DeInit,
	ATMO_MK64F_GPIO_SetPinConfiguration,
	ATMO_MK64F_GPIO_GetPinConfiguration,
	ATMO_MK64F_GPIO_RegisterInterruptAbilityHandle,
	ATMO_MK64F_GPIO_RegisterInterruptCallback,
	ATMO_MK64F_GPIO_SetPinState,
	ATMO_MK64F_GPIO_GetPinState,
	ATMO_MK64F_GPIO_Read,
	ATMO_MK64F_GPIO_Toggle
};

/* Imported function prototypes ----------------------------------------------*/
extern void ResetMCU(void);


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define PORT_COUNT   (sizeof(portBase) / sizeof(*portBase))
#define PORT_WIDTH   32


/* Private macro -------------------------------------------------------------*/

#define GET_PORTNUM(pin)   (uint8_t)(((uint16_t)pin >> 8) & 0xFF)
#define GET_PINNUM(pin)    (uint8_t)(((uint16_t)pin >> 0) & 0xFF)
#define MAKE_PIN(port, pin) (uint16_t)(((uint16_t)port << 8) | pin)


/* Private variables ---------------------------------------------------------*/

static PORT_Type *portBase[] = PORT_BASE_PTRS;
static GPIO_Type *gpioBase[] = GPIO_BASE_PTRS;

#define ATMO_MK64F_NUM_BUTTONS (4)
#define BUTTON_DEBOUNCE_MICROSECONDS (200000)
#define BUTTON_RST_MICROSECONDS (5000000) // 5 seconds

static uint64_t lastButtonChange[ATMO_MK64F_NUM_BUTTONS] = {0};
static bool buttonResetMask[ATMO_MK64F_NUM_BUTTONS] = {false, true, true, false};
static bool _ATMO_MK64F_GPIO_ResetComboEnabled = true;
static bool buttonDisplayToggleMask[ATMO_MK64F_NUM_BUTTONS] = {true, false, false, true};
static bool _ATMO_MK64F_GPIO_DisplayToggleComboEnabled = true;
static uint32_t buttonPins[ATMO_MK64F_NUM_BUTTONS] = {BOARD_INITPINS_USER_SW1_PIN_NAME, BOARD_INITPINS_USER_SW2_PIN_NAME, BOARD_INITPINS_USER_SW3_PIN_NAME, BOARD_INITPINS_USER_SW4_PIN_NAME};

typedef struct {
	bool abilityHandleRegistered;
	unsigned int abilityHandle;
	ATMO_Callback_t callback;
} ATMO_GPIO_PrivConfig_t;

static ATMO_GPIO_PrivConfig_t privConfig[PORT_COUNT][PORT_WIDTH];

typedef struct {
	uint32_t portNum;
	uint32_t pinNum;
	ATMO_GPIO_PinState_t pinState;
}ATMO_MK64F_GPIO_Rx_Payload_t;

#define ATMO_MK64F_GPIO_RX_QUEUE_SIZE (75)
static QueueHandle_t rxQueue = NULL;
static bool rxQueueInitialized = false;
static TaskHandle_t rxQueueCheckerTaskHandle = NULL;
static TaskHandle_t resetButtonCheckerTaskHandle = NULL;


/* Private functions ---------------------------------------------------------*/

static ATMO_GPIO_Status_t ATMO_GPIO_EnablePortClock(uint8_t portNum)
{
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	switch (portNum)
	{
		case 0 :
			CLOCK_EnableClock(kCLOCK_PortA);
			break;
		case 1 :
			CLOCK_EnableClock(kCLOCK_PortB);
			break;
		case 2 :
			CLOCK_EnableClock(kCLOCK_PortC);
			break;
		case 3 :
			CLOCK_EnableClock(kCLOCK_PortD);
			break;
		case 4 :
			CLOCK_EnableClock(kCLOCK_PortE);
			break;
		default :
			break;
	}

	return ATMO_GPIO_Status_Success;
}

static ATMO_GPIO_Status_t ATMO_GPIO_DisablePortClock(uint8_t portNum)
{
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	switch (portNum)
	{
		case 0 :
			CLOCK_DisableClock(kCLOCK_PortA);
			break;
		case 1 :
			CLOCK_DisableClock(kCLOCK_PortB);
			break;
		case 2 :
			CLOCK_DisableClock(kCLOCK_PortC);
			break;
		case 3 :
			CLOCK_DisableClock(kCLOCK_PortD);
			break;
		case 4 :
			CLOCK_DisableClock(kCLOCK_PortE);
			break;
		default :
			break;
	}

	return ATMO_GPIO_Status_Success;
}

static ATMO_GPIO_Status_t ATMO_GPIO_EnablePortInterrupt(uint8_t portNum)
{
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	switch (portNum)
	{
		case 0 :
			NVIC_SetPriority(PORTA_IRQn, 5);
			EnableIRQ(PORTA_IRQn);
			break;
		case 1 :
			NVIC_SetPriority(PORTB_IRQn, 5);
			EnableIRQ(PORTB_IRQn);
			break;
		case 2 :
			NVIC_SetPriority(PORTC_IRQn, 5);
			EnableIRQ(PORTC_IRQn);
			break;
		case 3 :
			NVIC_SetPriority(PORTD_IRQn, 5);
			EnableIRQ(PORTD_IRQn);
			break;
		case 4 :
			NVIC_SetPriority(PORTE_IRQn, 5);
			EnableIRQ(PORTE_IRQn);
			break;
		default :
			break;
	}

	return ATMO_GPIO_Status_Success;
}

static port_interrupt_t ATMO_GPIO_ConvertInterruptTrigger(ATMO_GPIO_InterruptTrigger_t trigger)
{
	switch(trigger)
	{
		case ATMO_GPIO_InterruptTrigger_None:
		{
			return kPORT_InterruptOrDMADisabled;
		}
		case ATMO_GPIO_InterruptTrigger_RisingEdge:
		{
			return kPORT_InterruptRisingEdge;
		}
		case ATMO_GPIO_InterruptTrigger_FallingEdge:
		{
			return kPORT_InterruptFallingEdge;
		}
		case ATMO_GPIO_InterruptTrigger_BothEdges:
		{
			return kPORT_InterruptEitherEdge;
		}
		case ATMO_GPIO_InterruptTrigger_LogicZero:
		{
			return kPORT_InterruptLogicZero;
		}
		case ATMO_GPIO_InterruptTrigger_LogicOne:
		{
			return kPORT_InterruptLogicOne;
		}
		default:
		{
			return kPORT_InterruptOrDMADisabled;
		}
	}

	return kPORT_InterruptOrDMADisabled;
}

void _ATMO_MK64F_GPIO_CallDisplayToggle(void *data)
{
	ATMO_UI_Page_SetDisplayEnabled(!ATMO_UI_Page_GetDisplayEnabled());
}

void ATMO_MK64F_GPIO_ButtonResetChecker( void* parameters )
{
	while(true)
	{
		bool reset = true;
		bool toggleDisplay = true;
		uint64_t time = TMR_GetTimestamp();

		// Are all of the required buttons currently pressed?
		for(int i = 0; i < ATMO_MK64F_NUM_BUTTONS; i++)
		{
			if(buttonResetMask[i] && (ATMO_MK64F_GPIO_Read(NULL, buttonPins[i]) != ATMO_GPIO_PinState_Low))
			{
				reset = false;
			}

			if(buttonDisplayToggleMask[i] && (ATMO_MK64F_GPIO_Read(NULL, buttonPins[i]) != ATMO_GPIO_PinState_Low))
			{
				toggleDisplay = false;
			}

			// Has the button been pressed long enough?
			if((time - lastButtonChange[i]) < BUTTON_RST_MICROSECONDS)
			{
				reset = false;
				toggleDisplay = false;
			}
		}

		if(reset && _ATMO_MK64F_GPIO_ResetComboEnabled)
		{
			ResetMCU();
		}

		if(toggleDisplay && _ATMO_MK64F_GPIO_DisplayToggleComboEnabled)
		{
			ATMO_AddCallbackExecute(_ATMO_MK64F_GPIO_CallDisplayToggle, NULL);

			// Reset counter
			for(int i = 0; i < ATMO_MK64F_NUM_BUTTONS; i++)
			{
				if(buttonDisplayToggleMask[i])
				{
					lastButtonChange[i] = time;
				}
			}
		}

		ATMO_PLATFORM_DelayMilliseconds(250);
	}

}

/**
 * Don't want to be executing callback from the UART RX ISR
 * All received bytes are sent here where they are dispatched
 */
void ATMO_MK64F_GPIO_RXQueueChecker( void* parameters )
{
	if(rxQueue == NULL)
	{
		return;
	}

	ATMO_MK64F_GPIO_Rx_Payload_t payload;
	while(xQueueReceive(rxQueue, &payload, portMAX_DELAY))
	{
        if(privConfig[payload.portNum][payload.pinNum].callback != NULL)
        {
            ATMO_Value_t val;
            ATMO_InitValue(&val);
            ATMO_CreateValueUnsignedInt(&val, payload.pinState);
            ATMO_Lock();
            ATMO_AddCallbackExecute(privConfig[payload.portNum][payload.pinNum].callback, &val);
            ATMO_Unlock();
            ATMO_FreeValue(&val);
        }
        if(privConfig[payload.portNum][payload.pinNum].abilityHandleRegistered)
        {
            ATMO_Value_t val;
            ATMO_InitValue(&val);
            ATMO_CreateValueUnsignedInt(&val, payload.pinState);
            ATMO_Lock();
            ATMO_AddAbilityExecute( privConfig[payload.portNum][payload.pinNum].abilityHandle, &val );
            ATMO_Unlock();
            ATMO_FreeValue(&val);
        }
	}
}

ATMO_Status_t ATMO_MK64F_GPIO_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber)
{
	ATMO_DriverInstanceData_t driver;
	driver.name = "MK64F GPIO";
	driver.initialized = false;
	driver.instanceNumber = *instanceNumber;
	driver.argument = NULL;

	return ATMO_GPIO_AddDriverInstance(&mk64fGPIODriverInstance, &driver, instanceNumber);
}


/* Exported functions ---------------------------------------------------------*/

/**
 *  ATMO_GPIO_Init
 *
 *    @param pin
 *    @param pinMode
 *    @param altFunc
 *    @param initialState
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_Init(ATMO_DriverInstanceData_t *instance)
{
	memset(privConfig, 0, sizeof(privConfig));

	// Enable interrupts on port E
	// This is for SPI bus sharing with the KW41z
	ATMO_GPIO_EnablePortClock(4);
	ATMO_GPIO_EnablePortInterrupt(4);

	if(!rxQueueInitialized)
	{
		rxQueue = xQueueCreate(ATMO_MK64F_GPIO_RX_QUEUE_SIZE, sizeof(ATMO_MK64F_GPIO_Rx_Payload_t));

		BaseType_t xReturned = xTaskCreate( ATMO_MK64F_GPIO_RXQueueChecker, "GPIO RX Queue Checker", 1024, NULL, configMAX_PRIORITIES - 1, &rxQueueCheckerTaskHandle );

		if( xReturned != pdPASS )
		{
			return ATMO_GPIO_Status_Fail;
		}


		xReturned = xTaskCreate( ATMO_MK64F_GPIO_ButtonResetChecker, "Button Reset Checker", 256, NULL, configMAX_PRIORITIES - 1, &resetButtonCheckerTaskHandle );

		if( xReturned != pdPASS )
		{
			return ATMO_GPIO_Status_Fail;
		}

		rxQueueInitialized = true;
	}

	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_DeInit
 *
 *    @param pin
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_DeInit(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_SetPinConfiguration
 *
 *    @param pin
 *    @param config
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_SetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, const ATMO_GPIO_Config_t *config)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);
	port_pin_config_t portPinConfig;
	gpio_pin_config_t gpioPinConfig;
	bool diasbleClock = true;
	uint8_t i;

	if (config == NULL)
		return ATMO_GPIO_Status_Invalid;

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	switch (config->pinMode)
	{
		case ATMO_GPIO_PinMode_Disabled :
			portPinConfig.mux = kPORT_PinDisabledOrAnalog;
			gpioPinConfig.pinDirection = kGPIO_DigitalInput;
			portPinConfig.pullSelect = kPORT_PullDisable;
			portPinConfig.openDrainEnable = kPORT_OpenDrainDisable;
			break;
		case ATMO_GPIO_PinMode_Input_HighImpedance :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalInput;
			portPinConfig.pullSelect = kPORT_PullDisable;
			portPinConfig.openDrainEnable = kPORT_OpenDrainDisable;
			break;
		case ATMO_GPIO_PinMode_Input_PullUp :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalInput;
			portPinConfig.pullSelect = kPORT_PullUp;
			portPinConfig.openDrainEnable = kPORT_OpenDrainDisable;
			break;
		case ATMO_GPIO_PinMode_Input_PullDown :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalInput;
			portPinConfig.pullSelect = kPORT_PullDown;
			portPinConfig.openDrainEnable = kPORT_OpenDrainDisable;
			break;
		case ATMO_GPIO_PinMode_Output_PushPull :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalOutput;
			portPinConfig.pullSelect = kPORT_PullDisable;
			portPinConfig.openDrainEnable = kPORT_OpenDrainDisable;
			break;
		case ATMO_GPIO_PinMode_Output_OpenDrain :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalOutput;
			portPinConfig.pullSelect = kPORT_PullDisable;
			portPinConfig.openDrainEnable = kPORT_OpenDrainEnable;
			break;
		case ATMO_GPIO_PinMode_Output_OpenDrainPullUp :
			portPinConfig.mux = kPORT_MuxAsGpio;
			gpioPinConfig.pinDirection = kGPIO_DigitalOutput;
			portPinConfig.pullSelect = kPORT_PullUp;
			portPinConfig.openDrainEnable = kPORT_OpenDrainEnable;
			break;
		default:
			return ATMO_GPIO_Status_Invalid;
	}

	portPinConfig.lockRegister = kPORT_UnlockRegister;

	// Output Logic Level
	gpioPinConfig.outputLogic = (config->initialState == ATMO_GPIO_PinState_High) ? 1 : 0;

	// Passive Filter Enable
	portPinConfig.passiveFilterEnable = kPORT_PassiveFilterDisable;

	// Drive Strength Config
	portPinConfig.driveStrength = kPORT_LowDriveStrength;

	// Slew Rate Config
	portPinConfig.slewRate = kPORT_FastSlewRate;

	// Enable Port Clock
	ATMO_GPIO_EnablePortClock(portNum);

	// Configure pin
	GPIO_PinInit(gpioBase[portNum], pinNum, &gpioPinConfig);
	PORT_SetPinConfig(portBase[portNum], pinNum, &portPinConfig);

	// Check if any pins in the port need the clock enabled. If not, disable the port clock.
	for (i=0; i<PORT_WIDTH; i++)
	{
		if (portBase[portNum]->PCR[i] & PORT_PCR_MUX_MASK)
		{
			diasbleClock = false;
			break;
		}
	}

	if (diasbleClock)
		ATMO_GPIO_DisablePortClock(portNum);

	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_GetPinConfiguration
 *
 *    @param pin
 *    @param config
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_GetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_Config_t *config)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	if (config == NULL)
		return ATMO_GPIO_Status_Invalid;

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	if ((portBase[portNum]->PCR[pinNum] & PORT_PCR_MUX_MASK) == PORT_PCR_MUX(kPORT_PinDisabledOrAnalog))  // disabled
	{
		config->pinMode = ATMO_GPIO_PinMode_Disabled;
	}
	else if (gpioBase[portNum]->PDDR & (1 << pinNum))  // output
	{
		if (((portBase[portNum]->PCR[pinNum] & PORT_PCR_ODE_MASK) >> PORT_PCR_ODE_SHIFT) == 0)  // push-pull
			config->pinMode = ATMO_GPIO_PinMode_Output_PushPull;
		else if ((portBase[portNum]->PCR[pinNum] & PORT_PCR_PS_MASK) >> PORT_PCR_PS_SHIFT)  // open-drain with pull-up
			config->pinMode = ATMO_GPIO_PinMode_Output_OpenDrainPullUp;
		else  // open-drain
			config->pinMode = ATMO_GPIO_PinMode_Output_OpenDrain;
	}
	else  // input
	{
		if (((portBase[portNum]->PCR[pinNum] & PORT_PCR_PE_MASK) >> PORT_PCR_PE_SHIFT) == 0)  // high impedance
			config->pinMode = ATMO_GPIO_PinMode_Input_HighImpedance;
		else if ((portBase[portNum]->PCR[pinNum] & PORT_PCR_PS_MASK) >> PORT_PCR_PS_SHIFT)  // pull-up
			config->pinMode = ATMO_GPIO_PinMode_Input_PullUp;
		else  // pull-down
			config->pinMode = ATMO_GPIO_PinMode_Input_PullDown;
	}

	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_RegisterCallback
 *
 *    @param pin
 *    @param cbFunc
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_RegisterInterruptAbilityHandle(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_InterruptTrigger_t trigger, unsigned int abilityHandle)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	PORT_SetPinInterruptConfig(portBase[portNum], pinNum, ATMO_GPIO_ConvertInterruptTrigger(trigger));

	privConfig[portNum][pinNum].abilityHandle = abilityHandle;
	privConfig[portNum][pinNum].abilityHandleRegistered = true;

	return ATMO_GPIO_EnablePortInterrupt(portNum);
}

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_RegisterInterruptCallback(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_InterruptTrigger_t trigger, ATMO_Callback_t callback)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	PORT_SetPinInterruptConfig(portBase[portNum], pinNum, ATMO_GPIO_ConvertInterruptTrigger(trigger));

	privConfig[portNum][pinNum].callback = callback;

	return ATMO_GPIO_EnablePortInterrupt(portNum);
}


/**
 *  ATMO_GPIO_SetPinState
 *
 *    @param pin
 *    @param state
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_SetPinState(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_PinState_t state)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	uint8_t output = (state == ATMO_GPIO_PinState_High) ? 1u : 0u;

	GPIO_WritePinOutput(gpioBase[portNum], pinNum, output);

	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_GetPinState
 *
 *    @param pin
 *    @param state
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_GetPinState(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_PinState_t *state)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	if (state == NULL)
		return ATMO_GPIO_Status_Invalid;

	// Invalid port number
	if (portNum >= (sizeof(portBase) / sizeof(*portBase)))
		return ATMO_GPIO_Status_NotSupported;

	// Invalid pin number
	if (pinNum >= PORT_WIDTH)
		return ATMO_GPIO_Status_NotSupported;

	*state = (GPIO_ReadPinInput(gpioBase[portNum], pinNum) == 1) ? ATMO_GPIO_PinState_High : ATMO_GPIO_PinState_Low;

	return ATMO_GPIO_Status_Success;
}

/**
 *  ATMO_GPIO_Read
 *
 *    @param pin
 */
ATMO_GPIO_PinState_t ATMO_MK64F_GPIO_Read(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin)
{
	ATMO_GPIO_PinState_t state;
	ATMO_GPIO_Status_t status = ATMO_MK64F_GPIO_GetPinState(instance, pin, &state);

	if( status != ATMO_GPIO_Status_Success )
	{
		state = ATMO_GPIO_PinState_Error;
	}

	return state;
}

/**
 *  ATMO_GPIO_Toggle
 *
 *    @param pin
 */
ATMO_GPIO_Status_t ATMO_MK64F_GPIO_Toggle(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin)
{
	uint8_t portNum = GET_PORTNUM(pin);
	uint8_t pinNum = GET_PINNUM(pin);

	GPIO_TogglePinsOutput(gpioBase[portNum], 0x01u << pinNum);
	return ATMO_GPIO_Status_Success;
}

void ATMO_MK64F_GPIO_EnableResetCombo(bool enabled)
{
	_ATMO_MK64F_GPIO_ResetComboEnabled = enabled;
}

void ATMO_MK64F_GPIO_EnableDisplayToggleCombo(bool enabled)
{
	_ATMO_MK64F_GPIO_DisplayToggleComboEnabled = enabled;
}


void ATMO_GPIO_IRQHandler(uint8_t port, uint32_t flags)
{
	uint8_t pin;

	if(rxQueue == NULL)
	{
		return;
	}

	for (pin=0; pin<PORT_WIDTH; pin++)
	{
		if ((flags & (1 << pin)))
		{
			uint16_t pinName = MAKE_PIN(port, pin);
			uint64_t time = TMR_GetTimestamp();

			// Button debouncing
			for(unsigned int i = 0; i < ATMO_MK64F_NUM_BUTTONS; i++)
			{
				if((pinName == buttonPins[i]))
				{
					if(ATMO_MK64F_GPIO_Read(NULL, pinName) == ATMO_GPIO_PinState_Low)
					{
						if((time - lastButtonChange[i]) <= BUTTON_DEBOUNCE_MICROSECONDS)
						{
							return;
						}
					}
					else
					{
						return;
					}

					lastButtonChange[i] = time;
				}
			}


			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	    	ATMO_MK64F_GPIO_Rx_Payload_t payload = {port,pin, ATMO_MK64F_GPIO_Read(NULL, pinName)};
	    	xQueueSendFromISR(rxQueue, &payload, &xHigherPriorityTaskWoken);
		}
	}
}

void PORTA_IRQHandler(void)
{
	uint32_t gpio_flags = GPIO_GetPinsInterruptFlags(GPIOA);
	GPIO_ClearPinsInterruptFlags(GPIOA, gpio_flags);
	ATMO_GPIO_IRQHandler(0, gpio_flags);
}

void PORTB_IRQHandler(void)
{
	uint32_t gpio_flags = GPIO_GetPinsInterruptFlags(GPIOB);
	GPIO_ClearPinsInterruptFlags(GPIOB, gpio_flags);
	ATMO_GPIO_IRQHandler(1, gpio_flags);
}

void PORTC_IRQHandler(void)
{
	uint32_t gpio_flags = GPIO_GetPinsInterruptFlags(GPIOC);
	GPIO_ClearPinsInterruptFlags(GPIOC, gpio_flags);
	ATMO_GPIO_IRQHandler(2, gpio_flags);
}

void PORTD_IRQHandler(void)
{
	uint32_t gpio_flags = GPIO_GetPinsInterruptFlags(GPIOD);
	GPIO_ClearPinsInterruptFlags(GPIOD, gpio_flags);
	ATMO_GPIO_IRQHandler(3, gpio_flags);
}

void PORTE_IRQHandler(void)
{
	uint32_t gpio_flags = GPIO_GetPinsInterruptFlags(GPIOE);
	BOARD_K41Z_RTS_IRQ_HANDLER(gpio_flags);
	GPIO_ClearPinsInterruptFlags(GPIOE, gpio_flags);
	ATMO_GPIO_IRQHandler(4, gpio_flags);
}
