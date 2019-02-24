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
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, trigger):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_trigger(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, triggered), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, displayPage):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_displayPage(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, topRightButtonPressed):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_topRightButtonPressed(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, topRightButtonPressed), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomRightButtonPressed):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_bottomRightButtonPressed(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, bottomRightButtonPressed), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, topLeftButtonPressed):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_topLeftButtonPressed(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, topLeftButtonPressed), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, bottomLeftButtonPressed):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_bottomLeftButtonPressed(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, bottomLeftButtonPressed), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, onDisplayed):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_onDisplayed(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_TriggerHandler(ATMO_TRIGGER(EmbeddedStaticTextDisplay, onDisplayed), &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, setText):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_setText(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
		case ATMO_ABILITY(EmbeddedStaticTextDisplay, setup):
		{
			ATMO_Value_t EmbeddedStaticTextDisplay_Value;
			ATMO_InitValue(&EmbeddedStaticTextDisplay_Value);
			EmbeddedStaticTextDisplay_setup(value, &EmbeddedStaticTextDisplay_Value);
			ATMO_FreeValue(&EmbeddedStaticTextDisplay_Value);
			break;
		}
	}

}

#ifdef __cplusplus
}
#endif
