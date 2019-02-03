#ifndef __BLE_DB_MK64F_H_
#define __BLE_DB_MK64F_H_

#include <stdint.h>
#include "ble.h"
#include "ble_sig_defines.h"
#include "cmd_ble.h"
#include "ble_mk64f_helpers.h"

typedef bool ( *AddService_t )( ATMO_UUID_t *uuid, ATMO_BLE_Handle_t *handle );
typedef bool ( *AddCharacteristic_t )( ATMO_UUID_t *charUuid, ATMO_BLE_Handle_t serviceHandle, uint8_t properties, uint8_t permissions, uint32_t maxLen, ATMO_BLE_Handle_t *handle );
typedef bool ( *DbFlushed_t )();
/**
 * Initialize DB Cache
 */
bool ATMO_BLE_DB_MK64F_Init();

/**
 * Register MK64F Specific functions for actually adding services and characteristics
 */
bool ATMO_BLE_DB_MK64F_RegisterFuncs( AddService_t addService, AddCharacteristic_t addCharacteristic, DbFlushed_t dbFlushed );

/**
 * Add a service to the cached database, get a virtual handle back
 */
bool ATMO_BLE_DB_MK64F_AddService( ATMO_UUID_t *uuid, ATMO_BLE_Handle_t *handle );

/**
 * Utility to convert atmosphere "virtual handle" to the actual Kw41Z handle
 */
bool ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t *kw41zHandle );

/**
 * Utility to convert kw41z handle to the atmosphere "virtual handle"
 */
bool ATMO_BLE_DB_MK64F_Kw41zHandleToAtmo( ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t *atmoHandle );

/**
 * Add characteristic to given service
 *
 * @param[in] serviceHandle - the virtual handle of the parent service
 * @param[in] charUuid - uuid of the characteristic
 * @param[in] properties
 * @param[in] maxLen
 * @param[out] handle - virtual handle for characteristic
 */
bool ATMO_BLE_DB_MK64F_AddCharacteristicToService( ATMO_BLE_Handle_t *serviceHandle, ATMO_UUID_t *charUuid, uint8_t properties, uint8_t permissions, uint32_t maxLen, ATMO_BLE_Handle_t *handle );

/**
 * Flush GATT DB to the KW41Z
 */
bool ATMO_BLE_DB_MK64F_Flush();

/**
* Print advertisement data to dev console
*/
void ATMO_MK64F_BLE_PrintAdvData(GAPSetAdvertisingDataRequest_t *pAdvData);


#endif
