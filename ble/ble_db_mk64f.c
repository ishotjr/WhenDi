#include "ble_db_mk64f.h"
#include "ble.h"
#include "atmosphere_platform.h"

#define ATMO_BLE_DB_MAX_CHARACTERISTICS_PER_SERVICE (10)
#define ATMO_BLE_DB_MAX_SERVICES (20)
#define ATMO_BLE_DB_MAX_CHARACTERISTICS (ATMO_BLE_DB_MAX_CHARACTERISTICS_PER_SERVICE * ATMO_BLE_DB_MAX_SERVICES)
#define ATMO_BLE_DB_MAX_HANDLES (ATMO_BLE_DB_MAX_CHARACTERISTICS + ATMO_BLE_DB_MAX_SERVICES)

// Storage for characteristic information
typedef struct
{
	ATMO_UUID_t uuid;
	ATMO_BLE_Handle_t handle;
	ATMO_BLE_Handle_t kw41zHandle; // Handle the kw41z uses
	uint8_t properties;
	uint8_t permissions;
	uint8_t maxLen;
} ATMO_BLE_DB_Characteristic_t;

typedef struct
{
	ATMO_UUID_t uuid;
	ATMO_BLE_Handle_t handle;
	ATMO_BLE_Handle_t kw41zHandle; // Handle the kw41z uses
	uint8_t numCharacteristics;
	uint8_t characteristics[ATMO_BLE_DB_MAX_CHARACTERISTICS_PER_SERVICE];
} ATMO_BLE_DB_Service_t;

static AddService_t addServiceFunc = NULL;
static AddCharacteristic_t addCharacteristicFunc = NULL;
static DbFlushed_t dbFlushedFunc = NULL;

// Current virtual handle
static uint8_t currentHandle = 0;
static uint8_t currentNumServices = 0;
static uint8_t currentNumCharacteristics = 0;

// Main list of all services
static ATMO_BLE_DB_Service_t services[ATMO_BLE_DB_MAX_SERVICES];
static ATMO_BLE_DB_Characteristic_t characteristics[ATMO_BLE_DB_MAX_CHARACTERISTICS];

// Table to translate atmosphere handle to k64 handle
// This is to avoid iterating through the whole linked list structure all the time
static ATMO_BLE_DB_Characteristic_t *atmoHandles[ATMO_BLE_DB_MAX_HANDLES] = {NULL};
static ATMO_BLE_DB_Characteristic_t *kw41zHandles[ATMO_BLE_DB_MAX_HANDLES] = {NULL};

/**
 * Retrieve a service given a virtual handle or uuid
 */
static ATMO_BLE_DB_Service_t *ATMO_BLE_DB_MK64F_GetService( ATMO_UUID_t *uuid, ATMO_BLE_Handle_t *handle )
{
	for ( uint32_t i = 0; i < currentNumServices; i++ )
	{
		if ( uuid != NULL )
		{
			if ( memcmp( uuid, &services[i].uuid, sizeof( ATMO_UUID_t ) ) == 0 )
			{
				return &services[i];
			}
		}
		else if ( handle != NULL )
		{
			if ( *handle == services[i].handle )
			{
				return &services[i];
			}
		}
	}

	return NULL;
}

bool ATMO_BLE_DB_MK64F_Init()
{
	return true;
}

bool ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t *kw41zHandle )
{
	if ( atmoHandles[*handle] == NULL )
	{
		return false;
	}

	*kw41zHandle = atmoHandles[*handle]->kw41zHandle;

	return true;
}

bool ATMO_BLE_DB_MK64F_Kw41zHandleToAtmo( ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t *atmoHandle )
{
	if ( kw41zHandles[*handle] == NULL )
	{
		return false;
	}

	*atmoHandle = kw41zHandles[*handle]->handle;

	return true;
}

bool ATMO_BLE_DB_MK64F_RegisterFuncs( AddService_t addService, AddCharacteristic_t addCharacteristic, DbFlushed_t dbFlushed )
{
	addServiceFunc = addService;
	addCharacteristicFunc = addCharacteristic;
	dbFlushedFunc = dbFlushed;
	return true;
}

bool ATMO_BLE_DB_MK64F_AddService( ATMO_UUID_t *uuid, ATMO_BLE_Handle_t *handle )
{
	if ( currentHandle >= ATMO_BLE_DB_MAX_HANDLES || currentNumServices >= ATMO_BLE_DB_MAX_SERVICES )
	{
		return false;
	}

	ATMO_BLE_DB_Service_t *service = ATMO_BLE_DB_MK64F_GetService( uuid, NULL );

	// Service is already in the list
	// Don't add it again
	if ( service != NULL )
	{
		*handle = service->handle;
		return true;
	}
	else
	{
		service = &services[currentNumServices];
	}

	if(currentNumServices >= ATMO_BLE_DB_MAX_SERVICES)
	{
		ATMO_PLATFORM_DebugPrint("ERROR: Unable to add service. Too many services. Limit: %d\r\n", ATMO_BLE_DB_MAX_SERVICES);
		return false;
	}

	// Create new service
	memcpy( &service->uuid, uuid, sizeof( ATMO_UUID_t ) );
	service->handle = currentHandle++;
	service->kw41zHandle = 0;
	service->numCharacteristics = 0;
	*handle = service->handle;
	currentNumServices++;

	return true;
}

bool ATMO_BLE_DB_MK64F_AddCharacteristicToService( ATMO_BLE_Handle_t *serviceHandle, ATMO_UUID_t *charUuid, uint8_t properties, uint8_t permissions, uint32_t maxLen, ATMO_BLE_Handle_t *handle )
{
	if ( currentHandle >= ATMO_BLE_DB_MAX_HANDLES || currentNumCharacteristics >= ATMO_BLE_DB_MAX_CHARACTERISTICS )
	{
		return false;
	}

	ATMO_BLE_DB_Service_t *service = ATMO_BLE_DB_MK64F_GetService( NULL, serviceHandle );

	if ( service == NULL )
	{
		return false;
	}

	// Create new characteristics
	characteristics[currentNumCharacteristics].handle = currentHandle++;
	characteristics[currentNumCharacteristics].kw41zHandle = 0;
	characteristics[currentNumCharacteristics].maxLen = maxLen;
	characteristics[currentNumCharacteristics].properties = properties;
	characteristics[currentNumCharacteristics].permissions = permissions;
	memcpy( &characteristics[currentNumCharacteristics].uuid, charUuid, sizeof( ATMO_UUID_t ) );

	// Save handle
	*handle = characteristics[currentNumCharacteristics].handle;

	ATMO_PLATFORM_DebugPrint("Adding Characteristic[%d] to Service %d\r\n", service->numCharacteristics, *serviceHandle);

	if(service->numCharacteristics >= ATMO_BLE_DB_MAX_CHARACTERISTICS_PER_SERVICE)
	{
		ATMO_PLATFORM_DebugPrint("ERROR: Unable to add characteristic. Service can only have %d characteristics!!\r\n", ATMO_BLE_DB_MAX_CHARACTERISTICS_PER_SERVICE);
		return false;
	}

	// Add characteristic to parent service
	service->characteristics[service->numCharacteristics] = currentNumCharacteristics;
	currentNumCharacteristics++;
	service->numCharacteristics++;

	return true;
}

bool ATMO_BLE_DB_MK64F_Flush()
{
	if ( addServiceFunc == NULL || addCharacteristicFunc == NULL || dbFlushedFunc == NULL )
	{
		return false;
	}

	for ( uint32_t serviceIndex = 0; serviceIndex < currentNumServices; serviceIndex++ )
	{
		addServiceFunc( &services[serviceIndex].uuid, &services[serviceIndex].kw41zHandle );

		for ( uint32_t charIndex = 0; charIndex < services[serviceIndex].numCharacteristics; charIndex++ )
		{
			ATMO_BLE_DB_Characteristic_t *characteristic = &characteristics[services[serviceIndex].characteristics[charIndex]];
			addCharacteristicFunc( &characteristic->uuid, services[serviceIndex].kw41zHandle, characteristic->properties, characteristic->permissions, characteristic->maxLen, &characteristic->kw41zHandle );
			ATMO_PLATFORM_DelayMilliseconds(50);
			// Save pointers to easily translate between the two different handles
			atmoHandles[characteristic->handle] = characteristic;
			kw41zHandles[characteristic->kw41zHandle] = characteristic;
		}
	}

	return dbFlushedFunc();
}
