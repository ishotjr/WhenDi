#ifndef __ATMO_UI_TEMPLATE_SYSTEMSTATUS_H_
#define __ATMO_UI_TEMPLATE_SYSTEMSTATUS_H_

#include "core.h"
#include "ui_pagecontrol.h"

#include <stdint.h>
#include <stdbool.h>

bool ATMO_UI_SYSTEMSTATUS_Init(ATMO_UI_PAGE_Config_t *config);
bool ATMO_UI_SYSTEMSTATUS_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed);
bool ATMO_UI_SYSTEMSTATUS_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex);
bool ATMO_UI_SYSTEMSTATUS_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle);
bool ATMO_UI_SYSTEMSTATUS_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb);
bool ATMO_UI_SYSTEMSTATUS_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle);


#endif
