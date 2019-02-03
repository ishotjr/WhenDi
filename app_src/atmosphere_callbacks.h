
#ifndef ATMO_CALLBACKS_H
#define ATMO_CALLBACKS_H

#include "atmosphere_platform.h"
#include "atmosphere_properties.h"
#include "atmosphere_variables.h"
#include "atmosphere_triggerHandler.h"
#include "atmosphere_interruptsHandler.h"
#include "atmosphere_embedded_libraries.h"
#include "atmosphere_abilityHandler.h"

#include "atmosphere_driverinstance.h"

#define ATMO_CALLBACK(ELEMENT, NAME) ELEMENT ## _ ## NAME

void ATMO_Setup();

ATMO_Status_t EmbeddedNxpRpkUserButtons_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedNxpRpkUserButtons_setup(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedNxpRpkUserButtons_topRightPushed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomRightPushed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedNxpRpkUserButtons_topLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedNxpRpkUserButtons_bottomLeftPushed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_displayPage(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_onDisplayed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_onLeave(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setIconLabelAndColor(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setIconLabel(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setup(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setLine1Text(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setLine2Text(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setLine3Text(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_setLine4Text(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_topRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_bottomRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_topLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedIconLinesDisplay_bottomLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_setup(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_startClock(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_stopClock(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_resetClock(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_setDatetime(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t DS1307RealTimeClock_getDatetime(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t Interval_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t Interval_setup(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t Interval_interval(ATMO_Value_t *in, ATMO_Value_t *out);
#endif
