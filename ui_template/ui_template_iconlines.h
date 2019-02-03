#ifndef __ATMO_UI_ICONLINES_H_
#define __ATMO_UI_ICONLINES_H_


#include "core.h"
#include "ui_pagecontrol.h"

#include <stdint.h>
#include <stdbool.h>

bool ATMO_UI_ICONLINES_Init(ATMO_UI_PAGE_Config_t *config, uint32_t numLines, bool drawBorders);
bool ATMO_UI_ICONLINES_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed);
bool ATMO_UI_ICONLINES_SetIcon(uint32_t instance, uint8_t *bmpData);
bool ATMO_UI_ICONLINES_SetIconLabel(uint32_t instance, const char* iconLabel);
bool ATMO_UI_ICONLINES_SetIconLabelColor(uint32_t instance, const char* iconLabel, GUI_COLOR color);
bool ATMO_UI_ICONLINES_SetMainText(uint32_t instance, uint32_t lineId, const char* text);
bool ATMO_UI_ICONLINES_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex);
bool ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle);
bool ATMO_UI_ICONLINES_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb);
bool ATMO_UI_ICONLINES_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle);
bool ATMO_UI_ICONLINES_RegisterOnLeaveAbilityHandle(uint32_t instance, unsigned int abilityHandle);

#endif
