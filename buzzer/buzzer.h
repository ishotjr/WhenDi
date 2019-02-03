#ifndef __ATMO_BUZZER_H_
#define __ATMO_BUZZER_H_

#include "core.h"

bool ATMO_MK64F_Buzzer_Init();
bool ATMO_MK64F_Buzzer_On();
bool ATMO_MK64F_Buzzer_Off();
bool ATMO_MK64F_Buzzer_Toggle(bool *newState);
bool ATMO_MK64F_Buzzer_GetState();

#endif
