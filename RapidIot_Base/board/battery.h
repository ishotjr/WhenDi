/*
 * Copyright (c) 2018 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file battery.h
 * This is the header file for the battery sensor driver.
 */

#ifndef BATTERY_H_
#define BATTERY_H_

/*!
 * @addtogroup battery Battery sensor
 * This module provides the API to monitor the battery level.
 * @{
 */

#include <EmbeddedTypes.h>
#include <stdbool.h>

#define VBAT_MV_MIN 3600
#define VBAT_MV_MAX 4280
#define HYSTERESIS_MV ((VBAT_MV_MAX-VBAT_MV_MIN)/100) /* range in mV for 1% */

typedef struct _battery_fct_t
{
    void (*WaitMsec)(uint32_t tms); /*!< Wait function */
} battery_fct_t, *ptbattery_fct_t;


/*! @brief Status return codes. */
typedef enum _battery_status
{
    battery_status_success,
    battery_status_noinit,
    battery_status_error
} battery_status_t;

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/*!
 * @brief Initialize the battery sensor driver
 *
 * @param FCT Pointer to a structure of external functions or values
 */
void BatterySensor_Init_Driver(ptbattery_fct_t FCT);

/*!
 * @brief De-initialize the battery sensor driver
 *
 */
void BatterySensor_Deinit_Driver();

/*!
 * @brief Initialize the battery sensor hardware
 *
 * @return Status value (0 for success)
 */
battery_status_t BatterySensor_Init_Hw();

/*!
 * @brief Get current battery state
 *
 * @param batteryPercentLevel  Pointer to battery level percentage (0-100)
 * @param batteryChargingState Pointer to charging state (0 - not charging, 1 - charging)
 * @return Status value (0 for success)
 */
battery_status_t BatterySensor_GetState(uint8_t *batteryPercentLevel, uint8_t *batteryChargingState);

/*! @}*/

#endif /* BATTERY_H_ */
