#include "buzzer.h"
#include "fsl_ftm.h"
#include "peripherals.h"
#include "pin_mux.h"

static bool isBuzzerRunning = false;

bool ATMO_MK64F_Buzzer_Init()
{
	const ftm_config_t BUZZER_config = {
	  .prescale = kFTM_Prescale_Divide_4,
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
	const ftm_chnl_pwm_signal_param_t BUZZER_centerPwmSignalParams[] = {
	  {
	    .chnlNumber = kFTM_Chnl_0,
	    .level = kFTM_LowTrue,
	    .dutyCyclePercent = 50
	  }
	};

  FTM_Init(BUZZER_PERIPHERAL, &BUZZER_config);
  FTM_SetupPwm(BUZZER_PERIPHERAL, BUZZER_centerPwmSignalParams, sizeof(BUZZER_centerPwmSignalParams) / sizeof(ftm_chnl_pwm_signal_param_t), kFTM_CenterAlignedPwm, 4000U, BUZZER_CLOCK_SOURCE);
  return true;
}

bool ATMO_MK64F_Buzzer_On()
{
	isBuzzerRunning = true;
	FTM_StartTimer(BOARD_INITPINS_BUZZER_PWM_PERIPHERAL, kFTM_SystemClock);
	return true;
}

bool ATMO_MK64F_Buzzer_Off()
{
	isBuzzerRunning = false;
	FTM_StopTimer(BOARD_INITPINS_BUZZER_PWM_PERIPHERAL);
	return true;
}

bool ATMO_MK64F_Buzzer_Toggle(bool *newState)
{
	if(isBuzzerRunning)
	{
		ATMO_MK64F_Buzzer_Off();
		*newState = false;
	
	}
	else
	{
		ATMO_MK64F_Buzzer_On();
		*newState = true;
	}

	return true;
}

bool ATMO_MK64F_Buzzer_GetState()
{
	return isBuzzerRunning;
}
