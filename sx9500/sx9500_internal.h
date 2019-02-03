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
 * @file sx9500.h
 * This is the header file for the SX9500 touch controller driver.
 */

#ifndef SX9500_INTERNAL_H_
#define SX9500_INTERNAL_H_

/*!
 * @addtogroup sx9500 SX9500 touch controller API
 * @{
 */

#include "../app_src/atmosphere_platform.h"


#define SX9500_I2C_ADDRESS                          0x28

#define SX9500_REG_IRQSRC                           0x00
#define SX9500_REG_STAT                             0x01
#define SX9500_REG_IRQMSK                           0x03
#define SX9500_REG_PROXCTRL0                        0x06
#define SX9500_REG_PROXCTRL1                        0x07
#define SX9500_REG_PROXCTRL2                        0x08
#define SX9500_REG_PROXCTRL3                        0x09
#define SX9500_REG_PROXCTRL4                        0x0A
#define SX9500_REG_PROXCTRL5                        0x0B
#define SX9500_REG_PROXCTRL6                        0x0C
#define SX9500_REG_PROXCTRL7                        0x0D
#define SX9500_REG_PROXCTRL8                        0x0E
#define SX9500_REG_SENSORSEL                        0x20
#define SX9500_REG_USEMSB                           0x21
#define SX9500_REG_USELSB                           0x22
#define SX9500_REG_AVGMSB                           0x23
#define SX9500_REG_AVGLSB                           0x24
#define SX9500_REG_DIFFMSB                          0x25
#define SX9500_REG_DIFFLSB                          0x26
#define SX9500_REG_OFFSETMSB                        0x27
#define SX9500_REG_OFFSETLSB                        0x28
#define SX9500_REG_RESET                            0x7F
 
#define SX9500_RESET_CMD                            0xDE
 
/*! @brief Status return codes. */
typedef enum
{
    SX9500_SUCCESS,
    SX9500_I2C_ERROR,
    SX9500_INTERNAL_ERROR,
	SX9500_NOINIT_ERROR
} SX9500_status;

typedef union {
    struct {    // sx9500 register 0x00
        uint8_t txen_stat   : 1;    // 0
        uint8_t reserved    : 2;    // 1,2
        uint8_t conv_done   : 1;    // 3
        uint8_t comp_done   : 1;    // 4
        uint8_t far         : 1;    // 5
        uint8_t close       : 1;    // 6
        uint8_t reset       : 1;    // 7
    } bits;
    uint8_t octet;
} RegIrqSrc_t;
 
typedef union {
    struct {    // sx9500 register 0x01
        uint8_t compstat         : 4;    // 0,1,2,3
        uint8_t proxstat0        : 1;    // 4
        uint8_t proxstat1        : 1;    // 5
        uint8_t proxstat2        : 1;    // 6
        uint8_t proxstat3        : 1;    // 7
   } bits;
    uint8_t octet;
} RegStat_t;
 
typedef union {
    struct {    // sx9500 register 0x06
        uint8_t sensor_en   : 4;    // 0,1,2,3
        uint8_t scan_period : 3;    // 4,5,6
        uint8_t reserved    : 1;    // 7
    } bits;
    uint8_t octet;
} RegProxCtrl0_t;
 
typedef union {
    struct {    // sx9500 register 0x09
        uint8_t raw_filt    : 2;    // 0,1
        uint8_t reserved    : 2;    // 2,3
        uint8_t doze_period : 2;    // 4,5
        uint8_t doze_en     : 1;    // 6
        uint8_t res7        : 1;    // 7
    } bits;
    uint8_t octet;
} RegProxCtrl3_t;

/*! @brief Structure of external functions or values. */
typedef struct _sx9500_fct_t
{
	/** Read hardware function */
    uint8_t (*I2C_Read)(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize);
    /** Write hardware register, 8bit aligned function */
    uint8_t (*I2C_Write)(uint8_t *writeBuf, uint32_t writeSize);
    /** Wait function **/
    void (*WaitMs)(uint32_t tms);
} sx9500_fct_t, *ptsx9500_fct_t;


/*****************************************************************************
 * Public functions
 ****************************************************************************/

/*!
 * @brief Initialize SX9500 driver.
 *
 * @param FCT Pointer to a structure with external functions
 */
void SX9500_Init_Driver(ptsx9500_fct_t  FCT);

/*!
 * @brief De-initialize SX9500 driver.
 *
 */
void SX9500_Deinit_Driver();

/*!
 * @brief Initialize SX9500 hardware.
 *
 * @return Status value (0 for success)
 */
SX9500_status SX9500_Init_Hw();

/*!
 * @brief Get information for selected sensor.
 *
 * @param  CSn Selected sensor (0-3)
 * @param  buf Pointer to an array with PROXUSEFUL/PROXAVG/PROXDIFF values
 * @return Status value (0 for success)
 */
SX9500_status SX9500_GetInfo_sensor(char CSn, uint8_t* buf);

/*!
 * @brief Get power mode.
 *
 * @return Power mode: active (true/1) or low power (false/0)
 */
bool SX9500_get_active();

/*!
 * @brief Set power mode.
 *
 * @param  bool Power mode: active (true/1) or low power (false/0)
 * @return Status value (0 for success)
 */
SX9500_status SX9500_set_active(bool);

/*!
 * @brief Function returning the triggered capacitive sensing interface.
 * @note Returns 0xFF if no interface has been triggered.
 *
 * @param  CSi Pointer to sensing interface index
 * @return Status value (0 for success)
 */
SX9500_status SX9500_CSi_Detected(uint8_t* CSi);

/*!
 * @brief Read IRQ Source
 * @note  Used to clear interrupts on SX9500
 *
 * @param irqReg Pointer to store value from IRQ register
 *
 * @return Status value (0 for success)
 *
 */
SX9500_status SX9500_Read_Irq(uint8_t* irqReg);

/*!
 * @brief Read Proximity Sensors
 *
 * @param data Pointer to store data from proximity sensor data
 *
 * @return Status value (0 for success)
 *
 */
SX9500_status SX9500_Read_Proximity_Sensors(uint8_t* data);

/*! @}*/

#endif
