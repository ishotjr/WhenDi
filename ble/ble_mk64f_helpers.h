#ifndef __ATMO_MK64F_BLE_HELPERS_H_
#define __ATMO_MK64F_BLE_HELPERS_H_

#include "ble_mk64f.h"
#include "ble_db_mk64f.h"
#include "cmd_ble.h"

#define BLE_FSCI_IF (1)

#define gFSCI_IpStackOpGCnf_c                   0xCFU
/*! FSCI utility Confirmations/Indications              */
#define gFSCI_CnfOpcodeGroup_c                  0xA4
/*! FSCI operation group for GATT Database (application) */
#define gFsciBleL2capOpcodeGroup_c              0x41
/*! FSCI operation group for GATT */
#define gFsciBleGattOpcodeGroup_c               0x44
/*! FSCI operation group for GATT Database (application) */
#define gFsciBleGattDbAppOpcodeGroup_c          0x45
/*! FSCI operation group for GAP */
#define gFsciBleGapOpcodeGroup_c                0x47

// Set the service changed indicator
bool ATMO_MK64F_BLE_SetServiceChanged(ATMO_BLE_Handle_t startHandle, ATMO_BLE_Handle_t endHandle);

// Add default BLE services (GATT and GAP)
bool ATMO_MK64F_BLE_AddDefaultServices();


// Convert ascii string to its hex value
// Directly from NXP example code
uint32_t ATMO_MK64F_BLE_AsciiToHex( uint8_t *pString, uint32_t strLen );

// Write Auth UID Characteristic
bool ATMO_MK64F_BLE_WriteAuthUid(uint8_t *uid);

// Write Auth Cert Characteristic
bool ATMO_MK64F_BLE_WriteAuthCert(uint8_t *cert);

// Add Auth Service and Characteristics
bool ATMO_MK64F_BLE_AddAuthService();

// Add OTAP service and characteristics
bool ATMO_MK64F_BLE_AddOtapService();

#endif
