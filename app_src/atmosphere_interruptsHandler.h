
#ifndef ATMO_INTERRUPTS_HANDLER_H
#define ATMO_INTERRUPTS_HANDLER_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../atmo/core.h"
#define ATMO_INTERRUPT(ELEMENT, NAME) ATMO_ ## ELEMENT ## _INTERRUPT_  ## NAME

void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_trigger();
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_setup();
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_topRightPushed();
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_bottomRightPushed();
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_topLeftPushed();
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_bottomLeftPushed();

void ATMO_Interval_INTERRUPT_trigger();
void ATMO_Interval_INTERRUPT_setup();
void ATMO_Interval_INTERRUPT_interval();

void ATMO_GetDateTime_INTERRUPT_trigger();

void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_trigger();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_displayPage();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_topRightButtonPressed();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_bottomRightButtonPressed();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_topLeftButtonPressed();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_bottomLeftButtonPressed();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_onDisplayed();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_setText();
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_setup();


#ifdef __cplusplus
}
#endif

#endif
