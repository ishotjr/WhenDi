
#ifndef ATMO_ABILITY_HANDLER_H
#define ATMO_ABILITY_HANDLER_H

#include "atmosphere_platform.h"
#include "atmosphere_callbacks.h"

#include "../atmo/core.h"

#ifdef __cplusplus
	extern "C"{
#endif

#define ATMO_ABILITY(ELEMENT, NAME) ATMO_ ## ELEMENT ## _ABILITY_  ## NAME

void ATMO_AbilityHandler(unsigned int abilityHandleId, ATMO_Value_t *value);

#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_trigger 0x1
#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_setup 0x2
#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_topRightPushed 0x3
#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_bottomRightPushed 0x4
#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_topLeftPushed 0x5
#define ATMO_EmbeddedNxpRpkUserButtons_ABILITY_bottomLeftPushed 0x6
#define ATMO_Interval_ABILITY_trigger 0x7
#define ATMO_Interval_ABILITY_setup 0x8
#define ATMO_Interval_ABILITY_interval 0x9
#define ATMO_GetDateTime_ABILITY_trigger 0xa
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_trigger 0xb
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_displayPage 0xc
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_topRightButtonPressed 0xd
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_bottomRightButtonPressed 0xe
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_topLeftButtonPressed 0xf
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_bottomLeftButtonPressed 0x10
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_onDisplayed 0x11
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_setText 0x12
#define ATMO_EmbeddedStaticTextDisplay_ABILITY_setup 0x13
#ifdef __cplusplus
}
#endif
#endif
