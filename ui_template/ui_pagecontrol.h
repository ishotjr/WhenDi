#ifndef __ATMO_UI_PAGECONTROL_H_
#define __ATMO_UI_PAGECONTROL_H_

#include "core.h"

/* Display */
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "GUI.h"
#include "MULTIPAGE.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "emwin_support.h"

#define DISP_WIDTH (176)
#define DISP_HEIGHT (176)

// Some C gore so we can use the struct within itself
typedef struct ATMO_UI_Page_DriverInstance_t ATMO_UI_Page_DriverInstance_t;

struct ATMO_UI_Page_DriverInstance_t{
	bool (*SetDisplayed)(ATMO_DriverInstanceData_t *instanceData, bool displayed);
	bool (*ProcessUserButton)(ATMO_DriverInstanceData_t *instanceData, uint8_t btnIndex);
};

#define ATMO_NUM_BUTTONS (4)
#define ATMO_BUTTON_TOP_RIGHT (0x1)
#define ATMO_BUTTON_BOTTOM_RIGHT (0x2)
#define ATMO_BUTTON_TOP_LEFT (0x4)
#define ATMO_BUTTON_BOTTOM_LEFT (0x8)


typedef enum {
	ATMO_UI_PAGE_NAV_UP = 0,
	ATMO_UI_PAGE_NAV_DOWN,
	ATMO_UI_PAGE_NAV_LEFT,
	ATMO_UI_PAGE_NAV_RIGHT,
} ATMO_UI_PAGE_NAV_t;

#define ATMO_BUTTON_LABEL_MAXLEN (16)

typedef struct {
	unsigned int top;
	unsigned int bottom;
	unsigned int left;
	unsigned int right;
} ATMO_UI_PAGE_Margin_t;

typedef struct {
	int x;
	int y;
	int spanX;
	int spanY;
	bool hidden;
	bool titleHidden;
	GUI_COLOR textColor;
	uint8_t activeButtons;
	char topLeftButtonLabel[ATMO_BUTTON_LABEL_MAXLEN];
	char topRightButtonLabel[ATMO_BUTTON_LABEL_MAXLEN];
	char bottomLeftButtonLabel[ATMO_BUTTON_LABEL_MAXLEN];
	char bottomRightButtonLabel[ATMO_BUTTON_LABEL_MAXLEN];
	uint32_t templateInstance; // Unique ID used within template
	uint32_t instance; // Template specific identifier
	char *title; // Page title
} ATMO_UI_PAGE_Config_t;

typedef struct {
	bool enableUpDownNavLabels;
	bool enableLeftRightNavLabels;
} ATMO_UI_PAGE_CONTROLLER_Config_t;

/* Exported Function Prototypes -----------------------------------------------*/


ATMO_Status_t ATMO_UI_Page_Init();
ATMO_Status_t ATMO_UI_Page_SetConfiguration(ATMO_UI_PAGE_CONTROLLER_Config_t *config);
ATMO_Status_t ATMO_UI_Page_PostInit();
ATMO_Status_t ATMO_UI_Page_AddPage(const ATMO_UI_Page_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber);
ATMO_Status_t ATMO_UI_Page_DisplayPage(unsigned int pageIndex);
ATMO_Status_t ATMO_UI_Page_DisplayPageByCoord(int x, int y, bool filterHidden);
ATMO_Status_t ATMO_UI_Page_DisplayRootPage();
ATMO_Status_t ATMO_UI_Page_ProcessNavButton(ATMO_UI_PAGE_NAV_t nav);
ATMO_Status_t ATMO_UI_Page_ProcessUserButton(uint8_t btnIndex);
ATMO_UI_PAGE_Config_t *ATMO_UI_Page_GetConfigStruct();
ATMO_UI_Page_DriverInstance_t *ATMO_UI_Page_GetDriverStruct();
ATMO_DriverInstanceData_t *ATMO_UI_Page_GetDriverInstanceDataStruct();
bool ATMO_UI_Page_GetInitComplete();
void ATMO_UI_Page_SetDisplayEnabled(bool enabled);
bool ATMO_UI_Page_GetDisplayEnabled();

// Get the margins for the currently displayed page
// This take into account the pages around the current page in order to maximize the canvas area
void ATMO_UI_Page_GetMarginsCurrentDispPage(ATMO_UI_PAGE_Margin_t *margins);

// Get the margins for any generic page
// These aren't page specific
// These can be called during page init
// They will always work but don't have the smarts to maximize your canvas area
uint32_t ATMO_UI_Page_GetTopMargin(bool withTitle);
uint32_t ATMO_UI_Page_GetBottomMargin();
uint32_t ATMO_UI_Page_GetLeftMargin();
uint32_t ATMO_UI_Page_GetRightMargin();
void ATMO_UI_Page_ClearCanvas(bool withTitle);

static inline uint8_t ATMO_UI_Page_GetButtonMask(bool button1Enabled, bool button2Enabled, bool button3Enabled, bool button4Enabled)
{
	uint8_t mask = button1Enabled ? ATMO_BUTTON_TOP_RIGHT : 0;
	mask |= button2Enabled ? ATMO_BUTTON_BOTTOM_RIGHT : 0;
	mask |= button3Enabled ? ATMO_BUTTON_TOP_LEFT : 0;
	mask |= button4Enabled ? ATMO_BUTTON_BOTTOM_LEFT : 0;
	return mask;
}

#endif
