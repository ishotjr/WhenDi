/**
 ******************************************************************************
 * @file    ble.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - Core BLE Driver
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
 *   3. Neither the name of Atmosphere IoT Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
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

#include "ble.h"

#define NUMBER_OF_BLE_DRIVER_INSTANCES 4

ATMO_DriverInstanceHandle_t numberOfBLEDriverInstance = 0;

const ATMO_BLE_DriverInstance_t *bleInstances[NUMBER_OF_BLE_DRIVER_INSTANCES];
ATMO_DriverInstanceData_t *bleInstancesData[NUMBER_OF_BLE_DRIVER_INSTANCES];

ATMO_Status_t ATMO_BLE_AddDriverInstance( const ATMO_BLE_DriverInstance_t* driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t* instanceNumber )
{
	if( !( numberOfBLEDriverInstance < NUMBER_OF_BLE_DRIVER_INSTANCES ) )
	{
		return ATMO_Status_OutOfMemory;
	}

	bleInstances[numberOfBLEDriverInstance] = driverInstance;
	bleInstancesData[numberOfBLEDriverInstance] = driverInstanceData;
	*instanceNumber = numberOfBLEDriverInstance;

	numberOfBLEDriverInstance++;

	return ATMO_Status_Success;
}

ATMO_BLE_Status_t ATMO_BLE_PeripheralInit( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->PeripheralInit( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_SetEnabled(ATMO_DriverInstanceHandle_t instance, bool enabled)
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->SetEnabled( bleInstancesData[instance], enabled );	
}

ATMO_BLE_Status_t ATMO_BLE_GetEnabled(ATMO_DriverInstanceHandle_t instance, bool *enabled)
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GetEnabled( bleInstancesData[instance], enabled );
}

ATMO_BLE_Status_t ATMO_BLE_PeripheralDeInit( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->PeripheralDeInit( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_GetMacAddress(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_MacAddress_t *address)
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GetMacAddress( bleInstancesData[instance], address );
}

ATMO_BLE_Status_t ATMO_BLE_GAPSetDeviceName( ATMO_DriverInstanceHandle_t instance, const char* name )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPSetDeviceName( bleInstancesData[instance], name );
}

ATMO_BLE_Status_t ATMO_BLE_GAPAdvertisingStart( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_AdvertisingParams_t* params )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPAdvertisingStart( bleInstancesData[instance], params );
}

ATMO_BLE_Status_t ATMO_BLE_GAPAdvertisingStop( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPAdvertisingStop( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_GAPSetAdvertisedServiceUUID( ATMO_DriverInstanceHandle_t instance, const char* uuid )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPSetAdvertisedServiceUUID( bleInstancesData[instance], uuid );
}

ATMO_BLE_Status_t ATMO_BLE_GAPAdverertisingSetManufacturerData( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_AdvertisingData_t* data )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPAdverertisingSetManufacturerData( bleInstancesData[instance], data );
}

ATMO_BLE_Status_t ATMO_BLE_GAPScanConfigure( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ScanParams_t* params )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPScanConfigure( bleInstancesData[instance], params );
}

ATMO_BLE_Status_t ATMO_BLE_GAPScanStart( ATMO_DriverInstanceHandle_t instance, uint32_t length )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPScanStart( bleInstancesData[instance], length );
}

ATMO_BLE_Status_t ATMO_BLE_GAPScanStop( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPScanStop( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_GAPScanSetData( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ScanData_t* data )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPScanSetData( bleInstancesData[instance], data );
}

ATMO_BLE_Status_t ATMO_BLE_GAPPairingCfg( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_PairingCfg_t* config )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPPairingCfg( bleInstancesData[instance], config );
}

ATMO_BLE_Status_t ATMO_BLE_GAPPair( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPPair( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_GAPConnect( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_DeviceID_t scannedDeviceID )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPConnect( bleInstancesData[instance], scannedDeviceID );
}

ATMO_BLE_Status_t ATMO_BLE_GAPDisconnect( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPDisconnect( bleInstancesData[instance] );
}

ATMO_BLE_Status_t ATMO_BLE_GAPPowerLevelConfigure( ATMO_DriverInstanceHandle_t instance,
		ATMO_BLE_PowerLevel_t advertisingTxPower, ATMO_BLE_PowerLevel_t connectionTxPower )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GAPPowerLevelConfigure( bleInstancesData[instance], advertisingTxPower, connectionTxPower );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSAddService( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t* handle,
		const char* serviceUUID )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSAddService( bleInstancesData[instance], handle, serviceUUID );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSAddCharacteristic( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t* handle,
		ATMO_BLE_Handle_t serviceHandle, const char* characteristicUUID, uint8_t properties, uint8_t permissions, uint32_t maxLen )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}
	
	if(maxLen == 0) {
		maxLen = 1;
	}

	return bleInstances[instance]->GATTSAddCharacteristic( bleInstancesData[instance], handle, serviceHandle, characteristicUUID, properties, permissions, maxLen );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSGetCharacteristicValue( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		uint8_t* valueBuf, uint32_t valueBufLen, uint32_t* valueLen )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSGetCharacteristicValue( bleInstancesData[instance], handle, valueBuf, valueBufLen, valueLen );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSRegisterCharacteristicCallback(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, ATMO_BLE_Characteristic_Event_t event, ATMO_Callback_t cbFunc)
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSRegisterCharacteristicCallback( bleInstancesData[instance], handle, event, cbFunc );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSRegisterCharacteristicAbilityHandle( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		ATMO_BLE_Characteristic_Event_t event, unsigned int abilityHandler )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSRegisterCharacteristicAbilityHandle( bleInstancesData[instance], handle, event, abilityHandler );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSSetCharacteristic( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		uint16_t length, uint8_t* value, ATMO_BLE_CharProperties_t* properties )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSSetCharacteristic( bleInstancesData[instance], handle, length, value, properties );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSWriteDescriptor( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		uint16_t length, uint8_t* value, ATMO_BLE_CharProperties_t* properties )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSWriteDescriptor( bleInstancesData[instance], handle, length, value, properties );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSSendIndicate( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size,
		uint8_t* value )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSSendIndicate( bleInstancesData[instance], handle, size, value );
}

ATMO_BLE_Status_t ATMO_BLE_GATTSSendNotify( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTSSendNotify( bleInstancesData[instance], handle, size, value );
}

ATMO_BLE_Status_t ATMO_BLE_GATTCDiscoverService( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_ServiceHandle_t serviceUUID,
		ATMO_BLE_ScanResults_t* result )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTCDiscoverService( bleInstancesData[instance], serviceUUID, result );
}

ATMO_BLE_Status_t ATMO_BLE_GATTCWriteCharacteristic( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		uint16_t size, uint8_t* value )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTCWriteCharacteristic( bleInstancesData[instance], handle, size, value );
}

ATMO_BLE_Status_t ATMO_BLE_GATTCReadCharacteristic( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		uint16_t size, uint8_t* payload )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTCReadCharacteristic( bleInstancesData[instance], handle, size, payload );
}

ATMO_BLE_Status_t ATMO_BLE_GATTCEnableNotification( ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Handle_t handle,
		bool indicate )
{
	if( !( instance < numberOfBLEDriverInstance ) )
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->GATTCEnableNotification( bleInstancesData[instance], handle, indicate );
}

ATMO_BLE_Status_t ATMO_BLE_RegisterEventCallback(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Event_t event, ATMO_Callback_t cb)
{
	if( !( instance < numberOfBLEDriverInstance )  || (event >= ATMO_BLE_EVENT_NumEvents))
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->RegisterEventCallback( bleInstancesData[instance], event, cb );	
}

ATMO_BLE_Status_t ATMO_BLE_RegisterEventAbilityHandle(ATMO_DriverInstanceHandle_t instance, ATMO_BLE_Event_t event, unsigned int abilityHandle)
{
	if( !( instance < numberOfBLEDriverInstance ) || (event >= ATMO_BLE_EVENT_NumEvents))
	{
		return ATMO_BLE_Status_Invalid;
	}

	return bleInstances[instance]->RegisterEventAbilityHandle( bleInstancesData[instance], event, abilityHandle );
}
