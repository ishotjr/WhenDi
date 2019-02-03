#ifndef __ATMO_BATTERY_MK64F_H
#define __ATMO_BATTERY_MK64F_H

#include "battery.h"
#include "atmosphere_platform.h"

typedef (*batt_statechange_cb_t)(uint8_t chargeLevel, uint8_t chargeStatus);

void ATMO_BATTERY_MK64F_Init();

void ATMO_BATTERY_MK64F_RegisterStateChangeCallback(batt_statechange_cb_t cb);

void ATMO_BATTERY_MK64F_Update();

void ATMO_BATTERY_MK64F_ForceUpdate();

#endif
