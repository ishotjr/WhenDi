/**
 ******************************************************************************
 * @file    pwm_mk64f.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - PWM source file for MCUXpresso SDK
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
#include "fsl_ftm.h"
#include "pwm.h"
#include "pwm_mk64f.h"
#include "pin_mapping.h"

/* Imported function prototypes ----------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef struct
{
	bool initialized;
	bool isRunning;
	ATMO_PWM_Config_t config;
} ATMO_Platform_PWM_Channel_t;

const ATMO_PWM_DriverInstance_t pwmDriverInstance = {
		ATMO_MK64F_PWM_Init,
		ATMO_MK64F_PWM_DeInit,
		ATMO_MK64F_PWM_SetPinConfiguration,
		ATMO_MK64F_PWM_GetPinConfiguration,
		ATMO_MK64F_PWM_Enable,
		ATMO_MK64F_PWM_Disable,
};


/* Private define ------------------------------------------------------------*/
#define ATMO_PWM_MAX_INSTANCES        4
#define ATMO_PWM_MAX_INSTANCE_INDEX   (ATMO_PWM_MAX_INSTANCES - 1)
#define ATMO_PWM_MAX_CHANNELS         8
#define ATMO_PWM_MAX_CHANNEL_INDEX    (ATMO_PWM_MAX_CHANNELS - 1)


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static FTM_Type *peripheralBase[] = FTM_BASE_PTRS;
static bool peripheralInitialized[ATMO_PWM_MAX_INSTANCES] = {false};
static ATMO_Platform_PWM_Channel_t ATMO_Platform_PWM_Channel[ATMO_PWM_MAX_INSTANCES][ATMO_PWM_MAX_CHANNELS];
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define ATMO_PWM_INVALID_PIN (0xFFFFFFFF)

const ftm_config_t pwm_defconfig = {
  .prescale = kFTM_Prescale_Divide_1,
  .bdmMode = kFTM_BdmMode_0,
  .pwmSyncMode = kFTM_SoftwareTrigger,
  .reloadPoints = 0,
  .faultMode = kFTM_Fault_Disable,
  .faultFilterValue = 0,
  .deadTimePrescale = kFTM_Deadtime_Prescale_1,
  .deadTimeValue = 0,
  .extTriggers = 0,
  .chnlInitState = 0,
  .chnlPolarity = 0,
  .useGlobalTimeBase = false
};

/* Private functions ---------------------------------------------------------*/

void ATMO_Platform_FTM_IRQHandler(uint32_t instance)
{
	uint32_t channelFlags;

	// Read then clear the interrupt status flags
	channelFlags = FTM_GetStatusFlags(peripheralBase[instance]);
	FTM_ClearStatusFlags(peripheralBase[instance], channelFlags);

	// We only want to service enabled channels
	channelFlags &= FTM_GetEnabledInterrupts(peripheralBase[instance]);
}

void FTM0_IRQHandler(void)
{
	ATMO_Platform_FTM_IRQHandler(0);
}

void FTM1_IRQHandler(void)
{
	ATMO_Platform_FTM_IRQHandler(1);
}

void FTM2_IRQHandler(void)
{
	ATMO_Platform_FTM_IRQHandler(2);
}

void FTM3_IRQHandler(void)
{
	ATMO_Platform_FTM_IRQHandler(3);
}

static void ATMO_MK64F_PWM_PinToChannel(ATMO_GPIO_Device_Pin_t pin, uint32_t *periph, uint32_t *channel)
{
	switch(pin)
	{
		case PTA1:
			*periph = 0;
			*channel = 6;
			break;
		case PTA2:
			*periph = 0;
			*channel = 7;
			break;
		case PTC2:
			*periph = 0;
			*channel = 1;
			break;
		case PTC3:
			*periph = 0;
			*channel = 2;
			break;
		case PTC4:
			*periph = 0;
			*channel = 3;
			break;
		case PTD0:
			*periph = 3;
			*channel = 0;
			break;
		case PTD2:
			*periph = 3;
			*channel = 2;
			break;
		case PTD3:
			*periph = 3;
			*channel = 3;
			break;
		case PTD1:
			*periph = 3;
			*channel = 1;
			break;
		case MB1_PWM:
			*periph = 2;
			*channel = 0;
			break;
		case MB2_PWM:
			*periph = 2;
			*channel = 1;
			break;
		case MB3_PWM:
			*periph = 1;
			*channel = 1;
			break;
		default:
			*periph = ATMO_PWM_INVALID_PIN;
			*channel = ATMO_PWM_INVALID_PIN;
			break;
	}
}


/* Exported functions ---------------------------------------------------------*/

ATMO_Status_t ATMO_MK64F_PWM_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	ATMO_Status_t status = ATMO_Status_Success;
	static ATMO_DriverInstanceData_t driver;
	static uint32_t periphNum = 0;
	driver.name = "PWM";
	driver.initialized = false;
	driver.instanceNumber = *instanceNumber;
	driver.argument = &periphNum;
	return ATMO_PWM_AddDriverInstance( &pwmDriverInstance, &driver, instanceNumber );

	return status;
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_Init(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_PWM_Status_Success;
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_SetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_PWM_Config_t *config)
{
	uint32_t channel = 0;
	uint32_t pwmNum = 0;
	
	ATMO_MK64F_PWM_PinToChannel(pin, &pwmNum, &channel);

	return ATMO_MK64F_PWM_SetChannelConfiguration(instance, pwmNum, channel, config);
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_GetPinConfiguration(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_PWM_Config_t *config)
{
	uint32_t channel = 0;
	uint32_t pwmNum = 0;
	
	ATMO_MK64F_PWM_PinToChannel(pin, &pwmNum, &channel);

	return ATMO_MK64F_PWM_GetChannelConfiguration(instance, pwmNum, channel, config);
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_DeInit(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_PWM_Status_NotSupported;
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_Enable(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin)
{
	uint32_t channel = 0;
	uint32_t pwmNum = 0;
	
	ATMO_MK64F_PWM_PinToChannel(pin, &pwmNum, &channel);

	ATMO_Platform_PWM_Channel[pwmNum][channel].isRunning = true;

	return ATMO_MK64F_PWM_PeripheralEnable(instance, pwmNum);
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_Disable(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin)
{
	uint32_t channel = 0;
	uint32_t pwmNum = 0;
	
	ATMO_MK64F_PWM_PinToChannel(pin, &pwmNum, &channel);

	ATMO_Platform_PWM_Channel[pwmNum][channel].isRunning = false;

	return ATMO_MK64F_PWM_PeripheralDisable(instance, pwmNum);
}

//=========================
// Advanced MK64F
//=========================
ATMO_PWM_Status_t ATMO_MK64F_PWM_SetChannelConfiguration(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum, uint32_t channel, ATMO_PWM_Config_t *config)
{
	if (pwmPeriphNum > ATMO_PWM_MAX_INSTANCE_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (channel > ATMO_PWM_MAX_CHANNEL_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (config == NULL)
		return ATMO_PWM_Status_Invalid;

	if(!peripheralInitialized[pwmPeriphNum])
	{
		FTM_Init(peripheralBase[pwmPeriphNum], &pwm_defconfig);
		peripheralInitialized[pwmPeriphNum] = true;
	}

	bool isRunning = ATMO_Platform_PWM_Channel[pwmPeriphNum][channel].isRunning;

	if(isRunning)
	{
		ATMO_MK64F_PWM_PeripheralDisable(instance, pwmPeriphNum);
	}

	status_t status = kStatus_Success;

	ftm_chnl_pwm_signal_param_t pwm_params;
	pwm_params.chnlNumber = channel;
	pwm_params.dutyCyclePercent = config->dutyCycle;
	pwm_params.firstEdgeDelayPercent = 0;
	pwm_params.level = kFTM_HighTrue;

	status = FTM_SetupPwm(peripheralBase[pwmPeriphNum], &pwm_params, 1, kFTM_EdgeAlignedPwm, config->pwmFreqHz, CLOCK_GetFreq(kCLOCK_BusClk));

	unsigned int maxDiv = kFTM_Prescale_Divide_128;
	unsigned int currentDiv = pwm_defconfig.prescale;

	// Setup might fail if you try to set a freq too low for default clock div
	while(status != kStatus_Success && (currentDiv <= maxDiv))
	{
		// Try to increase the clock div
		currentDiv++;
		ftm_config_t pwmConfig;
		memcpy(&pwmConfig, &pwm_defconfig, sizeof(ftm_config_t));
		pwmConfig.prescale = currentDiv;
		FTM_Init(peripheralBase[pwmPeriphNum], &pwmConfig);
		status = FTM_SetupPwm(peripheralBase[pwmPeriphNum], &pwm_params, 1, kFTM_EdgeAlignedPwm, config->pwmFreqHz, CLOCK_GetFreq(kCLOCK_BusClk));
	}


	// If currently running, we might be able to adjust on the fly
	if(isRunning)
	{
		ATMO_MK64F_PWM_PeripheralEnable(instance, pwmPeriphNum);
	}

	memcpy(&ATMO_Platform_PWM_Channel[pwmPeriphNum][channel].config, config, sizeof(ATMO_PWM_Config_t));

	ATMO_Platform_PWM_Channel[pwmPeriphNum][channel].initialized = true;

	if(status != kStatus_Success)
	{
		return ATMO_PWM_Status_Fail; // Probably an invalid freq
	}

	return ATMO_PWM_Status_Success;
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_GetChannelConfiguration(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum, uint32_t channel, ATMO_PWM_Config_t *config)
{
	if (pwmPeriphNum > ATMO_PWM_MAX_INSTANCE_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (channel > ATMO_PWM_MAX_CHANNEL_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (config == NULL)
		return ATMO_PWM_Status_Invalid;

	if (!ATMO_Platform_PWM_Channel[pwmPeriphNum][channel].initialized)
		return ATMO_PWM_Status_Fail;

	memcpy(config, &ATMO_Platform_PWM_Channel[pwmPeriphNum][channel].config, sizeof(ATMO_PWM_Config_t));

	return ATMO_PWM_Status_Success;
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_PeripheralEnable(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum)
{
	if (pwmPeriphNum > ATMO_PWM_MAX_INSTANCE_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (!peripheralInitialized[pwmPeriphNum])
		return ATMO_PWM_Status_Fail;

	FTM_StartTimer(peripheralBase[pwmPeriphNum], kFTM_SystemClock);

	return ATMO_PWM_Status_Success;	
}

ATMO_PWM_Status_t ATMO_MK64F_PWM_PeripheralDisable(ATMO_DriverInstanceData_t *instance, uint32_t pwmPeriphNum)
{
	if (pwmPeriphNum > ATMO_PWM_MAX_INSTANCE_INDEX)
		return ATMO_PWM_Status_NotSupported;

	if (!peripheralInitialized[pwmPeriphNum])
		return ATMO_PWM_Status_Fail;

	FTM_StopTimer(peripheralBase[pwmPeriphNum]);

	return ATMO_PWM_Status_Success;
}
