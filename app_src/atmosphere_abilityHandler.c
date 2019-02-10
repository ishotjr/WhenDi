#include "atmosphere_abilityHandler.h"
#include "atmosphere_triggerHandler.h"

#ifdef __cplusplus
	extern "C"{
#endif

void ATMO_AbilityHandler(unsigned int abilityHandleId, ATMO_Value_t *value) {
	switch(abilityHandleId) {
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, trigger):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_trigger(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, triggered), &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, setup):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_setup(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topRightPushed):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_topRightPushed(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, topRightPushed), &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomRightPushed):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_bottomRightPushed(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomRightPushed), &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, topLeftPushed):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_topLeftPushed(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, topLeftPushed), &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedNxpRpkUserButtons, bottomLeftPushed):
		{
			ATMO_Value_t EmbeddedNxpRpkUserButtons_Value;
			ATMO_InitValue(&EmbeddedNxpRpkUserButtons_Value);
			EmbeddedNxpRpkUserButtons_bottomLeftPushed(value, &EmbeddedNxpRpkUserButtons_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomLeftPushed), &EmbeddedNxpRpkUserButtons_Value);
			ATMO_FreeValue(&EmbeddedNxpRpkUserButtons_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, trigger):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_trigger(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, triggered), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, displayPage):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_displayPage(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, onDisplayed):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_onDisplayed(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, onDisplayed), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, onLeave):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_onLeave(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, onLeave), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setIconLabelAndColor):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setIconLabelAndColor(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setIconLabel):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setIconLabel(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setup):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setup(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine1Text):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setLine1Text(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine2Text):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setLine2Text(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine3Text):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setLine3Text(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine4Text):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_setLine4Text(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, topRightButtonPressed):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_topRightButtonPressed(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, topRightButtonPressed), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomRightButtonPressed):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_bottomRightButtonPressed(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, bottomRightButtonPressed), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, topLeftButtonPressed):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_topLeftButtonPressed(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, topLeftButtonPressed), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedIconLinesDisplay, bottomLeftButtonPressed):
		{
			ATMO_Value_t EmbeddedIconLinesDisplay_Value;
			ATMO_InitValue(&EmbeddedIconLinesDisplay_Value);
			EmbeddedIconLinesDisplay_bottomLeftButtonPressed(value, &EmbeddedIconLinesDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedIconLinesDisplay, bottomLeftButtonPressed), &EmbeddedIconLinesDisplay_Value);
			ATMO_FreeValue(&EmbeddedIconLinesDisplay_Value);
			break;
		}
		case ATMO_ABILITY(Interval, trigger):
		{
			ATMO_Value_t Interval_Value;
			ATMO_InitValue(&Interval_Value);
			Interval_trigger(value, &Interval_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(Interval, triggered), &Interval_Value);
			ATMO_FreeValue(&Interval_Value);
			break;
		}
		case ATMO_ABILITY(Interval, setup):
		{
			ATMO_Value_t Interval_Value;
			ATMO_InitValue(&Interval_Value);
			Interval_setup(value, &Interval_Value);
			ATMO_FreeValue(&Interval_Value);
			break;
		}
		case ATMO_ABILITY(Interval, interval):
		{
			ATMO_Value_t Interval_Value;
			ATMO_InitValue(&Interval_Value);
			Interval_interval(value, &Interval_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(Interval, interval), &Interval_Value);
			ATMO_FreeValue(&Interval_Value);
			break;
		}
		case ATMO_ABILITY(GetDateTime, trigger):
		{
			ATMO_Value_t GetDateTime_Value;
			ATMO_InitValue(&GetDateTime_Value);
			GetDateTime_trigger(value, &GetDateTime_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(GetDateTime, triggered), &GetDateTime_Value);
			ATMO_FreeValue(&GetDateTime_Value);
			break;
		}
	}

}

#ifdef __cplusplus
}
#endif
