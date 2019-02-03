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

#include "tsl2572_internal.h"

/* TSL2572 Register definitions */
#define TSL2572_I2C_SLAVE_ADDRESS        0x39
#define TSL2572_DEVICE_ID                0x34    // TSL25721
#define TSL2572_REG_ENABLE               0x00
#define TSL2572_REG_ATIME                0x01
#define TSL2572_REG_WTIME                0x03
#define TSL2572_REG_AILTL                0x04
#define TSL2572_REG_AILTH                0x05
#define TSL2572_REG_AIHTL                0x06
#define TSL2572_REG_AIHTH                0x07
#define TSL2572_REG_PERS                 0x0C
#define TSL2572_REG_CONFIG               0x0D
#define TSL2572_REG_CONTROL              0x0F
#define TSL2572_REG_ID                   0x12
#define TSL2572_REG_STATUS               0x13
#define TSL2572_REG_C0DATA               0x14
#define TSL2572_REG_C0DATAH              0x15
#define TSL2572_REG_C1DATA               0x16
#define TSL2572_REG_C1DATAH              0x17

/* tsl2572 COMMAND register masks */
#define TSL2572_COMMAND_MSK              0x80
#define TSL2572_TYPE_REPEAT_MSK          0x00
#define TSL2572_TYPE_AUTO_INC_MSK        0x20
#define TSL2572_TYPE_SPL_FN_MSK          0x60
#define TSL2572_ADD_ALS_INT_CLR_MSK      0x06

/* tsl2572 CONTROL register masks */
#define TSL2572_AGAIN_MASK               0x03

/* tsl2572 CONFIG register masks */
#define TSL2572_AGL_MSK                  0x04
#define TSL2572_WLONG_MSK                0x02

/* tsl2572 PERS register masks */
#define TSL2572_APERS_MSK                0x0F

/* tsl2572 ENABLE register masks */
#define TSL2572_AIEN_MSK                 0x10
#define TSL2572_AEN_MSK                  0x02
#define TSL2572_WEN_MSK                  0x08
#define TSL2572_PON_MSK                  0x01

enum tsl2572_status {
    TSL2572_sensor_success = 0,
    TSL2572_sensor_I2C_error = 1,
    TSL2572_sensor_invalid_ID = 2,
    TSL2572_sensor_wrong_parameter = 3
};

static const struct tsl2x7x_settings tsl2x7x_default_settings = {
        0xDB,
        0,
        true,
        74,
        false,
        true,
        1,
        100,
        300,
        true,
        true,
        true,
        1
};

/***********************************************************************************/
/* variables                                                                       */
/***********************************************************************************/
static tsl2572_IoFunc_t sTSL2572_Func;
uint8_t gain_val = 0;

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/********************************************************************************/
/* TSL2572_Init_Driver                                                          */
/* wrap the low level function (I2C write, I2C read, WaitMsec)                  */
/* required by TSL2572 driver                                                   */
/* Note : this function does not initialize the HW                              */
/********************************************************************************/
void TSL2572_Init_Driver(tsl2572_IoFunc_t* pIoFunc){
    sTSL2572_Func = *pIoFunc;
}

/********************************************************************************/
/* TSL2572_Init_HW                                                              */
/* initialize the HW                                                            */
/********************************************************************************/
uint8_t TSL2572_Init_HW(void){
    uint8_t wBuf[2];
    uint8_t rBuf;

    /* check device ID */
    wBuf[0] = TSL2572_REG_ID | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    if (rBuf != TSL2572_DEVICE_ID){
        return TSL2572_sensor_invalid_ID;
    }

    /* Set ALS gain */
    if (TSL2572_sensor_success != TSL2572_SetALSGain(tsl2x7x_default_settings.als_gain,tsl2x7x_default_settings.als_gain_level)){
        return TSL2572_sensor_I2C_error;
    }

    /* Set ALS time */
    if (TSL2572_sensor_success != TSL2572_SetALSTime(tsl2x7x_default_settings.als_time)){
        return TSL2572_sensor_I2C_error;
    }

    /* configure the wait time */
    if (TSL2572_sensor_success != TSL2572_SetWaitTime(tsl2x7x_default_settings.wait_time,tsl2x7x_default_settings.wlong)){
        return TSL2572_sensor_I2C_error;
    }

    /* set the ALS interrupt thresholds */
    if (TSL2572_sensor_success != TSL2572_SetALSThresholds(tsl2x7x_default_settings.als_thresh_low,tsl2x7x_default_settings.als_thresh_high)){
        return TSL2572_sensor_I2C_error;
    }

    /* set the ALS interrupt persistence */
    if (TSL2572_sensor_success != TSL2572_SetALSPersistence(tsl2x7x_default_settings.persistence)){
        return TSL2572_sensor_I2C_error;
    }

    /* ALS Enable */
    if (TSL2572_sensor_success != TSL2572_Enable_ALS(tsl2x7x_default_settings.als_enable)){
        return TSL2572_sensor_I2C_error;
    }

    /* Wait Enable */
    if (TSL2572_sensor_success != TSL2572_Enable_Wait(tsl2x7x_default_settings.wait_enable)){
        return TSL2572_sensor_I2C_error;
    }

    /* Power ON  */
    if (TSL2572_sensor_success != TSL2572_Power_ON(tsl2x7x_default_settings.power_on)){
        return TSL2572_sensor_I2C_error;
    }

    /* clear the sensor IRQ status */
    if (TSL2572_sensor_success != TSL2572_ClearALSInterrupt()){
        return TSL2572_sensor_I2C_error;
    }

    if (TSL2572_sensor_success != TSL2572_EnableALSInterrupts(tsl2x7x_default_settings.interrupts_enable)){
        return TSL2572_sensor_I2C_error;
    }

    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_ReadAmbientLight                                                     */
/* sample CH0 and CH1 photo_diodes and compute the human eye response to        */
/* light intensity (in lux)                                                     */
/********************************************************************************/
uint8_t TSL2572_ReadAmbientLight(float *lux){
    uint8_t wBuf[2];
    uint8_t rBuf[4];
    int c0,c1;
    float lux1,lux2,cpl;

    /* Read CH0 low data register, CH0 high data register, CH1 low data register and CH1 high data register */
    wBuf[0] = TSL2572_REG_C0DATA | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf[0], sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    c0 = rBuf[1]<<8 | rBuf[0];
    c1 = rBuf[3]<<8 | rBuf[2];

    //see TSL2572 datasheet
    cpl = (2.73 * (256-tsl2x7x_default_settings.als_time)) * gain_val / (tsl2x7x_default_settings.glass_attenuation * 60);
    if (tsl2x7x_default_settings.als_gain_level){
        cpl /= 6;
    }
    lux1 = ((float)c0 - (1.87 * (float)c1)) / cpl;
    lux2 = ((0.63 * (float)c0) - (float)c1) / cpl;
    cpl = lux1 >= lux2 ? lux1 : lux2; //max(lux1, lux2);
    *lux = ((cpl >= 0.0) ? cpl : 0.0); //max(cpl, 0.0);
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetALSGain                                                           */
/* AGAIN = 0     ALS Gain value = 1 * gain                                      */
/* AGAIN = 1     ALS Gain value = 8 * gain                                      */
/* AGAIN = 2     ALS Gain value = 16 * gain                                     */
/* AGAIN = 3     ALS Gain value = 120 * gain                                    */
/* AGL = 0       AGAIN = 0 or 1 or 2 or 3     -> scaling by 1                   */
/* AGL = 1       AGAIN = 0 or 1               -> scaling by 0.16                */
/* Do not use AGL = 1 with AGAIN = 2 or 3                                       */
/********************************************************************************/
uint8_t TSL2572_SetALSGain(uint8_t AGAIN, bool AGL){
    uint8_t wBuf[2];
    uint8_t rBuf;

    if ((AGAIN > 1) && (AGL)){
        return TSL2572_sensor_wrong_parameter;
    }

    wBuf[0] = TSL2572_REG_CONTROL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    wBuf[1] = AGAIN & TSL2572_AGAIN_MASK;

    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }

    /* read CONFIG register */
    wBuf[0] = TSL2572_REG_CONFIG | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask AGL bit */
    if (AGL){
        wBuf[1] = rBuf | TSL2572_AGL_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_AGL_MSK);
    }

    /* write CONFIG register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }

    if ((AGAIN & TSL2572_AGAIN_MASK) == 0) gain_val = 1;
    else if ((AGAIN & TSL2572_AGAIN_MASK) == 1) gain_val = 8;
    else if ((AGAIN & TSL2572_AGAIN_MASK) == 2) gain_val = 16;
    else if ((AGAIN & TSL2572_AGAIN_MASK) == 3) gain_val = 120;

    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetALSTime                                                           */
/* set ALS Time = the ALS ADC integration time                                  */
/* ATIME = 0xFF     ALS integration cycles = 1,   time = 2.73ms                 */
/* ATIME = 0xF6     ALS integration cycles = 10,  time = 27.3ms                 */
/* ATIME = 0xDB     ALS integration cycles = 37,  time = 101ms                  */
/* ATIME = 0xC0     ALS integration cycles = 64,  time = 175ms                  */
/* ATIME = 0x00     ALS integration cycles = 256, time = 699ms                  */
/********************************************************************************/
uint8_t TSL2572_SetALSTime(uint8_t ATIME){
    uint8_t wBuf[2];

    wBuf[0] = TSL2572_REG_ATIME | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    wBuf[1] = ATIME;

    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetALSThresholds                                                        */
/* set ALS interrupt threshold low and threshold high                             */
/* the thresholds refer to C0 photo_diode only                                     */
/* (C1 is not used to trigger interrupts)                                        */
/********************************************************************************/
uint8_t TSL2572_SetALSThresholds(uint16_t ALS_interrupt_Low_Threshold, uint16_t ALS_interrupt_High_Threshold){
    uint8_t wBuf[5];

    wBuf[0] = TSL2572_REG_AILTL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    wBuf[1] = (uint8_t)(ALS_interrupt_Low_Threshold & 0x00FF);
    wBuf[2] = (uint8_t)((ALS_interrupt_Low_Threshold & 0xFF00) >> 8);
    wBuf[3] = (uint8_t)(ALS_interrupt_High_Threshold & 0x00FF);
    wBuf[4] = (uint8_t)((ALS_interrupt_High_Threshold & 0xFF00) >> 8);

    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetALSThresholds                                                        */
/* get ALS interrupt threshold low and threshold high                             */
/********************************************************************************/
uint8_t TSL2572_GetALSThresholds(uint16_t *ALS_interrupt_Low_Threshold, uint16_t *ALS_interrupt_High_Threshold){
    uint8_t wBuf[2];
    uint8_t rBuf[4];

    /* Read CH0 low data register, CH0 high data register, CH1 low data register and CH1 high data register */
    wBuf[0] = TSL2572_REG_AILTL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf[0], sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    *ALS_interrupt_Low_Threshold = (uint16_t) (rBuf[1]<<8 | rBuf[0]);
    *ALS_interrupt_High_Threshold = (uint16_t) (rBuf[3]<<8 | rBuf[2]);
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetALSPersistence                                                    */
/* set ALS interrupt persistence filter                                            */
/* APERS = 0     every ALS cycle generates an interrupt                           */
/* APERS = 1     1 value outside of threshold range generates an interrupt        */
/* APERS = 2     2 consecutive values out of range generates an interrupt         */
/* APERS = 3     3 consecutive values out of range generates an interrupt         */
/* APERS = 4     5 consecutive values out of range generates an interrupt         */
/* APERS = 5     10 consecutive values out of range generates an interrupt         */
/* APERS = 6     15 consecutive values out of range generates an interrupt         */
/* APERS = 7     20 consecutive values out of range generates an interrupt         */
/* APERS = 8     25 consecutive values out of range generates an interrupt         */
/* APERS = 9     30 consecutive values out of range generates an interrupt         */
/* APERS = 10    35 consecutive values out of range generates an interrupt         */
/* APERS = 11    40 consecutive values out of range generates an interrupt         */
/* APERS = 12    45 consecutive values out of range generates an interrupt         */
/* APERS = 13    50 consecutive values out of range generates an interrupt         */
/* APERS = 14    55 consecutive values out of range generates an interrupt         */
/* APERS = 15    60 consecutive values out of range generates an interrupt         */
/********************************************************************************/
uint8_t TSL2572_SetALSPersistence(uint8_t APERS){
    uint8_t wBuf[2];

    wBuf[0] = TSL2572_REG_PERS | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    wBuf[1] = APERS & TSL2572_APERS_MSK;

    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_SetWaitTime                                                             */
/* WTIME = 0xFF     Wait time = 2.73ms (WLONG = 0), 0.033s (WLONG = 1)           */
/* WTIME = 0xB6     Wait time = 202ms (WLONG = 0), 2.4s (WLONG = 1)               */
/* WTIME = 0x00     Wait time = 699ms (WLONG = 0), 8.4s (WLONG = 1)               */
/* The Wait time register should be configured before TSL2572_Enable_ALS(true)    */
/********************************************************************************/
uint8_t TSL2572_SetWaitTime(uint8_t WTIME, bool WLONG){
    uint8_t wBuf[2];
    uint8_t rBuf;

    wBuf[0] = TSL2572_REG_WTIME | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    wBuf[1] = WTIME;

    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }

    /* read CONFIG register */
    wBuf[0] = TSL2572_REG_CONFIG | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask WLONG bit */
    if (WLONG){
        wBuf[1] = rBuf | TSL2572_WLONG_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_WLONG_MSK);
    }

    /* write CONFIG register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

uint8_t TSL2572_EnableALSInterrupts(bool AIEN){
    uint8_t wBuf[2];
    uint8_t rBuf;

    /* read ENABLE register */
    wBuf[0] = TSL2572_REG_ENABLE | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask AIEN bit */
    if (AIEN){
        wBuf[1] = rBuf | TSL2572_AIEN_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_AIEN_MSK);
    }

    /* write ENABLE register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

uint8_t TSL2572_ClearALSInterrupt(void){
    uint8_t wBuf;

    wBuf = TSL2572_COMMAND_MSK | TSL2572_TYPE_SPL_FN_MSK | TSL2572_ADD_ALS_INT_CLR_MSK;
    if (sTSL2572_Func.I2C_Write(&wBuf, sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_Enable_ALS                                                             */
/********************************************************************************/
uint8_t TSL2572_Enable_ALS(bool AEN){
    uint8_t wBuf[2];
    uint8_t rBuf;

    /* read ENABLE register */
    wBuf[0] = TSL2572_REG_ENABLE | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask AEN bit */
    if (AEN){
        wBuf[1] = rBuf | TSL2572_AEN_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_AEN_MSK);
    }

    /* write ENABLE register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_Enable_Wait                                                             */
/********************************************************************************/
uint8_t TSL2572_Enable_Wait(bool WEN){
    uint8_t wBuf[2];
    uint8_t rBuf;

    /* read ENABLE register */
    wBuf[0] = TSL2572_REG_ENABLE | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask WEN bit */
    if (WEN){
        wBuf[1] = rBuf | TSL2572_WEN_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_WEN_MSK);
    }

    /* write ENABLE register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_Power_ON                                                             */
/********************************************************************************/
uint8_t TSL2572_Power_ON(bool PON){
    uint8_t wBuf[2];
    uint8_t rBuf;

    /* read ENABLE register */
    wBuf[0] = TSL2572_REG_ENABLE | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf[0], 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }

    /* mask PON bit */
    if (PON){
        wBuf[1] = rBuf | TSL2572_PON_MSK;
    }
    else {
        wBuf[1] = rBuf & ~(TSL2572_PON_MSK);
    }

    /* write ENABLE register */
    if (sTSL2572_Func.I2C_Write(&wBuf[0], sizeof (wBuf) )){
        return TSL2572_sensor_I2C_error;
    }
    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_ReadAllRegisters                                                        */
/* RegData contains 16 * uint8_t                                                */
/********************************************************************************/
uint8_t TSL2572_ReadAllRegisters(uint8_t *RegData){
    uint8_t wBuf;
    uint8_t rBuf;

    /* Read ENABLE register */
    wBuf = TSL2572_REG_ENABLE | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    *RegData = (uint8_t)(rBuf);

    /* Read ATIME register */
    wBuf = TSL2572_REG_ATIME | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read WTIME register */
    wBuf = TSL2572_REG_WTIME | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read AILTL register */
    wBuf = TSL2572_REG_AILTL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read AILTH register */
    wBuf = TSL2572_REG_AILTH | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read AIHTL register */
    wBuf = TSL2572_REG_AIHTL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read AIHTH register */
    wBuf = TSL2572_REG_AIHTH | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read PERS register */
    wBuf = TSL2572_REG_PERS | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read CONFIG register */
    wBuf = TSL2572_REG_CONFIG | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read CONTROL register */
    wBuf = TSL2572_REG_CONTROL | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read ID register */
    wBuf = TSL2572_REG_ID | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read STATUS register */
    wBuf = TSL2572_REG_STATUS | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read C0DATA register */
    wBuf = TSL2572_REG_C0DATA | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read C0DATAH register */
    wBuf = TSL2572_REG_C0DATAH | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read C1DATA register */
    wBuf = TSL2572_REG_C1DATA | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    /* Read C1DATAH register */
    wBuf = TSL2572_REG_C1DATAH | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    return TSL2572_sensor_success;
}

/********************************************************************************/
/* TSL2572_ReadCH0                                                                 */
/* RegData contains 2 * uint8_t                                                    */
/********************************************************************************/
uint8_t TSL2572_ReadCH0(uint8_t *RegData){
    uint8_t wBuf;
    uint8_t rBuf;

    /* Read C0DATA register */
    wBuf = TSL2572_REG_C0DATA | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    *RegData = (uint8_t)(rBuf);

    /* Read C0DATAH register */
    wBuf = TSL2572_REG_C0DATAH | TSL2572_COMMAND_MSK | TSL2572_TYPE_AUTO_INC_MSK;
    if (sTSL2572_Func.I2C_Read(&wBuf, 1, &rBuf, sizeof (rBuf))){
        return TSL2572_sensor_I2C_error;
    }
    RegData++;
    *RegData = (uint8_t)(rBuf);

    return TSL2572_sensor_success;
}
