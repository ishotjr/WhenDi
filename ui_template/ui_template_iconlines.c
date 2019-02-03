#include "ui_template_iconlines.h"
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

#define MAIN_TEXT_FONT (GUI_Font16B_1)
#define MAIN_TEXT_COLOR (GUI_BLACK)
#define ICONLINE_MAX_LINES (1)

typedef struct {
	unsigned int onDisplayAbilityHandle;
	bool onDisplayAbilityHandleRegistered;
	bool onLeaveAbilityHandleRegistered;
	unsigned int onLeaveAbilityHandle;
	bool isDisplayed; // Is this page currently displayed?
	bool hasTitle;
	GUI_COLOR iconLabelColor;
	GUI_COLOR textColor;
	uint32_t numLines;
	bool drawBorders;
	char **text;
	uint8_t *icon;
	char *iconLabel;
	unsigned int abilityHandle[ATMO_NUM_BUTTONS];
	bool abilityHandleRegistered[ATMO_NUM_BUTTONS];
	ATMO_Callback_t callback[ATMO_NUM_BUTTONS];
}ui_iconlines_pg_t;

static ui_iconlines_pg_t pages[MAX_NUM_PAGES] = {0};
static uint8_t currentNumPages = 0;


/**
 * Redraw a whole page
 * @param[in] page - page index
 */
static bool ATMO_UI_ICONLINES_DisplayPage(uint32_t page, bool redrawSingleLine, uint32_t lineId)
{
	const ui_iconlines_pg_t *pg = &pages[page];

	ATMO_UI_PAGE_Margin_t margins;
	ATMO_UI_Page_GetMarginsCurrentDispPage(&margins);

	if(!pg->isDisplayed || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return false;
	}

	if(!redrawSingleLine)
	{
		ATMO_UI_Page_ClearCanvas(pg->hasTitle);
	}

	if(pg->icon != NULL)
	{

		// Get icon dimensions
		int xSize = GUI_BMP_GetXSize(pg->icon);
		int ySize = GUI_BMP_GetYSize(pg->icon);

		int canvasWidth = DISP_WIDTH - margins.right - margins.left;
		int canvasHeight = DISP_HEIGHT - margins.bottom - margins.top;

		if((xSize < canvasWidth) && (ySize < canvasHeight))
		{
			static unsigned int lastLabelLength = 0;

			if(lastLabelLength == 0)
			{
				lastLabelLength = xSize;
			}

			unsigned int iconYPos = margins.top + ((canvasHeight - ySize)/3);

			// Draw Icon
			if(!redrawSingleLine)
			{
				GUI_BMP_Draw(pg->icon, margins.left, iconYPos);
			}

			GUI_SetFont(&MAIN_TEXT_FONT);
			GUI_SetColor(GUI_WHITE);

			// Draw icon text
			GUI_RECT iconLabelRect;
			iconLabelRect.x0 = margins.left;
			iconLabelRect.x1 = iconLabelRect.x0 + lastLabelLength;
			iconLabelRect.y0 = iconYPos + ySize + 2;
			iconLabelRect.y1 = DISP_HEIGHT - margins.bottom;

			GUI_ClearRectEx(&iconLabelRect);

			if(pg->iconLabel)
			{
				GUI_SetColor(pg->iconLabelColor);
				iconLabelRect.x1 = iconLabelRect.x0 + GUI_GetStringDistX(pg->iconLabel);
				lastLabelLength = GUI_GetStringDistX(pg->iconLabel);
				GUI_DispStringInRectWrap(pg->iconLabel, &iconLabelRect, GUI_TA_HCENTER, GUI_WRAPMODE_WORD);
			}

			// Build text rectangles
			GUI_RECT textRect[pg->numLines];

			// How tall does each box need to be? Leave room for 1 line
			unsigned int boxHeight = GUI_GetFontSizeY() * (ICONLINE_MAX_LINES);
			unsigned int boxStartXPos = iconLabelRect.x1;
			unsigned int boxEndXPos = DISP_WIDTH - margins.right;
			unsigned int boxPadding = (pg->drawBorders) ? 3 : 0;

			unsigned int totalBoxSize = boxHeight * pg->numLines;
			unsigned int boxStartYPos = margins.top + ((canvasHeight - totalBoxSize)/3);

			for(int i = 0; i < pg->numLines; i++)
			{
				textRect[i].x0 = boxStartXPos;
				textRect[i].x1 = boxEndXPos;

				if(i > 0)
				{
					textRect[i].y0 = textRect[i-1].y1 + boxPadding;
				}
				else
				{
					textRect[i].y0 = boxStartYPos + (i * boxHeight);
				}

				textRect[i].y1 = textRect[i].y0 + boxHeight;

				if((!redrawSingleLine) || (redrawSingleLine && i == lineId))
				{
					GUI_ClearRectEx(&textRect[i]);

					GUI_SetColor(pg->textColor);

					if(pg->drawBorders)
					{
						GUI_DrawRoundedRect(textRect[i].x0, textRect[i].y0, textRect[i].x1, textRect[i].y1, 3);
					}

					if(pg->text[i] != NULL)
					{
						GUI_DispStringInRectWrap(pg->text[i], &textRect[i], GUI_TA_HCENTER| GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
					}
				}
			}
		}
	}

	return true;
}

bool ATMO_UI_ICONLINES_Init(ATMO_UI_PAGE_Config_t *config, uint32_t numLines, bool drawBorders)
{
	// Allocate driver info to send to Page Controller
	ATMO_UI_Page_DriverInstance_t *driver = ATMO_UI_Page_GetDriverStruct();
	ATMO_DriverInstanceData_t *instanceData = ATMO_UI_Page_GetDriverInstanceDataStruct();
	instanceData->argument = ATMO_UI_Page_GetConfigStruct();

	driver->SetDisplayed = ATMO_UI_ICONLINES_SetDisplayed;
	driver->ProcessUserButton = ATMO_UI_ICONLINES_ProcessButtonInput;

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
	pages[currentNumPages].numLines = numLines;
	pages[currentNumPages].drawBorders = drawBorders;
	pages[currentNumPages].hasTitle = (!config->titleHidden) && (config->title != NULL && strcmp(config->title,"") != 0);
	pages[currentNumPages].text = ATMO_Calloc(numLines, sizeof(char*));
	pages[currentNumPages].iconLabel = NULL;
	pages[currentNumPages].textColor = config->textColor;
	pages[currentNumPages].iconLabelColor = config->textColor;

	for(int i = 0; i < numLines; i++)
	{
		pages[currentNumPages].text[i] = NULL;
	}

	currentNumPages++;

	// Register page with page controller
	ATMO_UI_Page_AddPage(driver, instanceData, &instanceData->instanceNumber);

	return true;
}

bool ATMO_UI_ICONLINES_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed)
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

		return ATMO_UI_ICONLINES_DisplayPage(config->templateInstance, false, 0);
	}
	else
	{
		if(ATMO_UI_Page_GetInitComplete() && pages[config->templateInstance].onLeaveAbilityHandleRegistered)
		{
			ATMO_Value_t value;
			ATMO_InitValue(&value);
			ATMO_AddAbilityExecute(pages[config->templateInstance].onLeaveAbilityHandle, &value);
		}
	}

	return true;
}

bool ATMO_UI_ICONLINES_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex)
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

bool ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].abilityHandle[btnIndex - 1] = abilityHandle;
	pages[instance].abilityHandleRegistered[btnIndex - 1] = true;
	return true;
}

bool ATMO_UI_ICONLINES_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].callback[btnIndex - 1] = cb;
	return true;
}

bool ATMO_UI_ICONLINES_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onDisplayAbilityHandleRegistered = true;
	pages[instance].onDisplayAbilityHandle = abilityHandle;

	return true;
}

bool ATMO_UI_ICONLINES_RegisterOnLeaveAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onLeaveAbilityHandleRegistered = true;
	pages[instance].onLeaveAbilityHandle = abilityHandle;

	return true;	
}

bool ATMO_UI_ICONLINES_SetMainText(uint32_t instance, uint32_t lineId, const char* text)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	if(lineId >= pages[instance].numLines)
	{
		return false;
	}

	if(strcmp(text, pages[instance].text[lineId]) == 0)
	{
		return true;
	}

	if(pages[instance].text[lineId] != NULL)
	{
		ATMO_Free(pages[instance].text[lineId]);
	}

	pages[instance].text[lineId] = ATMO_Malloc(strlen(text) + 1);

	if(pages[instance].text[lineId] == NULL)
	{
		return false;
	}

	strcpy(pages[instance].text[lineId], text);
	ATMO_UI_ICONLINES_DisplayPage(instance, true, lineId);
	return true;
}

bool ATMO_UI_ICONLINES_SetIconLabelColor(uint32_t instance, const char* iconLabel, GUI_COLOR color)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	pages[instance].iconLabelColor = color;
	ATMO_UI_ICONLINES_SetIconLabel(instance, iconLabel);
	return true;

}

bool ATMO_UI_ICONLINES_SetIconLabel(uint32_t instance, const char* iconLabel)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	if(pages[instance].iconLabel != NULL)
	{
		// If the label is the same, ignore it
		if(strcmp(iconLabel, pages[instance].iconLabel) == 0)
		{
			return true;
		}

		ATMO_Free(pages[instance].iconLabel);
	}

	unsigned int labelLen = strlen(iconLabel);
	pages[instance].iconLabel = (labelLen == 0) ? NULL : ATMO_Malloc(labelLen);

	if(pages[instance].iconLabel == NULL)
	{
		return false;
	}

	strcpy(pages[instance].iconLabel, iconLabel);

	ATMO_UI_ICONLINES_DisplayPage(instance, false, 0);

	return true;
}

bool ATMO_UI_ICONLINES_SetIcon(uint32_t instance, uint8_t *bmpData)
{
	if(instance >= currentNumPages)
	{
		return false;
	}

	pages[instance].icon = bmpData;
	return true;
}





