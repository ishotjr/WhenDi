#ifndef __ATMO_UI_SELECTICON_H_
#define __ATMO_UI_SELECTICON_H_


#include "core.h"
#include "ui_pagecontrol.h"

#include <stdint.h>
#include <stdbool.h>

bool ATMO_UI_SELECTICON_Init(ATMO_UI_PAGE_Config_t *config, uint32_t numOptions, bool autoRoute, bool persist, bool differentStartup);
bool ATMO_UI_SELECTICON_OverlaySaved(uint32_t instance);
bool ATMO_UI_SELECTICON_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed);
bool ATMO_UI_SELECTICON_SetIcon(uint32_t instance, uint8_t *bmpData);
bool ATMO_UI_SELECTICON_SetOption(uint32_t instance, uint32_t optionId, bool selected);
bool ATMO_UI_SELECTICON_GetOption(uint32_t instance, uint32_t optionId, bool *selected);
bool ATMO_UI_SELECTICON_SetOptionText(uint32_t instance, uint32_t optionId, const char* text, bool selected);
bool ATMO_UI_SELECTICON_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex);
bool ATMO_UI_SELECTICON_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle);
bool ATMO_UI_SELECTICON_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb);
bool ATMO_UI_SELECTICON_RegisterOptionSelectedAbilityHandle(uint32_t instance, uint32_t optionId, unsigned int abilityHandle);
bool ATMO_UI_SELECTICON_RegisterOptionSelectedStartupAbilityHandle(uint32_t instance, uint32_t optionId, unsigned int abilityHandle);
bool ATMO_UI_SELECTICON_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle);

#endif
