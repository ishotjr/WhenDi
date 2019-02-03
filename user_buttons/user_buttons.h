#ifndef __ATMO_USER_INPUT_H_
#define __ATMO_USER_INPUT_H_

#include "core.h"

typedef enum {
	ATMO_MK64F_SW1_Pushed,
	ATMO_MK64F_SW2_Pushed,
	ATMO_MK64F_SW3_Pushed,
	ATMO_MK64F_SW4_Pushed,
} ATMO_MK64F_UserButton_t;

bool ATMO_MK64F_UserButton_Init(ATMO_DriverInstanceHandle_t gpioDriver);
bool ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_UserButton_t type, unsigned int abilityHandle);
bool ATMO_MK64F_UserButton_RegisterCallback(ATMO_MK64F_UserButton_t type, ATMO_Callback_t cb);

#endif
