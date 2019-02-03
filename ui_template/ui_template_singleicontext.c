#include "ui_template_singleicontext.h"
#include "ui_pagecontrol.h"
#include "core.h"

#include "battery_mk64f.h"

/* Display */
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "GUI.h"
#include "MULTIPAGE.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "emwin_support.h"
#include "TimersManager.h"

#include <stdio.h>

#define MAX_NUM_PAGES (20)

#define MAIN_TEXT_FONT (GUI_Font24B_1)
#define MAIN_TEXT_COLOR (GUI_BLACK)
#define MAX_TEXT_LEN (32)

#define ICON_TOP_PADDING (10)

typedef struct {
	unsigned int onDisplayAbilityHandle;
	bool onDisplayAbilityHandleRegistered;
	bool onLeaveAbilityHandleRegistered;
	unsigned int onLeaveAbilityHandle;
	bool isDisplayed; // Is this page currently displayed?
	bool hasTitle;
	GUI_COLOR textColor;
	char *text;
	uint8_t *icon;
	unsigned int abilityHandle[ATMO_NUM_BUTTONS];
	bool abilityHandleRegistered[ATMO_NUM_BUTTONS];
	ATMO_Callback_t callback[ATMO_NUM_BUTTONS];
}ui_singleicontext_pg_t;

static ui_singleicontext_pg_t pages[MAX_NUM_PAGES] = {0};
static uint8_t currentNumPages = 0;


/**
 * Redraw a whole page
 * @param[in] page - page index
 */
static bool ATMO_UI_SINGLEICONTEXT_DisplayPage(uint32_t page, bool fullRedraw)
{
	if(!ATMO_UI_Page_GetDisplayEnabled())
	{
		return false;
	}

	const ui_singleicontext_pg_t *pg = &pages[page];

	ATMO_UI_PAGE_Margin_t margins;
	ATMO_UI_Page_GetMarginsCurrentDispPage(&margins);

	if(!pg->isDisplayed)
	{
		// Not an error, just don't want to continue
		return true;
	}

	if(fullRedraw)
	{
		ATMO_UI_Page_ClearCanvas(pg->hasTitle);
	}

	if(pg->icon != NULL)
	{

		// Get icon dimensions
		int xSize = GUI_BMP_GetXSize(pg->icon);
		int ySize = GUI_BMP_GetYSize(pg->icon);

		// Image should be square
		// Scale it so that the biggest dimension is as expected
		int canvasWidth = DISP_WIDTH - margins.right - margins.left;
		int canvasHeight = DISP_HEIGHT - margins.bottom - margins.top;

		unsigned int iconXPos = margins.left + ((canvasWidth - xSize)/2);

		// Draw Icon
		if(fullRedraw)
		{
			GUI_BMP_Draw(pg->icon, iconXPos, margins.top + ICON_TOP_PADDING);
		}

		// Draw text
		if(pg->text && (strcmp(pg->text,"") != 0))
		{
			GUI_RECT textRect;
			textRect.x0 = margins.left;
			textRect.x1 = textRect.x0 + canvasWidth;
			textRect.y0 = margins.top + ySize + ICON_TOP_PADDING;
			textRect.y1 = DISP_HEIGHT - margins.bottom;
			GUI_SetColor(GUI_WHITE);
			GUI_ClearRectEx(&textRect);
			GUI_SetFont(&MAIN_TEXT_FONT);
			GUI_SetColor(pg->textColor);
			GUI_DispStringInRectWrap(pg->text, &textRect, GUI_TA_HCENTER| GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
		}

		
	}

	return true;
}

bool ATMO_UI_SINGLEICONTEXT_Init(ATMO_UI_PAGE_Config_t *config)
{
	ATMO_UI_Page_DriverInstance_t *driver = ATMO_UI_Page_GetDriverStruct();
	ATMO_DriverInstanceData_t *instanceData = ATMO_UI_Page_GetDriverInstanceDataStruct();
	instanceData->argument = ATMO_UI_Page_GetConfigStruct();

	driver->SetDisplayed = ATMO_UI_SINGLEICONTEXT_SetDisplayed;
	driver->ProcessUserButton = ATMO_UI_SINGLEICONTEXT_ProcessButtonInput;

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

	pages[currentNumPages].isDisplayed = false;
	pages[currentNumPages].hasTitle = (!config->titleHidden) && (config->title != NULL && strcmp(config->title,"") != 0);
	pages[currentNumPages].text = NULL;
	pages[currentNumPages].textColor = config->textColor;

	currentNumPages++;

	// Register page with page controller
	ATMO_UI_Page_AddPage(driver, instanceData, &instanceData->instanceNumber);

	return true;
}

bool ATMO_UI_SINGLEICONTEXT_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)instance->argument;

	if(config->templateInstance >= currentNumPages)
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

		return ATMO_UI_SINGLEICONTEXT_DisplayPage(config->templateInstance, true);
	}
	else
	{
		if(ATMO_UI_Page_GetInitComplete() && pages[config->templateInstance].onLeaveAbilityHandleRegistered)
		{
			ATMO_PLATFORM_DebugPrint("Leaving page %d\r\n", config->templateInstance);
			ATMO_Value_t value;
			ATMO_InitValue(&value);
			ATMO_AddAbilityExecute(pages[config->templateInstance].onLeaveAbilityHandle, &value);
		}
	}

	return true;
}

bool ATMO_UI_SINGLEICONTEXT_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)instance->argument;

	if(config->templateInstance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
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

bool ATMO_UI_SINGLEICONTEXT_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].abilityHandle[btnIndex - 1] = abilityHandle;
	pages[instance].abilityHandleRegistered[btnIndex - 1] = true;
	return true;
}

bool ATMO_UI_SINGLEICONTEXT_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].callback[btnIndex - 1] = cb;
	return true;
}

bool ATMO_UI_SINGLEICONTEXT_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onDisplayAbilityHandleRegistered = true;
	pages[instance].onDisplayAbilityHandle = abilityHandle;

	return true;
}

bool ATMO_UI_SINGLEICONTEXT_RegisterOnLeaveAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onLeaveAbilityHandleRegistered = true;
	pages[instance].onLeaveAbilityHandle = abilityHandle;

	return true;	
}

bool ATMO_UI_SINGLEICONTEXT_SetMainText(uint32_t instance, const char* text)
{
	if(pages[instance].text != NULL)
	{
		if(strcmp(pages[instance].text, text) == 0)
		{
			return true;
		}

		ATMO_Free(pages[instance].text);
	}

	pages[instance].text = ATMO_Malloc(strlen(text) + 1);

	if(pages[instance].text == NULL)
	{
		return false;
	}

	strcpy(pages[instance].text, text);

	ATMO_UI_SINGLEICONTEXT_DisplayPage(instance, false);

	return true;
}

bool ATMO_UI_SINGLEICONTEXT_SetIcon(uint32_t instance, uint8_t *bmpData)
{
	pages[instance].icon = bmpData;
	return true;
}





