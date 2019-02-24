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
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_trigger() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, trigger), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_displayPage() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, displayPage), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_topRightButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, topRightButtonPressed), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_bottomRightButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomRightButtonPressed), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_topLeftButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, topLeftButtonPressed), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_bottomLeftButtonPressed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomLeftButtonPressed), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_onDisplayed() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, onDisplayed), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_setText() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), NULL);
}
void ATMO_EmbeddedStaticTextDisplay_INTERRUPT_setup() {
	ATMO_AddAbilityExecute(ATMO_ABILITY(EmbeddedStaticTextDisplay, setup), NULL);
}
