#include "atmosphere_callbacks.h"

//HEADER START
static int s_feeds = 0;
static int s_changes = 0;
static int s_sleeps = 0;
static int s_wakes = 0;
//HEADER END

void ATMO_Setup() {

    // set RTC (since RIoT has no battery)
    
    // TODO: replace hard-coded with sync via app
	unsigned int syncTime = 1555344000;
	ATMO_PLATFORM_DebugPrint("RX Time: %d\r\n", syncTime);
	ATMO_DateTime_SetDateTimeEpoch(0, syncTime);
	return ATMO_Status_Success;
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_trigger(ATMO_Value_t *in, ATMO_Value_t *out) {
	return ATMO_Status_Success;
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_setup(ATMO_Value_t *in, ATMO_Value_t *out) {

    ATMO_MK64F_GPIO_EnableResetCombo(ATMO_PROPERTY(EmbeddedNxpRpkUserButtons, enableResetCombo));
    ATMO_MK64F_GPIO_EnableDisplayToggleCombo(ATMO_PROPERTY(EmbeddedNxpRpkUserButtons, enableDisplayToggleCombo));
    ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_SW1_Pushed, ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topRightPushed));
    ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_SW2_Pushed, ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomRightPushed));
    ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_SW3_Pushed, ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topLeftPushed));
    ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_SW4_Pushed, ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomLeftPushed));
	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_topRightPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_feeds++;
    
	char str[32];
	sprintf(str, "%d feeds", s_feeds);
	ATMO_CreateValueString(out, str);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomRightPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_changes++;
    
	char str[32];
	sprintf(str, "%d changes", s_changes);
	ATMO_CreateValueString(out, str);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_topLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_sleeps++;
    
	char str[32];
	sprintf(str, "%d sleeps", s_sleeps);
	ATMO_CreateValueString(out, str);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_wakes++;
    
	char str[32];
	sprintf(str, "%d wakes", s_wakes);
	ATMO_CreateValueString(out, str);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_trigger(ATMO_Value_t *in, ATMO_Value_t *out) {
	return ATMO_Status_Success;
}


ATMO_Status_t EmbeddedIconLinesDisplay_displayPage(ATMO_Value_t *in, ATMO_Value_t *out) {

	ATMO_UI_Page_DisplayPageByCoord(ATMO_PROPERTY(EmbeddedIconLinesDisplay, x), ATMO_PROPERTY(EmbeddedIconLinesDisplay, y), false);
	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedIconLinesDisplay_onDisplayed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_onLeave(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedIconLinesDisplay_setIconLabelAndColor(ATMO_Value_t *in, ATMO_Value_t *out) {

    struct {
        char str[32];
        GUI_COLOR color;
    } icon_data;
 
    ATMO_GetBinary(in, &icon_data, sizeof(icon_data));
    ATMO_UI_ICONLINES_SetIconLabelColor(ATMO_VARIABLE(EmbeddedIconLinesDisplay, pageHandle), icon_data.str, icon_data.color);
    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setIconLabel(ATMO_Value_t *in, ATMO_Value_t *out) {

    char str[32];
    ATMO_GetString(in, str, 32);
    ATMO_UI_ICONLINES_SetIconLabel(ATMO_VARIABLE(EmbeddedIconLinesDisplay, pageHandle), str);
    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setup(ATMO_Value_t *in, ATMO_Value_t *out) {

    ATMO_UI_PAGE_Config_t config;
    config.hidden = ATMO_PROPERTY(EmbeddedIconLinesDisplay, pageHidden);
    config.textColor = ATMO_PROPERTY(EmbeddedIconLinesDisplay, textColor);
    config.activeButtons = ATMO_UI_Page_GetButtonMask(ATMO_PROPERTY(EmbeddedIconLinesDisplay, topRightButtonEnabled),
    ATMO_PROPERTY(EmbeddedIconLinesDisplay,bottomRightButtonEnabled), ATMO_PROPERTY(EmbeddedIconLinesDisplay, topLeftButtonEnabled), ATMO_PROPERTY(EmbeddedIconLinesDisplay, bottomLeftButtonEnabled));
	config.x = ATMO_PROPERTY(EmbeddedIconLinesDisplay, x);
    config.y = ATMO_PROPERTY(EmbeddedIconLinesDisplay, y);
	strncpy(config.topLeftButtonLabel, ATMO_PROPERTY(EmbeddedIconLinesDisplay, topLeftButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.topRightButtonLabel, ATMO_PROPERTY(EmbeddedIconLinesDisplay, topRightButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.bottomLeftButtonLabel, ATMO_PROPERTY(EmbeddedIconLinesDisplay, bottomLeftButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.bottomRightButtonLabel, ATMO_PROPERTY(EmbeddedIconLinesDisplay, bottomRightButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
    config.spanX = ATMO_PROPERTY(EmbeddedIconLinesDisplay, spanX);
	config.spanY = ATMO_PROPERTY(EmbeddedIconLinesDisplay, spanY);
    config.title = ATMO_PROPERTY(EmbeddedIconLinesDisplay, pageTitle);
    config.titleHidden = ATMO_PROPERTY(EmbeddedIconLinesDisplay, titleHidden);
	ATMO_UI_ICONLINES_Init(&config, ATMO_PROPERTY(EmbeddedIconLinesDisplay, numLines), false);
	ATMO_VARIABLE(EmbeddedIconLinesDisplay, pageHandle) = config.templateInstance;
    ATMO_UI_ICONLINES_SetMainText(config.templateInstance, 0, ATMO_PROPERTY(EmbeddedIconLinesDisplay, line1Text));
    ATMO_UI_ICONLINES_SetMainText(config.templateInstance, 1, ATMO_PROPERTY(EmbeddedIconLinesDisplay, line2Text));
    ATMO_UI_ICONLINES_SetMainText(config.templateInstance, 2, ATMO_PROPERTY(EmbeddedIconLinesDisplay, line3Text));
    ATMO_UI_ICONLINES_SetMainText(config.templateInstance, 3, ATMO_PROPERTY(EmbeddedIconLinesDisplay, line4Text));
    ATMO_UI_ICONLINES_SetIconLabel(config.templateInstance, ATMO_PROPERTY(EmbeddedIconLinesDisplay, iconLabel));
    ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 1, ATMO_ABILITY(EmbeddedIconLinesDisplay, topRightButtonPressed));
	ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 2, ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomRightButtonPressed));
	ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 3, ATMO_ABILITY(EmbeddedIconLinesDisplay, topLeftButtonPressed));
    ATMO_UI_ICONLINES_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 4, ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomLeftButtonPressed));
    ATMO_UI_ICONLINES_SetIcon(config.templateInstance, ATMO_PROPERTY(EmbeddedIconLinesDisplay, icon));
    ATMO_UI_ICONLINES_RegisterOnDisplayedAbilityHandle(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), ATMO_ABILITY(EmbeddedIconLinesDisplay, onDisplayed));
    ATMO_UI_ICONLINES_RegisterOnLeaveAbilityHandle(config.templateInstance, ATMO_ABILITY(EmbeddedIconLinesDisplay, onLeave));
    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setLine1Text(ATMO_Value_t *in, ATMO_Value_t *out) {

    char label[32];
    if(ATMO_GetString(in, label, 32) == ATMO_Status_Success)
    {
        ATMO_UI_ICONLINES_SetMainText(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 0, label);
    }
    else
    {
        return ATMO_Status_Fail;
    }

    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setLine2Text(ATMO_Value_t *in, ATMO_Value_t *out) {

    char label[32];
    if(ATMO_GetString(in, label, 32) == ATMO_Status_Success)
    {
        ATMO_UI_ICONLINES_SetMainText(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 1, label);
    }
    else
    {
        return ATMO_Status_Fail;
    }

    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setLine3Text(ATMO_Value_t *in, ATMO_Value_t *out) {

    char label[32];
    if(ATMO_GetString(in, label, 32) == ATMO_Status_Success)
    {
        ATMO_UI_ICONLINES_SetMainText(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 2, label);
    }
    else
    {
        return ATMO_Status_Fail;
    }

    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_setLine4Text(ATMO_Value_t *in, ATMO_Value_t *out) {

    char label[32];
    if(ATMO_GetString(in, label, 32) == ATMO_Status_Success)
    {
        ATMO_UI_ICONLINES_SetMainText(ATMO_VARIABLE(EmbeddedIconLinesDisplay,pageHandle), 3, label);
    }
    else
    {
        return ATMO_Status_Fail;
    }

    return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedIconLinesDisplay_topRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedIconLinesDisplay_bottomRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedIconLinesDisplay_topLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedIconLinesDisplay_bottomLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t Interval_trigger(ATMO_Value_t *in, ATMO_Value_t *out) {
	return ATMO_Status_Success;
}


ATMO_Status_t Interval_setup(ATMO_Value_t *in, ATMO_Value_t *out) {

	ATMO_INTERVAL_Handle_t intervalHandle;
    ATMO_INTERVAL_AddAbilityInterval(
		ATMO_PROPERTY(Interval, instance), 
		ATMO_ABILITY(Interval, interval), 
		ATMO_PROPERTY(Interval, time), 
		&intervalHandle
	);
	
	return ATMO_Status_Success;
	
}


ATMO_Status_t Interval_interval(ATMO_Value_t *in, ATMO_Value_t *out) {
	return ATMO_Status_Success;
}


ATMO_Status_t GetDateTime_trigger(ATMO_Value_t *in, ATMO_Value_t *out) {
	
	// output DateTime based on RTC
	unsigned int bufferSize = 10;
	char dateTimeStr[bufferSize];
	// ATMO_DateTime_GetDateTimeIsoStr(0, dateTimeStr, sizeof(dateTimeStr));

	ATMO_DateTime_Time_t now;
    ATMO_DateTime_GetDateTime(0, &now);
	snprintf(dateTimeStr, bufferSize, "   %02d:%02d", now.hours, now.minutes);
	                                // ^^^ extra padding as workaround for icon drawing bug
    
    /*
    // huh: turns out strftime() works too!?
    time_t temp = time(NULL);
    struct tm *now = localtime(&temp);
    strftime(dateTimeStr, bufferSize, "   %H:%M", now);
    */
    /*
    time_t temp;
    struct tm *now;
    
    time(&temp);
    now = localtime(&temp);
    
    strftime(dateTimeStr, bufferSize, "   %H:%M", now);    
    */
    
	ATMO_CreateValueString(out, dateTimeStr);
	
	return ATMO_Status_Success;
}

//FOOTER START

//FOOTER END

