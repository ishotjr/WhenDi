#include "ui_template_selecticon.h"
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

#define ATMO_UI_SELECTICON_MAX_OPTS (4)

#define MAIN_TEXT_FONT (GUI_Font16B_1)
#define MAIN_TEXT_COLOR (GUI_BLACK)

#define SELECT_BOX_PADDING (5)

typedef struct {
	bool initComplete;
	bool differentStartup;
	unsigned int onDisplayAbilityHandle;
	bool onDisplayAbilityHandleRegistered;
	bool isDisplayed; // Is this page currently displayed?
	bool hasTitle;
	bool persist;
	GUI_COLOR textColor;
	uint32_t numOptions;
	bool selected[ATMO_UI_SELECTICON_MAX_OPTS];
	char *options[ATMO_UI_SELECTICON_MAX_OPTS];
	uint8_t *icon;
	bool autoRoute;
	unsigned int optSelectedAbilityHandle[ATMO_UI_SELECTICON_MAX_OPTS];
	unsigned int optSelectedStartupAbilityHandle[ATMO_UI_SELECTICON_MAX_OPTS];
	bool optSelectedAbilityHandleRegistered[ATMO_UI_SELECTICON_MAX_OPTS];
	bool optSelectedStartupAbilityHandleRegistered[ATMO_UI_SELECTICON_MAX_OPTS];
	unsigned int abilityHandle[ATMO_NUM_BUTTONS];
	bool abilityHandleRegistered[ATMO_NUM_BUTTONS];
	ATMO_Callback_t callback[ATMO_NUM_BUTTONS];
}ui_selecticon_pg_t;

typedef struct 
{
	uint32_t numOptions;
	bool selected[4];
} ui_selection_savedsettings_t;

static ui_selecticon_pg_t pages[MAX_NUM_PAGES] = {0};
static uint8_t currentNumPages = 0;

static bool ATMO_UI_SELECTICON_SaveSelection(uint32_t page)
{
	const ui_selecticon_pg_t *pg = &pages[page];

	if(!pg->persist)
	{
		return false;
	}

	char pgTitle[16];
	sprintf(pgTitle, "onoff_%d", page);

	ui_selection_savedsettings_t settings;
	settings.numOptions = pg->numOptions;
	memcpy(settings.selected, pg->selected, settings.numOptions * sizeof(bool));
	
	ATMO_FILESYSTEM_File_t file;
	if(ATMO_FILESYSTEM_FileOpen(0, &file, pgTitle, ATMO_CREAT | ATMO_RDWR | ATMO_TRUNC | ATMO_NORETRY) != ATMO_FILESYSTEM_Status_Success)
	{
		return false;
	}

	if(ATMO_FILESYSTEM_FileWrite(0, &file, &settings, sizeof(ui_selection_savedsettings_t)) != ATMO_FILESYSTEM_Status_Success)
	{
		ATMO_FILESYSTEM_FileClose(0, &file);
		return false;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);
	return true;
}

static bool ATMO_UI_SELECTICON_GetSavedSelection(uint32_t page, ui_selection_savedsettings_t *settings)
{
	const ui_selecticon_pg_t *pg = &pages[page];

	if(!pg->persist)
	{
		return false;
	}
	
	char pgTitle[16];
	sprintf(pgTitle, "onoff_%d", page);
	
	ATMO_FILESYSTEM_File_t file;
	if(ATMO_FILESYSTEM_FileOpen(0, &file, pgTitle, ATMO_RDONLY | ATMO_NORETRY) != ATMO_FILESYSTEM_Status_Success)
	{
		return false;
	}

	if(ATMO_FILESYSTEM_FileRead(0, &file, settings, sizeof(ui_selection_savedsettings_t)) != ATMO_FILESYSTEM_Status_Success)
	{
		ATMO_FILESYSTEM_FileClose(0, &file);
		return false;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);

	ATMO_PLATFORM_DelayMilliseconds(50);
	return true;
}


/**
 * Redraw a whole page
 * @param[in] page - page index
 */
static bool ATMO_UI_SELECTICON_DisplayPage(uint32_t page, bool fullRedraw)
{
	const ui_selecticon_pg_t *pg = &pages[page];

	if(!pg->isDisplayed || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return false;
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

		int canvasWidth = DISP_WIDTH - ATMO_UI_Page_GetRightMargin() - ATMO_UI_Page_GetLeftMargin();
		int canvasHeight = DISP_HEIGHT - ATMO_UI_Page_GetBottomMargin() - ATMO_UI_Page_GetTopMargin(pg->hasTitle);

		if((xSize < canvasWidth) && (ySize < canvasHeight))
		{
			unsigned int iconXPos = ATMO_UI_Page_GetLeftMargin() + ((canvasWidth - xSize)/2);

			// Draw Icon
			GUI_BMP_Draw(pg->icon, iconXPos, ATMO_UI_Page_GetTopMargin(pg->hasTitle));

			GUI_SetFont(&MAIN_TEXT_FONT);
			GUI_SetColor(pg->textColor);

			int selectBoxSize = GUI_GetFontSizeY();
			unsigned int rowHeight = (canvasHeight - ySize) / (pg->numOptions/2);
			unsigned int selectOptTextBoxWidth = (canvasWidth - (2 * selectBoxSize) - (4 * SELECT_BOX_PADDING)) / 2;
			unsigned int selectBoxOffset = (rowHeight - selectBoxSize) / 2;

			for(int row = 0; row < (pg->numOptions/2); row++)
			{
				GUI_RECT opts[4];

				// First option
				opts[0].x0 = ATMO_UI_Page_GetLeftMargin();
				opts[0].x1 = opts[0].x0 + selectOptTextBoxWidth;
				opts[0].y0 = ATMO_UI_Page_GetTopMargin(pg->hasTitle) + xSize + (rowHeight * row);
				opts[0].y1 = opts[0].y0 + rowHeight;

				GUI_DispStringInRectWrap(pg->options[(row * 2)], &opts[0], GUI_TA_RIGHT| GUI_TA_VCENTER, GUI_WRAPMODE_WORD);

				// First option select indicator
				opts[1].x0 = opts[0].x1 + SELECT_BOX_PADDING;
				opts[1].x1 = opts[1].x0 + selectBoxSize;
				opts[1].y0 = ATMO_UI_Page_GetTopMargin(pg->hasTitle) + xSize + (rowHeight * row) + selectBoxOffset;
				opts[1].y1 = opts[1].y0 + selectBoxSize;

				GUI_SetColor(GUI_WHITE);
				GUI_ClearRectEx(&opts[1]);
				GUI_SetColor(pg->textColor);

				if(pg->selected[(row * 2)])
				{
					GUI_FillRoundedRect(opts[1].x0, opts[1].y0, opts[1].x1, opts[1].y1, 3);
				}
				else
				{
					GUI_DrawRoundedRect(opts[1].x0, opts[1].y0, opts[1].x1, opts[1].y1, 3);
				}

				// Second option select indicator
				opts[2].x0 = opts[1].x1 + SELECT_BOX_PADDING;
				opts[2].x1 = opts[2].x0 + selectBoxSize;
				opts[2].y0 = ATMO_UI_Page_GetTopMargin(pg->hasTitle) + xSize + (rowHeight * row) + selectBoxOffset;
				opts[2].y1 = opts[2].y0 + selectBoxSize;

				GUI_SetColor(GUI_WHITE);
				GUI_ClearRectEx(&opts[2]);
				GUI_SetColor(pg->textColor);

				if(pg->selected[(row * 2) + 1])
				{
					GUI_FillRoundedRect(opts[2].x0, opts[2].y0, opts[2].x1, opts[2].y1, 3);
				}
				else
				{
					GUI_DrawRoundedRect(opts[2].x0, opts[2].y0, opts[2].x1, opts[2].y1, 3);
				}

				// Second option
				opts[3].x0 = opts[2].x1 + SELECT_BOX_PADDING;
				opts[3].x1 = opts[3].x0 + selectOptTextBoxWidth;
				opts[3].y0 = ATMO_UI_Page_GetTopMargin(pg->hasTitle) + xSize + (rowHeight * row);
				opts[3].y1 = opts[3].y0 + rowHeight;

				GUI_DispStringInRectWrap(pg->options[(row * 2) + 1], &opts[3], GUI_TA_LEFT| GUI_TA_VCENTER, GUI_WRAPMODE_WORD);

			}
		}
	}

	return true;
}

bool ATMO_UI_SELECTICON_Init(ATMO_UI_PAGE_Config_t *config, uint32_t numOptions, bool autoRoute, bool persist, bool differentStartup)
{
	if(numOptions != 2 && numOptions != 4)
	{
		return false;
	}

	// Allocate driver info to send to Page Controller
	ATMO_UI_Page_DriverInstance_t *driver = ATMO_UI_Page_GetDriverStruct();
	ATMO_DriverInstanceData_t *instanceData = ATMO_UI_Page_GetDriverInstanceDataStruct();
	instanceData->argument = ATMO_UI_Page_GetConfigStruct();

	driver->SetDisplayed = ATMO_UI_SELECTICON_SetDisplayed;
	driver->ProcessUserButton = ATMO_UI_SELECTICON_ProcessButtonInput;

	config->templateInstance = currentNumPages;
	memcpy((ATMO_UI_PAGE_Config_t*)instanceData->argument, config, sizeof(ATMO_UI_PAGE_Config_t));

	instanceData->initialized = true;
	instanceData->name = NULL;

	if(config->title != NULL)
	{
		unsigned int titleLen = strlen(config->title);
		instanceData->name = (titleLen == 0) ? NULL : ATMO_Malloc(titleLen + 1);

		if(instanceData->name != NULL)
		{
			strcpy(instanceData->name, config->title);
		}
	}

	pages[currentNumPages].isDisplayed = false;
	pages[currentNumPages].initComplete = false;
	pages[currentNumPages].differentStartup = differentStartup;
	pages[currentNumPages].numOptions = numOptions;
	pages[currentNumPages].autoRoute = autoRoute;
	pages[currentNumPages].hasTitle = (!config->titleHidden) && (config->title != NULL && strcmp(config->title,"") != 0);
	pages[currentNumPages].textColor = config->textColor;
	pages[currentNumPages].persist = persist;



	for(int i = 0; i < numOptions; i++)
	{
		pages[currentNumPages].options[i] = NULL;
		pages[currentNumPages].selected[i] = i % 2 == 0;
		pages[currentNumPages].optSelectedAbilityHandleRegistered[i] = false;
	}

	currentNumPages++;

	// Register page with page controller
	ATMO_UI_Page_AddPage(driver, instanceData, &instanceData->instanceNumber);

	return true;
}

bool ATMO_UI_SELECTICON_OverlaySaved(uint32_t instance)
{
	// Try to reg saved data
	ui_selection_savedsettings_t settings;
	ui_selecticon_pg_t *pg = &pages[instance];

	if(ATMO_UI_SELECTICON_GetSavedSelection(instance, &settings))
	{
		for(unsigned int i = 0; i < pages[instance].numOptions; i+=2)
		{
			ATMO_UI_SELECTICON_SetOption(instance, i, settings.selected[i]);
		}
	}

	// Once this is called, init is complete
	pg->initComplete = true;

	return true;
}

bool ATMO_UI_SELECTICON_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed)
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

		return ATMO_UI_SELECTICON_DisplayPage(config->templateInstance, true);
	}

	return true;
}

bool ATMO_UI_SELECTICON_GetOption(uint32_t instance, uint32_t optionId, bool *selected)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	ui_selecticon_pg_t *pg = &pages[instance];

	if(!pg->isDisplayed || optionId >= pg->numOptions)
	{
		return false;
	}

	*selected = pg->selected[optionId];

	return true;
}

bool ATMO_UI_SELECTICON_SetOption(uint32_t instance, uint32_t optionId, bool selected)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	ui_selecticon_pg_t *pg = &pages[instance];

	if(optionId >= pg->numOptions)
	{
		return false;
	}

	// If not already selected
	if(selected != pg->selected[optionId])
	{
		uint32_t companionOption = (optionId % 2 == 0) ? optionId + 1 : optionId - 1;
		pg->selected[optionId] = selected;
		pg->selected[companionOption] = !selected;

		uint32_t changedOptionId = (selected) ? optionId : companionOption;

		ATMO_UI_SELECTICON_DisplayPage(instance, false);
		
		// If there's no special startup behavior
		if(!pg->differentStartup)
		{
			if(pg->optSelectedAbilityHandleRegistered[changedOptionId])
			{
				ATMO_Value_t val;
				ATMO_InitValue(&val);
				ATMO_AddAbilityExecute(pg->optSelectedAbilityHandle[changedOptionId], &val);
			}
		}
		else
		{
			if(pg->initComplete)
			{
				if(pg->optSelectedAbilityHandleRegistered[changedOptionId])
				{
					ATMO_Value_t val;
					ATMO_InitValue(&val);
					ATMO_AddAbilityExecute(pg->optSelectedAbilityHandle[changedOptionId], &val);
				}
			}
			else
			{
				if(pg->optSelectedStartupAbilityHandleRegistered[changedOptionId])
				{
					ATMO_Value_t val;
					ATMO_InitValue(&val);
					ATMO_AddAbilityExecute(pg->optSelectedStartupAbilityHandle[changedOptionId], &val);
				}
			}
		}

		ATMO_UI_SELECTICON_SaveSelection(instance);
	}

	return true;

}

bool ATMO_UI_SELECTICON_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)instance->argument;
	ui_selecticon_pg_t *pg = &pages[config->templateInstance];

	bool redraw = false;
	uint32_t changedOptId = 0;

	if(config->templateInstance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	if(pg->abilityHandleRegistered[btnIndex - 1])
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		ATMO_AddAbilityExecute(pg->abilityHandle[btnIndex - 1], &val);
	}

	if(pg->callback[btnIndex - 1] != NULL)
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		ATMO_AddCallbackExecute(pg->callback[btnIndex - 1], &val);
	}


	if(!pg->autoRoute)
	{
		return true;
	}

	switch(btnIndex)
	{
		// Top left button
		case 3:
		{
			if(pg->numOptions == 4)
			{

				pg->selected[0] = !pg->selected[0];
				pg->selected[1] = !pg->selected[1];
				redraw = true;
				
				if(pg->selected[0])
				{
					changedOptId = 0;
				}
				else
				{
					changedOptId = 1;
				}
			
			}
			break;
		}
		// Bottom left
		case 4:
		{
			if(pg->numOptions == 4)
			{
				pg->selected[2] = !pg->selected[2];
				pg->selected[3] = !pg->selected[3];
				redraw = true;
				if(pg->selected[2])
				{
					changedOptId = 2;
				}
				else
				{
					changedOptId = 2;
				}
			}
			else
			{

				pg->selected[0] = !pg->selected[0];
				pg->selected[1] = !pg->selected[1];
				redraw = true;

				if(pg->selected[0])
				{
					changedOptId = 0;
				}
				else
				{
					changedOptId = 1;
				}
				
			}

			break;
		}
	}

	if(pg->isDisplayed && redraw)
	{
		ATMO_UI_SELECTICON_SaveSelection(config->templateInstance);
		ATMO_UI_SELECTICON_DisplayPage(config->templateInstance, false);
		if(pg->optSelectedAbilityHandleRegistered[changedOptId])
		{
			ATMO_Value_t val;
			ATMO_InitValue(&val);
			ATMO_AddAbilityExecute(pg->optSelectedAbilityHandle[changedOptId], &val);
		}
	}

	return true;
}

bool ATMO_UI_SELECTICON_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].abilityHandle[btnIndex - 1] = abilityHandle;
	pages[instance].abilityHandleRegistered[btnIndex - 1] = true;
	return true;
}

bool ATMO_UI_SELECTICON_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].callback[btnIndex - 1] = cb;
	return true;
}

bool ATMO_UI_SELECTICON_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onDisplayAbilityHandleRegistered = true;
	pages[instance].onDisplayAbilityHandle = abilityHandle;

	return true;
}

bool ATMO_UI_SELECTICON_SetOptionText(uint32_t instance, uint32_t optionId, const char* text, bool selected)
{
	if(optionId >= pages[instance].numOptions)
	{
		return false;
	}

	if(pages[instance].options[optionId] != NULL)
	{
		ATMO_Free(pages[instance].options[optionId]);
	}

	pages[instance].options[optionId] = ATMO_Malloc(strlen(text) + 1);

	if(pages[instance].options[optionId] == NULL)
	{
		return false;
	}

	pages[instance].selected[optionId] = selected;

	strcpy(pages[instance].options[optionId], text);
	return true;
}

bool ATMO_UI_SELECTICON_SetIcon(uint32_t instance, uint8_t *bmpData)
{
	pages[instance].icon = bmpData;
	return true;
}

bool ATMO_UI_SELECTICON_RegisterOptionSelectedAbilityHandle(uint32_t instance, uint32_t optionId, unsigned int abilityHandle)
{
	if(optionId >= pages[instance].numOptions)
	{
		return false;
	}

	pages[instance].optSelectedAbilityHandle[optionId] = abilityHandle;
	pages[instance].optSelectedAbilityHandleRegistered[optionId] = true;
	return true;
}

bool ATMO_UI_SELECTICON_RegisterOptionSelectedStartupAbilityHandle(uint32_t instance, uint32_t optionId, unsigned int abilityHandle)
{
	if(optionId >= pages[instance].numOptions)
	{
		return false;
	}

	pages[instance].optSelectedStartupAbilityHandle[optionId] = abilityHandle;
	pages[instance].optSelectedStartupAbilityHandleRegistered[optionId] = true;
	return true;
}






