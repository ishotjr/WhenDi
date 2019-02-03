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
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine1Text), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomRightPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine2Text), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, topLeftPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine3Text), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedNxpRpkUserButtons, bottomLeftPushed):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine4Text), value);
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, onDisplayed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, onLeave):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, topRightButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, bottomRightButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, topLeftButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(EmbeddedIconLinesDisplay, bottomLeftButtonPressed):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, clockStarted):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, clockStopped):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, clockReset):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, datetimeSet):
		{
			break;
		}

		case ATMO_TRIGGER(DS1307RealTimeClock, datetimeRetrieved):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(EmbeddedIconLinesDisplay, setLine4Text), value);
			break;
		}

		case ATMO_TRIGGER(Interval, triggered):
		{
			break;
		}

		case ATMO_TRIGGER(Interval, interval):
		{
			ATMO_AbilityHandler(ATMO_ABILITY(DS1307RealTimeClock, getDatetime), value);
			break;
		}

	}

}

#ifdef __cplusplus
}
#endif
