/**
 ******************************************************************************
 * @file    atmo_i2c.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - I2C source file for MCUXpresso SDK
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
#include "fsl_i2c.h"
#include "clock_config.h"

#include "i2c.h"
#include "i2c_mk64f.h"
#include <string.h>
#include <stdlib.h>

#define ATMO_I2C_TIMEOUT 100000

const ATMO_I2C_DriverInstance_t mk64fI2CDriverInstance = {
	ATMO_MK64F_I2C_Init,
	ATMO_MK64F_I2C_DeInit,
	ATMO_MK64F_I2C_SetConfiguration,
	ATMO_MK64F_I2C_GetConfiguration,
	ATMO_MK64F_I2C_MasterRegisterCallback,
	ATMO_MK64F_I2C_SlaveRegisterCallback,
	ATMO_MK64F_I2C_MasterWrite,
	ATMO_MK64F_I2C_MasterWriteNonBlocking,
	ATMO_MK64F_I2C_MasterRead,
	ATMO_MK64F_I2C_MasterReadNonBlocking,
	ATMO_MK64F_I2C_MasterGetStatus,
	ATMO_MK64F_I2C_SlaveWaitForReceive
};

/* Imported function prototypes ----------------------------------------------*/
extern uint32_t I2C_GetInstance( I2C_Type* base );


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	kI2C_Blocking    = 0x01u,
	kI2C_NonBlocking = 0x02u,
} kI2C_Blocking_t;


/* Private define ------------------------------------------------------------*/
#define ATMO_I2C_DEFAULT_OPERATING_MODE       ATMO_I2C_OperatingMode_Uninitialized
#define ATMO_I2C_DEFAULT_MASTER_BAUDRATE      ATMO_I2C_BaudRate_Standard_Mode
#define ATMO_I2C_DEFAULT_SLAVE_MODE_ADDRESS   0x30u
#define ATMO_I2C_DEFAULT_SLAVE_BUFFER_SIZE    32u
#define MK64F_NUM_I2C_INSTANCES (3)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static I2C_Type* peripheralBase[3] = I2C_BASE_PTRS;
static i2c_master_handle_t masterHandle[3];
static i2c_slave_handle_t slaveHandle[3];
static ATMO_I2C_Peripheral_t ATMO_Platform_I2C_Peripheral[3];

static ATMO_I2C_SlaveTransfer_t slaveXfer[3];
static uint8_t *slaveBuffer[3];


/* Private functions ---------------------------------------------------------*/

/**
 * This routine converts the baud rate values from Atmosphere API type to platform type.
 *
 * @param baudRate :The Atmo API baud rate.
 * @return Baud rate as platform type.
 */
static uint32_t ATMO_Platform_I2C_ConvertBaudRate_AtmoToPlatform( ATMO_I2C_BaudRate_t baudRate )
{
	uint32_t baudRate_bps;

	switch ( baudRate )
	{
		case ATMO_I2C_BaudRate_Low_Speed_Mode :
		case ATMO_I2C_BaudRate_10kHz :
			baudRate_bps = 10000;
			break;

		case ATMO_I2C_BaudRate_Standard_Mode :
		case ATMO_I2C_BaudRate_100kHz :
			baudRate_bps = 100000;
			break;

		case ATMO_I2C_BaudRate_Fast_Mode :
		case ATMO_I2C_BaudRate_400kHz :
			baudRate_bps = 400000;
			break;

		case ATMO_I2C_BaudRate_Fast_Mode_Plus :
		case ATMO_I2C_BaudRate_1000kHz :
			baudRate_bps = 1000000;
			break;

		// The MK64K I2C peripheral is limited to 1MHz
		case ATMO_I2C_BaudRate_High_Speed_Mode :
		case ATMO_I2C_BaudRate_3400kHz :
		case ATMO_I2C_BaudRate_Ultra_Speed_Mode :
		case ATMO_I2C_BaudRate_5000kHz :
			baudRate_bps = 1000000;
			break;

		default :
			baudRate_bps = 100000;
			break;
	}

	return baudRate_bps;
}

/**
 * This routine converts the status values from platform type to Atmosphere API type.
 *
 * @param status :The platform status.
 * @return Status as Atmosphere API type.
 */
static ATMO_I2C_Status_t ATMO_Platform_I2C_Convert_Status_PlatformToAtmo( status_t status )
{
	ATMO_I2C_Status_t returnStatus;

	switch ( status )
	{
		case kStatus_Success :
			returnStatus = ATMO_I2C_Status_Success;
			break;

		case kStatus_Fail :
			returnStatus = ATMO_I2C_Status_Fail;
			break;

		case kStatus_Timeout :
		case kStatus_I2C_Timeout :
			returnStatus = ATMO_I2C_Status_Timeout;
			break;

		case kStatus_I2C_Busy :
			returnStatus = ATMO_I2C_Status_Busy;
			break;

		case kStatus_I2C_Idle :
			returnStatus = ATMO_I2C_Status_Idle;
			break;

		case kStatus_I2C_Nak :
		case kStatus_I2C_Addr_Nak :
			returnStatus = ATMO_I2C_Status_ReceivedNak;
			break;

		case kStatus_I2C_ArbitrationLost :
			returnStatus = ATMO_I2C_Status_ArbitrationLost;
			break;

		default :
			returnStatus = ATMO_I2C_Status_Unknown;
			break;
	}

	return returnStatus;
}

/**
 * This routine is the callback function for the platform in master mode.
 *
 * @param base :The pointer to the I2C peripheral base.
 * @param handle :The pointer to the master handle structure.
 * @param status :The status of the transfer.
 * @param userData :The pointer to user defined data.
 * @return None.
 */
static void ATMO_Platform_I2C_Master_Callback( I2C_Type* base, i2c_master_handle_t* handle, status_t status, void* userData )
{
//	ATMO_I2C_MasterTransfer_t masterXfer;
//	uint32_t instance = I2C_GetInstance( base );
//	ATMO_I2C_Master_Callback_t callback = ATMO_Platform_I2C_Peripheral[instance].cbFunc;
//	size_t xferCount;
//
//	if ( ( handle->transfer.direction == kI2C_Read ) && ( status == kStatus_Success ) )
//	{
//		I2C_MasterTransferGetCount( base, handle, &xferCount );
//
//		masterXfer.event = ATMO_I2C_MasterEvent_Complete;
//		masterXfer.data = handle->transfer.data - xferCount;
//		masterXfer.dataSize = xferCount;
//
//		callback( &masterXfer, userData );
//	}
}

/**
 * This routine is the callback function for the platform in slave mode.
 *
 * @param base :The pointer to the I2C peripheral base.
 * @param xfer :The pointer to the slave transfer structure.
 * @param userData :The pointer to user defined data.
 * @return None.
 */
static void ATMO_Platform_I2C_Slave_Callback( I2C_Type* base, i2c_slave_transfer_t* xfer, void* userData )
{
	uint32_t instance = I2C_GetInstance( base );
	ATMO_I2C_Slave_Callback_t callback = ATMO_Platform_I2C_Peripheral[instance].cbFunc;

	switch ( xfer->event )
	{
		case kI2C_SlaveAddressMatchEvent :
			slaveXfer[instance].event = ATMO_I2C_SlaveEvent_AddressMatch;

			if( xfer->data != NULL ) // pointer will be valid after repeated START
			{
				// Get buffer information from application
				slaveXfer[instance].event = ATMO_I2C_SlaveEvent_ReadRequest;
				slaveXfer[instance].data = slaveBuffer[instance];
				slaveXfer[instance].dataSize = xfer->transferredCount;
				slaveXfer[instance].transferComplete = false;
				callback( &slaveXfer[instance], userData );

				// Provide transmit buffer pointer information
				xfer->data = slaveXfer[instance].data;
				xfer->dataSize = slaveXfer[instance].dataSize;
			}

			break;

		case kI2C_SlaveReceiveEvent :
			slaveXfer[instance].event = ATMO_I2C_SlaveEvent_WriteRequest;

			if ( xfer->data != NULL )
			{
				// Send data received up to the application
				slaveXfer[instance].data = slaveBuffer[instance];
				slaveXfer[instance].dataSize = xfer->transferredCount;
				slaveXfer[instance].transferComplete = false;
				callback( &slaveXfer[instance], userData );
			}

			// Provide receive buffer pointer information
			xfer->data = slaveBuffer[instance];
			xfer->dataSize = ATMO_Platform_I2C_Peripheral[instance].bufferSize;
			break;

		case kI2C_SlaveCompletionEvent :
			slaveXfer[instance].transferComplete = true;

			// slave receiving from master
			if ( slaveXfer[instance].event == ATMO_I2C_SlaveEvent_WriteRequest )
			{
				slaveXfer[instance].data = slaveBuffer[instance];
				slaveXfer[instance].dataSize = xfer->transferredCount;
				callback( &slaveXfer[instance], userData );
			}

			slaveXfer[instance].event = ATMO_I2C_SlaveEvent_Unknown;
			slaveXfer[instance].data = NULL;
			slaveXfer[instance].dataSize = 0;
			xfer->data = NULL;
			xfer->dataSize = 0;
			break;

		default :
			break;
	}
}

/**
 * This routine performs the data transfer (write or read) in master mode.
 *
 * @param direction :Specifies whether the operation is a write or read.
 * @param blocking :Specifies whether the function should wait until the transfer is complete before returning (blocking) or if it should return immediately (non-blocking)
 * @param instance :The peripheral instance used for writing/reading the specified slave device.
 * @param slaveAddress :The I2C address of the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the I2C peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the I2C peripheral. May be 0.
 * @param dataBytes :The pointer to a buffer for holding/storing the data bytes that will be transfered to/from the I2C peripheral.
 * @param numDataBytes :The number of bytes to be transferred.
 * @return status.
 */
static ATMO_I2C_Status_t ATMO_Platform_I2C_Master_Transfer( i2c_direction_t direction, kI2C_Blocking_t blocking,
		uint32_t instance, uint16_t slaveAddress, const uint8_t* cmdBytes, uint16_t numCmdBytes, uint8_t* dataBytes,
		uint16_t numDataBytes )
{
	status_t platformStatus = kStatus_Fail;
	i2c_master_transfer_t masterStructure;
	uint32_t cmd;

	if ( instance > MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( ATMO_Platform_I2C_Peripheral[instance].operatingMode == ATMO_I2C_OperatingMode_Slave )
	{
		return ATMO_I2C_Status_Invalid;
	}

	if ( numCmdBytes > 4 )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( ( dataBytes == NULL ) || ( numDataBytes == 0 ) )
	{
		return ATMO_I2C_Status_Invalid;
	}

	// if either cmdBytes or numCmdBytes is null/zero then set both to null/zero
	if ( cmdBytes == NULL )
	{
		numCmdBytes = 0;
	}
	else if	( numCmdBytes == 0 )
	{
		cmdBytes = NULL;
	}

	if ( numCmdBytes == 1 )
	{
		cmd = cmdBytes[0];
	}
	else if ( numCmdBytes == 2 )
	{
		cmd = ( cmdBytes[0] << 8 ) | cmdBytes[1];
	}
	else if ( numCmdBytes == 3 )
	{
		cmd = ( cmdBytes[0] << 16 ) | ( cmdBytes[1] << 8 ) | cmdBytes[2];
	}
	else if ( numCmdBytes == 4 )
	{
		cmd = ( cmdBytes[0] << 24 ) | ( cmdBytes[1] << 16 ) | ( cmdBytes[2] << 8 ) | cmdBytes[3];
	}

	masterStructure.flags = kI2C_TransferDefaultFlag;
	masterStructure.slaveAddress = slaveAddress;
	masterStructure.direction = direction;
	masterStructure.subaddress = cmd;
	masterStructure.subaddressSize = numCmdBytes;
	masterStructure.data = dataBytes;
	masterStructure.dataSize = numDataBytes;

	if ( blocking == kI2C_Blocking )
	{
		uint32_t timeoutCounter = ATMO_I2C_TIMEOUT;
		platformStatus = I2C_MasterTransferNonBlocking( peripheralBase[instance], &masterHandle[instance], &masterStructure );

		for(int i = 0; i < 1000; i++)
		{

		}

		while ( peripheralBase[instance]->S & I2C_S_BUSY_MASK && (timeoutCounter > 0))
		{
			timeoutCounter--;
		}

		if(timeoutCounter == 0)
		{
			return ATMO_I2C_Status_Timeout;
		}
	}
	else
	{
		platformStatus = I2C_MasterTransferNonBlocking( peripheralBase[instance], &masterHandle[instance], &masterStructure );
	}

	return ATMO_Platform_I2C_Convert_Status_PlatformToAtmo( platformStatus );
}

ATMO_Status_t ATMO_MK64F_I2C_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	static ATMO_DriverInstanceData_t driverData[MK64F_NUM_I2C_INSTANCES];
	static uint32_t periphNums[MK64F_NUM_I2C_INSTANCES];
	ATMO_Status_t status = ATMO_Status_Success;

	for( int i = 0; ( i < MK64F_NUM_I2C_INSTANCES ) && ( status == ATMO_Status_Success ); i++ )
	{
		periphNums[i] = i;

		driverData[i].name = "MK64F I2C";
		driverData[i].initialized = false;
		driverData[i].instanceNumber = *instanceNumber;
		driverData[i].argument = &periphNums[i];
		status = ATMO_I2C_AddDriverInstance( &mk64fI2CDriverInstance, &driverData[i], instanceNumber );
	}

	return status;
}


/* Exported functions ---------------------------------------------------------*/
/**
 *  ATMO_I2C_Init
 *
 *    @param instance
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_Init( ATMO_DriverInstanceData_t *instance )
{
	ATMO_I2C_Peripheral_t config;
	config.operatingMode = ATMO_I2C_DEFAULT_OPERATING_MODE;
	config.baudRate = ATMO_I2C_DEFAULT_MASTER_BAUDRATE;
	config.cbFunc = NULL;
	config.cbParam = NULL;

	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	return ATMO_MK64F_I2C_SetConfiguration( instance, &config );
}

/**
 *  ATMO_I2C_SetConfiguration
 *
 *    @param instance
 *    @param config
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_SetConfiguration( ATMO_DriverInstanceData_t *instance, const ATMO_I2C_Peripheral_t* config )
{
	clock_name_t clockSource;

	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( config == NULL )
	{
		return ATMO_I2C_Status_Invalid;
	}

	switch ( periphNum )
	{
		case 0 :
			clockSource = I2C0_CLK_SRC;
			break;

		case 1 :
			clockSource = I2C1_CLK_SRC;
			break;

		case 2 :
			clockSource = I2C2_CLK_SRC;
			break;

		default :
			clockSource = BUS_CLK;
			break;
	}

	if ( config->operatingMode == ATMO_I2C_OperatingMode_Master )
	{
		i2c_master_config_t masterConfig;

		I2C_MasterGetDefaultConfig( &masterConfig );
		masterConfig.enableMaster = true;
		masterConfig.baudRate_Bps = ATMO_Platform_I2C_ConvertBaudRate_AtmoToPlatform( config->baudRate );
		masterConfig.enableStopHold = false;
		masterConfig.glitchFilterWidth = 0;
		I2C_MasterInit( peripheralBase[periphNum], &masterConfig, CLOCK_GetFreq( clockSource ) );
		I2C_MasterTransferCreateHandle( peripheralBase[periphNum], &masterHandle[periphNum], &ATMO_Platform_I2C_Master_Callback,
										config->cbParam );
		ATMO_Platform_I2C_Peripheral[periphNum].cbFunc = ( ATMO_I2C_Master_Callback_t )config->cbFunc;
	}
	else if ( config->operatingMode == ATMO_I2C_OperatingMode_Slave )
	{
		i2c_slave_config_t slaveConfig;

		I2C_SlaveGetDefaultConfig( &slaveConfig );
		slaveConfig.enableSlave = true;
		slaveConfig.enableBaudRateCtl = false;
		slaveConfig.enableGeneralCall = false;
		slaveConfig.enableWakeUp = false;
		slaveConfig.addressingMode = kI2C_Address7bit;
		slaveConfig.slaveAddress = config->localAddress;
//	    slaveConfig.upperAddress = ;
//	    slaveConfig.sclStopHoldTime_ns = ;
		I2C_SlaveInit( peripheralBase[periphNum], &slaveConfig, CLOCK_GetFreq( clockSource ) );
		I2C_SlaveTransferCreateHandle( peripheralBase[periphNum], &slaveHandle[periphNum], &ATMO_Platform_I2C_Slave_Callback,
									   config->cbParam );
//	    ATMO_Platform_I2C_Peripheral[periphNum].cbFunc = (ATMO_I2C_Slave_Callback_t)config->cbFunc;

		slaveBuffer[periphNum] = ( uint8_t* ) ATMO_Malloc( config->bufferSize );

		slaveXfer[periphNum].data = NULL;
		slaveXfer[periphNum].dataSize = 0;
		slaveXfer[periphNum].transferComplete = false;
		slaveXfer[periphNum].event = ATMO_I2C_SlaveEvent_Unknown;
	}
	else // invalid (should never get here)
	{
		return ATMO_I2C_Status_Invalid;
	}

	// Make a copy of the config structure in the peripheral config array.
	memcpy( &ATMO_Platform_I2C_Peripheral[periphNum], config, sizeof( ATMO_I2C_Peripheral_t ) );

	return ATMO_I2C_Status_Success;
}

/**
 *  ATMO_I2C_GetConfiguration
 *
 *    @param instance
 *    @param config
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_GetConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_I2C_Peripheral_t* config )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( config == NULL )
	{
		return ATMO_I2C_Status_Invalid;
	}

	memcpy( config, &ATMO_Platform_I2C_Peripheral[periphNum], sizeof( ATMO_I2C_Peripheral_t ) );

	return ATMO_I2C_Status_Success;
}

/**
 *  ATMO_I2C_DeInit
 *
 *    @param instance
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_DeInit( ATMO_DriverInstanceData_t *instance )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	ATMO_Platform_I2C_Peripheral[periphNum].cbFunc = NULL;


	if ( slaveBuffer[periphNum] != NULL )
	{
		ATMO_Free( slaveBuffer[periphNum] );
	}


	return ATMO_I2C_Status_Success;
}

/**
 *  ATMO_I2C_Master_Write
 *
 *    @param instance
 *    @param slaveAddress
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param writeBytes
 *    @param numWriteBytes
 *    @param timeout_ms
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterWrite( ATMO_DriverInstanceData_t *instance, uint16_t slaveAddress,
		const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	return ATMO_Platform_I2C_Master_Transfer( kI2C_Write, kI2C_Blocking, periphNum, slaveAddress, cmdBytes, numCmdBytes,
			( uint8_t* )writeBytes, numWriteBytes );
}

/**
 *  ATMO_I2C_Master_Write_NonBlocking
 *
 *    @param instance
 *    @param slaveAddress
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param writeBytes
 *    @param numWriteBytes
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterWriteNonBlocking( ATMO_DriverInstanceData_t *instance, uint16_t slaveAddress,
		const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	return ATMO_Platform_I2C_Master_Transfer( kI2C_Write, kI2C_NonBlocking, periphNum, slaveAddress, cmdBytes, numCmdBytes,
			( uint8_t* )writeBytes, numWriteBytes );
}

/**
 *  ATMO_I2C_Master_Read
 *
 *    @param instance
 *    @param slaveAddress
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param readBytes
 *    @param numReadBytes
 *    @param timeout_ms
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterRead( ATMO_DriverInstanceData_t *instance, uint16_t slaveAddress,
		const uint8_t* cmdBytes, uint16_t numCmdBytes, uint8_t* readBytes, uint16_t numReadBytes, uint16_t timeout_ms )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	return ATMO_Platform_I2C_Master_Transfer( kI2C_Read, kI2C_Blocking, periphNum, slaveAddress, cmdBytes, numCmdBytes, readBytes,
			numReadBytes );
}

/**
 *  ATMO_I2C_Master_Read_NonBlocking
 *
 *    @param instance
 *    @param slaveAddress
 *    @param cmdBytes
 *    @param numCmdBytes
 *    @param readBytes
 *    @param numReadBytes
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterReadNonBlocking( ATMO_DriverInstanceData_t *instance, uint16_t slaveAddress,
		const uint8_t* cmdBytes, uint16_t numCmdBytes, uint8_t* readBytes, uint16_t numReadBytes )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	return ATMO_Platform_I2C_Master_Transfer( kI2C_Read, kI2C_NonBlocking, periphNum, slaveAddress, cmdBytes, numCmdBytes, readBytes,
			numReadBytes );
}

/**
 *  ATMO_I2C_Master_GetStatus
 *
 *    @param instance
 *    @param bytesRemaining
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterGetStatus( ATMO_DriverInstanceData_t *instance, uint16_t* bytesRemaining )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( bytesRemaining == NULL )
	{
		return ATMO_I2C_Status_Invalid;
	}

	if ( ATMO_Platform_I2C_Peripheral[periphNum].operatingMode == ATMO_I2C_OperatingMode_Slave )
	{
		return ATMO_I2C_Status_Invalid;
	}

	*bytesRemaining = masterHandle[periphNum].transfer.dataSize;

	if ( masterHandle[periphNum].state != 0 ) // kIdleState
	{
		return ATMO_I2C_Status_Busy;
	}

	return ATMO_I2C_Status_Success;
}

/**
 *  AIR_I2C_Master_RegisterCallback
 *
 *    @param instance
 *    @param cbFunc
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_MasterRegisterCallback( ATMO_DriverInstanceData_t *instance, ATMO_I2C_Master_Callback_t cbFunc )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( cbFunc == NULL )
	{
		return ATMO_I2C_Status_Invalid;
	}

	if ( ATMO_Platform_I2C_Peripheral[periphNum].operatingMode == ATMO_I2C_OperatingMode_Slave )
	{
		return ATMO_I2C_Status_Invalid;
	}

	ATMO_Platform_I2C_Peripheral[periphNum].cbFunc = ( ATMO_I2C_Master_Callback_t )cbFunc;

	return ATMO_I2C_Status_Success;
}

/**
 *  AIR_I2C_Slave_RegisterCallback
 *
 *    @param instance
 *    @param cbFunc
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_SlaveRegisterCallback( ATMO_DriverInstanceData_t *instance, ATMO_I2C_Slave_Callback_t cbFunc )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	if ( cbFunc == NULL )
	{
		return ATMO_I2C_Status_Invalid;
	}

	if ( ATMO_Platform_I2C_Peripheral[periphNum].operatingMode == ATMO_I2C_OperatingMode_Master )
	{
		return ATMO_I2C_Status_Invalid;
	}

	ATMO_Platform_I2C_Peripheral[periphNum].cbFunc = ( ATMO_I2C_Slave_Callback_t )cbFunc;

	return ATMO_I2C_Status_Success;
}

/**
 *  ATMO_I2C_Slave_WaitForReceive
 *
 *    @param instance
 *    @return status.
 */
ATMO_I2C_Status_t ATMO_MK64F_I2C_SlaveWaitForReceive( ATMO_DriverInstanceData_t *instance )
{
	uint32_t periphNum = *((uint32_t *)instance->argument);

	if ( periphNum >= MK64F_NUM_I2C_INSTANCES )
	{
		return ATMO_I2C_Status_NotSupported;
	}

	status_t platformStatus = kStatus_Fail;

	if ( ATMO_Platform_I2C_Peripheral[periphNum].operatingMode == ATMO_I2C_OperatingMode_Master )
	{
		return ATMO_I2C_Status_Invalid;
	}

	platformStatus = I2C_SlaveTransferNonBlocking( peripheralBase[periphNum], &slaveHandle[periphNum],
					 kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent );

	return ATMO_Platform_I2C_Convert_Status_PlatformToAtmo( platformStatus );
}
