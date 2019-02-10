#include "atmosphere_interruptsHandler.h"
#include "atmosphere_abilityHandler.h"

void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_trigger() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, trigger), NULL);
}
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_setup() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, setup), NULL);
}
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_topRightPushed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topRightPushed), NULL);
}
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_bottomRightPushed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomRightPushed), NULL);
}
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_topLeftPushed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topLeftPushed), NULL);
}
void ATMO_EmbeddedNxpRpkUserButtons_INTERRUPT_bottomLeftPushed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomLeftPushed), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_trigger() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, trigger), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_displayPage() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, displayPage), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_onDisplayed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, onDisplayed), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_onLeave() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, onLeave), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setIconLabelAndColor() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setIconLabelAndColor), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setIconLabel() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setIconLabel), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setup() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setup), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setLine1Text() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine1Text), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setLine2Text() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine2Text), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setLine3Text() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine3Text), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_setLine4Text() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine4Text), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_topRightButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, topRightButtonPressed), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_bottomRightButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomRightButtonPressed), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_topLeftButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, topLeftButtonPressed), NULL);
}
void ATMO_EmbeddedIconLinesDisplay_INTERRUPT_bottomLeftButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomLeftButtonPressed), NULL);
}
void ATMO_Interval_INTERRUPT_trigger() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(Interval, trigger), NULL);
}
void ATMO_Interval_INTERRUPT_setup() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(Interval, setup), NULL);
}
void ATMO_Interval_INTERRUPT_interval() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(Interval, interval), NULL);
}
void ATMO_GetDateTime_INTERRUPT_trigger() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(GetDateTime, trigger), NULL);
}
