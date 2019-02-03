/**
 ******************************************************************************
 * @file    uart.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - UART header file
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

/** \addtogroup UART
 *  @{
 */

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef ATMO_UART_H_
#define ATMO_UART_H_

#ifdef __cplusplus
	extern "C"{
#endif

#include "../atmo/core.h"
#include "regex.h"

/**
 * UART Baud Rate
 */
typedef enum
{
	ATMO_UART_BaudRate_1200    = 1200u,
	ATMO_UART_BaudRate_2400    = 2400u,
	ATMO_UART_BaudRate_4800    = 4800u,
	ATMO_UART_BaudRate_9600    = 9600u,
	ATMO_UART_BaudRate_19200   = 19200u,
	ATMO_UART_BaudRate_38400   = 38400u,
	ATMO_UART_BaudRate_57600   = 57600u,
	ATMO_UART_BaudRate_115200  = 115200u,
	ATMO_UART_BaudRate_230400  = 230400u,
	ATMO_UART_BaudRate_460800  = 460800u,
	ATMO_UART_BaudRate_921600  = 921600u,
} ATMO_UART_BaudRate_t;

/**
 * Number of data bits per frame
 */
typedef enum
{
	ATMO_UART_NumDataBits_5  = 0x05u,
	ATMO_UART_NumDataBits_6  = 0x06u,
	ATMO_UART_NumDataBits_7  = 0x07u,
	ATMO_UART_NumDataBits_8  = 0x08u,
	ATMO_UART_NumDataBits_9  = 0x09u,
} ATMO_UART_NumDataBits_t;

/**
 * Number of stop bits
 */
typedef enum
{
	ATMO_UART_NumStopBits_1  = 0x01u,
	ATMO_UART_NumStopBits_2  = 0x02u,
} ATMO_UART_NumStopBits_t;

/**
 * UART Parity
 */
typedef enum
{
	ATMO_UART_Parity_None   = 0x00u,
	ATMO_UART_Parity_Odd    = 0x01u,
	ATMO_UART_Parity_Even   = 0x02u,
	ATMO_UART_Parity_Mark   = 0x03u,
	ATMO_UART_Parity_Space  = 0x04u,
} ATMO_UART_Parity_t;

/**
 * Flow control mode
 */
typedef enum
{
	ATMO_UART_FlowControl_None      = 0x00u,
	ATMO_UART_FlowControl_RTS_CTS   = 0x01u,
	ATMO_UART_FlowControl_Xon_Xoff  = 0x02u,
} ATMO_UART_FlowControl_t;

/**
 * UART Driver Return Enumerations
 */
typedef enum
{
	ATMO_UART_Status_Success              = 0x00u,  /**< Operation was successful */
	ATMO_UART_Status_Fail                 = 0x01u,  /**< Operation failed */
	ATMO_UART_Status_Initialized          = 0x02u,  /**< Peripheral already initialized */
	ATMO_UART_Status_Invalid              = 0x03u,  /**< Invalid operation */
	ATMO_UART_Status_NotSupported         = 0x04u,  /**< Feature not supported by platform */
	ATMO_UART_Status_Unknown              = 0x05u,  /**< Some other status not defined */
	ATMO_UART_Status_Busy                 = 0x06u,  /**< Transfer in progress */
	ATMO_UART_Status_Timeout              = 0x07u,  /**< Transfer timed out */
	ATMO_UART_Status_Idle                 = 0x08u,  //
} ATMO_UART_Status_t;

/**
 * Uart Nonblocking Transmit Event
 */
typedef enum
{
	ATMO_UART_TxEvent_Unknown   = 0x0000u,  //
	ATMO_UART_TxEvent_Complete  = 0x0001u,  /**< Transfer Complete */
} ATMO_UART_TxEvent_t;

/**
 * Uart Nonblocking Receive Event
 */
typedef enum
{
	ATMO_UART_RxEvent_Unknown   = 0x0000u,  //
	ATMO_UART_RxEvent_Complete  = 0x0001u,  /**< Transfer Complete */
} ATMO_UART_RxEvent_t;

#define MAX_REGEX_SPLIT_LENGTH (64)

/* Exported Structures -------------------------------------------------------*/

/**
 * Uart Peripheral Configuration
 */
typedef struct
{
	ATMO_UART_BaudRate_t baudRate;
	ATMO_UART_NumDataBits_t numDataBits;
	ATMO_UART_NumStopBits_t numStopBits;
	ATMO_UART_Parity_t parity;
	ATMO_UART_FlowControl_t flowControl;
	bool isBinaryData;
	bool rxBuffer;
	bool rxTrim;
	char splitRegex[MAX_REGEX_SPLIT_LENGTH];
} ATMO_UART_Peripheral_t;

typedef struct {
	ATMO_UART_Peripheral_t config;
	bool initialized;
	ATMO_Callback_t cb;
	unsigned int  abilityHandle;
	bool abilityHandleRegistered;
} ATMO_UART_InternalConfig_t;

typedef struct ATMO_UART_DriverInstance_t ATMO_UART_DriverInstance_t;

struct ATMO_UART_DriverInstance_t{
	ATMO_UART_Status_t (*Init)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_UART_Status_t (*DeInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_UART_Status_t (*SetConfiguration)(ATMO_DriverInstanceData_t *instanceData, const ATMO_UART_Peripheral_t *config);
	ATMO_UART_Status_t (*GetConfiguration)(ATMO_DriverInstanceData_t *instanceData, ATMO_UART_Peripheral_t *config);
	ATMO_UART_Status_t (*WriteBlocking)(ATMO_DriverInstanceData_t *instanceData, const char *buffer, uint32_t length, uint32_t *numBytesSent, uint16_t timeoutMs);
	ATMO_UART_Status_t (*ReadBlocking)(ATMO_DriverInstanceData_t *instanceData, char *buffer, uint32_t length, uint32_t *numBytesReceived, uint16_t timeoutMs);
	ATMO_UART_Status_t (*FlushRx)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_UART_Status_t (*FlushTx)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_UART_Status_t (*RegisterRxAbilityHandle)(ATMO_DriverInstanceData_t *instanceData, unsigned int abilityHandle);
	ATMO_UART_Status_t (*RegisterRxCbFunc)(ATMO_DriverInstanceData_t *instanceData, ATMO_Callback_t cb);
};

/**
 * @param instance :
 * @return status.
 */
ATMO_Status_t ATMO_UART_AddDriverInstance(const ATMO_UART_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber);

/**
 * This routine initializes the UART peripheral.
 *
 * @param instance :The peripheral instance to be configured.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_Init(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine de-initializes the UART peripheral.
 *
 * @param peripheral :The pointer to the UART peripheral.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_DeInit(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine configures the specified UART peripheral instance. It is similar to the init
 * function except the peripheral configuration settings are passed in via the config structure.
 *
 * @param instance :The peripheral instance to be configured.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_SetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_UART_Peripheral_t *config);

/**
 * This routine retrieves the current configuration of the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be read.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_GetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_UART_Peripheral_t *config);

/**
 * This routine will send one or more bytes via the specified UART peripheral. This is a blocking function
 * and will not return until all data has been sent. In the even of a buffer overflow, the actual number of
 * bytes transferred will be returned in numBytesSent.
 *
 * @param instance :The peripheral instance to be written.
 * @param buffer :The pointer to a buffer holding the data bytes that will be sent by the UART peripheral.
 * @param length :The number of bytes in the buffer that will be sent by the UART peripheral.
 * @param numBytesSent :The pointer to a variable for storing the actual number of bytes sent. May be NULL.
 * @param timeoutMs :The number of milliseconds to wait before terminating the transfer.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_WriteBlocking(ATMO_DriverInstanceHandle_t instance, const char *buffer, uint32_t length, uint32_t *numBytesSent, uint16_t timeoutMs);

/**
 * This routine will read one or more bytes from the specified UART peripheral receive buffer. This is a
 * blocking function and will not return until the specified number of characters have been received or the
 * timeout expires.
 *
 * @param instance :The peripheral instance to be read.
 * @param buffer :The pointer to a buffer for storing the data read from the UART peripheral.
 * @param length :The number of bytes to read from the UART peripheral receive buffer.
 * @param numBytesReceived :The pointer to a variable for storing the actual number of bytes received. May be NULL.
 * @param timeoutMs :The number of milliseconds to wait before terminating the transfer.
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_ReadBlocking(ATMO_DriverInstanceHandle_t instance, char *buffer, uint32_t length, uint32_t *numBytesReceived, uint16_t timeoutMs);

/**
 * This routine will empty the contents of the UART Tx buffer.
 *
 * @param instance :The peripheral instance.
 * @return status
 */
ATMO_UART_Status_t ATMO_UART_FlushTx(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine will empty the contents of the UART Rx buffer.
 *
 * @param instance :The peripheral instance.
 * @return status
 */
ATMO_UART_Status_t ATMO_UART_FlushRx(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine will register or unregister a user-defined callback function for the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param abilityHandle :Ability handle to execute when a byte is received
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_RegisterRxAbilityHandle(ATMO_DriverInstanceHandle_t instance, unsigned int abilityHandle);

/**
 * This routine will register or unregister a user-defined callback function for the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param cbFunc :Callback fn to execute when a byte is received
 * @return status.
 */
ATMO_UART_Status_t ATMO_UART_RegisterRxCbFunc(ATMO_DriverInstanceHandle_t instance, ATMO_Callback_t cbFunc);

/**
 * Internal functions
 */

/**
 * Process received UART data
 * 
 * This function is called by subdrivers when they receive async UART data. It will process/buffer the data and execute abilities as needed.
 * 
 * @param[in] config - Driver internal configuration
 * @param[in] rxData - received data
 * @param[in] rxDataLen - number of bytes received
 * @param[in] buffer - buffer to store data
 * @param[in] bufferLen - total size of buffer
 * @param[in] currentBufferFill - number of bytes currently in buffer not including null terminator
 */
ATMO_UART_Status_t ATMO_UART_ProcessRxData(ATMO_UART_InternalConfig_t *config, uint8_t *rxData, unsigned int rxDataLen, uint8_t *buffer, unsigned int bufferLen, unsigned int *currentBufferFill);

#ifdef __cplusplus
}
#endif
#endif
/** @}*/