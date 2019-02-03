#ifndef __ATMO_UI_SINGLEICONTEXT_H_
#define __ATMO_UI_SINGLEICONTEXT_H_


#include "core.h"
#include "ui_pagecontrol.h"

#include <stdint.h>
#include <stdbool.h>

bool ATMO_UI_SINGLEICONTEXT_Init(ATMO_UI_PAGE_Config_t *config);
bool ATMO_UI_SINGLEICONTEXT_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed);
bool ATMO_UI_SINGLEICONTEXT_SetIcon(uint32_t instance, uint8_t *bmpData);
bool ATMO_UI_SINGLEICONTEXT_SetMainText(uint32_t instance, const char* text);
bool ATMO_UI_SINGLEICONTEXT_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex);
bool ATMO_UI_SINGLEICONTEXT_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle);
bool ATMO_UI_SINGLEICONTEXT_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb);
bool ATMO_UI_SINGLEICONTEXT_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle);
bool ATMO_UI_SINGLEICONTEXT_RegisterOnLeaveAbilityHandle(uint32_t instance, unsigned int abilityHandle);
#endif
