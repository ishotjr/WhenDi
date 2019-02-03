/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fxos8700_internal.h"

#define EN_FFMT_INTERRUPT 1

/******************************************************************************
 * Code
 ******************************************************************************/

fxos8700_status_t FXOS8700_SetStandby(fxos8700_handle_t *fxos8700_handle)
{
	uint8_t tmp[1];

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp[0] & (uint8_t)~FXOS8700_ACTIVE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Read again to make sure we are in standby mode. */
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if ((tmp[0] & FXOS8700_ACTIVE_MASK) == FXOS8700_ACTIVE_MASK)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}

fxos8700_status_t FXOS8700_SetActive(fxos8700_handle_t *fxos8700_handle)
{
	uint8_t tmp[1];

    /* Set Active Mode*/ 
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp[0] | (uint8_t)FXOS8700_ACTIVE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

     /* Read Control register again to ensure we are in active mode */
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if ((tmp[0] & FXOS8700_ACTIVE_MASK) != FXOS8700_ACTIVE_MASK)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;

}

fxos8700_status_t FXOS8700_Init(fxos8700_handle_t *fxos8700_handle)
{
    uint8_t tmp[1] = {0};

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_WHO_AM_I_REG, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if (tmp[0] != FXOS8700_kFXOS_WHO_AM_I_Device_ID)
    {
        return FXOS8700_Status_Fail;
    }

    /* go to standby */
    FXOS8700_SetStandby(fxos8700_handle);

    /* Disable the FIFO */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_F_SETUP_REG, FXOS8700_F_MODE_DISABLED) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef LPSLEEP_HIRES
    /* enable auto-sleep, low power in sleep, high res in wake */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG2, FXOS8700_SLPE_MASK | FXOS8700_SMOD_LOW_POWER | FXOS8700_MOD_HIGH_RES) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
#else

    /* Accel OSR mode: High Resolution*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG2, FXOS8700_MOD_HIGH_RES) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#endif

    /* set MSR OSR=7 ,set FXOS8700 to hybrid mode (both accel and mag on), One-shot magnetic reset is enabled*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_M_CTRL_REG1, (FXOS8700_M_RST_MASK | FXOS8700_M_OSR_MASK | FXOS8700_M_HMS_MASK)) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Enable hyrid mode auto increment using M_CTRL_REG2 */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_M_CTRL_REG2, (FXOS8700_M_HYB_AUTOINC_MASK)) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef EN_AUTO_SLEEP
    /* set auto-sleep wait period to 5s (=5/0.64=~8) */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_ASLP_COUNT_REG, 8) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
#endif

#ifdef EN_FXOS_DATAREADY_INTERRUPT

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
     {
         return FXOS8700_Status_Fail;
     }

     if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] | FXOS8700_INT_EN_DRDY_MASK) != FXOS8700_Status_Success)
     {
         return FXOS8700_Status_Fail;
     }

     /* Set Data Ready Interrupt to route to INT1 */
     if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
     {
         return FXOS8700_Status_Fail;
     }
     if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|FXOS8700_INT_CFG_DRDY_MASK) != FXOS8700_Status_Success)
     {
         return FXOS8700_Status_Fail;
     }

#endif

    /* default set to 4g mode */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_XYZ_DATA_CFG_REG, FXOS8700_FULL_SCALE_4G) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }


    /* Setup the ODR for 200 Hz (Hybrid) and activate the accelerometer */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG1, (FXOS8700_HYB_DATA_RATE_200HZ | FXOS8700_ACTIVE_MASK)) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Read Control register again to ensure we are in active mode */
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG1, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if ((tmp[0] & FXOS8700_ACTIVE_MASK) != FXOS8700_ACTIVE_MASK)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}

fxos8700_status_t FXOS8700_ReadSensorData(fxos8700_handle_t *fxos8700_handle, fxos8700_data_t *sensorData)
{
    fxos8700_status_t status = FXOS8700_Status_Success;
    uint8_t tmp_buff[6] = {0};
    uint8_t i = 0;

    if (!FXOS8700_ReadReg(fxos8700_handle, FXOS8700_OUT_X_MSB_REG, tmp_buff, 6) == FXOS8700_Status_Success)
    {
        status = FXOS8700_Status_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i] = tmp_buff[i];
    }

    if (!FXOS8700_ReadReg(fxos8700_handle, FXOS8700_M_OUT_X_MSB_REG, tmp_buff, 6) == FXOS8700_Status_Success)
    {
        status = FXOS8700_Status_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i + 6] = tmp_buff[i];
    }

    return status;
}

fxos8700_status_t FXOS8700_ReadReg(fxos8700_handle_t *handle, uint8_t reg, uint8_t *val, uint8_t bytesNumber)
{
    /* Configure I2C xfer */
    return handle->I2C_Read(&reg, 1, val, bytesNumber) == 0 ? FXOS8700_Status_Success : FXOS8700_Status_Fail;
}

fxos8700_status_t FXOS8700_WriteReg(fxos8700_handle_t *handle, uint8_t reg, uint8_t val)
{
    uint8_t buff[2];
    buff[0] = reg;
    buff[1] = val;
    return handle->I2C_Write(buff, 2) == 0 ? FXOS8700_Status_Success : FXOS8700_Status_Fail;
}

fxos8700_status_t FXOS8700_MotionDetect_Init(fxos8700_handle_t *fxos8700_handle)
{

    uint8_t tmp[1];

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* enable FFMT for motion detect for X and Y axes, latch enable.
    If Z axis were included in this motion detect setup and the threshold is less than 1G, it will cause motion detect to be triggered.
    (assuming Z-axis is experiencing gravity)*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FF_MT_CFG_REG, (FXOS8700_XEFE_MASK | FXOS8700_YEFE_MASK | FXOS8700_OAE_MASK|FXOS8700_ELE_MASK)) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set threshold to about 0.5g (0.5g/0.063 g/count = 8)*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FT_MT_THS_REG, 0x08) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set debounce to zero */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FF_MT_COUNT_REG, 0x00) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef EN_FFMT_INTERRUPT
    /*Enable Motion Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] | FXOS8700_INT_EN_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Default behavior for CTRL_REG5 is that FFMT Interrupt will go to INT2 pin. */
    /* To route FFMT interrupt to INT1 pin define below */
    #ifdef EN_FFMT_INT1_PIN

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|FXOS8700_INT_CFG_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    #endif
#endif 

    /* Set Active Mode*/ 
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    
    return FXOS8700_Status_Success;

}

fxos8700_status_t FXOS8700_MotionDetectFreefall_DeInit(fxos8700_handle_t *fxos8700_handle)
{
    uint8_t tmp[1];
    
    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Reset Freefall/Motion Config Register to 0. Disables FF and Motion Detection*/ 
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FF_MT_CFG_REG, 0x0 != FXOS8700_Status_Success))
    {
        return FXOS8700_Status_Fail;
    }


#ifdef EN_FFMT_INTERRUPT
    /*Disable Motion Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] & (uint8_t)~FXOS8700_INT_EN_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    #ifdef EN_FFMT_INT1_PIN
    /* Reset to default behavior */
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|(uint8_t)~FXOS8700_INT_CFG_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    #endif 

#endif 

    /* Set Active Mode*/ 
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}



fxos8700_status_t FXOS8700_FreefallDetect_Init(fxos8700_handle_t *fxos8700_handle)
{
    uint8_t tmp[1] = {0};

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Freefall detect is triggered when combined X,Y,Z magnitude is less than the threshold. Latch also enabled. */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FF_MT_CFG_REG, FXOS8700_ZEFE_MASK|FXOS8700_YEFE_MASK|FXOS8700_XEFE_MASK|FXOS8700_ELE_MASK)  != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set threshold to about 0.25g (0.25g/0.063 g/count = 4)*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FT_MT_THS_REG, 0x04) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set debounce to zero */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_FF_MT_COUNT_REG, 0x00) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef EN_FFMT_INTERRUPT
    /*Enable Motion Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] | FXOS8700_INT_EN_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Default behavior for CTRL_REG5 is that FFMT Interrupt will go to INT2 pin. */
    /* To route FFMT interrupt to INT1 pin define below */
    #ifdef EN_FFMT_INT1_PIN

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|FXOS8700_INT_CFG_FF_MT_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    #endif

#endif 

    /* Set Active Mode*/ 
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}



fxos8700_status_t FXOS8700_TapDetect_Init(fxos8700_handle_t *fxos8700_handle)
{
	uint8_t tmp[1];

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* enable PULSE_CFG for only single tap  */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_CFG_REG, FXOS8700_PELE_MASK|FXOS8700_ZSPEFE_MASK|FXOS8700_YSPEFE_MASK|FXOS8700_XSPEFE_MASK ) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* enable PULSE_CFG for only double tap  */
    #if DOUBLE_TAP && !SINGLE_TAP
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_CFG_REG, FXOS8700_PELE_MASK|FXOS8700_ZDPEFE_MASK|FXOS8700_YDPEFE_MASK|FXOS8700_XDPEFE_MASK ) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    #endif 

	#if DOUBLE_TAP && SINGLE_TAP
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_CFG_REG, FXOS8700_PELE_MASK|FXOS8700_ZDPEFE_MASK|FXOS8700_ZSPEFE_MASK|FXOS8700_YDPEFE_MASK|FXOS8700_YSPEFE_MASK|FXOS8700_XDPEFE_MASK|FXOS8700_XSPEFE_MASK ) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
	#endif

    /* set PULSE_THSX to about 2g */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_THSX_REG, 0x20) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set PULSE_THSY to about 2g */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_THSY_REG, 0x20) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set PULSE_THSZ to about 3g */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_THSZ_REG, 0x0C) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set the Pulse Time Window (PULSE_TMLT) to 6 counts */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_TMLT_REG, 0x06) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set the Pulse Latency Timer to 40 counts */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_LTCY_REG, 0x28) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set the Second Pulse Time Window to 15 counts */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_WIND_REG, 0x0F) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef EN_TAP_INTERRUPT

    /*Enable Tap/Pulse Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] | FXOS8700_INT_EN_PULSE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Default behavior for CTRL_REG5 is that Pulse Interrupt will go to INT2 pin. */
    /* To route Pulse interrupt to INT1 pin define below */
    #ifdef EN_PULSE_INT1_PIN

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|FXOS8700_INT_CFG_PULSE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    #endif
#endif

    /* Set Active Mode*/ 
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
   
    return FXOS8700_Status_Success;
}



fxos8700_status_t FXOS8700_TapDetect_DeInit(fxos8700_handle_t *fxos8700_handle)
{

	uint8_t tmp[1];

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* enable PULSE_CFG for only single tap  */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_PULSE_CFG_REG, 0x0 ) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }


#ifdef EN_TAP_INTERRUPT

    /*Disable Pulse Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] & (uint8_t)~FXOS8700_INT_EN_PULSE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Default behavior for CTRL_REG5 is that Pulse Interrupt will go to INT2 pin. */
    /* To route Pulse interrupt to INT1 pin define below */
    #ifdef EN_PULSE_INT1_PIN

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|(uint8_t)~FXOS8700_INT_CFG_PULSE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    
    #endif
#endif 

    /* Set Active Mode*/ 
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
   
    return FXOS8700_Status_Success;
}


fxos8700_status_t FXOS8700_TransientDetect_Init(fxos8700_handle_t *fxos8700_handle)
{
    uint8_t tmp[1] = {0};

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* enable TRANSIENT CFG REG for Transient detect for X,Y,Z axis.latch enabled */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_TRANSIENT_CFG_REG, FXOS8700_TELE_MASK |FXOS8700_ZTEFE_MASK|FXOS8700_YTEFE_MASK|FXOS8700_XTEFE_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set threshold to about 0.25g (0.25g/0.063 g/count = 4)*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_TRANSIENT_THS_REG, 0x04) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set debounce to zero */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_TRANSIENT_COUNT_REG, 0x00) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* set High-Pass Filter to 2Hz (Sampling Rate = 200Hz, Oversample Mode = High Res) Check APP note AN4071 */
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_HP_FILTER_CUTOFF_REG, 0x03) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /*Enable Transient Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] | FXOS8700_INT_EN_TRANS_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Default behavior for CTRL_REG5 is that FFMT Interrupt will go to INT2 pin. */
    /* To route TRANS interrupt to INT1 pin define below */
#ifdef EN_TRANS_INT1_PIN

    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|FXOS8700_INT_CFG_TRANS_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
#endif

    /* Set Active Mode*/
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}

fxos8700_status_t FXOS8700_TransientDetect_DeInit(fxos8700_handle_t *fxos8700_handle)
{

    uint8_t tmp[1];

    /* go to standby */
    if (FXOS8700_SetStandby(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    /* Reset Transient Config Register to 0. Disables Transient Detection*/
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_TRANSIENT_CFG_REG, 0x0 != FXOS8700_Status_Success))
    {
        return FXOS8700_Status_Fail;
    }

    /*Disable Motion Detect Interrupt*/
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG4, tmp[0] & (uint8_t)~FXOS8700_INT_EN_TRANS_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

#ifdef EN_TRANS_INT1_PIN
    /* Reset to default behavior */
    if(FXOS8700_ReadReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp, 1) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
    if(FXOS8700_WriteReg(fxos8700_handle, FXOS8700_CTRL_REG5, tmp[0]|(uint8_t)~FXOS8700_INT_CFG_TRANS_MASK) != FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }
#endif

    /* Set Active Mode*/
    if (FXOS8700_SetActive(fxos8700_handle)!= FXOS8700_Status_Success)
    {
        return FXOS8700_Status_Fail;
    }

    return FXOS8700_Status_Success;
}
