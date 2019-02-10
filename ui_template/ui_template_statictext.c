#include "ui_template_statictext.h"
#include "ui_pagecontrol.h"
#include "core.h"

#include "battery.h"
/* Display */
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "GUI.h"
#include "MULTIPAGE.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "emwin_support.h"

#include <stdio.h>

#define MAX_NUM_PAGES (20)
#define MAX_CONTENT_LEN (64)

// Each page can have up to MAX_NUM_MEASUREMENTS measurements on it
typedef struct {
	const GUI_FONT *font;
	char content[MAX_CONTENT_LEN];
	unsigned int onDisplayAbilityHandle;
	bool onDisplayAbilityHandleRegistered;
	bool isDisplayed; // Is this page currently displayed?
	bool hasTitle;
	unsigned int alignment;
	unsigned int abilityHandle[ATMO_NUM_BUTTONS];
	bool abilityHandleRegistered[ATMO_NUM_BUTTONS];
	ATMO_Callback_t callback[ATMO_NUM_BUTTONS];
}ui_onoff_pg_t;

static ui_onoff_pg_t pages[MAX_NUM_PAGES] = {0};
static uint8_t currentNumPages = 0;

/**
 * Redraw a whole page
 * @param[in] page - page index
 */
static bool ATMO_UI_STATICTEXT_DisplayPage(uint32_t page)
{
	if(!ATMO_UI_Page_GetDisplayEnabled())
	{
		return false;
	}

	const ui_onoff_pg_t *pg = &pages[page];

	GUI_RECT labelRect;
	labelRect.x0 = ATMO_UI_Page_GetLeftMargin();
	labelRect.x1 = DISP_WIDTH - ATMO_UI_Page_GetRightMargin();
	labelRect.y0 = ATMO_UI_Page_GetTopMargin(pg->hasTitle);
	labelRect.y1 = DISP_HEIGHT - ATMO_UI_Page_GetBottomMargin();

	const GUI_FONT *font = pg->font;

	GUI_SetFont(font);
	GUI_SetColor(GUI_BLACK);

	// Clear the label rectangle and redraw
	GUI_ClearRectEx(&labelRect);
	GUI_DispStringInRectWrap(pg->content, &labelRect, pg->alignment | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);

	return true;
}

bool ATMO_UI_STATICTEXT_Init(ATMO_UI_PAGE_Config_t *config)
{
	ATMO_UI_Page_DriverInstance_t *driver = ATMO_UI_Page_GetDriverStruct();
	ATMO_DriverInstanceData_t *instanceData = ATMO_UI_Page_GetDriverInstanceDataStruct();
	instanceData->argument = ATMO_UI_Page_GetConfigStruct();

	driver->SetDisplayed = ATMO_UI_STATICTEXT_SetDisplayed;
	driver->ProcessUserButton = ATMO_UI_STATICTEXT_ProcessButtonInput;
	config->templateInstance = currentNumPages;

	memcpy((ATMO_UI_PAGE_Config_t*)instanceData->argument, config, sizeof(ATMO_UI_PAGE_Config_t));

	instanceData->initialized = true;
	instanceData->name = NULL;

	if(config->title != NULL)
	{
		instanceData->name = ATMO_Malloc(strlen(config->title) + 1);

		if(instanceData->name != NULL)
		{
			strcpy(instanceData->name, config->title);
		}
	}

	pages[currentNumPages].font = &GUI_Font20_ASCII;
	pages[currentNumPages].isDisplayed = false;
	pages[currentNumPages].alignment = GUI_TA_HCENTER;
	pages[currentNumPages].hasTitle = (!config->titleHidden) && (config->title != NULL && strcmp(config->title,"") != 0);
	pages[currentNumPages].content[0] = 0;

	currentNumPages++;

	// Register page with page controller
	ATMO_UI_Page_AddPage(driver, instanceData, &instanceData->instanceNumber);

	return true;
}

bool ATMO_UI_STATICTEXT_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)instance->argument;

	if(config->templateInstance >= MAX_NUM_PAGES)
	{
		return false;
	}

	pages[config->templateInstance].isDisplayed = isDisplayed;

	if( isDisplayed == true )
	{
		if(pages[config->templateInstance].onDisplayAbilityHandleRegistered)
		{
			ATMO_Value_t value;
			ATMO_InitValue(&value);
			ATMO_AddAbilityExecute(pages[config->templateInstance].onDisplayAbilityHandle, &value);
		}

		return ATMO_UI_STATICTEXT_DisplayPage(config->templateInstance);
	}

	return true;
}

bool ATMO_UI_STATICTEXT_SetText(uint32_t instance, const char* text)
{
	if(instance >= MAX_NUM_PAGES)
	{
		return false;
	}

	if(strlen(text) > MAX_CONTENT_LEN)
	{
		return false;
	}

	if(strncmp(pages[instance].content, text, MAX_CONTENT_LEN) != 0)
	{
		strncpy(pages[instance].content, text, MAX_CONTENT_LEN);

		if(pages[instance].isDisplayed)
		{
			ATMO_UI_STATICTEXT_DisplayPage(instance);
		}
	}

	return true;
}

bool ATMO_UI_STATICTEXT_SetAlignment(uint32_t instance, unsigned int alignment)
{
	if(instance >= MAX_NUM_PAGES)
	{
		return false;
	}

	pages[instance].alignment = alignment;

	return true;	
}

bool ATMO_UI_STATICTEXT_SetTextSize(uint32_t instance, ATMO_FONT_SIZE_t size)
{
	if(instance >= MAX_NUM_PAGES)
	{
		return false;
	}

	switch(size)
	{
		case ATMO_FONT_SIZE_8PT:
		{
			pages[instance].font = &GUI_Font8_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_10PT:
		{
			pages[instance].font = &GUI_Font10_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_13PT:
		{
			pages[instance].font = &GUI_Font13_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_16PT:
		{
			pages[instance].font = &GUI_Font16_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_20PT:
		{
			pages[instance].font = &GUI_Font20_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_24PT:
		{
			pages[instance].font = &GUI_Font24_ASCII;
			break;
		}
		case ATMO_FONT_SIZE_32PT:
		{
			pages[instance].font = &GUI_Font32_ASCII;
			break;
		}
		default:
		{
			return false;
		}
	}

	return true;
}

bool ATMO_UI_STATICTEXT_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)instance->argument;

	if(config->templateInstance >= MAX_NUM_PAGES || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	if(pages[config->templateInstance].abilityHandleRegistered[btnIndex - 1])
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		ATMO_AddAbilityExecute(pages[config->templateInstance].abilityHandle[btnIndex - 1], &val);
	}

	if(pages[config->templateInstance].callback[btnIndex - 1] != NULL)
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		ATMO_AddCallbackExecute(pages[config->templateInstance].callback[btnIndex - 1], &val);
	}

	return true;
}

bool ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle)
{
	if(instance >= MAX_NUM_PAGES || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].abilityHandle[btnIndex - 1] = abilityHandle;
	pages[instance].abilityHandleRegistered[btnIndex - 1] = true;
	return true;
}

bool ATMO_UI_STATICTEXT_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb)
{
	if(instance >= MAX_NUM_PAGES || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].callback[btnIndex - 1] = cb;
	return true;
}

bool ATMO_UI_STATICTEXT_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= MAX_NUM_PAGES)
	{
		return false;
	}
	pages[instance].onDisplayAbilityHandleRegistered = true;
	pages[instance].onDisplayAbilityHandle = abilityHandle;

	return true;
}





