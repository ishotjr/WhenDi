#include "ui_pagecontrol.h"
#include "battery_mk64f.h"

#define ATMO_UI_MAX_NUM_PAGES (50)
#define ATMO_UI_BG_COLOR (GUI_WHITE)

#define ATMO_UI_TITLE_HEIGHT (20)
#define ATMO_UI_TITLE_FONT (GUI_Font20_1)
#define ATMO_UI_TITLE_COLOR (GUI_BLACK)

#define BTN_ARROW_BASE (18)
#define BTN_ARROW_HEIGHT (9)
#define BTN_ARROW_TOP_BOT_PAD (6)
#define BTN_ARROW_COLOR (GUI_BLACK)

#define BTN_ARROW_LABEL_FONT (GUI_Font13B_ASCII)
#define BTN_ARROW_LABEL_FONT_COLOR (GUI_BLACK)
#define BTN_ARROW_LABEL_FONT_HEIGHT (13)

#define NAV_BAR_COLOR (GUI_BLACK)
#define NAV_BAR_LENGTH (DISP_HEIGHT / 3)
#define NAV_BAR_WIDTH (3)
#define NAV_BAR_EDGE_PADDING (2)

#define NAV_BAR_TITLE_PADDING (3)
#define NAV_BAR_TITLE_FONT (GUI_Font13B_ASCII)
#define NAV_BAR_TITLE_WIDTH (13)
#define NAV_BAR_TITLE_FONT_COLOR (GUI_BLACK)
#define NAV_BAR_TITLE_LENGTH (DISP_WIDTH - 30)

static ATMO_DriverInstanceHandle_t numberOfPageDriverInstance = 0;

static unsigned int currentDisplayedPage = 0;

static ATMO_UI_PAGE_Config_t pageConfigs[ATMO_UI_MAX_NUM_PAGES];
static ATMO_UI_Page_DriverInstance_t pages[ATMO_UI_MAX_NUM_PAGES];
static ATMO_DriverInstanceData_t pageInstanceData[ATMO_UI_MAX_NUM_PAGES];

static unsigned int currentNumPages = 0;

#define ATMO_UI_PAGE_DIR_MASK_UP (0x1)
#define ATMO_UI_PAGE_DIR_MASK_DOWN (0x2)
#define ATMO_UI_PAGE_DIR_MASK_LEFT (0x4)
#define ATMO_UI_PAGE_DIR_MASK_RIGHT (0x8)

static GUI_RECT titleRect;

static ATMO_UI_PAGE_CONTROLLER_Config_t controllerConfig;

static unsigned int ATMO_UI_Page_GetNextPageUp();
static unsigned int ATMO_UI_Page_GetNextPageDown();
static unsigned int ATMO_UI_Page_GetNextPageLeft();
static unsigned int ATMO_UI_Page_GetNextPageRight();
static uint8_t ATMO_UI_Page_GetPossibleDirMask(unsigned int *pageHandles);
static void ATMO_UI_Page_DrawArrow(GUI_POINT *points, uint32_t numPoints, bool erase, const char *label, bool isLeft, unsigned int labelId);
static void ATMO_UI_Page_DrawActiveButtons();
static void ATMO_UI_Page_DrawNavBar(GUI_RECT *rect, GUI_RECT *navTitleRect, unsigned int pgHandle, bool erase, const GUI_ROTATION * rotateFlags, unsigned int dir);
static void ATMO_UI_Page_DrawNavBars();

static bool pageInitComplete = false;
static bool _ATMO_UI_Page_DisplayEnabled = true;

ATMO_Status_t ATMO_UI_Page_Init()
{
	memset(pages, 0, sizeof(pages));
	memset(pageInstanceData, 0, sizeof(pageInstanceData));
	controllerConfig.enableUpDownNavLabels = true;
	controllerConfig.enableLeftRightNavLabels = true;

	// Build rect
	titleRect.x0 = 0;
	titleRect.x1 = DISP_WIDTH;
	titleRect.y0 = BTN_ARROW_HEIGHT + NAV_BAR_TITLE_WIDTH + NAV_BAR_TITLE_PADDING + 8;
	titleRect.y1 = titleRect.y0 + ATMO_UI_TITLE_HEIGHT;

	return ATMO_Status_Success;
}

ATMO_Status_t ATMO_UI_Page_SetConfiguration(ATMO_UI_PAGE_CONTROLLER_Config_t *config)
{
	memcpy(&controllerConfig, config, sizeof(controllerConfig));
	return ATMO_Status_Success;
}

ATMO_Status_t ATMO_UI_Page_PostInit()
{
	GUI_SetBkColor(ATMO_UI_BG_COLOR);
	GUI_Clear();
	pageInitComplete = true;
	return ATMO_Status_Success;
}

ATMO_Status_t ATMO_UI_Page_AddPage(const ATMO_UI_Page_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber)
{
	ATMO_PLATFORM_DebugPrint("Adding Page: %d\r\n", currentNumPages);

	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)driverInstanceData->argument;

	if(currentNumPages < ATMO_UI_MAX_NUM_PAGES)
	{
		currentNumPages++;
	}
	else
	{
		return ATMO_Status_Fail;
	}

	// TODO nick this number is pretty much useless now
	*instanceNumber = numberOfPageDriverInstance++;

	return ATMO_Status_Success;
}

static void ATMO_UI_Page_DisplayPageTitle(unsigned int pageNum)
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[pageNum].argument;
	GUI_SetFont(&ATMO_UI_TITLE_FONT);
	GUI_SetColor(config->textColor);

	// Clear the invisible bounding rectangle the measurement lives inside of
	GUI_ClearRectEx(&titleRect);

	// Display the new value
	GUI_DispStringInRectWrap(((!config->titleHidden) && (pageInstanceData[pageNum].name != NULL)) ? pageInstanceData[pageNum].name : "", &titleRect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
}

static bool isWithin(int targetP1, int targetP2, int currentP1, int currentP2)
{
	// the smallest one should be within the biggest one
	if(abs(targetP2 - targetP1) > abs(currentP2 - currentP1))
	{
		// Target is bigger
		// current should be within target
		return (currentP1 >= targetP1 && currentP2 <= targetP2);
	}
	else
	{
		return (targetP1 >= currentP1 && targetP2 <= currentP2);
	}

}

static int ATMO_UI_Page_FindByCoord(int x, int y, bool filterHidden)
{
	for(unsigned int i = 0; i < currentNumPages; i++)
	{

		ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(isWithin(config->x, config->x + config->spanX - 1, x, x) && isWithin(config->y, config->y + config->spanY - 1, y, y))
		{
			return i;
		}
	}

	return -1;
}

ATMO_Status_t ATMO_UI_Page_DisplayRootPage()
{
	if( ATMO_UI_Page_DisplayPageByCoord(0,0, true) == ATMO_Status_Fail )
	{
		return ATMO_UI_Page_DisplayPage(0);
	}

	return ATMO_Status_Success;
}

ATMO_Status_t ATMO_UI_Page_DisplayPageByCoord(int x, int y, bool filterHidden)
{
	int pageIndex = ATMO_UI_Page_FindByCoord(x,y, filterHidden);
	if(pageIndex < 0)
	{
		return ATMO_Status_Fail;
	}

	return ATMO_UI_Page_DisplayPage(pageIndex);
}

ATMO_Status_t ATMO_UI_Page_DisplayPage(unsigned int pageIndex)
{
	if(pageIndex >= currentNumPages || !_ATMO_UI_Page_DisplayEnabled)
	{
		return ATMO_Status_Fail;
	}

	// Set current page displayed to false
	const ATMO_UI_Page_DriverInstance_t *pg = &pages[currentDisplayedPage];
	if( !pg->SetDisplayed(&pageInstanceData[currentDisplayedPage], false) )
	{
		return ATMO_Status_Fail;
	}

	// Clear the whole screen
	GUI_RECT screenRect;
	screenRect.x0 = 0;
	screenRect.x1 = DISP_WIDTH;
	screenRect.y0 = 0;
	screenRect.y1 = DISP_HEIGHT;
	GUI_SetColor(ATMO_UI_BG_COLOR);
	GUI_ClearRectEx(&screenRect);

	currentDisplayedPage = pageIndex;

	ATMO_UI_Page_DisplayPageTitle(pageIndex);
	ATMO_UI_Page_DrawNavBars();
	ATMO_UI_Page_DrawActiveButtons();

	// Display next page
	pg = &pages[pageIndex];
	if( !pg->SetDisplayed(&pageInstanceData[pageIndex], true) )
	{
		return ATMO_Status_Fail;
	}

	return ATMO_Status_Success;

}
static unsigned int ATMO_UI_Page_GetNextPageUp()
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;

	int lowestVal = 0xFFFF;
	int lowestIndex = -1;

	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(i == currentDisplayedPage)
		{
			continue;
		}

		// X must be within span
		if(!isWithin(pg->x, pg->x + pg->spanX, config->x, config->x + config->spanX))
		{
			continue;
		}

		// The difference between the top of our span
		// and the bottom of the target's Y span
		int diff = (config->y) - (pg->y + pg->spanY - 1);

		if(diff > 0 && diff < lowestVal)
		{
			lowestVal = diff;
			lowestIndex = i;

			if(abs(diff) == 1)
			{
				break;
			}
		}
	}

	if(lowestIndex == -1)
	{
		return currentDisplayedPage;
	}

	ATMO_UI_PAGE_Config_t *lowestPg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[lowestIndex].argument;
	int closestX = 0xFFFF;

	// Now that we've found the first page closest in terms of Y
	// Find the page on that Y axis that's closest to our X starting position
	// This could be more efficient, but it's fine...
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(pg->y == lowestPg->y && isWithin(pg->x, pg->x + pg->spanX, config->x, config->x + config->spanX))
		{
			int xDiff = abs(config->x - pg->x);

			if(xDiff < closestX)
			{
				closestX = xDiff;
				lowestIndex = i;
			}
		}
	}

	if(pageConfigs[lowestIndex].hidden)
	{
		return currentDisplayedPage;
	}

	return lowestIndex;

}

static unsigned int ATMO_UI_Page_GetNextPageDown()
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;

	int lowestVal = 0xFFFF;
	int lowestIndex = -1;

	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(i == currentDisplayedPage)
		{
			continue;
		}

		// X must be within span
		if(!isWithin(pg->x, pg->x + pg->spanX, config->x, config->x + config->spanX))
		{
			continue;
		}

		// The difference between the bottom of our Y span
		// and the top of the target's Y span
		int diff = (config->y + config->spanY - 1) - (pg->y);

		if(diff < 0 && abs(diff) < lowestVal)
		{
			lowestVal = abs(diff);
			lowestIndex = i;

			if(abs(diff) == 1)
			{
				break;
			}
		}
	}

	if(lowestIndex == -1)
	{
		return currentDisplayedPage;
	}

	ATMO_UI_PAGE_Config_t *lowestPg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[lowestIndex].argument;
	int closestX = 0xFFFF;

	// Now that we've found the first page closest in terms of Y
	// Find the page on that Y axis that's closest to our X starting position
	// This could be more efficient, but it's fine...
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(pg->y == lowestPg->y && isWithin(pg->x, pg->x + pg->spanX, config->x, config->x + config->spanX))
		{
			int xDiff = abs(config->x - pg->x);

			if(xDiff < closestX)
			{
				closestX = xDiff;
				lowestIndex = i;
			}
		}
	}

	if(pageConfigs[lowestIndex].hidden)
	{
		return currentDisplayedPage;
	}

	return lowestIndex;
}

static unsigned int ATMO_UI_Page_GetNextPageLeft()
{
	// Find the page with the next lowest
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;

	int lowestVal = 0xFFFF;
	int lowestIndex = -1;

	// Go through every page
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(i == currentDisplayedPage)
		{
			continue;
		}

		// Y must be within span
		if(!isWithin(pg->y, pg->y + pg->spanY, config->y, config->y + config->spanY))
		{
			continue;
		}

		// The difference between the our X span
		// And the rightmode side of the target's X span
		int diff = (config->x) - (pg->x + pg->spanX - 1);

		if(diff > 0 && diff < lowestVal)
		{
			lowestVal = diff;
			lowestIndex = i;

			if(abs(diff) == 1)
			{
				break;
			}
		}
	}

	if(lowestIndex == -1)
	{
		return currentDisplayedPage;
	}

	ATMO_UI_PAGE_Config_t *lowestPg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[lowestIndex].argument;
	int closestY = 0xFFFF;

	// Now that we've found the first page closest in terms of X
	// Find the page on that Y axis that's closest to our Y starting position
	// This could be more efficient, but it's fine...
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(pg->x == lowestPg->x && isWithin(pg->y, pg->y + pg->spanY, config->y, config->y + config->spanY))
		{
			int yDiff = abs(config->y - pg->y);

			if(yDiff < closestY)
			{
				closestY = yDiff;
				lowestIndex = i;
			}
		}
	}

	if(pageConfigs[lowestIndex].hidden)
	{
		return currentDisplayedPage;
	}


	return lowestIndex;
}

static unsigned int ATMO_UI_Page_GetNextPageRight()
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;

	int lowestVal = 0xFFFF;
	int lowestIndex = -1;

	// Go through every page
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(i == currentDisplayedPage)
		{
			continue;
		}

		// Y must be within span
		if(!isWithin(pg->y, pg->y + pg->spanY, config->y, config->y + config->spanY))
		{
			continue;
		}

		// The difference between the rightmode side of our X span
		// and the leftmode side of the target's X span
		int diff = (config->x + config->spanX - 1) - (pg->x);

		if(diff < 0 && abs(diff) < lowestVal)
		{
			lowestVal = abs(diff);
			lowestIndex = i;

			if(abs(diff) == 1)
			{
				break;
			}
		}
	}

	if(lowestIndex == -1)
	{
		return currentDisplayedPage;
	}

	ATMO_UI_PAGE_Config_t *lowestPg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[lowestIndex].argument;
	int closestY = 0xFFFF;

	// Now that we've found the first page closest in terms of X
	// Find the page on that Y axis that's closest to our Y starting position
	// This could be more efficient, but it's fine...
	for(int i = 0; i < currentNumPages; i++)
	{
		ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[i].argument;

		if(pg->x == lowestPg->x && isWithin(pg->y, pg->y + pg->spanY, config->y, config->y + config->spanY))
		{
			int yDiff = abs(config->y - pg->y);

			if(yDiff < closestY)
			{
				closestY = yDiff;
				lowestIndex = i;
			}
		}
	}

	if(pageConfigs[lowestIndex].hidden)
	{
		return currentDisplayedPage;
	}

	return lowestIndex;
}

static uint8_t ATMO_UI_Page_GetPossibleDirMask(unsigned int *pageHandles)
{
	uint8_t mask = 0;

	unsigned int pageHandle = ATMO_UI_Page_GetNextPageUp();

	if(pageHandle != currentDisplayedPage)
	{
		pageHandles[ATMO_UI_PAGE_NAV_UP] = pageHandle;
		mask |= ATMO_UI_PAGE_DIR_MASK_UP;
	}

	pageHandle = ATMO_UI_Page_GetNextPageDown();

	if(pageHandle != currentDisplayedPage)
	{
		pageHandles[ATMO_UI_PAGE_NAV_DOWN] = pageHandle;
		mask |= ATMO_UI_PAGE_DIR_MASK_DOWN;
	}

	pageHandle = ATMO_UI_Page_GetNextPageLeft();

	if(pageHandle != currentDisplayedPage)
	{
		pageHandles[ATMO_UI_PAGE_NAV_LEFT] = pageHandle;
		mask |= ATMO_UI_PAGE_DIR_MASK_LEFT;
	}

	pageHandle = ATMO_UI_Page_GetNextPageRight();

	if(pageHandle != currentDisplayedPage)
	{
		pageHandles[ATMO_UI_PAGE_NAV_RIGHT] = pageHandle;
		mask |= ATMO_UI_PAGE_DIR_MASK_RIGHT;
	}

	return mask;
}

static void ATMO_UI_Page_DrawArrow(GUI_POINT *points, uint32_t numPoints, bool erase, const char *label, bool isLeft, unsigned int labelId)
{
	if(!erase)
	{
		GUI_SetColor(BTN_ARROW_COLOR);
		GUI_FillPolygon(points, numPoints, 0, 0);
	}

	// If there's a label
	if((!erase) && (strlen(label) > 0))
	{
		GUI_RECT buttonLabelRect;
		uint32_t labelOffset = (BTN_ARROW_LABEL_FONT_HEIGHT - BTN_ARROW_HEIGHT) / 2;
		GUI_SetFont(&BTN_ARROW_LABEL_FONT);
		unsigned int dist = GUI_GetStringDistX(label);
		buttonLabelRect.x0 = isLeft ? (BTN_ARROW_HEIGHT + 2) : (DISP_WIDTH - BTN_ARROW_HEIGHT - 2 - dist);
		buttonLabelRect.x1 = buttonLabelRect.x0 + dist;
		buttonLabelRect.y0 = points[0].y - labelOffset;
		buttonLabelRect.y1 = buttonLabelRect.y0 + GUI_GetFontSizeY();

		GUI_SetColor(ATMO_UI_BG_COLOR);
		GUI_ClearRectEx(&buttonLabelRect);
		GUI_SetColor(BTN_ARROW_LABEL_FONT_COLOR);
		GUI_DispStringInRectWrap(label, &buttonLabelRect, GUI_TA_VCENTER | (isLeft ? GUI_TA_LEFT : GUI_TA_RIGHT), GUI_WRAPMODE_NONE);
	}
}

static void ATMO_UI_Page_DrawActiveButtons()
{
	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;
	GUI_POINT points[3];

	// Draw top right arrow
	points[0].x = DISP_WIDTH - BTN_ARROW_HEIGHT;
	points[0].y = BTN_ARROW_TOP_BOT_PAD;

	points[1].x = points[0].x;
	points[1].y = points[0].y + BTN_ARROW_HEIGHT;

	points[2].x = DISP_WIDTH;
	points[2].y = (points[0].y + points[1].y) / 2;

	ATMO_UI_Page_DrawArrow(points, 3, !(config->activeButtons & ATMO_BUTTON_TOP_RIGHT), config->topRightButtonLabel, false, 0);

	// Draw bottom right
	points[0].x = DISP_WIDTH - BTN_ARROW_HEIGHT;
	points[0].y = DISP_WIDTH - BTN_ARROW_HEIGHT - BTN_ARROW_TOP_BOT_PAD;

	points[1].x = points[0].x;
	points[1].y = points[0].y + BTN_ARROW_HEIGHT;

	points[2].x = DISP_WIDTH;
	points[2].y = (points[0].y + points[1].y) / 2;

	ATMO_UI_Page_DrawArrow(points, 3, !(config->activeButtons & ATMO_BUTTON_BOTTOM_RIGHT), config->bottomRightButtonLabel, false, 1);

	// Draw top left
	points[0].x = BTN_ARROW_HEIGHT;
	points[0].y = BTN_ARROW_TOP_BOT_PAD;

	points[1].x = points[0].x;
	points[1].y = points[0].y + BTN_ARROW_HEIGHT;

	points[2].x = 0;
	points[2].y = (points[0].y + points[1].y) / 2;

	ATMO_UI_Page_DrawArrow(points, 3, !(config->activeButtons & ATMO_BUTTON_TOP_LEFT), config->topLeftButtonLabel, true, 2);

	// Draw bottom right
	points[0].x = BTN_ARROW_HEIGHT;
	points[0].y = DISP_WIDTH - BTN_ARROW_HEIGHT - BTN_ARROW_TOP_BOT_PAD;

	points[1].x = points[0].x;
	points[1].y = points[0].y + BTN_ARROW_HEIGHT;

	points[2].x = 0;
	points[2].y = (points[0].y + points[1].y) / 2;

	ATMO_UI_Page_DrawArrow(points, 3, !(config->activeButtons & ATMO_BUTTON_BOTTOM_LEFT), config->bottomLeftButtonLabel, true, 3);
}

static void ATMO_UI_Page_DrawNavBar(GUI_RECT *rect, GUI_RECT *navTitleRect, unsigned int pgHandle, bool erase, const GUI_ROTATION * rotateFlags, unsigned int dir)
{
	ATMO_UI_PAGE_Config_t *pg = (ATMO_UI_PAGE_Config_t *)pageInstanceData[pgHandle].argument;

	if(pg->hidden)
	{
		erase = true;
	}

	if(erase)
	{
		GUI_SetColor(ATMO_UI_BG_COLOR);
	}
	else
	{
		GUI_SetColor(NAV_BAR_COLOR);
	}

	GUI_FillRoundedRect(rect->x0, rect->y0, rect->x1, rect->y1, NAV_BAR_WIDTH/3);

	if(pageInstanceData[pgHandle].name != NULL && !pg->hidden)
	{
		unsigned int nameLen = strlen(pageInstanceData[pgHandle].name);
		char nameUpper[nameLen + 1];
		nameUpper[nameLen] = 0;

		for(unsigned int i = 0; i < nameLen; i++)
		{
			nameUpper[i] = toupper(pageInstanceData[pgHandle].name[i]);
		}
		
		GUI_SetFont(&NAV_BAR_TITLE_FONT);
		GUI_SetColor(ATMO_UI_BG_COLOR);
		GUI_ClearRectEx(navTitleRect);
		if(!erase)
		{
			if((controllerConfig.enableUpDownNavLabels && (dir == ATMO_UI_PAGE_NAV_UP)) ||
				(controllerConfig.enableUpDownNavLabels && (dir == ATMO_UI_PAGE_NAV_DOWN)) ||
				(controllerConfig.enableLeftRightNavLabels && (dir == ATMO_UI_PAGE_NAV_LEFT)) ||
				(controllerConfig.enableLeftRightNavLabels && (dir == ATMO_UI_PAGE_NAV_RIGHT)))
			{
				GUI_SetColor(NAV_BAR_TITLE_FONT_COLOR);
				GUI_DispStringInRectWrapEx(nameUpper, navTitleRect, GUI_TA_VCENTER | GUI_TA_HCENTER, GUI_WRAPMODE_NONE, rotateFlags);
			}
		}
	}
	

}

static void ATMO_UI_Page_DrawNavBars()
{
	unsigned int pageHandles[4] = {0};
	uint8_t mask = ATMO_UI_Page_GetPossibleDirMask(pageHandles); // Which ways can the user navigate?
	GUI_RECT rect, navTitleRect;

	// Draw left bar
	rect.x0 = NAV_BAR_EDGE_PADDING;
	rect.x1 = NAV_BAR_EDGE_PADDING + NAV_BAR_WIDTH;
	rect.y0 = (DISP_HEIGHT - NAV_BAR_LENGTH)/2;
	rect.y1 = rect.y0 + NAV_BAR_LENGTH;

	navTitleRect.x0 = rect.x1 + NAV_BAR_TITLE_PADDING;
	navTitleRect.x1 = navTitleRect.x0 + NAV_BAR_TITLE_WIDTH;
	navTitleRect.y0 = (DISP_HEIGHT - NAV_BAR_TITLE_LENGTH)/2;
	navTitleRect.y1 = navTitleRect.y0 + NAV_BAR_TITLE_LENGTH;

	ATMO_UI_Page_DrawNavBar(&rect, &navTitleRect, pageHandles[ATMO_UI_PAGE_NAV_LEFT], !(mask & ATMO_UI_PAGE_DIR_MASK_LEFT), GUI_ROTATE_CW, ATMO_UI_PAGE_NAV_LEFT);

	// Draw down bar
	rect.x0 = (DISP_WIDTH - NAV_BAR_LENGTH)/2;
	rect.x1 = rect.x0 + NAV_BAR_LENGTH;
	rect.y0 = DISP_HEIGHT - NAV_BAR_WIDTH - NAV_BAR_EDGE_PADDING;
	rect.y1 = rect.y0 + NAV_BAR_WIDTH;

	navTitleRect.y0 = rect.y0 - NAV_BAR_TITLE_WIDTH - NAV_BAR_TITLE_PADDING;
	navTitleRect.y1 = navTitleRect.y0 + NAV_BAR_TITLE_WIDTH;
	navTitleRect.x0 = (DISP_WIDTH - NAV_BAR_TITLE_LENGTH)/2;
	navTitleRect.x1 = navTitleRect.x0 + NAV_BAR_TITLE_LENGTH;

	ATMO_UI_Page_DrawNavBar(&rect, &navTitleRect, pageHandles[ATMO_UI_PAGE_NAV_DOWN], !(mask & ATMO_UI_PAGE_DIR_MASK_DOWN), GUI_ROTATE_0, ATMO_UI_PAGE_NAV_DOWN);

	// Draw right bar
	rect.x0 = DISP_WIDTH - NAV_BAR_WIDTH - NAV_BAR_EDGE_PADDING;
	rect.x1 = rect.x0 + NAV_BAR_WIDTH;
	rect.y0 = (DISP_HEIGHT - NAV_BAR_LENGTH)/2;
	rect.y1 = rect.y0 + NAV_BAR_LENGTH;

	navTitleRect.x0 = rect.x0 - NAV_BAR_TITLE_WIDTH - NAV_BAR_TITLE_PADDING;
	navTitleRect.x1 = navTitleRect.x0 + NAV_BAR_TITLE_WIDTH;
	navTitleRect.y0 = (DISP_HEIGHT - NAV_BAR_TITLE_LENGTH)/2;
	navTitleRect.y1 = navTitleRect.y0 + NAV_BAR_TITLE_LENGTH;

	ATMO_UI_Page_DrawNavBar(&rect, &navTitleRect, pageHandles[ATMO_UI_PAGE_NAV_RIGHT], !(mask & ATMO_UI_PAGE_DIR_MASK_RIGHT), GUI_ROTATE_CCW, ATMO_UI_PAGE_NAV_RIGHT);

	// Draw up bar
	rect.x0 = (DISP_WIDTH - NAV_BAR_LENGTH)/2;
	rect.x1 = rect.x0 + NAV_BAR_LENGTH;
	rect.y0 = NAV_BAR_EDGE_PADDING;
	rect.y1 = NAV_BAR_EDGE_PADDING + NAV_BAR_WIDTH;

	navTitleRect.y0 = rect.y1 + NAV_BAR_TITLE_PADDING;
	navTitleRect.y1 = navTitleRect.y0 + NAV_BAR_TITLE_WIDTH;
	navTitleRect.x0 = (DISP_WIDTH - NAV_BAR_TITLE_LENGTH)/2;
	navTitleRect.x1 = navTitleRect.x0 + NAV_BAR_TITLE_LENGTH;

	ATMO_UI_Page_DrawNavBar(&rect, &navTitleRect, pageHandles[ATMO_UI_PAGE_NAV_UP], !(mask & ATMO_UI_PAGE_DIR_MASK_UP), GUI_ROTATE_0, ATMO_UI_PAGE_NAV_UP);


}

ATMO_Status_t ATMO_UI_Page_ProcessNavButton(ATMO_UI_PAGE_NAV_t nav)
{
	if(!ATMO_PLATFORM_IsInitComplete() || !ATMO_UI_Page_GetDisplayEnabled())
	{
		return ATMO_Status_Success;
	}
	
	unsigned int nextIndex = 0;
	switch(nav)
	{
		case ATMO_UI_PAGE_NAV_UP:
		{
			nextIndex = ATMO_UI_Page_GetNextPageUp();
			break;
		}
		case ATMO_UI_PAGE_NAV_DOWN:
		{
			nextIndex = ATMO_UI_Page_GetNextPageDown();
			break;
		}
		case ATMO_UI_PAGE_NAV_LEFT:
		{
			nextIndex = ATMO_UI_Page_GetNextPageLeft();
			break;
		}
		case ATMO_UI_PAGE_NAV_RIGHT:
		{
			nextIndex = ATMO_UI_Page_GetNextPageRight();
			break;
		}
		default:
		{
			break;
		}
	}

	if(nextIndex == currentDisplayedPage || pageConfigs[nextIndex].hidden)
	{
		return ATMO_Status_Success;
	}

	ATMO_UI_Page_DisplayPage(nextIndex);

	return ATMO_Status_Success;
}

ATMO_Status_t ATMO_UI_Page_ProcessUserButton(uint8_t btnIndex)
{
	if(!ATMO_UI_Page_GetDisplayEnabled())
	{
		return ATMO_Status_Success;
	}

	ATMO_UI_Page_DriverInstance_t *pg = &pages[currentDisplayedPage];
	pg->ProcessUserButton(&pageInstanceData[currentDisplayedPage], btnIndex);
	return ATMO_Status_Success;
}

static inline uint32_t ATMO_UI_Page_GetBaseMargin(bool enableLabels)
{
	return BTN_ARROW_TOP_BOT_PAD + BTN_ARROW_HEIGHT + (enableLabels ? NAV_BAR_TITLE_WIDTH + NAV_BAR_TITLE_PADDING : 0);
}

void ATMO_UI_Page_GetMarginsCurrentDispPage(ATMO_UI_PAGE_Margin_t *margins)
{
	unsigned int pageHandles[4];
	uint8_t mask = ATMO_UI_Page_GetPossibleDirMask(pageHandles);

	ATMO_UI_PAGE_Config_t *config = (ATMO_UI_PAGE_Config_t *)pageInstanceData[currentDisplayedPage].argument;

	// Does the page have a title?
	bool hasTitle = ((!config->titleHidden) && (pageInstanceData[currentDisplayedPage].name != NULL) && (strcmp(pageInstanceData[currentDisplayedPage].name, "") != 0));

	if(mask & ATMO_UI_PAGE_DIR_MASK_UP)
	{
		margins->top = ATMO_UI_Page_GetTopMargin(hasTitle);
	}
	else
	{
		if(hasTitle)
		{
			margins->top = titleRect.y1 + 5;
		}
		else
		{
			margins->top = ATMO_UI_Page_GetBaseMargin(false);
		}
	}

	if(mask & ATMO_UI_PAGE_DIR_MASK_DOWN)
	{
		margins->bottom = ATMO_UI_Page_GetBottomMargin();
	}
	else
	{
		margins->bottom = ATMO_UI_Page_GetBaseMargin(false);
	}

	if(mask & ATMO_UI_PAGE_DIR_MASK_LEFT)
	{
		margins->left = ATMO_UI_Page_GetLeftMargin();
	}
	else
	{
		margins->left = ATMO_UI_Page_GetBaseMargin(false);
	}

	if(mask & ATMO_UI_PAGE_DIR_MASK_RIGHT)
	{
		margins->right = ATMO_UI_Page_GetRightMargin();
	}
	else
	{
		margins->right = ATMO_UI_Page_GetBaseMargin(false);
	}
}

uint32_t ATMO_UI_Page_GetTopMargin(bool withTitle)
{
	if(withTitle)
	{
		return titleRect.y1 + 2;
	}

	return ATMO_UI_Page_GetBaseMargin(controllerConfig.enableUpDownNavLabels);
}

uint32_t ATMO_UI_Page_GetBottomMargin()
{
	return ATMO_UI_Page_GetBaseMargin(controllerConfig.enableUpDownNavLabels);
}

uint32_t ATMO_UI_Page_GetLeftMargin()
{
	return ATMO_UI_Page_GetBaseMargin(controllerConfig.enableLeftRightNavLabels);
}

uint32_t ATMO_UI_Page_GetRightMargin()
{
	return ATMO_UI_Page_GetBaseMargin(controllerConfig.enableLeftRightNavLabels);
}

void ATMO_UI_Page_ClearCanvas(bool withTitle)
{
	int canvasWidth = DISP_WIDTH - ATMO_UI_Page_GetRightMargin() - ATMO_UI_Page_GetLeftMargin();
	int canvasHeight = DISP_HEIGHT - ATMO_UI_Page_GetBottomMargin() - ATMO_UI_Page_GetTopMargin(withTitle);
	GUI_RECT canvasRect;
	canvasRect.x0 = ATMO_UI_Page_GetLeftMargin();
	canvasRect.y0 = ATMO_UI_Page_GetTopMargin(withTitle);
	canvasRect.x1 = canvasRect.x0 + canvasWidth;
	canvasRect.y1 = canvasRect.y0 + canvasHeight;
	GUI_SetColor(GUI_WHITE);
	GUI_ClearRectEx(&canvasRect);
	return;
}

bool ATMO_UI_Page_GetInitComplete()
{
	return pageInitComplete;
}

ATMO_UI_PAGE_Config_t *ATMO_UI_Page_GetConfigStruct()
{
	return &pageConfigs[currentNumPages];
}

ATMO_UI_Page_DriverInstance_t *ATMO_UI_Page_GetDriverStruct()
{
	return &pages[currentNumPages];
}

ATMO_DriverInstanceData_t *ATMO_UI_Page_GetDriverInstanceDataStruct()
{
	return &pageInstanceData[currentNumPages];
}

// Turn display off
void ATMO_UI_Page_SetDisplayEnabled(bool enabled)
{
	// Turning the display on
	if(enabled && !_ATMO_UI_Page_DisplayEnabled)
	{
		Display_Connect();
		Backlight_SetLevel(BLIGHT_LEVEL_HIGH);
		_ATMO_UI_Page_DisplayEnabled = true;
		ATMO_UI_Page_PostInit();
		ATMO_UI_Page_DisplayPage(currentDisplayedPage);
	}
	else if(!enabled && _ATMO_UI_Page_DisplayEnabled)
	{
		_ATMO_UI_Page_DisplayEnabled = false;
		Display_Disconnect();
		Backlight_SetLevel(BLIGHT_LEVEL_OFF);
	}


}

bool ATMO_UI_Page_GetDisplayEnabled()
{
	return _ATMO_UI_Page_DisplayEnabled;
}
