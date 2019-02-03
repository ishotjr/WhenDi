/**
 ******************************************************************************
 * @file		ds1307.h
 * @author Atmosphere IoT Corp.
 * @version
 * @date
 * @brief	 Atmosphere Driver - DS1307 header file
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *	 1. Redistributions of source code must retain the above copyright notice,
 *			this list of conditions and the following disclaimer.
 *	 2. Redistributions in binary form must reproduce the above copyright notice,
 *			this list of conditions and the following disclaimer in the documentation
 *			and/or other materials provided with the distribution.
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

#ifndef DS1307_H
#define DS1307_H

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"

//------------------------------------------------------------------------------
/**
 *  Defines, enumerations, and structure definitions
 */

#define DS1307_SLAVE_BASE_ADDR	0x68
#define DS1307_SECONDS_ADDR	0x00
#define DS1307_MINUTES_ADDR	0x01
#define DS1307_HOURS_ADDR	0x02
#define DS1307_DAY_ADDR		0x03
#define DS1307_DATE_ADDR	0x04
#define DS1307_MONTH_ADDR	0x05
#define DS1307_YEAR_ADDR	0x06
#define DS1307_CONTROL_ADDR	0x07
//This is the first address of general purpose ram on the device
#define DS1307_LOW_RAM_ADDR	0x08
//There is no more memory at this address and beyond on the chip
#define DS1307_HIGH_RAM_ADDR	0x40
#define DS1307_EPOCH_YEAR	2000

typedef enum
{
	DS1307_SQW_OUT_PIN_FREQUENCY_1Hz = 		0x10,
	DS1307_SQW_OUT_PIN_FREQUENCY_4_096kHz = 	0x11,
	DS1307_SQW_OUT_PIN_FREQUENCY_8_192kHz = 	0x12,
	DS1307_SQW_OUT_PIN_FREQUENCY_32_768kHz = 	0x13,
	DS1307_SQW_OUT_PIN_FREQUENCY_Low = 		0x0,
	DS1307_SQW_OUT_PIN_FREQUENCY_High = 		0x80
} DS1307_SQW_Setting;

// void DS1307_Init();

/**
* 
*
* @param
* @return
*/
void DS1307_WriteRegister(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char addr, unsigned char value);

/**
* 
*
* @param
* @return
*/
// void DS1307_WriteRegisters(unsigned char addr, unsigned char *buffer, unsigned char length);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_ReadRegister(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char addr);

/**
* 
*
* @param
* @return
*/
// unsigned char DS1307_ReadRegisters(unsigned char addr, unsigned char *buffer, unsigned char length);

/**
* 
*
* @param
* @return
*/
void DS1307_SetSQWOutput(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, DS1307_SQW_Setting value);

/**
* 
*
* @param
* @return
*/
void DS1307_StopClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
void DS1307_StartClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
void DS1307_ResetClock(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
void DS1307_SetSeconds(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetMinutes(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetHour(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetDay(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetDate(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetMonth(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetYear(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned int value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetControl(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
void DS1307_SetHours(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, unsigned char value);

/**
* 
*
* @param
* @return
*/
DS1307_SQW_Setting DS1307_GetSQWOutput(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetSeconds(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetMinutes(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetHour(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetDay(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetDate(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetMonth(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned int DS1307_GetYear(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
unsigned char DS1307_GetControl(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr);

/**
* 
*
* @param
* @return
*/
void DS1307_GetISO8601Time(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, char *isoRetBuffer);

/**
* 
*
* @param
* @return
*/
void DS1307_SetISO8601Time(ATMO_DriverInstanceHandle_t i2cInstance, uint8_t i2cAddr, char *value);

#endif  /* DS1307_H */
