
#ifndef ATMO_PROPERTIES_H
#define ATMO_PROPERTIES_H

#ifdef __cplusplus
	extern "C"{
#endif

#define ATMO_PROPERTY(ELEMENT, NAME) ATMO_ ## ELEMENT ## _PROPERTY_  ## NAME

#define ATMO_EmbeddedNxpRpkUserButtons_PROPERTY_errorData NULL
#define ATMO_EmbeddedNxpRpkUserButtons_PROPERTY_codeUserChanged NULL
#define ATMO_EmbeddedNxpRpkUserButtons_PROPERTY_enableResetCombo true
#define ATMO_EmbeddedNxpRpkUserButtons_PROPERTY_enableDisplayToggleCombo true

#define ATMO_Interval_PROPERTY_errorData NULL
#define ATMO_Interval_PROPERTY_codeUserChanged NULL
#define ATMO_Interval_PROPERTY_instance ATMO_DRIVERINSTANCE_INTERVAL_INTERVAL1
#define ATMO_Interval_PROPERTY_time 1000

#define ATMO_GetDateTime_PROPERTY_errorData NULL
#define ATMO_GetDateTime_PROPERTY_codeUserChanged NULL

#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_errorData NULL
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_codeUserChanged NULL
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_textColor GUI_BLUE
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_pageTitle "WhenDi"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_titleHidden true
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_pageHidden false
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_topRightButtonLabel "Ate"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_topRightButtonEnabled true
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_bottomRightButtonLabel "Changed"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_bottomRightButtonEnabled true
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_topLeftButtonLabel "Slept"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_topLeftButtonEnabled true
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_bottomLeftButtonLabel "Woke"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_bottomLeftButtonEnabled true
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_x 0
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_y 0
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_spanX 1
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_spanY 1
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_text "Press a button to start!"
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_fontSize ATMO_FONT_SIZE_20PT
#define ATMO_EmbeddedStaticTextDisplay_PROPERTY_horizontalAlignment GUI_TA_LEFT


#ifdef __cplusplus
}
#endif

#endif
