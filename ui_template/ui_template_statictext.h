#ifndef __ATMO_UI_TEMPLATE_STATICTEXT_H_
#define __ATMO_UI_TEMPLATE_STATICTEXT_H_

#include "core.h"
#include "ui_pagecontrol.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	ATMO_FONT_SIZE_8PT,
	ATMO_FONT_SIZE_10PT,
	ATMO_FONT_SIZE_13PT,
	ATMO_FONT_SIZE_16PT,
	ATMO_FONT_SIZE_20PT,
	ATMO_FONT_SIZE_24PT,
	ATMO_FONT_SIZE_32PT,
} ATMO_FONT_SIZE_t;


bool ATMO_UI_STATICTEXT_Init(ATMO_UI_PAGE_Config_t *config);
bool ATMO_UI_STATICTEXT_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed);
bool ATMO_UI_STATICTEXT_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex);
bool ATMO_UI_STATICTEXT_SetAlignment(uint32_t instance, unsigned int alignment);
bool ATMO_UI_STATICTEXT_SetTextSize(uint32_t instance, ATMO_FONT_SIZE_t size);
bool ATMO_UI_STATICTEXT_SetText(uint32_t instance, const char *text);
bool ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle);
bool ATMO_UI_STATICTEXT_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb);
bool ATMO_UI_STATICTEXT_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle);

#endif
