/**
 ******************************************************************************
 * @file    ble.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - BLE header file
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
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

/** \addtogroup BLE
 *  @{
 */

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_BLE__H
#define __ATMO_BLE__H

/* Includes ------------------------------------------------------------------*/
#include "../atmo/core.h"

#ifdef __cplusplus
  extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/
typedef uint16_t ATMO_BLE_ServiceHandle_t;
typedef uint16_t ATMO_BLE_Handle_t;
typedef uint16_t ATMO_BLE_GAPAddress_t;
typedef uint16_t ATMO_BLE_Address_t;
typedef uint16_t ATMO_BLE_DeviceID_t;
typedef uint16_t ATMO_BLE_Interface_t;
typedef uint16_t ATMO_BLE_ServiceID_t;

/**
 * BLE Driver function return enumerations
 */
typedef enum
{
    ATMO_BLE_Status_Success              = 0x0,  /**< Operation was successful */
    ATMO_BLE_Status_Fail                 = -0x1,  /**< Operation failed */
    ATMO_BLE_Status_Initialized          = -0x2,  /**< Peripheral already initialized */
    ATMO_BLE_Status_Invalid              = -0x3,  /**< Invalid operation */
    ATMO_BLE_Status_NotSupported         = -0x4,  /**< Feature not supported by platform */
    ATMO_BLE_Status_Unspecified          = -0x5,  /**< Some other status not defined */
    ATMO_BLE_Status_Busy                 = -0x20,  /**< Transfer in progress */
    ATMO_BLE_Status_Timeout              = -0x21,  /**< Transfer timed out */
} ATMO_BLE_Status_t;

/**
 * BLE Transmit Power
 */
typedef enum
{
  ATMO_BLE_PowerTx_Low    = 0x01,
  ATMO_BLE_PowerTx_Medium   = 0x02,
  ATMO_BLE_PowerTx_High     = 0x03,
  ATMO_BLE_PowerTx_Max    = 0x04,
} ATMO_BLE_PowerLevel_t ;

typedef enum
{
  ATMO_BLE_EVENT_Connected = 0,
  ATMO_BLE_EVENT_Disconnected,
  ATMO_BLE_EVENT_PairingRequested,
  ATMO_BLE_EVENT_PairingFailed,
  ATMO_BLE_EVENT_PairingSuccess,
  ATMO_BLE_EVENT_NumEvents,
} ATMO_BLE_Event_t;

/***************************************************************************************
*********************************** Advertising Parms **********************************
****************************************************************************************/

/**
 * BLE Advertising Type
 */
typedef enum
{
  ATMO_BLE_ADV_TYPE_INDIRECT              =  0x00,  /**< General advertise */
  ATMO_BLE_ADV_TYPE_DIRECT                =  0x01,  /**< Advertise to specific device */
  ATMO_BLE_ADV_TYPE_NONCONNECT_INDIRECT   =  0x02,  /**< Advertise but device isn't connectable */
  ATMO_BLE_ADV_TYPE_SCAN_INDIRECT         =  0x03,  /**< Similar to NONCONNECT with addition info via scan responses */
} ATMO_BLE_AdvertisingType_t;

/// \cond DO_NOT_DOCUMENT
typedef enum
{
    ATMO_BLE_AD_TYPE_FLAG                     = 0x01,
    ATMO_BLE_AD_TYPE_16SRV_PART               = 0x02,
    ATMO_BLE_AD_TYPE_16SRV_CMPL               = 0x03,
    ATMO_BLE_AD_TYPE_32SRV_PART               = 0x04,
    ATMO_BLE_AD_TYPE_32SRV_CMPL               = 0x05,
    ATMO_BLE_AD_TYPE_128SRV_PART              = 0x06,
    ATMO_BLE_AD_TYPE_128SRV_CMPL              = 0x07,
    ATMO_BLE_AD_TYPE_NAME_SHORT               = 0x08,
    ATMO_BLE_AD_TYPE_NAME_CMPL                = 0x09,
    ATMO_BLE_AD_TYPE_TX_PWR                   = 0x0A,
    ATMO_BLE_AD_TYPE_DEV_CLASS                = 0x0D,
    ATMO_BLE_AD_TYPE_SM_TK                    = 0x10,
    ATMO_BLE_AD_TYPE_SM_OOB_FLAG              = 0x11,
    ATMO_BLE_AD_TYPE_INT_RANGE                = 0x12,
    ATMO_BLE_AD_TYPE_SOL_SRV_UUID             = 0x14,
    ATMO_BLE_AD_TYPE_128SOL_SRV_UUID          = 0x15,
    ATMO_BLE_AD_TYPE_SERVICE_DATA             = 0x16,
    ATMO_BLE_AD_TYPE_PUBLIC_TARGET            = 0x17,
    ATMO_BLE_AD_TYPE_RANDOM_TARGET            = 0x18,
    ATMO_BLE_AD_TYPE_APPEARANCE               = 0x19,
    ATMO_BLE_AD_TYPE_ADV_INT                  = 0x1A,
    ATMO_BLE_AD_TYPE_LE_DEV_ADDR              = 0x1b,
    ATMO_BLE_AD_TYPE_LE_ROLE                  = 0x1c,
    ATMO_BLE_AD_TYPE_SPAIR_C256               = 0x1d,
    ATMO_BLE_AD_TYPE_SPAIR_R256               = 0x1e,
    ATMO_BLE_AD_TYPE_32SOL_SRV_UUID           = 0x1f,
    ATMO_BLE_AD_TYPE_32SERVICE_DATA           = 0x20,
    ATMO_BLE_AD_TYPE_128SERVICE_DATA          = 0x21,
    ATMO_BLE_AD_TYPE_LE_SECURE_CONFIRM        = 0x22,
    ATMO_BLE_AD_TYPE_LE_SECURE_RANDOM         = 0x23,
    ATMO_BLE_AD_TYPE_URI                      = 0x24,
    ATMO_BLE_AD_TYPE_INDOOR_POSITION          = 0x25,
    ATMO_BLE_AD_TYPE_TRANS_DISC_DATA          = 0x26,
    ATMO_BLE_AD_TYPE_LE_SUPPORT_FEATURE       = 0x27,
    ATMO_BLE_AD_TYPE_CHAN_MAP_UPDATE          = 0x28,
    ATMO_BLE_AD_MANUFACTURER_SPECIFIC_TYPE    = 0xFF,
} ATMO_BLE_AdvertisingDataType_t;
/// \endcond

/**
 * BLE Advertising Configuration Parameters
 */
typedef struct
{
  uint16_t advertisingInterval;       /**< Time in ms */
  ATMO_BLE_AdvertisingType_t type;    /**< Type of advertising */
} ATMO_BLE_AdvertisingParams_t;

/**
 * Manufacturer specific advertising data
 */
typedef struct
{
  uint8_t length;             /**< Payload length in bytes */
  uint8_t *payload;             /**< Pointer to adv data buffer */
} ATMO_BLE_AdvertisingData_t;

/**
 * BLE MAC Address
 */
typedef struct
{
	uint8_t data[6]; /**< Raw MAC Address Data */
} ATMO_BLE_MacAddress_t;

#define ATMO_UUID_STR_LEN (37)

/***************************************************************************************
*********************************** Scanning Params ************************************
****************************************************************************************/
/// \cond DO_NOT_DOCUMENT

/**
 * BLE Master Scan Type
 */
typedef enum
{
  ATMO_BLE_SCAN_TYPE_PASSIVE  = 0x00,
  ATMO_BLE_SCAN_TYPE_ACTIVE   = 0x01,
} ATMO_BLE_ScanType_t;

/**
 * BLE Master Scan Configuration Parameters
 */
typedef struct
{
  uint16_t interval; /**< Scan interval in milliseconds */
  uint16_t timeout; /**< Timeout in milliseconds */
  ATMO_BLE_ScanType_t type; /**< Scan type */
} ATMO_BLE_ScanParams_t;

typedef struct
{
  uint16_t length;
  uint8_t *payload;
} ATMO_BLE_ScanData_t;

typedef struct
{
  /* TODO */
} ATMO_BLE_ScanResults_t;

typedef struct
{
  /* TODO */
} ATMO_BLE_CharProperties_t ;

/// \endcond

/***************************************************************************************
******************************* Pairing/Security Params ********************************
****************************************************************************************/
/**
 * BLE Security Mode
 */
typedef enum
{
  ATMO_BLE_PairingType_JustWorks,
  ATMO_BLE_PairingType_RandomKey,
  ATMO_BLE_PairingType_UserKey
} ATMO_BLE_PairingType_t;

/**
 * BLE Pairing Configuration
 */
typedef struct
{
  ATMO_BLE_PairingType_t type;
  uint32_t pairingKey;
} ATMO_BLE_PairingCfg_t;


typedef enum {
	ATMO_BLE_Property_Broadcast				= 0x01,
	ATMO_BLE_Property_Read					= 0x02,
	ATMO_BLE_Property_WriteWithoutResponse	= 0x04,
	ATMO_BLE_Property_Write					= 0x08,
	ATMO_BLE_Property_Notify				= 0x10,
	ATMO_BLE_Property_Indicate				= 0x20,
	ATMO_BLE_Property_Auth					= 0x40,
	ATMO_BLE_Property_ExtProp				= 0x80
} ATMO_BLE_Property_t;

/**
 * BLE Characteristic Permission
 */
typedef enum {
	ATMO_BLE_Permission_Read = 0x01,
	ATMO_BLE_Permission_ReadEncrypted = 0x02,
	ATMO_BLE_Permission_ReadEncryptedMITM = 0x04,
	ATMO_BLE_Permission_Write = 0x10,
	ATMO_BLE_Permission_WriteEncrypted = 0x20,
	ATMO_BLE_Permission_WriteEncryptedMITM = 0x40,
	ATMO_BLE_Permission_WriteSigned = 0x80,
	ATMO_BLE_Permission_WriteSIgnedMITM = 0x100,
} ATMO_BLE_Permission_t;

/**
 * BLE Characteristic Event. User can register callback on specific event for any characteristic.
 */
typedef enum {
	ATMO_BLE_Characteristic_Written,
	ATMO_BLE_Characteristic_Subscribed,
	ATMO_BLE_Characteristic_Unsubscribed,
	ATMO_BLE_Characteristic_NumEvents
} ATMO_BLE_Characteristic_Event_t;

typedef struct ATMO_BLE_DriverInstance_t ATMO_BLE_DriverInstance_t;

struct ATMO_BLE_DriverInstance_t{
	ATMO_BLE_Status_t (*PeripheralInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*PeripheralDeInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*SetEnabled)(ATMO_DriverInstanceData_t *instanceData, bool enabled);
	ATMO_BLE_Status_t (*GetEnabled)(ATMO_DriverInstanceData_t *instanceData, bool *enabled);
	ATMO_BLE_Status_t (*GetMacAddress)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_MacAddress_t *address);
	ATMO_BLE_Status_t (*GAPSetDeviceName)(ATMO_DriverInstanceData_t *instanceData, const char *name);
	ATMO_BLE_Status_t (*GAPAdvertisingStart)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_AdvertisingParams_t *params);
	ATMO_BLE_Status_t (*GAPAdvertisingStop)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*GAPSetAdvertisedServiceUUID)(ATMO_DriverInstanceData_t *instanceData, const char* uuid);
	ATMO_BLE_Status_t (*GAPAdverertisingSetManufacturerData)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_AdvertisingData_t *data);
	ATMO_BLE_Status_t (*GAPScanConfigure)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_ScanParams_t *params);
	ATMO_BLE_Status_t (*GAPScanStart)(ATMO_DriverInstanceData_t *instanceData, uint32_t length);
	ATMO_BLE_Status_t (*GAPScanStop)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*GAPScanSetData)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_ScanData_t *data);
	ATMO_BLE_Status_t (*GAPPairingCfg)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_PairingCfg_t *config);
	ATMO_BLE_Status_t (*GAPPair)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*GAPConnect)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_DeviceID_t scannedDeviceID);
	ATMO_BLE_Status_t (*GAPDisconnect)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_BLE_Status_t (*GAPPowerLevelConfigure)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_PowerLevel_t advertisingTxPower, ATMO_BLE_PowerLevel_t connectionTxPower);
	ATMO_BLE_Status_t (*GATTSAddService)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t *handle, const char *serviceUUID);
	ATMO_BLE_Status_t (*GATTSAddCharacteristic)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t serviceHandle, const char *characteristicUUID, uint8_t properties, uint8_t permissions, uint32_t maxLen);
	ATMO_BLE_Status_t (*GATTSGetCharacteristicValue)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint8_t *valueBuf, uint32_t valueBufLen, uint32_t *valueLen);
	ATMO_BLE_Status_t (*GATTSRegisterCharacteristicCallback)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, ATMO_BLE_Characteristic_Event_t event, ATMO_Callback_t cbFunc);
	ATMO_BLE_Status_t (*GATTSRegisterCharacteristicAbilityHandle)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, ATMO_BLE_Characteristic_Event_t event, unsigned int abilityHandler);
	ATMO_BLE_Status_t (*GATTSSetCharacteristic)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t length, uint8_t *value, ATMO_BLE_CharProperties_t *properties);
	ATMO_BLE_Status_t (*GATTSWriteDescriptor)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t length, uint8_t *value, ATMO_BLE_CharProperties_t *properties);
	ATMO_BLE_Status_t (*GATTSSendIndicate)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);
	ATMO_BLE_Status_t (*GATTSSendNotify)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);
	ATMO_BLE_Status_t (*GATTCDiscoverService)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_ServiceHandle_t serviceUUID, ATMO_BLE_ScanResults_t *result);
	ATMO_BLE_Status_t (*GATTCWriteCharacteristic)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);
	ATMO_BLE_Status_t (*GATTCReadCharacteristic)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *payload);
	ATMO_BLE_Status_t (*GATTCEnableNotification)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Handle_t handle, bool indicate);
	ATMO_BLE_Status_t (*RegisterEventCallback)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Event_t event, ATMO_Callback_t cb);
  ATMO_BLE_Status_t (*RegisterEventAbilityHandle)(ATMO_DriverInstanceData_t *instanceData, ATMO_BLE_Event_t event, unsigned int abilityHandle);
};

/* Exported Function Prototypes -----------------------------------------------*/


ATMO_Status_t ATMO_BLE_AddDriverInstance( const ATMO_BLE_DriverInstance_t* driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t* instanceNumber );

/**
 * This routine initializes BLE.
 *
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_PeripheralInit(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine de-initializes BLE.
 *
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_PeripheralDeInit(ATMO_DriverInstanceHandle_t instance);

/**
 * Set the enabled/disabled status of the BLE driver
 * 
 * This may behave differently based on your specific peripheral.
 * 
 * @param[in] enabled
 */
ATMO_BLE_Status_t ATMO_BLE_SetEnabled(ATMO_DriverInstanceHandle_t instance, bool enabled);

/**
 * Get the enabled/disabled status of the BLE driver
 * 
 * This may behave differently based on your specific peripheral.
 * 
 * @param[out] enabled
 */
ATMO_BLE_Status_t ATMO_BLE_GetEnabled(ATMO_DriverInstanceHandle_t instance, bool *enabled);



/***************************************************************************************
*********************************** BLE GAP Functions **********************************
****************************************************************************************/

/**
 * This routine gets the device MAC address
 */
ATMO_BLE_Status_t ATMO_BLE_GetMacAddress(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_MacAddress_t *address);

/**
 * This routine sets the device name.
 *
 * @param  name Device name
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPSetDeviceName(ATMO_DriverInstanceHandle_t instance, const char *name);

/**
 * This routine starts advertising. If advertising is already running then it should
 * turn off the current advertiment data and restart using the new advertisment data.
 *
 * @param params pointer to struct containing advertising parameters.
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPAdvertisingStart(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_AdvertisingParams_t *params);

/**
 * This routine stops advertising.
 *
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPAdvertisingStop(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine sets the advertised service UUID
 * 
 * @param uuid as string
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GAPSetAdvertisedServiceUUID(ATMO_DriverInstanceHandle_t instance, const char* uuid);

/**
 * This routine sets the advertising data if we want to change from default
 *
 * @param data Pointer to user defined advertising data struct
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPAdverertisingSetManufacturerData(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_AdvertisingData_t *data);

/**
 * This routine configures scanning with the parameters provided
 *
 * @param params pointer to struct containing scanning parameters.
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPScanConfigure(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ScanParams_t *params);

/**
 * This routine starts the device to scan for other devices.
 *
 * @param length Length of time in seconds the device should scan for.
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPScanStart(ATMO_DriverInstanceHandle_t instance, uint32_t length);

/**
 * This routine stops the device from scanning.
 *
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPScanStop(ATMO_DriverInstanceHandle_t instance);


/**
 * This routine sets the scan response data for BLE.
 *
 * @param data Struct containing the scan response data
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GAPScanSetData(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ScanData_t *data);


/**
 * This routine configures pairing parameters.
 *
 * @param config Struct containing available security keys, bonding preferences, etc.
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GAPPairingCfg(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_PairingCfg_t *config);


/**
 * TODO
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GAPPair(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine connects this device to another scanned device
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GAPConnect(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_DeviceID_t scannedDeviceID);

/**
 * This routine is to disconnect from a device
 *
 * @param device Address of peer device.
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GAPDisconnect(ATMO_DriverInstanceHandle_t instance);


/**
 * This routine allows to override default Tx power levels
 *
 * @param advertisingTxPower Level of advertising Tx power
 * @param connectionTxPower  Level of connection Tx power
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GAPPowerLevelConfigure(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_PowerLevel_t advertisingTxPower, ATMO_BLE_PowerLevel_t connectionTxPower);

/***************************************************************************************
***************************** BLE GATT SERVER Functions ********************************
*********************************** (GATTDB) *******************************************
****************************************************************************************/

/**
 * This routine creates and starts a service
 *
 * @param handle
 * @param serviceID UUID for this service
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSAddService(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t *handle, const char *serviceUUID);

/**
 * This routine creates a characteristic
 * 
 * @param handle
 * @param serviceHandle - parent service
 * @param characteristicUUID
 * @param properties - Bitfield of ATMO_BLE_Property_t
 * @param maxLen - maximum length of characteristic value
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSAddCharacteristic(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t *handle, ATMO_BLE_Handle_t serviceHandle, const char *characteristicUUID, uint8_t properties, uint8_t permissions, uint32_t maxLen);

/**
 * Get a pointer to the characteristic value
 * 
 * @param handle
 * @param valueBuf - User supplied buffer that the characteristic value will be copied into
 * @param valueBufLen - size of user specified buffer
 * @param valueLen - size of characteristic copied into buffer (bytes)
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSGetCharacteristicValue(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint8_t *valueBuf, uint32_t valueBufLen, uint32_t *valueLen);

/**
 * Register callback for GATTS Characteristic Events
 * 
 * @param handle
 * @param event
 * @param cbFunc
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSRegisterCharacteristicCallback(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, ATMO_BLE_Characteristic_Event_t event, ATMO_Callback_t cbFunc);

/**
 * Register ability handle for GATTS Characteristic Events
 * 
 * @param handle
 * @param event
 * @param cbFunc
 * @return status
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSRegisterCharacteristicAbilityHandle(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, ATMO_BLE_Characteristic_Event_t event, unsigned int abilityHandler);

/**
 * This routine writes to a characteristic in the gattdb.
 *
 * @param handle Characteristic handle
 * @param length Length of data
 * @param value Pointer to data
 * @param properties TBD
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSSetCharacteristic(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t length, uint8_t *value, ATMO_BLE_CharProperties_t *properties);

/**
 * This routine writes to a characteristic descriptor in gattdb.
 *
 * @param handle Characteristic handle
 * @param length Length of data
 * @param value Pointer to data
 * @param properties TBD
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSWriteDescriptor(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t length, uint8_t *value, ATMO_BLE_CharProperties_t *properties);

/**
 * This routine sends an indicate to the GATT Client
 *
 * @param handle
 * @param size
 * @param value
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSSendIndicate(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);

/**
 * @brief This routine sends a notification to the GATT Client
 * 
 * @param instance 
 * @param handle 
 * @param size 
 * @param value - Optional value. If NULL, current characteristic value will be sent.
 * @return ATMO_BLE_Status_t 
 */
ATMO_BLE_Status_t ATMO_BLE_GATTSSendNotify(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);


/***************************************************************************************
***************************** BLE GATT CLIENT functions ********************************
****************************************************************************************/

/**
 * This routine executes a GATT service discovery for all or specific services.
 * This will return from the service(s) from the gattdb.
 *
 * @param serviceUUID If set to 0 discover all services, else discover service with this UUID.
 * @param result Pointer to struct containing results of discovery
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GATTCDiscoverService(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ServiceHandle_t serviceUUID, ATMO_BLE_ScanResults_t *result);

/**
 * This routine writes a value to a characteristic in the gattdb.
 *
 * @param interface
 * @param connectionId
 * @param characteristicHandle
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GATTCWriteCharacteristic(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value);

/**
 * This routine reads a value from a characteristic in the gattdb.
 *
 * @param interface
 * @param connectionId
 * @param characteristicHandle
 * @return
 */
ATMO_BLE_Status_t ATMO_BLE_GATTCReadCharacteristic(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *payload);

/**
 * This routine registers this device for notifications from a characteristic in gattdb.
 *
 * @param handle Characteristic we want to receive notifications for.
 * @param indicate Set to false if we want notification, true if we want notifications
 * @return status.
 */
ATMO_BLE_Status_t ATMO_BLE_GATTCEnableNotification(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, bool indicate);


/***************************************************************************************
***************************** BLE Callback functions ***********************************
****************************************************************************************/
ATMO_BLE_Status_t ATMO_BLE_RegisterEventCallback(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Event_t event, ATMO_Callback_t cb);
ATMO_BLE_Status_t ATMO_BLE_RegisterEventAbilityHandle(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Event_t event, unsigned int abilityHandle);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_BLE__H */

/** @}*/
