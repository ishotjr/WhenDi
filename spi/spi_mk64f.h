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

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_MK64F_SPI__H
#define __ATMO_MK64F_SPI__H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../atmo/core.h"
#include "spi.h"

#ifdef __cplusplus
	extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

/* Exported Structures -------------------------------------------------------*/

ATMO_Status_t ATMO_MK64F_SPI_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber);

ATMO_SPI_Status_t ATMO_MK64F_SPI_Init( ATMO_DriverInstanceData_t *instance );

ATMO_SPI_Status_t ATMO_MK64F_SPI_DeInit( ATMO_DriverInstanceData_t *instance );

ATMO_SPI_Status_t ATMO_MK64F_SPI_SetConfiguration( ATMO_DriverInstanceData_t *instance, const ATMO_SPI_Peripheral_t* config );

ATMO_SPI_Status_t ATMO_MK64F_SPI_GetConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_SPI_Peripheral_t* config );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_SetDeviceConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const ATMO_SPI_Device_t* config );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_GetDeviceConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, ATMO_SPI_Device_t* config );

// Interrupts with SPI in Arduino API aren't really supported
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_RegisterCallback( ATMO_DriverInstanceData_t *instance, ATMO_SPI_Master_Callback_t cbFunc );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Slave_RegisterCallback( ATMO_DriverInstanceData_t *instance, ATMO_SPI_Slave_Callback_t cbFunc );

// Arduino API is higher level than this
ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_AssertSlaveSelect( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, bool ssActiveLow );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_DeAssertSlaveSelect( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, bool ssActiveLow );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_Write( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes, uint16_t numCmdBytes,
		const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_WriteNonBlocking( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes,
		uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_Read( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes, uint16_t numCmdBytes,
										uint8_t* readBytes, uint16_t numReadBytes, uint16_t timeout_ms );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Master_ReadNonBlocking( ATMO_DriverInstanceData_t *instance, ATMO_SPI_CS_t ssPin, const uint8_t* cmdBytes,
		uint16_t numCmdBytes, uint8_t* readBytes, uint16_t numReadBytes );

ATMO_SPI_Status_t ATMO_MK64F_SPI_Slave_WaitForReceive( ATMO_DriverInstanceData_t *instance, uint16_t xferSize );

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_MK64F_SPI__H */
