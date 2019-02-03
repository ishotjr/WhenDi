/*
 * Copyright 2017 NXP
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

#include <string.h>
#include "fsl_gpio.h"
#if defined(CPU_MK64FN1M0VMD12)
#include "fsl_ftm.h"
#endif
#include "spi_flash_driver.h"
#include "flash_ica_driver.h"
#include "app_program_ext.h"
#include "pin_mux.h"
#include "pin_mux_rpk.h"
#include "board.h"
#include "microseconds.h"
#include "clock_config.h"
#if defined(CPU_MK64FN1M0VMD12)
#include "peripherals.h"
#include "rgb_led.h"
#endif
#include "rpk_led.h"

//! @addtogroup rpk_led
//! @{


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static bool BlinkToggle = true;
static uint64_t BlinkTicks;
static uint64_t StartTicks;
static bool BlinkInitialized = false;

#if defined(CPU_MK64FN1M0VMD12)
static rgbled_fct_t RPKRGBLED_fct;
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


#if defined(CPU_MK64FN1M0VMD12)
uint8_t Init_rpk_rgb_led(void)
{
	RPKRGBLED_fct.connect_hw = Rgb_Led_Connect;
	RPKRGBLED_fct.disconnect_hw = Rgb_Led_Disconnect;
	RPKRGBLED_fct.set_rgb_colors = Rgb_Led_Set_Colors;

    RGB_Led_Init_Driver(&RPKRGBLED_fct);

    if (RGB_Led_Init_Hw())
        return(1);

    return(0);
}
#endif

void BOARD_Init_RPK_LEDs(void)
{
#if defined(CPU_MKW41Z512VHT4)
	BOARD_InitLEDs();
	GPIO_PinInit(GPIOA, BOARD_INITLEDS_LED_WHITE_GPIO_PIN, &rpk_gpio_output_high_config);
	GPIO_PinInit(GPIOA, BOARD_INITLEDS_LED_BLUE_GPIO_PIN, &rpk_gpio_output_high_config);
#elif defined(CPU_MK64FN1M0VMD12)
	Init_rpk_rgb_led();
#endif
}

//! @brief Initializes Blink functions
void Blink_Init()
{
	BlinkToggle = true;
	BlinkTicks = microseconds_convert_to_ticks(BLINK_RATE);
	StartTicks = microseconds_get_ticks();
	BlinkInitialized = true;
}

#if defined(CPU_MK64FN1M0VMD12)
//! @brief Blinks LED with passed brightness and color
void Blink_LED_RGB(uint8_t brightness, uint8_t color)
{
    if(!BlinkInitialized)
    	Blink_Init();

    uint64_t currentTicks = microseconds_get_ticks();
    if((currentTicks-StartTicks)>BlinkTicks || (StartTicks>currentTicks))
    {
        StartTicks = microseconds_get_ticks();

    	// blink an led so user knows its working
    	if(BlinkToggle)
    	{
    	    RGB_Led_Set_State(brightness, color);
    		BlinkToggle = false;
    	}
    	else
    	{
    	    RGB_Led_Set_State(RGB_LED_BRIGHT_OFF, RGB_LED_COLOR_BLACK); /* off is black */
    		BlinkToggle = true;
    	}
    }
}
#elif defined(CPU_MKW41Z512VHT4)
//! @brief Blinks LED with passed GPIO
void Blink_LED(uint32_t gpio_led)
{
    if(!BlinkInitialized)
        Blink_Init();

    uint64_t currentTicks = microseconds_get_ticks();
    if((currentTicks-StartTicks)>BlinkTicks || (StartTicks>currentTicks))
    {
        StartTicks = microseconds_get_ticks();

        // blink an led so user knows its working
        if(BlinkToggle)
        {
        	GPIO_TogglePinsOutput(GPIOA, (uint32_t)(1 << gpio_led));
            BlinkToggle = false;
        }
        else
        {
            GPIO_TogglePinsOutput(GPIOA, (uint32_t)(1 << gpio_led));
            BlinkToggle = true;
        }
    }
}
#endif

//! @brief Blink failure led a few times
void Blink_Fail_LED()
{
    if(!BlinkInitialized)
    {
    	Blink_Init();
    }

	for(int i = 0; i < 100; i++)  // Blink 4 times
	{
#if defined(CPU_MK64FN1M0VMD12)
		Blink_LED_RGB(RGB_LED_BRIGHT_LOW, RGB_LED_COLOR_RED);
		OS_App_WaitUsec(BLINK_RATE+2);
#else
		Blink_LED(BOARD_INITLEDS_LED_WHITE_GPIO_PIN);
		OS_App_WaitUsec(BLINK_RATE+2);
#endif
	}
}


//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
