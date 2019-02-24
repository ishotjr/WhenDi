
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

ATMO_Status_t Interval_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t Interval_setup(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t Interval_interval(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t GetDateTime_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_trigger(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_displayPage(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_topRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_bottomRightButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_topLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_bottomLeftButtonPressed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_onDisplayed(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_setText(ATMO_Value_t *in, ATMO_Value_t *out);

ATMO_Status_t EmbeddedStaticTextDisplay_setup(ATMO_Value_t *in, ATMO_Value_t *out);
#endif
