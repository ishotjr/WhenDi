/**
 ******************************************************************************
 * @file    ATMO_MK64F_UART.h
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

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_UART_MK64_H
#define __ATMO_UART_MK64_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "uart.h"

#ifdef __cplusplus
	extern "C"{
#endif

	ATMO_Status_t ATMO_MK64F_UART_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber );

/**
 * This routine initializes the UART peripheral.
 *
 * @param instance :The peripheral instance to be configured.
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_Init(ATMO_DriverInstanceData_t *instance);

/**
 * This routine de-initializes the UART peripheral.
 *
 * @param peripheral :The pointer to the UART peripheral.
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_DeInit(ATMO_DriverInstanceData_t *instance);

/**
 * This routine configures the specified UART peripheral instance. It is similar to the init
 * function except the peripheral configuration settings are passed in via the config structure.
 *
 * @param instance :The peripheral instance to be configured.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_SetConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_UART_Peripheral_t *config);

/**
 * This routine retrieves the current configuration of the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be read.
 * @param config :The pointer to a structure for storing the peripheral configuration.
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_GetConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_UART_Peripheral_t *config);

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
ATMO_UART_Status_t ATMO_MK64F_UART_WriteBlocking(ATMO_DriverInstanceData_t *instance, const char *buffer, uint32_t length, uint32_t *numBytesSent, uint16_t timeoutMs);

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
ATMO_UART_Status_t ATMO_MK64F_UART_ReadBlocking(ATMO_DriverInstanceData_t *instance, char *buffer, uint32_t length, uint32_t *numBytesReceived, uint16_t timeoutMs);

/**
 * This routine will empty the contents of the UART Tx buffer.
 *
 * @param instance :The peripheral instance.
 * @return status
 */
ATMO_UART_Status_t ATMO_MK64F_UART_FlushTx(ATMO_DriverInstanceData_t *instance);

/**
 * This routine will empty the contents of the UART Rx buffer.
 *
 * @param instance :The peripheral instance.
 * @return status
 */
ATMO_UART_Status_t ATMO_MK64F_UART_FlushRx(ATMO_DriverInstanceData_t *instance);

/**
 * This routine will register or unregister a user-defined callback function for the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param abilityHandle :Ability handle to execute when a byte is received
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_RegisterRxAbilityHandle(ATMO_DriverInstanceData_t *instance, unsigned int abilityHandle);

/**
 * This routine will register or unregister a user-defined callback function for the specified UART peripheral instance.
 *
 * @param instance :The peripheral instance to be assigned the callback function.
 * @param cbFunc :Callback fn to execute when a byte is received
 * @return status.
 */
ATMO_UART_Status_t ATMO_MK64F_UART_RegisterRxCbFunc(ATMO_DriverInstanceData_t *instance, ATMO_Callback_t cbFunc);


#ifdef __cplusplus
}
#endif

#endif /* __ATMO_MK64F_UART__H */
