/**
 ******************************************************************************
 * @file    spi.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - SPI header file
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
 *   3. Neither the name of Atmosphere IoT Corp. nor the names of its contributors
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

/** \addtogroup SPI
 *  @{
 */

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_SPI__H
#define __ATMO_SPI__H


/* Includes ------------------------------------------------------------------*/
#include "../atmo/core.h"

#ifdef __cplusplus
	extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

/**
 * SPI Operating Mode
 */
typedef enum
{
	ATMO_SPI_OperatingMode_Uninitialized  = 0x00u,
	ATMO_SPI_OperatingMode_Master         = 0x01u,
	ATMO_SPI_OperatingMode_Slave          = 0x02u,
} ATMO_SPI_OperatingMode_t;

/**
 * SPI Clock Mode
 */
typedef enum
{
	ATMO_SPI_ClockMode_0  = 0x00u,  /**< clock idle low, data sampled rising edge */
	ATMO_SPI_ClockMode_1  = 0x01u,  /**< clock idle low, data sampled falling edge */
	ATMO_SPI_ClockMode_2  = 0x02u,  /**< clock idle high, data sampled falling edge */
	ATMO_SPI_ClockMode_3  = 0x03u,  /**< clock idle high, data sampled rising edge */
} ATMO_SPI_ClockMode_t;

/**
 * SPI Driver Return Enumerations
 */
typedef enum
{
    ATMO_SPI_Status_Success       = 0x00u,  /**< Common - Operation was successful */
    ATMO_SPI_Status_Fail          = 0x01u,  /**< Common - Operation failed */
    ATMO_SPI_Status_Initialized   = 0x02u,  /**< Common - Peripheral already initialized */
    ATMO_SPI_Status_Invalid       = 0x03u,  /**< Common - Invalid operation */
    ATMO_SPI_Status_NotSupported  = 0x04u,  /**< Common - Feature not supported by platform */
    ATMO_SPI_Status_Unknown       = 0x05u,  /**< Common - Some other status not defined */
    ATMO_SPI_Status_Busy          = 0x20u,  /**< Master - Transfer in progress */
    ATMO_SPI_Status_Timeout       = 0x21u,  /**< Master - Transfer timed out */
    ATMO_SPI_Status_TxUnderrun    = 0x40u,  /**< Slave  - TX Underrun error */
    ATMO_SPI_Status_RxOverrun     = 0x41u,  /**< Slave  - RX Overrun error */
    ATMO_SPI_Status_Idle          = 0x42u,  /**< Slave  - Bus is Idle */
} ATMO_SPI_Status_t;

/**
 * SPI Master Nonblocking Event
 */
typedef enum
{
	ATMO_SPI_MasterEvent_Unknown   = 0x00u,  //
	ATMO_SPI_MasterEvent_Complete  = 0x01u,  /**< Transfer Complete */
} ATMO_SPI_MasterEvent_t;

/**
 * SPI Slave Nonblocking Event
 */
typedef enum
{
	ATMO_SPI_SlaveEvent_Unknown   = 0x0000u,  //
	ATMO_SPI_SlaveEvent_Complete  = 0x0001u,  /**< Transfer Complete */
} ATMO_SPI_SlaveEvent_t;


/* Exported Structures -------------------------------------------------------*/

/**
 * SPI Single Device Configuration
 */
typedef struct
{
	bool ssActiveLow; /**< Slave select active low */
	bool ssContinuous; /**< Slave select continuous */
	bool clockContinuous; /**< Clock continuous */
	uint32_t baudRate; /**< Baud rate in hz */
	ATMO_SPI_ClockMode_t clockMode; /**< SPI Clock Mode */
	bool msbFirst; /**< MSB First */
} ATMO_SPI_Device_t;

/**
 * SPI Peripheral Configuration
 */
typedef struct
{
	ATMO_SPI_OperatingMode_t operatingMode; /**< Operating mode */
	ATMO_SPI_Device_t deviceConfig; /**< Default device configuration */
	uint32_t bufferSize; /**< Buffer to be used for slave operation */
	void *cbFunc; /**< UNUSED */
	void *cbParam; /**< UNUSED */
} ATMO_SPI_Peripheral_t;

/**
 * SPI Nonblocking Slave Transfer Event
 */
typedef struct
{
	ATMO_SPI_SlaveEvent_t event; /**< Event type */
    uint8_t *data; /**< Event data */
    uint32_t dataSize;/**< Event data size */
    bool transferComplete; /**< Transfer complete flag */
} ATMO_SPI_SlaveTransfer_t;

/**
 * SPI Nonblocking Master Transfer Event
 */
typedef struct
{
	ATMO_SPI_MasterEvent_t event; /**< Event type */
    uint8_t *data; /**< Event data */
    uint32_t dataSize; /**< Event data size */
    bool transferComplete; /**< Transfer complete flag */
} ATMO_SPI_MasterTransfer_t;

typedef int16_t ATMO_SPI_CS_t;
#define ATMO_SPI_CS_NONE (-1) //*< No CS Pin, it will be ignored */

typedef void(*ATMO_SPI_Master_Callback_t)(ATMO_SPI_MasterTransfer_t *xfer, void *cbParam);
typedef void(*ATMO_SPI_Slave_Callback_t)(ATMO_SPI_SlaveTransfer_t *xfer, void *cbParam);

typedef struct ATMO_SPI_DriverInstance_t ATMO_SPI_DriverInstance_t;

struct ATMO_SPI_DriverInstance_t{
	ATMO_SPI_Status_t (*Init)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_SPI_Status_t (*DeInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_SPI_Status_t (*SetConfiguration)(ATMO_DriverInstanceData_t *instanceData, const ATMO_SPI_Peripheral_t *config);
	ATMO_SPI_Status_t (*GetConfiguration)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_Peripheral_t *config);
	ATMO_SPI_Status_t (*MasterSetDeviceConfiguration)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, const ATMO_SPI_Device_t *config);
	ATMO_SPI_Status_t (*MasterGetDeviceConfiguration)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, ATMO_SPI_Device_t *config);
	ATMO_SPI_Status_t (*MasterRegisterCallback)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_Master_Callback_t cbFunc);
	ATMO_SPI_Status_t (*SlaveRegisterCallback)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_Slave_Callback_t cbFunc);
	ATMO_SPI_Status_t (*MasterAssertSlaveSelect)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, bool ssActiveLow);
	ATMO_SPI_Status_t (*MasterDeAssertSlaveSelect)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, bool ssActiveLow);
	ATMO_SPI_Status_t (*MasterWrite)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms);
	ATMO_SPI_Status_t (*MasterWriteNonBlocking)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes);
	ATMO_SPI_Status_t (*MasterRead)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes, uint16_t timeout_ms);
	ATMO_SPI_Status_t (*MasterReadNonBlocking)(ATMO_DriverInstanceData_t *instanceData, ATMO_SPI_CS_t csPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes);
	ATMO_SPI_Status_t (*SlaveWaitForReceive)(ATMO_DriverInstanceData_t *instanceData, uint16_t xferSize);
};

/* Exported Function Prototypes -----------------------------------------------*/

/**
 * This routine will add an instance of a driver for SPI into the SPI driver handler.
 * 
 */
ATMO_Status_t ATMO_SPI_AddDriverInstance(const ATMO_SPI_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber);

/**
 * This routine initializes the SPI peripheral.
 *
 * @param instance :The peripheral instance to be configured.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_Init(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine de-initializes the SPI peripheral.
 *
 * @param peripheral :The pointer to the SPI peripheral.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_DeInit(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine configures the specified SPI peripheral instance. It is similar to the init
 * function except the peripheral configuration settings are passed in via the config structure.
 *
 * @param instance :The peripheral instance to be configured.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_SetConfiguration(ATMO_DriverInstanceHandle_t instance, const ATMO_SPI_Peripheral_t *config);

/**
 * This routine retrieves the current configuration of the specified SPI peripheral instance.
 *
 * @param instance :The peripheral instance to be read.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_GetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_Peripheral_t *config);

/** 
 * This routine configures the device-specific parameters for the specified SPI peripheral.
 *
 * @param instance :The peripheral instance to be configured.
 * @param csPin: Slave select pin
 * @param config :The pointer to a structure for storing the device configuration.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterSetDeviceConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, const ATMO_SPI_Device_t *config);

/**
 * This routine retrieves the current configuration of the specified SPI peripheral instance
 * device-specific parameters.
 *
 * @param instance :The peripheral instance to be read.
 * @param csPin: Slave select pin
 * @param config :The pointer to a structure for storing the device configuration.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterGetDeviceConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, ATMO_SPI_Device_t *config);

/**
 * This routine will register or unregister a user-defined callback function for the specified SPI peripheral instance.
 * Applicable only for SPI peripherals configured as master.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param cbFunc :The pointer to the callback function. Set to NULL to unregister a previously registered callback function.
 * @return status.
 */
ATMO_SPI_Status_t AIR_SPI_MasterRegisterCallback(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_Master_Callback_t cbFunc);

/**
 * This routine will register or unregister a user-defined callback function for the specified SPI peripheral instance.
 * Applicable only for SPI peripherals configured as slave.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param cbFunc :The pointer to the callback function. Set to NULL to unregister a previously registered callback function.
 * @return status.
 */
ATMO_SPI_Status_t AIR_SPI_SlaveRegisterCallback(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_Slave_Callback_t cbFunc);

/**
 * This routine drives a GPIO output pin to the active level.
 *
 * @param csPin :The pin used to select the slave device.
 * @param ssActiveLow :Pin driven low when true, high when false.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterAssertSlaveSelect(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, bool ssActiveLow);

/**
 * This routine drives a GPIO output pin to the inactive level.
 *
 * @param csPin :The pin used to select the slave device.
 * @param ssActiveLow :Pin driven high when true, low when false.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterDeAssertSlaveSelect(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, bool ssActiveLow);

/**
 * This routine will send one or more bytes to a slave device via the specified SPI peripheral. This is a blocking
 * operation and the function will not return until the SPI transfer completes or the timeout expires.
 *
 * @param instance :The peripheral instance used to write to the specified slave device.
 * @param csPin :The pin used to select the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the SPI peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the SPI peripheral. May be 0.
 * @param writeBytes :The pointer to a buffer holding the data bytes that will be sent by the SPI peripheral.
 * @param numBytes :The number of bytes in the writeBytes buffer that will be sent by the SPI peripheral.
 * @param timeout_ms :The number of milliseconds to wait before the SPI transfer is aborted.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterWrite(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms);

/**
* This routine will send one or more bytes to a slave device via the specified SPI peripheral. This function does not wait
* for the SPI transfer to complete before returning (i.e. returns immediately). It is up to the application to wait for the
* SPI peripheral to become available before performing another data transfer (write or read).
 *
 * @param instance :The peripheral instance used to write to the specified slave device.
 * @param csPin :The pin used to select the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the SPI peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the SPI peripheral. May be 0.
 * @param writeBytes :The pointer to a buffer holding the data bytes that will be sent by the SPI peripheral.
 * @param numBytes :The number of bytes in the writeBytes buffer that will be sent by the SPI peripheral.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterWriteNonBlocking(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes);

/**
 * This routine will read one or more bytes from a slave device via the specified SPI peripheral. If the command buffer has
 * a valid pointer (i.e. not NULL) and the number of command bytes is greater than zero then the values in the command buffer are
 * sent to the device during the read process, otherwise the output data is zero. This is a blocking operation and the function
 * will not return until the SPI transfer completes or the timeout expires.
 *
 * @param instance :The peripheral instance used to read from the specified slave device.
 * @param csPin :The pin used to select the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the SPI peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the SPI peripheral. May be 0.
 * @param readBytes :The pointer to a buffer for holding the bytes that will be received by the SPI peripheral.
 * @param numReadBytes :The number of bytes that are to be read and placed into the readBytes buffer.
 * @param timeout_ms :The number of milliseconds to wait before the SPI transfer is aborted.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterRead(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes, uint16_t timeout_ms);

/**
 * This routine will read one or more bytes from a slave device via the specified SPI peripheral. If the command buffer has
 * a valid pointer (i.e. not NULL) and the number of command bytes is greater than zero then the values in the command buffer are
 * sent to the device during the read process, otherwise the output data is zero. This function does not wait for the SPI transfer
 * to complete before returning (i.e. returns immediately). It is up to the application to wait for the SPI peripheral to become
 * available before performing another data transfer (write or read).
 *
 * @param instance :The peripheral instance used to read from the specified slave device.
 * @param csPin :The pin used to select the slave device.
 * @param cmdBytes :The pointer to a buffer holding the command that will be sent by the SPI peripheral. May be NULL.
 * @param numCmdBytes :The number of bytes in the cmdBytes buffer that will be sent by the SPI peripheral. May be 0.
 * @param readBytes :The pointer to a buffer for holding the bytes that will be received by the SPI peripheral.
 * @param numReadBytes :The number of bytes that are to be read and placed into the readBytes buffer.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_MasterReadNonBlocking(ATMO_DriverInstanceHandle_t instance, ATMO_SPI_CS_t csPin, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes);

/**
 * This routine starts the receive process for a SPI peripheral configured in Slave mode. It only needs to be called once.
 *
 * @param instance :The slave peripheral instance.
 * @return status.
 */
ATMO_SPI_Status_t ATMO_SPI_SlaveWaitForReceive(ATMO_DriverInstanceHandle_t instance, uint16_t xferSize);


#ifdef __cplusplus
}
#endif

#endif /* __ATMO_SPI__H */
/** @}*/