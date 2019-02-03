#include "ui_template_systemstatus.h"
#include "ui_pagecontrol.h"
#include "core.h"

#include "battery_mk64f.h"
#include "datetime_mk64f.h"
#include "atmosphere_platform.h"

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

#include "threadmini_icon.h"
#include "bluetoothmini_icon.h"

#include <stdio.h>
#include <time.h>

#define MAX_NUM_PAGES (1)
#define NUM_STATUS_SLOTS (4)
#define DEFAULT_SETTINGS_COLOR (GUI_BLACK)
#define SLOT_LABEL_FONT (GUI_Font16B_1)
#define BATTERY_PCT_FONT (GUI_Font16B_1)
#define BATTERY_HEIGHT_RATIO (.8) // Ratio of battery height to the box it lives in
#define BATT_OUTLINE_COLOR (GUI_BLACK)
#define BATT_INSIDE_PADDING (3)
#define BATT_INSIDE_COLOR (GUI_BLACK)

#define MAIN_TEXT_FONT (GUI_Font24B_1)
#define MAIN_TEXT_COLOR (GUI_BLACK)

#define BUILD_INFO_FONT (GUI_Font10_1)
#define BUILD_INFO_COLOR (GUI_BLACK)

#define DATE_TIME_COLOR (GUI_BLACK)
#define TIME_FONT (GUI_Font20_1)
#define RAPIDIOT_FONT (GUI_Font24B_1)

#define CONNECTIVITY_FONT (GUI_Font16B_1)
#define CONNECTIVITY_FONT_COLOR (GUI_BLACK)
#define CONNECTIVITY_BLE_CONNECTED_COLOR (GUI_BLUE)
#define CONNECTIVITY_THREAD_CONNECTED_COLOR (GUI_GREEN)
#define CONNECTIVITY_BLE_STR "BLE"
#define CONNECTIVITY_THREAD_STR "THREAD"

#define MAX_TEXT_LEN (32)

static bool isBleConnected = false;
static bool isThreadConnected = false;
static bool bleEnabled = false;
static bool threadEnabled = false;

// Each page can have up to MAX_NUM_MEASUREMENTS measurements on it
typedef struct {
	unsigned int onDisplayAbilityHandle;
	bool onDisplayAbilityHandleRegistered;
	bool isDisplayed; // Is this page currently displayed?
	bool hasTitle;
	unsigned int abilityHandle[ATMO_NUM_BUTTONS];
	bool abilityHandleRegistered[ATMO_NUM_BUTTONS];
	ATMO_Callback_t callback[ATMO_NUM_BUTTONS];
}ui_status_pg_t;

static ui_status_pg_t pages[MAX_NUM_PAGES] = {0};
static uint8_t currentNumPages = 0;
static GUI_RECT containerSlots[NUM_STATUS_SLOTS];
static GUI_RECT batteryRect;
static GUI_RECT threadBleIconRect;

static void drawBattery(GUI_RECT *batteryRect, GUI_RECT *slotRect, bool charging, unsigned int percentage)
{
	if(!pages[0].isDisplayed || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return;
	}

	GUI_SetColor(GUI_WHITE);
	GUI_ClearRectEx(batteryRect);

	// Draw outline
	GUI_SetColor(BATT_OUTLINE_COLOR);
	GUI_DrawRoundedRect(batteryRect->x0, batteryRect->y0, batteryRect->x1, batteryRect->y1, 3);

	// Draw tip
	unsigned int battTipHeight = (batteryRect->y1 - batteryRect->y0) / 3;
	unsigned int battTipWidth = (batteryRect->x1 - batteryRect->x0) / 10;
	unsigned int battTipMargin = ((batteryRect->y1 - batteryRect->y0) - battTipHeight) / 2;
	GUI_RECT battTip;
	battTip.x0 = batteryRect->x1;
	battTip.x1 = battTip.x0 + battTipWidth;
	battTip.y0 = batteryRect->y0 + battTipMargin;
	battTip.y1 = battTip.y0 + battTipHeight;
	GUI_FillRoundedRect(battTip.x0, battTip.y0, battTip.x1, battTip.y1, 1);

	// How long is the inside?
	unsigned int insideLen = (unsigned int)((batteryRect->x1 - batteryRect->x0 - (BATT_INSIDE_PADDING * 2)) * (percentage/100.0));

	// Draw inside rectangle
	GUI_RECT battInside;
	battInside.x0 = batteryRect->x0 + BATT_INSIDE_PADDING;
	battInside.x1 = battInside.x0 + insideLen;
	battInside.y0 = batteryRect->y0 + BATT_INSIDE_PADDING;
	battInside.y1 = batteryRect->y1 - BATT_INSIDE_PADDING;

	GUI_SetColor(BATT_INSIDE_COLOR);
	GUI_FillRoundedRect(battInside.x0, battInside.y0, battInside.x1, battInside.y1, 1);

	// Write percentage as text
	GUI_SetFont(&BATTERY_PCT_FONT);
	GUI_RECT battTextRect;
	battTextRect.x0 = battTip.x1 + 2;
	battTextRect.x1 = battTextRect.x0 + GUI_GetStringDistX("100%");
	battTextRect.y0 = slotRect->y0;
	battTextRect.y1 = slotRect->y1;
	GUI_SetColor(GUI_WHITE);
	GUI_ClearRectEx(&battTextRect);
	char battText[16];
	sprintf(battText, "%3d%%", percentage);
	GUI_SetColor(DEFAULT_SETTINGS_COLOR);
	GUI_DispStringInRectWrap(battText, &battTextRect, GUI_TA_VCENTER, GUI_WRAPMODE_WORD);

	// Is the battery charging
	int batteryChargeXSize = GUI_BMP_GetXSize(icon_batterycharging);
	int batteryChargeYSize = GUI_BMP_GetYSize(icon_batterycharging);

	if( charging )
	{ 
		GUI_BMP_Draw(icon_batterycharging, containerSlots[0].x0 + 5, battTextRect.y0 + 5);
	}
	else
	{
		GUI_SetColor(GUI_WHITE);
		GUI_RECT battChargeRect;
		battChargeRect.x0 = containerSlots[0].x0 + 5;
		battChargeRect.x1 = battChargeRect.x0 + batteryChargeXSize;
		battChargeRect.y0 = slotRect->y0 + 5;
		battChargeRect.y1 = battChargeRect.y0 + batteryChargeYSize;
		GUI_ClearRectEx(&battChargeRect);
	}
}

static void drawBleThreadIndicators()
{
	if(!pages[0].isDisplayed || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return;
	}

	GUI_SetFont(&CONNECTIVITY_FONT);
	GUI_ClearRectEx(&threadBleIconRect);

	ATMO_BLE_GetEnabled(0, &bleEnabled);
	ATMO_THREAD_VOLANSYS_GetEnabled(&threadEnabled);

	if(bleEnabled)
	{
		GUI_SetColor(isBleConnected ? CONNECTIVITY_BLE_CONNECTED_COLOR : CONNECTIVITY_FONT_COLOR);
		GUI_DispStringInRectWrap(CONNECTIVITY_BLE_STR, &threadBleIconRect, GUI_TA_VCENTER | GUI_TA_RIGHT, GUI_WRAPMODE_WORD);
	}
	else if(threadEnabled)
	{
		GUI_SetColor(isBleConnected ? CONNECTIVITY_THREAD_CONNECTED_COLOR : CONNECTIVITY_FONT_COLOR);
		GUI_DispStringInRectWrap(CONNECTIVITY_THREAD_STR, &threadBleIconRect, GUI_TA_VCENTER | GUI_TA_RIGHT, GUI_WRAPMODE_WORD);
	}
}

static void drawDateTime()
{
	if(!pages[0].isDisplayed || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return;
	}
	GUI_SetColor(GUI_WHITE);
	GUI_ClearRectEx(&containerSlots[2]);

	// Draw NXP Logo centered
	int nxpLogoXPos = (DISP_WIDTH - GUI_BMP_GetXSize(icon_nxplogoblack))/2;
	GUI_BMP_Draw(icon_nxplogoblack, nxpLogoXPos, containerSlots[1].y0 + 15);

	// Get time
	ATMO_DateTime_Time_t time;
	ATMO_DateTime_GetDateTime(0, &time);
	struct tm standardTime = ATMO_MK64F_DateTime_AtmoTimeToTm(&time);

	char timeStr[20];
	strftime(timeStr, 20, "%l:%M%P", &standardTime);

	GUI_SetColor(DATE_TIME_COLOR);
	GUI_SetFont(&RAPIDIOT_FONT);
	GUI_DispStringInRectWrap("Rapid IoT", &containerSlots[1], GUI_TA_BOTTOM | GUI_TA_HCENTER, GUI_WRAPMODE_WORD);

	GUI_SetFont(&TIME_FONT);
	GUI_DispStringInRectWrap(timeStr, &containerSlots[2], GUI_TA_BOTTOM | GUI_TA_HCENTER, GUI_WRAPMODE_WORD);

}

static void minChangedCb(void *data)
{
	drawDateTime();
}

static void threadConnectedCb(void *data)
{
	isThreadConnected = true;
	drawBleThreadIndicators();
}

static void threadDisconnectedCb(void *data)
{
	isThreadConnected = false;
	drawBleThreadIndicators();
}

static void bleConnectedCb(void *data)
{
	isBleConnected = true;
	drawBleThreadIndicators();
}

static void bleDisconnectedCb(void *data)
{
	isBleConnected = false;
	drawBleThreadIndicators();
}

static void drawBuildInfo()
{
	// Reserved for build information
}

/**
 * Redraw a whole page
 * @param[in] page - page index
 */
static bool ATMO_UI_SYSTEMSTATUS_DisplayPage(uint32_t page)
{
	const ui_status_pg_t *pg = &pages[page];

	ATMO_UI_PAGE_Margin_t margins;
	ATMO_UI_Page_GetMarginsCurrentDispPage(&margins);

	// Battery
	containerSlots[0].y0 = margins.top;
	containerSlots[0].y1 = containerSlots[0].y0 + 20;

	// Date
	containerSlots[1].y0 = containerSlots[0].y1;
	containerSlots[1].y1 = containerSlots[1].y0 + 80;

	// Time
	containerSlots[2].y0 = containerSlots[1].y1;
	containerSlots[2].y1 = containerSlots[2].y0 + 30;

	// Build Info
	containerSlots[NUM_STATUS_SLOTS - 1].y0 = DISP_HEIGHT - margins.bottom - 10;
	containerSlots[NUM_STATUS_SLOTS - 1].y1 = DISP_HEIGHT - ATMO_UI_Page_GetBottomMargin();

	for(int i = 0; i < NUM_STATUS_SLOTS; i++)
	{
		containerSlots[i].x0 = margins.left;
		containerSlots[i].x1 = DISP_WIDTH - margins.right;
	}

	unsigned int battHeight = (unsigned int)((containerSlots[0].y1 - containerSlots[0].y0) * BATTERY_HEIGHT_RATIO);
	unsigned int battMargin = (containerSlots[0].y1 - containerSlots[0].y0 - battHeight) / 2;
	batteryRect.x0 = containerSlots[0].x0 + 5 + GUI_BMP_GetXSize(icon_batterycharging) + 3;

	// Battery length is adjusted based on the height of its container
	batteryRect.x1 = batteryRect.x0 + 30;
	batteryRect.y0 = containerSlots[0].y0 + battMargin;
	batteryRect.y1 = batteryRect.y0 + battHeight;

	threadBleIconRect.x0 = DISP_WIDTH - margins.right - 65;
	threadBleIconRect.x1 = DISP_WIDTH - margins.right - 5;
	threadBleIconRect.y0 = containerSlots[0].y0;
	threadBleIconRect.y1 = threadBleIconRect.y0 + 20;

	ATMO_UI_Page_ClearCanvas(pg->hasTitle);

	// Draw battery
	ATMO_BATTERY_MK64F_ForceUpdate();

	// Draw main text
	drawBuildInfo();
	drawDateTime();
	drawBleThreadIndicators();
	return true;
}

static void battChangedCb(uint8_t chargeLevel, uint8_t chargeStatus)
{
	drawBattery(&batteryRect, &containerSlots[0], chargeStatus, chargeLevel);
}

bool ATMO_UI_SYSTEMSTATUS_Init(ATMO_UI_PAGE_Config_t *config)
{
	ATMO_UI_Page_DriverInstance_t *driver = ATMO_UI_Page_GetDriverStruct();
	ATMO_DriverInstanceData_t *instanceData = ATMO_UI_Page_GetDriverInstanceDataStruct();
	instanceData->argument = ATMO_UI_Page_GetConfigStruct();

	driver->SetDisplayed = ATMO_UI_SYSTEMSTATUS_SetDisplayed;
	driver->ProcessUserButton = ATMO_UI_SYSTEMSTATUS_ProcessButtonInput;
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

	currentNumPages++;

	// Register page with page controller
	ATMO_UI_Page_AddPage(driver, instanceData, &instanceData->instanceNumber);

	// Register battery callback
	ATMO_BATTERY_MK64F_RegisterStateChangeCallback(battChangedCb);

	ATMO_MK64F_DateTime_RegisterMinChangeCb(minChangedCb);

	ATMO_THREAD_VOLANSYS_RegisterCallbackConnected(threadConnectedCb);
	ATMO_THREAD_VOLANSYS_RegisterCallbackDisconnected(threadDisconnectedCb);
	ATMO_BLE_RegisterEventCallback(0, ATMO_BLE_EVENT_Connected, bleConnectedCb);
	ATMO_BLE_RegisterEventCallback(0, ATMO_BLE_EVENT_Disconnected, bleDisconnectedCb);

	return true;
}

bool ATMO_UI_SYSTEMSTATUS_SetDisplayed(ATMO_DriverInstanceData_t *instance, bool isDisplayed)
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

		return ATMO_UI_SYSTEMSTATUS_DisplayPage(config->templateInstance);
	}

	return true;
}

bool ATMO_UI_SYSTEMSTATUS_ProcessButtonInput(ATMO_DriverInstanceData_t *instance, uint8_t btnIndex)
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

bool ATMO_UI_SYSTEMSTATUS_RegisterButtonAbilityHandle(uint32_t instance, uint8_t btnIndex, unsigned int abilityHandle)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].abilityHandle[btnIndex - 1] = abilityHandle;
	pages[instance].abilityHandleRegistered[btnIndex - 1] = true;
	return true;
}

bool ATMO_UI_SYSTEMSTATUS_RegisterButtonCallback(uint32_t instance, uint8_t btnIndex, ATMO_Callback_t cb)
{
	if(instance >= currentNumPages || btnIndex > ATMO_NUM_BUTTONS || btnIndex < 1 )
	{
		return false;
	}

	pages[instance].callback[btnIndex - 1] = cb;
	return true;
}

bool ATMO_UI_SYSTEMSTATUS_RegisterOnDisplayedAbilityHandle(uint32_t instance, unsigned int abilityHandle)
{
	if(instance >= currentNumPages)
	{
		return false;
	}
	pages[instance].onDisplayAbilityHandleRegistered = true;
	pages[instance].onDisplayAbilityHandle = abilityHandle;

	return true;
}





