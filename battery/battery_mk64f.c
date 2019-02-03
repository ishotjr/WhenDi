#include "battery_mk64f.h"
#include "battery.h"

#define ATMO_BATT_NUM_CALLBACKS (10)

static batt_statechange_cb_t battCb[ATMO_BATT_NUM_CALLBACKS] = {NULL};
static unsigned int currentNumBattCallbacks = 0;
static uint8_t lastState = 0;
static uint8_t lastChargeLevel = 256;

#define ATMO_BATT_REPORT_THRESH (100)
static bool initialReading = false;
static bool reported = false;
static uint8_t numTimesSame = ATMO_BATT_REPORT_THRESH;

void ATMO_BATTERY_MK64F_Init()
{
	battery_fct_t battConfig;
	battConfig.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
	BatterySensor_Init_Driver(&battConfig);
	BatterySensor_Init_Hw();
}

void ATMO_BATTERY_MK64F_RegisterStateChangeCallback(batt_statechange_cb_t cb)
{
	if(currentNumBattCallbacks >= ATMO_BATT_NUM_CALLBACKS)
	{
		return;
	}

	battCb[currentNumBattCallbacks++] = cb;
	ATMO_BATTERY_MK64F_ForceUpdate();
}

void ATMO_BATTERY_MK64F_Update()
{
	uint8_t currentState = 0;
	uint8_t currentLevel = 0;
	BatterySensor_GetState(&currentLevel, &currentState);

	if((currentState != lastState))
	{
		for(unsigned int i = 0; i < currentNumBattCallbacks; i++)
		{
			battCb[i](currentLevel, currentState);
		}
		lastState = currentState;
		lastChargeLevel = currentLevel;
		return;
	}

	if(lastChargeLevel != currentLevel)
	{
		reported = false;
		numTimesSame = 0;
	}
	else
	{
		numTimesSame++;
	}

	if(((numTimesSame >= ATMO_BATT_REPORT_THRESH) || !initialReading) && !reported)
	{
		initialReading = true;
		reported = true;
		for(unsigned int i = 0; i < currentNumBattCallbacks; i++)
		{
			battCb[i](currentLevel, currentState);
		}
	}

	lastState = currentState;
	lastChargeLevel = currentLevel;

}

void ATMO_BATTERY_MK64F_ForceUpdate()
{
	uint8_t currentState = 0;
	uint8_t currentLevel = 0;
	BatterySensor_GetState(&currentLevel, &currentState);

	for(unsigned int i = 0; i < currentNumBattCallbacks; i++)
	{
		battCb[i](currentLevel, currentState);
	}
}
