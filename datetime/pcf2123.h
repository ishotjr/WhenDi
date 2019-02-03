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
 * @file
 * This is the header file for the real time clock sensor PCF2123 driver.
 */

#ifndef PCF2123_H_
#define PCF2123_H_

/*!
 * @addtogroup pcf2123 PCF2123 real time clock sensor API
 * @{
 */

#include <stdbool.h>
#include <EmbeddedTypes.h>

typedef struct _pcf2123_IoFunc_t
{
  uint8_t   (*SPI_Read)(uint8_t *writeBuf, uint8_t *readBuf, uint32_t readSize);
  uint8_t   (*SPI_Write)(uint8_t *writeBuf, uint32_t writeSize);
  void      (*WaitMsec)(uint32_t millisec);
} pcf2123_IoFunc_t, *ppcf2123_IoFunc_t;

/*****************************************************************************
 * Types/enumerations/variables
 ****************************************************************************/

#define PCF2123_NB_REG           (16)    // Nb of registers

/* READ/WRITE ADDRESS BITS */
#define PCF2123_WRITE            (1<<4)
#define PCF2123_READ             ((1<<7) | (1<<4))
#define PCF2123_ADDRESS_MASK     (0x0f)

typedef enum
{
  PCF2123_SUCCESS,
  PCF2123_I2C_ERROR,
  PCF2123_INIT_ERROR,
  PCF2123_TIMEOUT
} pcf2123_status_t;

//***************************************
// Structure that contains PCF settings.
//***************************************

typedef enum {
  PCF2123_MODE_24HOURS    = 0,
  PCF2123_MODE_12HOURS    = (1<<2),
} modePCF_t;

typedef enum {
    Sunday        = 0,
    Monday        = 1,
    Tuesday       = 2,
    Wednesday     = 3,
    Thursday      = 4,
    Friday        = 5,
    Saturday      = 6,
} weekdaysPCF_t;

typedef enum {
    January       = 0x1,
    February      = 0x2,
    March         = 0x3,
    April         = 0x4,
    May           = 0x5,
    June          = 0x6,
    July          = 0x7,
    August        = 0x8,
    September     = 0x9,
    October       = 0x10,
    November      = 0x11,
    December      = 0x12,
} monthsPCF_t;

typedef enum {
    clkoutFreq_32768   = 0,        // Clock output frequency = 32768Hz
    clkoutfreq_16384   = 1,
    clkoutFreq_8192    = 2,
    clkoutFreq_4096    = 3,
    clkoutFreq_2408    = 4,
    clkoutFreq_1024    = 5,
    clkoutFreq_1       = 6,        // Clock output frequency = 1Hz
    clkout_High_Z      = 7,        // Clock output is High Z
} clockOutputPCF_t;

typedef enum
{
    CD_TMR_4096KHZ        = 0,    /* 4096 KHz countdown timer */
    CD_TMR_64HZ           = 1,    /* 64 Hz countdown timer */
    CD_TMR_1HZ            = 2,    /* 1 Hz countdown timer */
    CD_TMR_60th_HZ        = 3,    /* 60th Hz countdown timer */
} cdmodePCF_t;

typedef struct {
    modePCF_t           mode12_24;          // 12H or 24H mode
    bool                Softreset;          // Force software reset => Date, time and interrupts need to be set after
    uint8_t             seconds;            // seconds in BCD
    uint8_t             minutes;            // minutes in BCD
    uint8_t             hours;              // hours in BCD.
                                            /* 24H mode: bit 5,4 used for first digit
                                               12H mode: bit 5=AM/PM, bit 4 used for first digit */
    uint8_t             days;               // day in BCD
    weekdaysPCF_t       weekdays;           //
    monthsPCF_t         months;             // actual month coded in BCD
    uint8_t             years;              // 2 digits
    clockOutputPCF_t    clockOutputFreq;    // Clock output pin frequency from 1H to 32768Hz or Disabled = high Z
    bool                MinInterrupt;       // Minute interrupt
    bool                SecInterrupt;       // Second interrupt
    bool                PulseInterrupt;     // Interrupt generate a pulse when true. Follows timer flag when false
} settingsPCF_t;


/*****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/*!
 * @brief   Initialize PCF2123 driver.
 * @param   pIoFunc  Pointer to a structure of external functions or values
 */
void PCF2123_Init_Driver(pcf2123_IoFunc_t* pIoFunc);

/*!
 * @brief   De-initialize PCF2123 driver.
 */
void PCF2123_Denit_Driver();

/*!
 * @brief   Initialize PCF2123 hardware.
 * @param   pcfSettings  Pointer to a configuration structure
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_Init_Hw(const settingsPCF_t* pcfSettings, bool initTime);

/*!
 * @brief Read all chip registers.
 * @param   pcfReg  Pointer to register dump.
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_Dump(uint8_t *pcfReg);

/*!
 * @brief Set Offset register
 * The offset is made once every two hours in the normal mode, or once every hour in the
 * course mode. Each LSB will introduce an offset of 2.17 ppm for normal mode and
 * 4.34 ppm for course mode. The values of 2.17 ppm and 4.34 ppm are based on a nominal
 * 32.768 kHz clock. The offset value is coded in two’s complement giving a range of +63 LSB to -64 LSB.
 * Normal mode = 0, Course mode = 1
 * @param   mode (normal = 0, course = 1)
 * @param   Offset (+63..-64, two's complement)
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetOffset(bool mode, uint8_t Offset);

/*!
 * @brief Read Date and Time
 * @param   pcfDateTime  Pointer to the updated structure
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_GetDateTime(settingsPCF_t* pcfDateTime);

/*!
 * @brief Set Date and Time
 * @param   pcfDateTime  Pointer to the reference structure
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetDateTime(settingsPCF_t* pcfDateTime);

/*!
 * @brief Set Clock output pin frequency from 1Hz up to 32768Hz, or disabled = High Z
 * @param   pcfClockOutputFreq  Pointer to the clock output frequency value
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetClockOutputFreq(clockOutputPCF_t pcfClockOutputFreq);

/*!
 * @brief Set Minute and second interrupt
 * The minute and second interrupts (bits MI and SI) are pre-defined timers for generating
 * periodic interrupts. The timers can be enabled independently from one another. However,
 * a minute interrupt enabled on top of a second interrupt will not be distinguishable since it
 * will occur at the same time
 * @param   Minute_Int Enable the minute interrupt
 * @param   Second_Int Enable the second interrupt
 * @param   Pulse_Int  Enable the pulse interrupt
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetMinSecInterrupt(bool Minute_Int, bool Second_Int, bool Pulse_Int);

/*!
 * @brief Set Alarm interrupt
 * Interrupt generated when alarm flag set.
 * @param   Alarm_Int Enable the alarm interrupt
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetAlarmInterrupt(bool Alarm_Int);

/*!
 * @brief Set Countdown interrupt
 * Interrupt generated when countdown flag set.
 * @param   Countdown_Int Enable the countdown interrupt
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetCountdownInterrupt(bool Countdown_Int);

/*!
 * @brief Set Countdown timer source clock
 * @param   CountDownEn Enable the countdown timer source clock
 * @param   CountDownMode Countdown mode (CD_TMR_4096KHZ, CD_TMR_64HZ, CD_TMR_1HZ, CD_TMR_60th_HZ)
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetCountdownMode(bool CountDownEn, cdmodePCF_t CountDownMode);

/*!
 * @brief Set Countdown timer value
 * @param   CDT_Value Countdown timer value
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetCountdownTimer(uint8_t CDT_Value);

/*!
 * @brief Set Minute alarm
 * Minute alarm information coded in *** BCD format ***
 * When the register Minute_alarm is loaded with a valid minute and it's corresponding alarm bit is logic 0
 * that information will be compared with the current minute.
 * @param   AlarmEn Enable alarm
 * @param   Minute  Number of minutes
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetMinuteAlarm(bool AlarmEn, uint8_t Minute);

/*!
 * @brief Set Hour alarm
 * Hour alarm information coded in *** BCD format ***.
 * When the register Hour is loaded with a valid hour and it's corresponding alarm bit is logic 0
 * that information will be compared with the current hour.
 * AMPM = 0 indicates AM (only valid in 12 hours mode)
 * @param   AlarmEn Enable alarm
 * @param   AMPM  AMPM mode (0 is AM, 1 is PM)
 * @param   Hour  Number of hours
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetHourAlarm(bool AlarmEn, bool AMPM, uint8_t Hour);

/*!
 * @brief Set Day alarm
 * Day alarm information coded in *** BCD format ***.
 * When the register Day_alarm is loaded with a valid day and it's corresponding alarm bit is logic 0
 * that information will be compared with the current Day.
 * @param   AlarmEn Enable alarm
 * @param   Day     Number of days
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetDayAlarm(bool AlarmEn, uint8_t Day);

/*!
 * @brief Set Weekday alarm
 * Weekday alarm information coded in *** BCD format ***.
 * When the register weekday_alarm is loaded with a valid day and it's corresponding alarm bit is logic 0
 * that information will be compared with the current weekday.
 * @param   AlarmEn Enable alarm
 * @param   Weekday Day of the week (0-6: 0 is Sunday, etc.)
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_SetWeekdayAlarm(bool AlarmEn, uint8_t Weekday);

/*!
 * @brief Clear minute or second interrupt flag MSF
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_ClearMinSecInterruptFlag();

/*!
 * @brief Clear Timer countdown flag TF
 * Flag set as the end of a timer countdown.
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_ClearCountdownInterruptFlag();

/*!
 * @brief Clear Alarm flag AF
 * Flag set by alarm event.
 * @return  The return status value (0 for success)
 */
pcf2123_status_t PCF2123_ClearAlarmInterruptFlag();

/*! @}*/

#endif /* PCF2123_H_ */
