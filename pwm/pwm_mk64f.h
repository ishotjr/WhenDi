/**
 ******************************************************************************
 * @file    atmo_pwm.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - PWM header file
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
#ifndef __ATMO_MK64_PWM__H
#define __ATMO_MK64_PWM__H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "pwm.h"
#include "../gpio/gpio.h"

#ifdef __cplusplus
	extern "C"{
#endif


/* Exported Function Prototypes -----------------------------------------------*/

/**
 * This routine registers the MK64F PWM driver to the main Atmosphere PWM driver
 */
ATMO_Status_t ATMO_MK64F_PWM_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber );

/**
 * Initialize PWM driver
 *
 * @param instance: The peripheral instance to be configured
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_Init(ATMO_DriverInstanceData_t *instance);


/**
 * This routine configures the specified PWM peripheral pin.
 *
 * @param instance :The peripheral instance to be configured.
 * @param pin :The pin to be configured.
 * @param config :The pointer to a structure for storing the channel configuration.
 * @return status.
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_SetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_PWM_Config_t *config);

/**
 * This routine retrieves the current configuration of the specified PWM peripheral pin.
 *
 * @param instance :The peripheral instance to be read.
 * @param pin :The pin to be read.
 * @param config :The pointer to a structure for storing the channel configuration.
 * @return status.
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_GetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_PWM_Config_t *config);

/**
 * This routine de-initializes the specified PWM peripheral instance.
 *
 * @param instance :The peripheral instance to be de-initialized.
 * @return status.
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_DeInit(ATMO_DriverInstanceData_t *instance);

/**
 * This routine enables the specified PWM peripheral.
 *
 * @param instance :The peripheral instance to be enable.
 * @param pin: The pin to enable
 * @return status.
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_Enable(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin);

/**
 * This routine disables the specified PWM peripheral.
 *
 * NOTE: For the K64, this just enables/disables the clock to the FTM peripheral that connects to the supplied pin.
 * If two pins use the same FTM peripheral, both will be disabled if you call this function with one of them.
 *
 * @param instance :The peripheral instance to be disabled.
 * @param pin: Pin to disable.
 * @return status.
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_Disable(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin);

//=============================
// Advanced MK64F Functionality
//=============================

/**
 * @brief Set configuration for specific peripheral and channe;
 * 
 * @param instance 
 * @param pwmPeriphNum 
 * @param channel 
 * @param config 
 * @return ATMO_PWM_Status_t 
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_SetChannelConfiguration(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum, uint32_t channel, ATMO_PWM_Config_t *config);

/**
 * @brief Retrieve configuration for specific peripheral and channel
 * 
 * @param instance 
 * @param pwmPeriphNum 
 * @param channel 
 * @param config 
 * @return ATMO_PWM_Status_t 
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_GetChannelConfiguration(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum, uint32_t channel, ATMO_PWM_Config_t *config);


/**
 * @brief Enable clock on specific peripheraal
 * 
 * @param instance 
 * @param pwmPeriphNum 
 * @return ATMO_PWM_Status_t 
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_PeripheralEnable(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum);

/**
 * @brief Disable clock on specific peripheral
 * 
 * @param instance 
 * @param pwmPeriphNum 
 * @return ATMO_PWM_Staus_t 
 */
ATMO_PWM_Status_t ATMO_MK64F_PWM_PeripheralDisable(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_PWM__H */
