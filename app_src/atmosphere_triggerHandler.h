
#ifndef ATMO_TRIGGER_HANDLER_H
#define ATMO_TRIGGER_HANDLER_H

#include "atmosphere_platform.h"
#include "../atmo/core.h"

#ifdef __cplusplus
	extern "C"{
#endif

#define ATMO_TRIGGER(ELEMENT, NAME) ATMO_ ## ELEMENT ## _TRIGGER_  ## NAME

void ATMO_TriggerHandler(unsigned int triggerHandleId, ATMO_Value_t *value);

#define ATMO_EmbeddedNxpRpkUserButtons_TRIGGER_triggered 0x1
#define ATMO_EmbeddedNxpRpkUserButtons_TRIGGER_topRightPushed 0x2
#define ATMO_EmbeddedNxpRpkUserButtons_TRIGGER_bottomRightPushed 0x3
#define ATMO_EmbeddedNxpRpkUserButtons_TRIGGER_topLeftPushed 0x4
#define ATMO_EmbeddedNxpRpkUserButtons_TRIGGER_bottomLeftPushed 0x5
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_triggered 0x6
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_onDisplayed 0x7
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_onLeave 0x8
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_topRightButtonPressed 0x9
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_bottomRightButtonPressed 0xa
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_topLeftButtonPressed 0xb
#define ATMO_EmbeddedIconLinesDisplay_TRIGGER_bottomLeftButtonPressed 0xc
#define ATMO_Interval_TRIGGER_triggered 0xd
#define ATMO_Interval_TRIGGER_interval 0xe
#define ATMO_GetDateTime_TRIGGER_triggered 0xf
#ifdef __cplusplus
}
#endif
#endif
