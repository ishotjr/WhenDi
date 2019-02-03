
/**
 ******************************************************************************
 * @file    interval_mk64f.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - RapidIoT Interval API Implementation
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "interval_mk64f.h"
#include "atmosphere_platform.h"

#define MK64F_MAX_INTERVALS (64)

const ATMO_INTERVAL_DriverInstance_t mk64fIntervalDriverInstance = {
	ATMO_MK64F_INTERVAL_Init,
	ATMO_MK64F_INTERVAL_RemoveInterval,
	ATMO_MK64F_INTERVAL_AddAbilityInterval,
	ATMO_MK64F_INTERVAL_AddCallbackInterval,
};

typedef enum {
	ATMO_EXECUTE_ABILITY,
	ATMO_EXECUTE_CALLBACK,
} MK64F_INTERVAL_ExecuteType_t;

typedef struct
{
	MK64F_INTERVAL_ExecuteType_t type;
	ATMO_Callback_t cb;
	uint16_t abilityHandle;
	uint16_t milliseconds;
	bool enabled;
	ATMO_Value_t value;
} MK64F_INTERVAL_ExecuteEntry;


static MK64F_INTERVAL_ExecuteEntry intervalEntries[MK64F_MAX_INTERVALS] = {0};
static uint8_t numIntervalEntries = 0;

#define ATMO_MK64F_INTERVAL_STACK_SIZE (200)

void intervalTask( void *pvParameters )
{
	MK64F_INTERVAL_ExecuteEntry *params = (MK64F_INTERVAL_ExecuteEntry *)pvParameters;
	for( ;; )
	{
		ATMO_Lock();
		if(params->type == ATMO_EXECUTE_ABILITY)
		{
			ATMO_AddAbilityExecute(params->abilityHandle, NULL);
		}
		else
		{
			ATMO_AddCallbackExecute(params->cb, NULL);
		}
		ATMO_Unlock();
		ATMO_PLATFORM_DelayMilliseconds(params->milliseconds);
	}
}

ATMO_Status_t ATMO_MK64F_INTERVAL_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	ATMO_DriverInstanceData_t driverInstance;

	driverInstance.name = "MK64F Interval";
	driverInstance.initialized = false;
	driverInstance.instanceNumber = 0;
	
	return ATMO_INTERVAL_AddDriverInstance( &mk64fIntervalDriverInstance, &driverInstance, instanceNumber );
}

ATMO_INTERVAL_Status_t ATMO_MK64F_INTERVAL_Init(ATMO_DriverInstanceData_t *instance)
{

	return ATMO_INTERVAL_Status_Success;
}

ATMO_INTERVAL_Status_t ATMO_MK64F_INTERVAL_AddAbilityInterval(ATMO_DriverInstanceData_t *instance, unsigned int abilityHandle, unsigned int interval, ATMO_INTERVAL_Handle_t *intervalHandle)
{
	if(!(numIntervalEntries < MK64F_MAX_INTERVALS)) 
	{
		return ATMO_INTERVAL_Status_OutOfMemory;
	}

	intervalEntries[numIntervalEntries].milliseconds = interval;
	intervalEntries[numIntervalEntries].abilityHandle = abilityHandle;
	intervalEntries[numIntervalEntries].type = ATMO_EXECUTE_ABILITY;
	
	TaskHandle_t xHandle = NULL;
    BaseType_t xReturned = xTaskCreate( intervalTask, "User Interval", ATMO_MK64F_INTERVAL_STACK_SIZE, (void *)&intervalEntries[numIntervalEntries], configMAX_PRIORITIES - 1, &xHandle );

    if( xReturned != pdPASS )
    {
    	return ATMO_INTERVAL_Status_Fail;
    }
	
	numIntervalEntries++;

	return ATMO_INTERVAL_Status_Success;
}

ATMO_INTERVAL_Status_t ATMO_MK64F_INTERVAL_AddCallbackInterval(ATMO_DriverInstanceData_t *instance, ATMO_Callback_t cb, unsigned int interval, ATMO_INTERVAL_Handle_t *intervalHandle)
{
	if(!(numIntervalEntries < MK64F_MAX_INTERVALS))
	{
		return ATMO_INTERVAL_Status_OutOfMemory;
	}

	intervalEntries[numIntervalEntries].milliseconds = interval;
	intervalEntries[numIntervalEntries].cb = cb;
	intervalEntries[numIntervalEntries].type = ATMO_EXECUTE_CALLBACK;

	TaskHandle_t xHandle = NULL;
    BaseType_t xReturned = xTaskCreate( intervalTask, "User Interval", ATMO_MK64F_INTERVAL_STACK_SIZE, (void *)&intervalEntries[numIntervalEntries], configMAX_PRIORITIES - 1, &xHandle );

    if( xReturned != pdPASS )
    {
    	return ATMO_INTERVAL_Status_Fail;
    }

	numIntervalEntries++;

	return ATMO_INTERVAL_Status_Success;
}

ATMO_INTERVAL_Status_t ATMO_MK64F_INTERVAL_RemoveInterval(ATMO_DriverInstanceData_t *instance, ATMO_INTERVAL_Handle_t intervalHandle)
{
	return ATMO_INTERVAL_Status_NotSupported;
}
