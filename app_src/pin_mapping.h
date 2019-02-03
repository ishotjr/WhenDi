/**
 ******************************************************************************
 * @file    atmo_gpio_device.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - GPIO MK64 Device Specific header file
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
#ifndef __ATMO_PIN_MAPPING_H_
#define __ATMO_PIN_MAPPING_H_


/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
	extern "C"{
#endif


// Valid part numbers:
// MK64FN1M0CAJ12 (1MB, 142-WLCSP)
// MK64FN1M0VDC12 (1MB, 121-XFBGA)
// MK64FN1M0VLL12 (1MB, 100-LQFP)
// MK64FN1M0VLQ12 (1MB, 144-LQFP)
// MK64FN1M0VMD12 (1MB, 144-MAPBGA)
// MK64FX512VDC12 (512KB, 121-XFBGA)
// MK64FX512VLL12 (512KB, 100-LQFP)
// MK64FX512VLQ12 (512KB, 144-LQFP)
// MK64FX512VMD12 (512KB, 144-MAPBGA)

		
/* Must define the device package type (only define one). */

//#define __MK64_PACKAGE_AJ_142WLCSP
//#define __MK64_PACKAGE_DC_121XFBGA
//#define __MK64_PACKAGE_LL_100LQFP
#define __MK64_PACKAGE_LQ_144LQFP
//#define __MK64_PACKAGE_MD_144MAPBGA
		
// Check to see device package is defined
#if (!(defined __MK64_PACKAGE_AJ_142WLCSP) && !(defined __MK64_PACKAGE_DC_121XFBGA) && !(defined __MK64_PACKAGE_LL_100LQFP) && !(defined __MK64_PACKAGE_LQ_144LQFP) && !(defined __MK64_PACKAGE_MD_144MAPBGA))
	#error"Device package not defined."
#endif

// Ensure only one package is defined
#if (((defined __MK64_PACKAGE_AJ_142WLCSP)  && ((defined __MK64_PACKAGE_DC_121XFBGA) || (defined __MK64_PACKAGE_LL_100LQFP)  || (defined __MK64_PACKAGE_LQ_144LQFP)  || (defined __MK64_PACKAGE_MD_144MAPBGA))) ||  \
	 ((defined __MK64_PACKAGE_DC_121XFBGA)  && ((defined __MK64_PACKAGE_AJ_142WLCSP) || (defined __MK64_PACKAGE_LL_100LQFP)  || (defined __MK64_PACKAGE_LQ_144LQFP)  || (defined __MK64_PACKAGE_MD_144MAPBGA))) ||   \
	 ((defined __MK64_PACKAGE_LL_100LQFP)   && ((defined __MK64_PACKAGE_DC_121XFBGA) || (defined __MK64_PACKAGE_AJ_142WLCSP) || (defined __MK64_PACKAGE_LQ_144LQFP)  || (defined __MK64_PACKAGE_MD_144MAPBGA))) ||   \
	 ((defined __MK64_PACKAGE_LQ_144LQFP)   && ((defined __MK64_PACKAGE_DC_121XFBGA) || (defined __MK64_PACKAGE_LL_100LQFP)  || (defined __MK64_PACKAGE_AJ_142WLCSP) || (defined __MK64_PACKAGE_MD_144MAPBGA))) ||   \
	 ((defined __MK64_PACKAGE_MD_144MAPBGA) && ((defined __MK64_PACKAGE_DC_121XFBGA) || (defined __MK64_PACKAGE_LL_100LQFP)  || (defined __MK64_PACKAGE_LQ_144LQFP)  || (defined __MK64_PACKAGE_AJ_142WLCSP))))
	#error"Multiple device packages defined. Must only define one package type."
#endif


/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

/* Pins are defined as 16-bit unsigned numbers where the upper byte represents
 * a port number and the lower byte represents the bit number in that port.
 *
 * For example, 0x020Cu is the pin on port 2 bit 12.
 *
 *
 * For this device port numbers are 0 thru 4 and mapped to the SDK nomenclature as
 * follows:
 *
 * Port 0 = PTA
 * Port 1 = PTB
 * Port 2 = PTC
 * Port 3 = PTD
 * Port 4 = PTE
 *
 * Pin numbers are 0 thru 31 for this device.
 */
typedef enum
{
	PTA0   = 0x0000u,  // all packages
	PTA1   = 0x0001u,  // all packages
	PTA2   = 0x0002u,  // all packages
	PTA3   = 0x0003u,  // all packages
	PTA4   = 0x0004u,  // all packages
	PTA5   = 0x0005u,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA))
	PTA6   = 0x0006u,  // 144-pin packages only
	PTA7   = 0x0007u,  // 144-pin packages only
	PTA8   = 0x0008u,  // 144-pin packages only
	PTA9   = 0x0009u,  // 144-pin packages only
#endif
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA) || (defined __MK64_PACKAGE_DC_121XFBGA))
	PTA10  = 0x000Au,  // 121-pin and 144-pin packages only
	PTA11  = 0x000Bu,  // 121-pin and 144-pin packages only
#endif
	PTA12  = 0x000Cu,  // all packages
	PTA13  = 0x000Du,  // all packages
	PTA14  = 0x000Eu,  // all packages
	PTA15  = 0x000Fu,  // all packages
	PTA16  = 0x0010u,  // all packages
	PTA17  = 0x0011u,  // all packages
	PTA18  = 0x0012u,  // all packages
	PTA19  = 0x0013u,  // all packages
//	PTA20  = 0x0014u,
//	PTA21  = 0x0015u,
//	PTA22  = 0x0016u,
//	PTA23  = 0x0017u,
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA))
	PTA24  = 0x0018u,  // 144-pin packages only
	PTA25  = 0x0019u,  // 144-pin packages only
	PTA26  = 0x001Au,  // 144-pin packages only
	PTA27  = 0x001Bu,  // 144-pin packages only
	PTA28  = 0x001Cu,  // 144-pin packages only
#endif
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA) || (defined __MK64_PACKAGE_DC_121XFBGA))
	PTA29  = 0x001Du,  // 121-pin and 144-pin packages only
#endif
//	PTA30  = 0x001Eu,
//	PTA31  = 0x001Fu,

	PTB0   = 0x0100u,  // all packages
	PTB1   = 0x0101u,  // all packages
	PTB2   = 0x0102u,  // all packages
	PTB3   = 0x0103u,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA))
	PTB4   = 0x0104u,  // 144-pin packages only
	PTB5   = 0x0105u,  // 144-pin packages only
#endif
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA) || (defined __MK64_PACKAGE_DC_121XFBGA))
	PTB6   = 0x0106u,  // 121-pin and 144-pin packages only
	PTB7   = 0x0107u,  // 121-pin and 144-pin packages only
	PTB8   = 0x0108u,  // 121-pin and 144-pin packages only
#endif
	PTB9   = 0x0109u,  // all packages
	PTB10  = 0x010Au,  // all packages
	PTB11  = 0x010Bu,  // all packages
#ifdef __MK64_PACKAGE_DC_121XFBGA
	PTB12  = 0x010Cu,  // 121-pin package only
	PTB13  = 0x010Du,  // 121-pin package only
#endif
//	PTB14  = 0x010Eu,
//	PTB15  = 0x010Fu,
	PTB16  = 0x0110u,  // all packages
	PTB17  = 0x0111u,  // all packages
	PTB18  = 0x0112u,  // all packages
	PTB19  = 0x0113u,  // all packages
	PTB20  = 0x0114u,  // all packages
	PTB21  = 0x0115u,  // all packages
	PTB22  = 0x0116u,  // all packages
	PTB23  = 0x0117u,  // all packages
//	PTB24  = 0x0118u,
//	PTB25  = 0x0119u,
//	PTB26  = 0x011Au,
//	PTB27  = 0x011Bu,
//	PTB28  = 0x011Cu,
//	PTB29  = 0x011Du,
//	PTB30  = 0x011Eu,
//	PTB31  = 0x011Fu,

	PTC0   = 0x0200u,  // all packages
	PTC1   = 0x0201u,  // all packages
	PTC2   = 0x0202u,  // all packages
	PTC3   = 0x0203u,  // all packages
	PTC4   = 0x0204u,  // all packages
	PTC5   = 0x0205u,  // all packages
	PTC6   = 0x0206u,  // all packages
	PTC7   = 0x0207u,  // all packages
	PTC8   = 0x0208u,  // all packages
	PTC9   = 0x0209u,  // all packages
	PTC10  = 0x020Au,  // all packages
	PTC11  = 0x020Bu,  // all packages
	PTC12  = 0x020Cu,  // all packages
	PTC13  = 0x020Du,  // all packages
	PTC14  = 0x020Eu,  // all packages
	PTC15  = 0x020Fu,  // all packages
	PTC16  = 0x0210u,  // all packages
	PTC17  = 0x0211u,  // all packages
	PTC18  = 0x0212u,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA) || (defined __MK64_PACKAGE_DC_121XFBGA))
	PTC19  = 0x0213u,  // 121-pin and 144-pin packages only
#endif
//	PTC20  = 0x0214u,
//	PTC21  = 0x0215u,
//	PTC22  = 0x0216u,
//	PTC23  = 0x0217u,
//	PTC24  = 0x0218u,
//	PTC25  = 0x0219u,
//	PTC26  = 0x021Au,
//	PTC27  = 0x021Bu,
//	PTC28  = 0x021Cu,
//	PTC29  = 0x021Du,
//	PTC30  = 0x021Eu,
//	PTC31  = 0x021Fu,

	PTD0   = 0x0300u,  // all packages
	PTD1   = 0x0301u,  // all packages
	PTD2   = 0x0302u,  // all packages
	PTD3   = 0x0303u,  // all packages
	PTD4   = 0x0304u,  // all packages
	PTD5   = 0x0305u,  // all packages
	PTD6   = 0x0306u,  // all packages
	PTD7   = 0x0307u,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA) || (defined __MK64_PACKAGE_DC_121XFBGA))
	PTD8   = 0x0308u,  // 121-pin and 144-pin packages only
	PTD9   = 0x0309u,  // 121-pin and 144-pin packages only
	PTD10  = 0x030Au,  // 121-pin and 144-pin packages only
	PTD11  = 0x030Bu,  // 121-pin and 144-pin packages only
	PTD12  = 0x030Cu,  // 121-pin and 144-pin packages only
	PTD13  = 0x030Du,  // 121-pin and 144-pin packages only
	PTD14  = 0x030Eu,  // 121-pin and 144-pin packages only
	PTD15  = 0x030Fu,  // 121-pin and 144-pin packages only
#endif
//	PTD16  = 0x0310u,
//	PTD17  = 0x0311u,
//	PTD18  = 0x0312u,
//	PTD19  = 0x0313u,
//	PTD20  = 0x0314u,
//	PTD21  = 0x0315u,
//	PTD22  = 0x0316u,
//	PTD23  = 0x0317u,
//	PTD24  = 0x0318u,
//	PTD25  = 0x0319u,
//	PTD26  = 0x031Au,
//	PTD27  = 0x031Bu,
//	PTD28  = 0x031Cu,
//	PTD29  = 0x031Du,
//	PTD30  = 0x031Eu,
//	PTD31  = 0x031Fu,

	PTE0   = 0x0400u,  // all packages
	PTE1   = 0x0401u,  // all packages
	PTE2   = 0x0402u,  // all packages
	PTE3   = 0x0403u,  // all packages
	PTE4   = 0x0404u,  // all packages
	PTE5   = 0x0405u,  // all packages
	PTE6   = 0x0406u,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA))
	PTE7   = 0x0407u,  // 144-pin packages only
	PTE8   = 0x0408u,  // 144-pin packages only
	PTE9   = 0x0409u,  // 144-pin packages only
	PTE10  = 0x040Au,  // 144-pin packages only
	PTE11  = 0x040Bu,  // 144-pin packages only
	PTE12  = 0x040Cu,  // 144-pin packages only
#endif
//	PTE13  = 0x040Du,
//	PTE14  = 0x040Eu,
//	PTE15  = 0x040Fu,
//	PTE16  = 0x0410u,
//	PTE17  = 0x0411u,
//	PTE18  = 0x0412u,
//	PTE19  = 0x0413u,
//	PTE20  = 0x0414u,
//	PTE21  = 0x0415u,
//	PTE22  = 0x0416u,
//	PTE23  = 0x0417u,
	PTE24  = 0x0418u,  // all packages
	PTE25  = 0x0419u,  // all packages
	PTE26  = 0x041Au,  // all packages
#if ((defined __MK64_PACKAGE_LQ_144LQFP) || (defined __MK64_PACKAGE_MD_144MAPBGA))
	PTE27  = 0x041Bu,  // 144-pin packages only
	PTE28  = 0x041Cu,  // 144-pin packages only
#endif
//	PTE29  = 0x041Du,
//	PTE30  = 0x041Eu,
//	PTE31  = 0x041Fu,
} ATMO_MK64F_GPIO_Device_Pin_t;

// For FRDM K64
#define A0 (PTB2)
#define A1 (PTB3)
#define A2 (PTB10)
#define A3 (PTB11)
#define A4 (PTC11)
#define A5 (PTC10)

#define ATMO_DEFAULT_GPIO_INSTANCE 0
#define ATMO_DEFAULT_GPIO PTC8
#define ATMO_DEFAULT_I2C 1
#define ATMO_DEFAULT_SPI 0
#define ATMO_DEFAULT_ADC A0

// Hexinext Docking Station

// PWM
#define MB1_PWM (PTA10)
#define MB2_PWM (PTA11)
#define MB3_PWM (PTB1)

// RST
#define MB1_RST (PTB11)
#define MB2_RST (PTA26)
#define MB3_RST (PTB10)

// INT
#define MB1_INT (PTB4)
#define MB2_INT (PTB8)
#define MB3_INT (PTB7)

// SPI0_CS
#define MB1_CS (PTC4)
#define MB2_CS (PTC3)
#define MB3_CS (PTC2)

// SPI0_SCK
#define MB1_SCK (PTC5)
#define MB2_SCK (PTC5)
#define MB3_SCK (PTC5)

// SPI0_MISO
#define MB1_MISO (PTC7)
#define MB2_MISO (PTC7)
#define MB3_MISO (PTC7)

// SPI0_MOSI
#define MB1_MOSI (PTC6)
#define MB2_MOSI (PTC6)
#define MB3_MOSI (PTC6)

// I2C0
#define MB1_SCL (PTB2)
#define MB1_SDA (PTB3)
#define MB2_SCL (PTB2)
#define MB2_SDA (PTB3)
#define MB3_SCL (PTB2)
#define MB3_SDA (PTB3)

// UART0
#define MB1_RX (PTB16)
#define MB1_TX (PTB17)

// UART1
#define MB2_RX (PTD2)
#define MB2_TX (PTD3)

//UART2
#define MB3_RX (PTC16)
#define MB3_TX (PTC17)

// ADC
#define MB1_AN (0xAABBCC) // Arbitrary. These two pins cannot be used for general GPIO
#define MB2_AN (0xAABBCD)
#define MB3_AN (PTB6)


/* Exported Structures -------------------------------------------------------*/

/* Exported Function Prototypes -----------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif
