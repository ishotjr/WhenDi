/**
 ******************************************************************************
 * @file    adc_mk64f.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - ADC source file for MCUXpresso SDK
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

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "fsl_adc16.h"
#include "adc.h"
#include "adc_mk64f.h"
#include "pin_mapping.h"

const ATMO_ADC_DriverInstance_t mk64fADCDriver = {
	ATMO_MK64F_ADC_Init,
	ATMO_MK64F_ADC_DeInit,
	ATMO_MK64F_ADC_ReadRaw,
	ATMO_MK64F_ADC_Read
};

/* Imported function prototypes ----------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ATMO_ADC_MAX_INSTANCES        2
#define ATMO_ADC_MAX_INSTANCE_INDEX   (ATMO_ADC_MAX_INSTANCES - 1)
#define ATMO_ADC_MAX_CHANNELS         32
#define ATMO_ADC_MAX_CHANNEL_INDEX    (ATMO_ADC_MAX_CHANNELS - 1)
#define ATMO_ADC_INVALID_PIN (0xFFFFFFFF)
#define ATMO_ADC_VREF_MV (3300)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ADC_Type *peripheralBase[] = ADC_BASE_PTRS;
static ATMO_DriverInstanceData_t driverInstanceData;
static uint32_t periphNum = 0;


ATMO_Status_t ATMO_MK64F_ADC_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	ATMO_Status_t status = ATMO_Status_Success;
	ATMO_DriverInstanceData_t* driver = &driverInstanceData;

	driver->name = "MK64F ADC";
	driver->initialized = false;
	driver->instanceNumber = 0;
	driver->argument = &periphNum;

	status = ATMO_ADC_AddDriverInstance( &mk64fADCDriver, driver, instanceNumber );
	

	return status;
}


static ATMO_ADC_Status_t ATMO_Platform_ADC_ConvertRawToVoltage(int32_t rawValue, uint32_t vref_mV, unsigned int resolution, ATMO_ADC_VoltageUnits_t units, int32_t *voltage)
{
	int64_t tmpValue;

	if (voltage == NULL)
		return ATMO_ADC_Status_Invalid;

	// V = adcReading * Vref / (2^resolution)
	tmpValue = (int64_t)rawValue;
	tmpValue *= vref_mV;

	switch (units)
	{
		case ATMO_ADC_VoltageUnits_Volts :
			tmpValue /= 1000;
			break;
		case ATMO_ADC_VoltageUnits_MilliVolts :
			// Nothing to do here (value already referenced to mV)
			break;
		case ATMO_ADC_VoltageUnits_MicroVolts :
			tmpValue *= 1000;
			break;
		case ATMO_ADC_VoltageUnits_NanoVolts :
			tmpValue *= 1000000;
			break;
		default :
			break;
	}

	tmpValue >>= (uint8_t)resolution;

	*voltage = (int32_t)tmpValue;

	return ATMO_ADC_Status_Success;
}

static void ATMO_MK64F_ADC_PinToChannel(ATMO_GPIO_Device_Pin_t pin, uint32_t *adcNum, uint32_t *channel)
{
	switch(pin)
	{
		case MB1_AN:
		{
			*adcNum = 0;
			*channel = 22;
			break;
		}
		case MB2_AN:
		{
			*adcNum = 0;
			*channel = 23;
			break;
		}
		case MB3_AN:
		{
			*adcNum = 1;
			*channel = 12;
			break;
		}
		default:
		{
			*adcNum = ATMO_ADC_INVALID_PIN;
			*channel = ATMO_ADC_INVALID_PIN;
			break;
		}

	}
}

// Get closest HW average
static adc16_hardware_average_mode_t _ATMO_MK64F_ADC_GetHwAverage(uint32_t desiredAverage)
{
	if(desiredAverage < 4)
	{
		return kADC16_HardwareAverageDisabled;
	}
	else if(desiredAverage < 8)
	{
		return kADC16_HardwareAverageCount4;
	}
	else if(desiredAverage < 16)
	{
		return kADC16_HardwareAverageCount8;
	}
	else if(desiredAverage < 32)
	{
		return kADC16_HardwareAverageCount16;
	}
	else
	{
		return kADC16_HardwareAverageCount32;
	}

	return kADC16_HardwareAverageDisabled;
}

/* Exported functions ---------------------------------------------------------*/

ATMO_ADC_Status_t ATMO_MK64F_ADC_Init(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_ADC_Status_Success;
}

ATMO_ADC_Status_t ATMO_MK64F_ADC_DeInit(ATMO_DriverInstanceData_t *instance)
{
	for(uint32_t adcNum = 0; adcNum < ATMO_ADC_MAX_INSTANCES; adcNum++)
	{
		ADC16_Deinit(peripheralBase[adcNum]);
	}

	return ATMO_ADC_Status_Success;
}

ATMO_ADC_Status_t ATMO_MK64F_ADC_ReadRaw(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, int32_t *value, uint32_t numSamplesToAverage)
{
	uint32_t adcNum = 0;
	uint32_t channel = 0;

	ATMO_MK64F_ADC_PinToChannel(pin, &adcNum, &channel);

	// Get default config
	adc16_config_t adcConfig;
	ADC16_GetDefaultConfig(&adcConfig);
	ADC16_Init(peripheralBase[adcNum], &adcConfig);
	ADC16_EnableHardwareTrigger(peripheralBase[adcNum], false);
	ADC16_SetHardwareAverage(peripheralBase[adcNum], _ATMO_MK64F_ADC_GetHwAverage(numSamplesToAverage));
	ADC16_SetChannelMuxMode(peripheralBase[adcNum], kADC16_ChannelMuxA);
	ADC16_DoAutoCalibration(peripheralBase[adcNum]);

    adc16_channel_config_t adc16ChannelConfigStruct = {
            .channelNumber = channel,
            .enableInterruptOnConversionCompleted = false,
            .enableDifferentialConversion = false
    };
    ADC16_SetChannelConfig(peripheralBase[adcNum], 0, &adc16ChannelConfigStruct);

    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(peripheralBase[adcNum], 0))) {}
    *value = (int32_t)ADC16_GetChannelConversionValue(peripheralBase[adcNum], 0);

    return ATMO_ADC_Status_Success;
}

ATMO_ADC_Status_t ATMO_MK64F_ADC_Read(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_ADC_VoltageUnits_t units, int32_t *voltage, uint32_t numSamplesToAverage)
{
	uint32_t adcNum = 0;
	uint32_t channel = 0;

	ATMO_MK64F_ADC_PinToChannel(pin, &adcNum, &channel);

	int32_t rawValue = 0;

	if(ATMO_MK64F_ADC_ReadRaw(instance, pin, &rawValue, numSamplesToAverage) != ATMO_ADC_Status_Success)
	{
		return ATMO_ADC_Status_Fail;
	}

	return ATMO_Platform_ADC_ConvertRawToVoltage(rawValue, ATMO_ADC_VREF_MV, 12, units, voltage);
}
