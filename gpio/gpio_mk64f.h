/**
 ******************************************************************************
 * @file    gpio_mk64f.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - Rapid IoT GPIO header file
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
#ifndef __ATMO_MK64F_GPIO__H
#define __ATMO_MK64F_GPIO__H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "gpio.h"


#ifdef __cplusplus
	extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

ATMO_Status_t ATMO_MK64F_GPIO_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber);

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_Init( ATMO_DriverInstanceData_t *instance );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_DeInit( ATMO_DriverInstanceData_t *instance );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_SetPinConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, const ATMO_GPIO_Config_t* config );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_GetPinConfiguration( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_Config_t* config );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_RegisterInterruptAbilityHandle(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_InterruptTrigger_t trigger, unsigned int abilityHandle);

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_RegisterInterruptCallback(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_InterruptTrigger_t trigger, ATMO_Callback_t cb);


ATMO_GPIO_Status_t ATMO_MK64F_GPIO_SetPinState( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_PinState_t state );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_GetPinState( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_GPIO_PinState_t* state );

ATMO_GPIO_PinState_t ATMO_MK64F_GPIO_Read( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin );

ATMO_GPIO_Status_t ATMO_MK64F_GPIO_Toggle( ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin );

void ATMO_MK64F_GPIO_EnableResetCombo(bool enabled);

void ATMO_MK64F_GPIO_EnableDisplayToggleCombo(bool enabled);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_MK64F_GPIO__H */
