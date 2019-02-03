/**
 ******************************************************************************
 * @file    atmo_spi.c
 * @author  
 * @version 
 * @date    
 * @brief   Atmosphere API - SPI source file for MCUXpresso SDK
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
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

#include "spi.h"
#include "spi_mk64f.h"
#include <stdlib.h>

const ATMO_SPI_DriverInstance_t MK64FSPIDriverInstance = {
	ATMO_MK64F_SPI_Init,
	ATMO_MK64F_SPI_DeInit,
	ATMO_MK64F_SPI_SetConfiguration,
	ATMO_MK64F_SPI_GetConfiguration,
	ATMO_MK64F_SPI_Master_SetDeviceConfiguration,
	ATMO_MK64F_SPI_Master_GetDeviceConfiguration,
	ATMO_MK64F_SPI_Master_RegisterCallback,
	ATMO_MK64F_SPI_Slave_RegisterCallback,
	ATMO_MK64F_SPI_Master_AssertSlaveSelect,
	ATMO_MK64F_SPI_Master_DeAssertSlaveSelect,
	ATMO_MK64F_SPI_Master_Write,
	ATMO_MK64F_SPI_Master_WriteNonBlocking,
	ATMO_MK64F_SPI_Master_Read,
	ATMO_MK64F_SPI_Master_ReadNonBlocking,
	ATMO_MK64F_SPI_Slave_WaitForReceive
};



/* Imported function prototypes ----------------------------------------------*/
extern uint32_t DSPI_GetInstance(SPI_Type *base);


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ATMO_SPI_DEFAULT_OPERATING_MODE        ATMO_SPI_OperatingMode_Uninitialized
#define ATMO_SPI_DEFAULT_BAUDRATE              1000000u
#define ATMO_SPI_DEFAULT_CLOCK_MODE            ATMO_SPI_ClockMode_0
#define ATMO_SPI_DEFAULT_CLOCK_IS_CONTINUOUS   false
#define ATMO_SPI_DEFAULT_MSB_FIRST             true
#define ATMO_SPI_DEFAULT_SS_ACTIVE_LOW         true
#define ATMO_SPI_DEFAULT_SS_IS_CONTINUOUS      true
#define ATMO_SPI_DEFAULT_SLAVE_BUFFER_SIZE     32u



/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static PORT_Type *portBase[] = PORT_BASE_PTRS;
static GPIO_Type *gpioBase[] = GPIO_BASE_PTRS;
static SPI_Type *peripheralBase[] = SPI_BASE_PTRS;
static dspi_master_handle_t *masterHandle[3];
static dspi_slave_handle_t *slaveHandle[3];
static ATMO_SPI_Peripheral_t ATMO_Platform_SPI_Peripheral[3];

static ATMO_SPI_SlaveTransfer_t *slaveXfer[3];
static uint8_t *slaveBuffer[3];


/* Private functions ---------------------------------------------------------*/

/**
 * This routine converts the status values from platform type to Atmosphere API type.
 *
 * @param status :The platform status.
 * @return Status as Atmosphere API type.
 */
static ATMO_SPI_Status_t ATMO_Platform_SPI_Convert_Status_PlatformToAtmo(status_t status)
{
	ATMO_SPI_Status_t returnStatus;

	switch (status)
	{
		case kStatus_Success :
			returnStatus = ATMO_SPI_Status_Success;
			break;
		case kStatus_Fail :
		case kStatus_DSPI_Error :
			returnStatus = ATMO_SPI_Status_Fail;
			break;
		case kStatus_Timeout :
			returnStatus = ATMO_SPI_Status_Timeout;
			break;
		case kStatus_DSPI_Busy :
			returnStatus = ATMO_SPI_Status_Busy;
			break;
		case kStatus_DSPI_Idle :
			returnStatus = ATMO_SPI_Status_Idle;
			break;
		default :
			returnStatus = ATMO_SPI_Status_Unknown;
			break;
	}

	return returnStatus;
}

/**
 * This routine determines the PCS number based on the SS pin and peripheral instance.
 *
 * @param instance :The SPI peripheral instance.
 * @param ssPin :The SPI Slave Select pin.
 * @return PCS as an integer.
 */
static int8_t ATMO_Platform_SPI_GetPcs(uint32_t instance, ATMO_SPI_CS_t ssPin)
{
	if(ssPin < 0)
	{
		return 0;
	}

	uint8_t portNum = (uint8_t)((ssPin >> 8) & 0xFF);
	uint8_t pinNum = (uint8_t)((ssPin >> 0) & 0xFF);
	port_mux_t portMux = (port_mux_t)((portBase[portNum]->PCR[pinNum] & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT);

	return 	((instance == 0) && (portNum == 0) && (pinNum == 14) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 0) && (portNum == 1) && (pinNum == 23) && (portMux == kPORT_MuxAlt3)) ? 5 :
			((instance == 0) && (portNum == 2) && (pinNum ==  0) && (portMux == kPORT_MuxAlt2)) ? 4 :
			((instance == 0) && (portNum == 2) && (pinNum ==  1) && (portMux == kPORT_MuxAlt2)) ? 3 :
			((instance == 0) && (portNum == 2) && (pinNum ==  2) && (portMux == kPORT_MuxAlt2)) ? 2 :
			((instance == 0) && (portNum == 2) && (pinNum ==  3) && (portMux == kPORT_MuxAlt2)) ? 1 :
			((instance == 0) && (portNum == 2) && (pinNum ==  4) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 0) && (portNum == 3) && (pinNum ==  0) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 0) && (portNum == 3) && (pinNum ==  4) && (portMux == kPORT_MuxAlt2)) ? 1 :
			((instance == 0) && (portNum == 3) && (pinNum ==  5) && (portMux == kPORT_MuxAlt2)) ? 2 :
			((instance == 0) && (portNum == 3) && (pinNum ==  6) && (portMux == kPORT_MuxAlt2)) ? 3 :
			((instance == 1) && (portNum == 1) && (pinNum ==  9) && (portMux == kPORT_MuxAlt2)) ? 1 :
			((instance == 1) && (portNum == 1) && (pinNum == 10) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 1) && (portNum == 3) && (pinNum ==  4) && (portMux == kPORT_MuxAlt7)) ? 0 :
			((instance == 1) && (portNum == 4) && (pinNum ==  0) && (portMux == kPORT_MuxAlt2)) ? 1 :
			((instance == 1) && (portNum == 4) && (pinNum ==  4) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 1) && (portNum == 4) && (pinNum ==  5) && (portMux == kPORT_MuxAlt2)) ? 2 :
			((instance == 1) && (portNum == 4) && (pinNum ==  6) && (portMux == kPORT_MuxAlt2)) ? 3 :
			((instance == 2) && (portNum == 1) && (pinNum == 20) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 2) && (portNum == 3) && (pinNum == 11) && (portMux == kPORT_MuxAlt2)) ? 0 :
			((instance == 2) && (portNum == 3) && (pinNum == 15) && (portMux == kPORT_MuxAlt2)) ? 1 :
			-1;
}

/**
 * This routine is the callback function for the platform in master mode.
 *
 * @param base :The pointer to the SPI peripheral base.
 * @param handle :The pointer to the master handle structure.
 * @param status :The status of the transfer.
 * @param userData :The pointer to user defined data.
 * @return None.
 */
static void ATMO_Platform_SPI_Master_Callback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
	ATMO_SPI_MasterTransfer_t masterXfer;
	uint32_t instance = DSPI_GetInstance(base);
	ATMO_SPI_Master_Callback_t callback = ATMO_Platform_SPI_Peripheral[instance].cbFunc;

	masterXfer.event = ATMO_SPI_MasterEvent_Complete;
	masterXfer.data = handle->txData;
	masterXfer.dataSize = handle->totalByteCount - handle->remainingSendByteCount;

	callback(&masterXfer, userData);
}

/**
 * This routine is the callback function for the platform in slave mode.
 *
 * @param base :The pointer to the SPI peripheral base.
 * @param handle :The pointer to the slave handle structure.
 * @param status :The status of the transfer.
 * @param userData :The pointer to user defined data.
 * @return None.
 */
static void ATMO_Platform_SPI_Slave_Callback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
	uint32_t instance = DSPI_GetInstance(base);
	ATMO_SPI_Slave_Callback_t callback = ATMO_Platform_SPI_Peripheral[instance].cbFunc;
	dspi_transfer_t transfer;

    if (status == kStatus_Success)
    {
        slaveXfer[instance]->data = slaveBuffer[instance];
        slaveXfer[instance]->dataSize = handle->totalByteCount - handle->remainingReceiveByteCount;
        slaveXfer[instance]->transferComplete = true;
        slaveXfer[instance]->event = ATMO_SPI_SlaveEvent_Complete;
        callback(slaveXfer[instance], userData);
    }

	// setup the SPI peripheral to start receiving data again
	transfer.txData = slaveXfer[instance]->data;
	transfer.rxData = slaveBuffer[instance];
	transfer.dataSize = slaveXfer[instance]->dataSize;
	transfer.configFlags = kDSPI_SlaveCtar0;
	DSPI_SlaveTransferNonBlocking(peripheralBase[instance], slaveHandle[instance], &transfer);
}

/**
 * This routine performs the data transfer (write or read) in master mode.
 *
 * @param read :Specifies whether the operation is a write or read.
 * @param blocking :Specifies whether the function should wait until the transfer is complete before returning (blocking) or if it should return immediately (non-blocking)
 * @param instance :The peripheral instance used for writing/reading the specified slave device.
 * @param slaveAddress :The SPI address of the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the SPI peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the SPI peripheral. May be 0.
 * @param dataBytes :The pointer to a buffer for holding/storing the data bytes that will be transfered to/from the SPI peripheral.
 * @param numDataBytes :The number of bytes to be transferred.
 * @return status.
 */
static ATMO_SPI_Status_t ATMO_Platform_SPI_Master_Transfer(ATMO_DriverInstanceData_t *driverInstance, bool read, bool blocking, uint32_t instance, ATMO_SPI_CS_t ssPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *dataBytes, uint16_t numDataBytes)
{
	
	uint8_t portNum = ssPin >= 0 ?(uint8_t)((ssPin >> 8) & 0xFF) : 0;
	uint8_t pinNum = ssPin >=0 ? (uint8_t)((ssPin >> 0) & 0xFF) : 0;
	port_mux_t portMux = ssPin >= 0 ? (port_mux_t)((portBase[portNum]->PCR[pinNum] & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT) : kPORT_MuxAsGpio;
	status_t platformStatus = kStatus_Fail;
	dspi_transfer_t masterStructure;
	uint8_t *txData;
	bool isPcsPinGpio = false;
	int8_t pcs;

	if (instance > 2)
		return ATMO_SPI_Status_NotSupported;

	if(masterHandle[instance] == NULL)
		return ATMO_SPI_Status_Invalid;

	if (ATMO_Platform_SPI_Peripheral[instance].operatingMode == ATMO_SPI_OperatingMode_Slave)
		return ATMO_SPI_Status_Invalid;

	if ((dataBytes == NULL) || (numDataBytes == 0))
		return ATMO_SPI_Status_Invalid;

	if (portMux == kPORT_PinDisabledOrAnalog)
		return ATMO_SPI_Status_Invalid;

	masterStructure.configFlags = 0;

	if (portMux == kPORT_MuxAsGpio)
	{
		isPcsPinGpio = true;
	}
	else
	{
		pcs = ATMO_Platform_SPI_GetPcs(instance, ssPin);

		// Select which PCS to use
		if (pcs >= 0)
			masterStructure.configFlags |= (pcs << DSPI_MASTER_PCS_SHIFT);
		else
			return ATMO_SPI_Status_Invalid;

		// Configure PCS continuous/pulsed mode
		if (ATMO_Platform_SPI_Peripheral[instance].deviceConfig.ssContinuous)
			masterStructure.configFlags |= kDSPI_MasterPcsContinuous;
		else
			masterStructure.configFlags &= ~kDSPI_MasterPcsContinuous;
	}

	bool mallocTx = false;

	if (read)
	{
		masterStructure.txData = (uint8_t *)cmdBytes;
		masterStructure.dataSize = numCmdBytes + numDataBytes;
		masterStructure.rxData = dataBytes;
	}
	else if (numCmdBytes !=0)
	{
		mallocTx = true;
		txData = ATMO_Malloc(numCmdBytes + numDataBytes);

		memcpy(txData, cmdBytes, numCmdBytes);
		memcpy(txData+numCmdBytes, dataBytes, numDataBytes);

		masterStructure.txData = txData;
		masterStructure.dataSize = numCmdBytes + numDataBytes;
		masterStructure.rxData = NULL;
	}
	else
	{
		masterStructure.txData = dataBytes;
		masterStructure.dataSize = numDataBytes;
		masterStructure.rxData = NULL;
	}

	if (isPcsPinGpio && (ssPin >= 0))
		ATMO_MK64F_SPI_Master_AssertSlaveSelect(driverInstance, ssPin, ATMO_Platform_SPI_Peripheral[instance].deviceConfig.ssActiveLow);

	if (blocking)
		platformStatus = DSPI_MasterTransferBlocking(peripheralBase[instance], &masterStructure);
	else
		platformStatus = DSPI_MasterTransferNonBlocking(peripheralBase[instance], masterHandle[instance], &masterStructure);

	if (isPcsPinGpio && (ssPin >= 0))
		ATMO_MK64F_SPI_Master_DeAssertSlaveSelect(driverInstance, ssPin, ATMO_Platform_SPI_Peripheral[instance].deviceConfig.ssActiveLow);

	if (mallocTx)
		ATMO_Free(txData);

	return ATMO_Platform_SPI_Convert_Status_PlatformToAtmo(platformStatus);
}

ATMO_Status_t ATMO_MK64F_SPI_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber)
{
	ATMO_Status_t status = ATMO_Status_Success;

	for( int i = 0; (i < 3) && (status == ATMO_Status_Success); i++ )
	{
		ATMO_DriverInstanceData_t *instance = ATMO_Malloc(sizeof(ATMO_SPI_DriverInstance_t));

		uint32_t *spiPeriphNum = ATMO_Malloc(sizeof(uint32_t));
		*spiPeriphNum = i;

		instance->name = "MK64F SPI";
		instance->initialized = false;
		instance->instanceNumber = *instanceNumber;
		instance->argument = spiPeriphNum;

		status = ATMO_SPI_AddDriverInstance(&MK64FSPIDriverInstance, instance, instanceNumber);
	
	}

	return status;
}


/* Exported functions ---------------------------------------------------------*/

/**
 *  ATMO_SPI_Init
 *
 *    @param instance
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Init(ATMO_DriverInstanceData_t *instance)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	ATMO_SPI_Peripheral_t config;

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	config.operatingMode = ATMO_SPI_OperatingMode_Master;
	config.cbFunc = NULL;
	config.cbParam = NULL;
	config.deviceConfig.baudRate = 2000000;
	config.deviceConfig.clockContinuous = false;
	config.deviceConfig.clockMode = ATMO_SPI_ClockMode_0;
	config.deviceConfig.msbFirst = true;
	config.deviceConfig.ssActiveLow = true;
	config.deviceConfig.ssContinuous = false;

	return ATMO_MK64F_SPI_SetConfiguration(instance, &config);
}

/**
 *  ATMO_SPI_DeInit
 *
 *    @param instance
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_DeInit(ATMO_DriverInstanceData_t *instance)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	ATMO_Platform_SPI_Peripheral[spiPeriphNum].cbFunc = NULL;

	if ((masterHandle[spiPeriphNum] != NULL) || (slaveHandle[spiPeriphNum] != NULL))
		DSPI_Deinit(peripheralBase[spiPeriphNum]);

	if (masterHandle[spiPeriphNum] != NULL)
		ATMO_Free(masterHandle[spiPeriphNum]);

	if (slaveHandle[spiPeriphNum] != NULL)
		ATMO_Free(slaveHandle[spiPeriphNum]);

	if (slaveXfer[spiPeriphNum] != NULL)
		ATMO_Free(slaveXfer[spiPeriphNum]);

	if (slaveBuffer[spiPeriphNum] != NULL)
		ATMO_Free(slaveBuffer[spiPeriphNum]);

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_Init
 *
 *    @param instance
 *    @param config
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_SetConfiguration(ATMO_DriverInstanceData_t *instance, const ATMO_SPI_Peripheral_t *config)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

    if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	if (config == NULL)
		return ATMO_SPI_Status_Invalid;

	if ((masterHandle[spiPeriphNum] != NULL) || (slaveHandle[spiPeriphNum]) != NULL)
		return ATMO_SPI_Status_Initialized;

	if (config->operatingMode == ATMO_SPI_OperatingMode_Master)
	{
		clock_name_t clockSource;
	    dspi_master_config_t masterConfig;

	    // Start with the default config settings
	    DSPI_MasterGetDefaultConfig(&masterConfig);

	    switch (config->deviceConfig.clockMode)
	    {
	    	default :
			case ATMO_SPI_ClockMode_0 :
			    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
				masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
				break;
			case ATMO_SPI_ClockMode_1 :
			    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
				masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
				break;
			case ATMO_SPI_ClockMode_2 :
			    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
				masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
				break;
			case ATMO_SPI_ClockMode_3 :
			    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
				masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
				break;
	    }

	    masterConfig.enableContinuousSCK = config->deviceConfig.clockContinuous;
	    masterConfig.ctarConfig.baudRate = config->deviceConfig.baudRate;
		masterConfig.ctarConfig.direction = (config->deviceConfig.msbFirst) ? kDSPI_MsbFirst : kDSPI_LsbFirst;
	    masterConfig.pcsActiveHighOrLow = (config->deviceConfig.ssActiveLow) ? kDSPI_PcsActiveLow : kDSPI_PcsActiveHigh;
	    masterConfig.whichPcs = kDSPI_Pcs0;

	    switch (spiPeriphNum)
	    {
			case 0 :
				clockSource = DSPI0_CLK_SRC;
				break;
			case 1 :
				clockSource = DSPI1_CLK_SRC;
				break;
			case 2 :
				clockSource = DSPI2_CLK_SRC;
				break;
			default :
				clockSource = BUS_CLK;
				break;
	    }

	    DSPI_MasterInit(peripheralBase[spiPeriphNum], &masterConfig, CLOCK_GetFreq(clockSource));
	    masterHandle[spiPeriphNum] = (dspi_master_handle_t *) ATMO_Malloc(sizeof(dspi_master_handle_t));
	    DSPI_MasterTransferCreateHandle(peripheralBase[spiPeriphNum], masterHandle[spiPeriphNum], &ATMO_Platform_SPI_Master_Callback, config->cbParam);
	}
	else if (config->operatingMode == ATMO_SPI_OperatingMode_Slave)
	{
	    dspi_slave_config_t slaveConfig;

	    DSPI_SlaveGetDefaultConfig(&slaveConfig);

	    switch (config->deviceConfig.clockMode)
	    {
	    	default :
			case ATMO_SPI_ClockMode_0 :
				slaveConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
				slaveConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
				break;
			case ATMO_SPI_ClockMode_1 :
				slaveConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
				slaveConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
				break;
			case ATMO_SPI_ClockMode_2 :
				slaveConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
				slaveConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
				break;
			case ATMO_SPI_ClockMode_3 :
				slaveConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
				slaveConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
				break;
	    }

	    slaveConfig.enableContinuousSCK = config->deviceConfig.clockContinuous;

	    DSPI_SlaveInit(peripheralBase[spiPeriphNum], &slaveConfig);
	    slaveHandle[spiPeriphNum] = (dspi_slave_handle_t *) ATMO_Malloc(sizeof(dspi_slave_handle_t));
	    DSPI_SlaveTransferCreateHandle(peripheralBase[spiPeriphNum], slaveHandle[spiPeriphNum], &ATMO_Platform_SPI_Slave_Callback, config->cbParam);

	    slaveXfer[spiPeriphNum] = (ATMO_SPI_SlaveTransfer_t *) ATMO_Malloc(sizeof(ATMO_SPI_SlaveTransfer_t));
	    slaveBuffer[spiPeriphNum] = (uint8_t *) ATMO_Malloc(config->bufferSize);

	    slaveXfer[spiPeriphNum]->data = NULL;
	    slaveXfer[spiPeriphNum]->dataSize = 0;
	    slaveXfer[spiPeriphNum]->transferComplete = false;
	    slaveXfer[spiPeriphNum]->event = ATMO_SPI_SlaveEvent_Unknown;
	}
	else // invalid (should never get here)
	{
		return ATMO_SPI_Status_Invalid;
	}

	// Make a copy of the config structure in the peripheral config array.
	memcpy(&ATMO_Platform_SPI_Peripheral[spiPeriphNum], config, sizeof(ATMO_SPI_Peripheral_t));

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_GetConfiguration
 *
 *    @param instance
 *    @param config
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_GetConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_SPI_Peripheral_t *config)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

    memcpy(config, &ATMO_Platform_SPI_Peripheral[spiPeriphNum], sizeof(ATMO_SPI_Peripheral_t));

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_Master_SetDeviceConfiguration
 *
 *    @param instance
 *    @param ssPin
 *    @param config
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_SetDeviceConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const ATMO_SPI_Device_t *config)
{
	clock_name_t clockSource;
    dspi_master_config_t masterConfig;
	uint32_t tempVar;

	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	uint8_t pcs = ATMO_Platform_SPI_GetPcs(spiPeriphNum, ssPin);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

    switch (config->clockMode)
    {
    	default :
		case ATMO_SPI_ClockMode_0 :
		    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
			masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
			break;
		case ATMO_SPI_ClockMode_1 :
		    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
			masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
			break;
		case ATMO_SPI_ClockMode_2 :
		    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
			masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
			break;
		case ATMO_SPI_ClockMode_3 :
		    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
			masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
			break;
    }

    masterConfig.enableContinuousSCK = config->clockContinuous;
    masterConfig.ctarConfig.baudRate = config->baudRate;
	masterConfig.ctarConfig.direction = (config->msbFirst) ? kDSPI_MsbFirst : kDSPI_LsbFirst;
    masterConfig.pcsActiveHighOrLow = (config->ssActiveLow) ? kDSPI_PcsActiveLow : kDSPI_PcsActiveHigh;

    switch (spiPeriphNum)
    {
		case 0 :
			clockSource = DSPI0_CLK_SRC;
			break;
		case 1 :
			clockSource = DSPI1_CLK_SRC;
			break;
		case 2 :
			clockSource = DSPI2_CLK_SRC;
			break;
		default :
			clockSource = BUS_CLK;
			break;
    }

    // halt SPI peripheral
    DSPI_StopTransfer(peripheralBase[spiPeriphNum]);

	if(ssPin >= 0)
	{
		// update continuous clock and pcs level settings
		tempVar = peripheralBase[spiPeriphNum]->MCR & ~(SPI_MCR_CONT_SCKE_MASK | SPI_MCR_PCSIS(1 << pcs));
		tempVar |= SPI_MCR_CONT_SCKE(masterConfig.enableContinuousSCK) | SPI_MCR_PCSIS(masterConfig.pcsActiveHighOrLow << pcs);
		peripheralBase[spiPeriphNum]->MCR = tempVar;
	}

    // update lsb_first and clock polarity/phase settings
    tempVar = peripheralBase[spiPeriphNum]->CTAR[kDSPI_Ctar0] & ~(SPI_CTAR_CPOL_MASK | SPI_CTAR_CPHA_MASK | SPI_CTAR_LSBFE_MASK);
    tempVar |= SPI_CTAR_CPOL(masterConfig.ctarConfig.cpol) | SPI_CTAR_CPHA(masterConfig.ctarConfig.cpha) | SPI_CTAR_LSBFE(masterConfig.ctarConfig.direction);
    peripheralBase[spiPeriphNum]->CTAR[kDSPI_Ctar0] = tempVar;

	// update baud rate (this takes a long time so skip if it's not changing)
    if (masterConfig.ctarConfig.baudRate != ATMO_Platform_SPI_Peripheral[spiPeriphNum].deviceConfig.baudRate)
    	tempVar = DSPI_MasterSetBaudRate(peripheralBase[spiPeriphNum], kDSPI_Ctar0, masterConfig.ctarConfig.baudRate, CLOCK_GetFreq(clockSource));
    else
        tempVar = masterConfig.ctarConfig.baudRate;

    // resume SPI peripheral
    DSPI_StartTransfer(peripheralBase[spiPeriphNum]);

    memcpy(&ATMO_Platform_SPI_Peripheral[spiPeriphNum].deviceConfig, config, sizeof(ATMO_SPI_Device_t));

    // update with actual calculated baud rate
    ATMO_Platform_SPI_Peripheral[spiPeriphNum].deviceConfig.baudRate = tempVar;

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_Master_GetDeviceConfiguration
 *
 *    @param instance
 *    @param config
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_GetDeviceConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, ATMO_SPI_Device_t *config)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

    memcpy(config, &ATMO_Platform_SPI_Peripheral[spiPeriphNum].deviceConfig, sizeof(ATMO_SPI_Device_t));

	return ATMO_SPI_Status_Success;
}

/**
 *  AIR_SPI_RegisterMasterCallback
 *
 *    @param instance
 *    @param cbFunc
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_RegisterCallback(ATMO_DriverInstanceData_t *instance, ATMO_SPI_Master_Callback_t cbFunc)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	if (cbFunc == NULL)
		return ATMO_SPI_Status_Invalid;

	if (ATMO_Platform_SPI_Peripheral[spiPeriphNum].operatingMode != ATMO_SPI_OperatingMode_Master)
		return ATMO_SPI_Status_Invalid;

	ATMO_Platform_SPI_Peripheral[spiPeriphNum].cbFunc = (ATMO_SPI_Master_Callback_t)cbFunc;

	return ATMO_SPI_Status_Success;
}

/**
 *  AIR_SPI_RegisterSlaveCallback
 *
 *    @param instance
 *    @param cbFunc
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Slave_RegisterCallback(ATMO_DriverInstanceData_t *instance, ATMO_SPI_Slave_Callback_t cbFunc)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	if (cbFunc == NULL)
		return ATMO_SPI_Status_Invalid;

	if (ATMO_Platform_SPI_Peripheral[spiPeriphNum].operatingMode != ATMO_SPI_OperatingMode_Slave)
		return ATMO_SPI_Status_Invalid;

	ATMO_Platform_SPI_Peripheral[spiPeriphNum].cbFunc = (ATMO_SPI_Slave_Callback_t)cbFunc;

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_Master_Assert_SlaveSelect
 *
 *    @param ssPin
 *    @param ssActiveLow
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_AssertSlaveSelect(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, bool ssActiveLow)
{
	if(ssPin < 0)
	{
		return ATMO_SPI_Status_Success;
	}

	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	uint8_t portNum = (uint8_t)((ssPin >> 8) & 0xFF);
	uint8_t pinNum = (uint8_t)((ssPin >> 0) & 0xFF);
	uint8_t value = (ssActiveLow) ? 0 : 1;

	// Check if pin is configured as an output
	if (!(gpioBase[portNum]->PDDR & (1 << pinNum)))
		return ATMO_SPI_Status_Invalid;

	GPIO_WritePinOutput(gpioBase[portNum], pinNum, value);

	return ATMO_SPI_Status_Success;
}

/**
 *  ATMO_SPI_Master_DeAssert_SlaveSelect
 *
 *    @param ssPin
 *    @param ssActiveLow
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_DeAssertSlaveSelect(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, bool ssActiveLow)
{
	if(ssPin < 0)
	{
		return ATMO_SPI_Status_Success;
	}
	
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	return ATMO_MK64F_SPI_Master_AssertSlaveSelect(instance, ssPin, !ssActiveLow);
}

/**
 *  ATMO_SPI_Master_Write
 *
 *    @param instance
 *    @param ssPin
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param writeBytes
 *    @param numWriteBytes
 *    @param timeout_ms
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_Write(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	return ATMO_Platform_SPI_Master_Transfer(instance, false, true, spiPeriphNum, ssPin, cmdBytes, numCmdBytes, (uint8_t *)writeBytes, numWriteBytes);
}

/**
 *  ATMO_SPI_Master_Write_NonBlocking
 *
 *    @param instance
 *    @param ssPin
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param writeBytes
 *    @param numWriteBytes
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_WriteNonBlocking(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	return ATMO_Platform_SPI_Master_Transfer(instance, false, false, spiPeriphNum, ssPin, cmdBytes, numCmdBytes, (uint8_t *)writeBytes, numWriteBytes);
}

/**
 *  ATMO_SPI_Master_Read
 *
 *    @param instance
 *    @param ssPin
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param readBytes
 *    @param numReadBytes
 *    @param timeout_ms
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_Read(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes, uint16_t timeout_ms)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	return ATMO_Platform_SPI_Master_Transfer(instance, true, true, spiPeriphNum, ssPin, cmdBytes, numCmdBytes, readBytes, numReadBytes);
}

/**
 *  ATMO_SPI_Master_Read
 *
 *    @param instance
 *    @param ssPin
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param readBytes
 *    @param numReadBytes
 */
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_ReadNonBlocking(ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes)
{
	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	return ATMO_Platform_SPI_Master_Transfer(instance, true, false, spiPeriphNum, ssPin, cmdBytes, numCmdBytes, readBytes, numReadBytes);
}

ATMO_SPI_Status_t ATMO_MK64F_SPI_Slave_WaitForReceive(ATMO_DriverInstanceData_t *instance, uint16_t xferSize)
{
	status_t platformStatus = kStatus_Fail;
	dspi_transfer_t transfer;

	uint32_t spiPeriphNum = *((uint32_t *)instance->argument);

	if (spiPeriphNum > 2)
		return ATMO_SPI_Status_NotSupported;

	if(slaveHandle[spiPeriphNum] == NULL)
		return ATMO_SPI_Status_Invalid;

	if (ATMO_Platform_SPI_Peripheral[spiPeriphNum].operatingMode != ATMO_SPI_OperatingMode_Slave)
		return ATMO_SPI_Status_Invalid;

	transfer.txData = NULL;
	transfer.rxData = slaveBuffer[spiPeriphNum];
	transfer.dataSize = xferSize;
	transfer.configFlags = kDSPI_SlaveCtar0;

	platformStatus = DSPI_SlaveTransferNonBlocking(peripheralBase[spiPeriphNum], slaveHandle[spiPeriphNum], &transfer);

	return ATMO_Platform_SPI_Convert_Status_PlatformToAtmo(platformStatus);
}
