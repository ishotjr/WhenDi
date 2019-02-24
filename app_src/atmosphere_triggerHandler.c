#include "atmosphere_triggerHandler.h"
#include "atmosphere_abilityHandler.h"

#ifdef __cplusplus
	extern "C"{
#endif

void ATMO_TriggerHandler(unsigned int triggerHandleId, ATMO_Value_t *value) {
	switch(triggerHandleId) {
		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, topRightPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomRightPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, topLeftPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomLeftPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), value);
			break;
		}

		case ATMO_TRIGGER(Interval, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(Interval, interval):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(GetDateTime, trigger), value);
			break;
		}

		case ATMO_TRIGGER(GetDateTime, triggered):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedStaticTextDisplay, setText), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, topRightButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, bottomRightButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, topLeftButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, bottomLeftButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedStaticTextDisplay, onDisplayed):
		{
			break;
		}

	}

}

#ifdef __cplusplus
}
#endif
