#include "atmosphere_callbacks.h"

//HEADER START
#define BUFFER_SIZE 60

static int s_feeds = 0;
static int s_changes = 0;
static int s_sleeps = 0;
static int s_wakes = 0;
static ATMO_DateTime_Time_t s_feed_time;
static ATMO_DateTime_Time_t s_change_time;
static ATMO_DateTime_Time_t s_sleep_time;
static ATMO_DateTime_Time_t s_wake_time;
static char s_display[BUFFER_SIZE];

static void refresh_display() {
    
    // EmbeddedStaticTextDisplay behaves as a single line of text -
    // unless we hack some newlines in like this! :D
    
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
    
    int formatted_size = 6;
    char sleep_time_formatted[formatted_size];
    char feed_time_formatted[formatted_size];
    char wake_time_formatted[formatted_size];
    char changes_formatted[formatted_size];
    char colon[2] = ":";
    
    // only display if set (not sure how safe not initializing is, but ...)
    if (s_sleep_time.years != 0) {
        snprintf(sleep_time_formatted, formatted_size, "%02d:%02d", s_sleep_time.hours, s_sleep_time.minutes);
    }
    if (s_feed_time.years != 0) {
        snprintf(feed_time_formatted, formatted_size, "%02d:%02d", s_feed_time.hours, s_feed_time.minutes);
    }
    if (s_wake_time.years != 0) {
        snprintf(wake_time_formatted, formatted_size, "%02d:%02d", s_wake_time.hours, s_wake_time.minutes);
    }
    if (s_change_time.years != 0) {
        snprintf(changes_formatted, formatted_size, "%02d:%02d", s_change_time.hours, s_change_time.minutes);
    }
    
    
	ATMO_DateTime_Time_t now;
    ATMO_DateTime_GetDateTime(0, &now);
    
    // blinking seconds like a digital watch
    if (now.seconds % 2) {
        colon[0] = ':';
    } else {
        colon[0] = ' ';
    }
    
    // TODO: handle > 9 woke / chg'd without wrap
	snprintf(s_display, BUFFER_SIZE, "%02d%s%02d\n%d slept %s\n%d ate %s\n%d woke %s\n%d chg'd %s", 
	    now.hours, colon, now.minutes, 
	    s_sleeps, sleep_time_formatted,
	    s_feeds, feed_time_formatted,
	    s_wakes, wake_time_formatted,
	    s_changes, changes_formatted);
	
    ATMO_PLATFORM_DebugPrint("s_display:\r\n%s\r\n", s_display);
    
}
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
	ATMO_DateTime_GetDateTime(0, &s_feed_time);
    
    refresh_display();

	ATMO_CreateValueString(out, s_display);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomRightPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_changes++;
	ATMO_DateTime_GetDateTime(0, &s_change_time);
	
    refresh_display();

	ATMO_CreateValueString(out, s_display);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_topLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_sleeps++;
	ATMO_DateTime_GetDateTime(0, &s_sleep_time);
	
    refresh_display();

	ATMO_CreateValueString(out, s_display);
	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out) {

    s_wakes++;
	ATMO_DateTime_GetDateTime(0, &s_wake_time);
	
    refresh_display();

	ATMO_CreateValueString(out, s_display);
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
    
    refresh_display();

	ATMO_CreateValueString(out, s_display);
	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_trigger(ATMO_Value_t *in, ATMO_Value_t *out) {
	return ATMO_Status_Success;
}


ATMO_Status_t EmbeddedStaticTextDisplay_displayPage(ATMO_Value_t *in, ATMO_Value_t *out) {

	ATMO_UI_Page_DisplayPageByCoord(ATMO_PROPERTY(EmbeddedStaticTextDisplay, x), ATMO_PROPERTY(EmbeddedStaticTextDisplay, y), false);
	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_topRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_bottomRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_topLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_bottomLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_onDisplayed(ATMO_Value_t *in, ATMO_Value_t *out) {

	return ATMO_Status_Success;
    
}


ATMO_Status_t EmbeddedStaticTextDisplay_setText(ATMO_Value_t *in, ATMO_Value_t *out) {

    ATMO_Value_t strVal;
    ATMO_InitValue(&strVal);
    ATMO_CreateValueConverted(&strVal, ATMO_DATATYPE_STRING, in);
    ATMO_UI_STATICTEXT_SetText(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), strVal.data);
    ATMO_FreeValue(&strVal);
	return ATMO_Status_Success;
	
}


ATMO_Status_t EmbeddedStaticTextDisplay_setup(ATMO_Value_t *in, ATMO_Value_t *out) {

    ATMO_UI_PAGE_Config_t config;
	config.hidden = ATMO_PROPERTY(EmbeddedStaticTextDisplay, pageHidden);
	config.textColor = ATMO_PROPERTY(EmbeddedStaticTextDisplay, textColor);
	config.activeButtons = ATMO_UI_Page_GetButtonMask(ATMO_PROPERTY(EmbeddedStaticTextDisplay, topRightButtonEnabled),
		ATMO_PROPERTY(EmbeddedStaticTextDisplay,bottomRightButtonEnabled), ATMO_PROPERTY(EmbeddedStaticTextDisplay, topLeftButtonEnabled), ATMO_PROPERTY(EmbeddedStaticTextDisplay, bottomLeftButtonEnabled));
    config.x = ATMO_PROPERTY(EmbeddedStaticTextDisplay, x);
	config.y = ATMO_PROPERTY(EmbeddedStaticTextDisplay, y);
	strncpy(config.topLeftButtonLabel, ATMO_PROPERTY(EmbeddedStaticTextDisplay, topLeftButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.topRightButtonLabel, ATMO_PROPERTY(EmbeddedStaticTextDisplay, topRightButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.bottomLeftButtonLabel, ATMO_PROPERTY(EmbeddedStaticTextDisplay, bottomLeftButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
	strncpy(config.bottomRightButtonLabel, ATMO_PROPERTY(EmbeddedStaticTextDisplay, bottomRightButtonLabel), ATMO_BUTTON_LABEL_MAXLEN);
    config.spanX = ATMO_PROPERTY(EmbeddedStaticTextDisplay, spanX);
	config.spanY = ATMO_PROPERTY(EmbeddedStaticTextDisplay, spanY);
    config.title = ATMO_PROPERTY(EmbeddedStaticTextDisplay, pageTitle);
    config.titleHidden = ATMO_PROPERTY(EmbeddedStaticTextDisplay, titleHidden);
	ATMO_UI_STATICTEXT_Init(&config);
	ATMO_VARIABLE(EmbeddedStaticTextDisplay, pageHandle) = config.templateInstance;
    ATMO_UI_STATICTEXT_SetText(config.templateInstance, ATMO_PROPERTY(EmbeddedStaticTextDisplay, text));
	ATMO_UI_STATICTEXT_SetTextSize(config.templateInstance, ATMO_PROPERTY(EmbeddedStaticTextDisplay, fontSize));
	ATMO_UI_STATICTEXT_SetAlignment(config.templateInstance, ATMO_PROPERTY(EmbeddedStaticTextDisplay, horizontalAlignment));
	ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), 1, ATMO_ABILITY(EmbeddedStaticTextDisplay, topRightButtonPressed));
	ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), 2, ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomRightButtonPressed));
	ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), 3, ATMO_ABILITY(EmbeddedStaticTextDisplay, topLeftButtonPressed));
    ATMO_UI_STATICTEXT_RegisterButtonAbilityHandle(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), 4, ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomLeftButtonPressed));
    
	if(!config.hidden)
	{
		ATMO_UI_STATICTEXT_RegisterOnDisplayedAbilityHandle(ATMO_VARIABLE(EmbeddedStaticTextDisplay,pageHandle), ATMO_ABILITY(EmbeddedStaticTextDisplay, onDisplayed));
	}
    return ATMO_Status_Success;
    
}

//FOOTER START

//FOOTER END

