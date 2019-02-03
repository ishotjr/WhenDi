/**
 ******************************************************************************
 * @file    cloud_ble.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - CLOUD over BLE Driver
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

#include "cloud_ble.h"
#include "cloud_config.h"
#include "../app_src/atmosphere_platform.h"
#include "../app_src/atmosphere_globals.h"
#include "../datetime/datetime.h"
#include "../wifi/wifi.h"

const ATMO_CLOUD_DriverInstance_t cloudBleDriverInstance = {
	ATMO_CLOUD_BLE_Init,
	ATMO_CLOUD_BLE_SubDriverSyncRegistration,
	ATMO_CLOUD_BLE_SendEvent,
	ATMO_CLOUD_BLE_PopCommand
};

typedef struct
{
	bool connectionVerified;
	ATMO_DriverInstanceHandle_t bleInstanceNumber;
	ATMO_BLE_Handle_t bleServiceHandle;
	ATMO_BLE_Handle_t bleCharacteristicHandle;
	ATMO_CLOUD_RegistrationInfo_t currentRegistrationInfo;
} __ATMO_CLOUD_BLE_DriverInstanceConfig_t;

typedef enum
{
	ATMO_CLOUD_BLE_PROVISIONING,
	ATMO_CLOUD_BLE_PROVISIONED,
	ATMO_CLOUD_BLE_WAITING_FOR_GATEWAY,
	ATMO_CLOUD_BLE_NUM_PROVISION_STATUS,
} ATMO_CLOUD_BLE_ProvisionStatus_t;

char provisioningStatusChar[ATMO_CLOUD_BLE_NUM_PROVISION_STATUS] = {'P', 'D', 'G'};

static __ATMO_CLOUD_BLE_DriverInstanceConfig_t *currentBleDriverInstanceConfig = NULL;

static void __ATMO_CLOUD_BLE_GetAdvName(ATMO_CLOUD_BLE_ProvisionStatus_t status, char *name)
{
	ATMO_BLE_MacAddress_t macAddress;
	if(ATMO_BLE_GetMacAddress(currentBleDriverInstanceConfig->bleInstanceNumber, &macAddress) == ATMO_BLE_Status_Success)
	{
		sprintf(name, "%c%02X%02X", provisioningStatusChar[status], macAddress.data[4], macAddress.data[5]);
	}
	else
	{
		sprintf(name, "%cFFFF", provisioningStatusChar[status]);
	}
}

ATMO_CLOUD_Status_t __ATMO_CLOUD_BLE_StartAdvertisingForProvisioning(__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config)
{
	char deviceName[32];
	__ATMO_CLOUD_BLE_GetAdvName(ATMO_CLOUD_BLE_PROVISIONING, deviceName);

	ATMO_PLATFORM_DebugPrint("Setting device name: %s\r\n", deviceName);
	ATMO_BLE_GAPSetDeviceName(config->bleInstanceNumber, deviceName);

	ATMO_PLATFORM_DebugPrint("Setting service uuid\r\n");
	ATMO_BLE_GAPSetAdvertisedServiceUUID(config->bleInstanceNumber, ATMO_GLOBALS_PROJECTUUID);

	ATMO_BLE_AdvertisingParams_t advParams;
	advParams.type = ATMO_BLE_ADV_TYPE_INDIRECT;
	advParams.advertisingInterval = 1000;
	ATMO_PLATFORM_DebugPrint("Starting advertising\r\n");
	ATMO_BLE_GAPAdvertisingStart(config->bleInstanceNumber, &advParams);
	
	return ATMO_CLOUD_Status_Success;
}

ATMO_CLOUD_Status_t __ATMO_CLOUD_BLE_StartAdvertisingForRegistered(__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config)
{
	char deviceName[32];
	__ATMO_CLOUD_BLE_GetAdvName(ATMO_CLOUD_BLE_PROVISIONED, deviceName);

	ATMO_BLE_GAPSetDeviceName(config->bleInstanceNumber, deviceName);

	if(config->currentRegistrationInfo.registered == false)
	{
		return ATMO_CLOUD_Status_Invalid;
	}
	
	char uuidBuffer[40];
	
	ATMO_CLOUD_GetRegistrationInfoUuid(&config->currentRegistrationInfo, uuidBuffer, 40);
	
	ATMO_BLE_GAPSetAdvertisedServiceUUID(config->bleInstanceNumber, uuidBuffer);

	ATMO_BLE_AdvertisingParams_t advParams;
	advParams.type = ATMO_BLE_ADV_TYPE_INDIRECT;
	advParams.advertisingInterval = 1000;
	ATMO_BLE_GAPAdvertisingStart(config->bleInstanceNumber, &advParams);
	
	return ATMO_CLOUD_Status_Success;
}

ATMO_CLOUD_Status_t __ATMO_CLOUD_BLE_StartAdvertising(__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config)
{
	ATMO_BLE_GAPAdvertisingStop(config->bleInstanceNumber);
	if(config->currentRegistrationInfo.registered)
	{
		return __ATMO_CLOUD_BLE_StartAdvertisingForRegistered(config);
	}
	
	else
	{
		return __ATMO_CLOUD_BLE_StartAdvertisingForProvisioning(config);
	}
}

void __ATMO_CLOUD_BLE_ConnectedCallback(void *value)
{
	currentBleDriverInstanceConfig->connectionVerified = false;
	ATMO_BLE_GAPAdvertisingStop(currentBleDriverInstanceConfig->bleInstanceNumber);
}

void __ATMO_CLOUD_BLE_DisconnectedCallback(void *value)
{
	currentBleDriverInstanceConfig->connectionVerified = false;
	__ATMO_CLOUD_BLE_StartAdvertising(currentBleDriverInstanceConfig);
}

ATMO_CLOUD_Status_t __ATMO_CLOUD_BLE_StartAdvertisingForGateway(__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config)
{
	return ATMO_CLOUD_Status_NotSupported;
}

static void __ATMO_CLOUD_BLE_SendCommandResponse(uint8_t *data, unsigned int dataLen)
{
		ATMO_BLE_GATTSSetCharacteristic(
			currentBleDriverInstanceConfig->bleInstanceNumber, 
			currentBleDriverInstanceConfig->bleCharacteristicHandle, 
			dataLen, 
			data, 
			NULL);
}

static void __ATMO_CLOUD_BLE_SendGenericSuccess(ATMO_CLOUD_BLE_CommandType_t command)
{
	uint8_t response[2] = {command, ATMO_CLOUD_Status_Success};
	__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
}

static void __ATMO_CLOUD_BLE_SendGenericFail(ATMO_CLOUD_BLE_CommandType_t command)
{
	uint8_t response[2] = {command, ATMO_CLOUD_Status_Fail};
	__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
}

static void __ATMO_CLOUD_BLE_HandleSigfoxSubCommand(ATMO_CLOUD_BLE_SubCommand_Sigfox_t subCommand, uint8_t *data, unsigned int dataLen)
{
	switch(subCommand)
	{
		case ATMO_CLOUD_BLE_SubCommand_SigfoxExtraSettings_GetDeviceId:
		{
			ATMO_Value_t deviceId;
			ATMO_InitValue(&deviceId);
			if( ATMO_CLOUD_GetConfig(ATMO_CLOUD_CONFIG(SIGFOX,DEVICE_ID), &deviceId) != ATMO_CLOUD_Status_Success )
			{
				ATMO_FreeValue(&deviceId);
				__ATMO_CLOUD_BLE_SendGenericFail(ATMO_CLOUD_BLE_CommandType_ExtraSettingsSigfox);
			}
			else
			{
				unsigned int deviceIdVal;
				ATMO_GetUnsignedInt(&deviceId, &deviceIdVal);
				ATMO_FreeValue(&deviceId);

				// Need to do this because some compilers get picky if you put a uint32_t into ATMO_GetUnsignedInt
				uint32_t deviceIdVal32Bit = (uint32_t)(deviceIdVal);
				
				uint8_t response[7] = 
				{	
					ATMO_CLOUD_BLE_CommandType_ExtraSettingsSigfox,
					ATMO_CLOUD_BLE_SubCommand_SigfoxExtraSettings_GetDeviceId,
					ATMO_CLOUD_Status_Success,
					deviceIdVal32Bit & 0xFF,
					(deviceIdVal32Bit >> 8) & 0xFF,
					(deviceIdVal32Bit >> 16) & 0xFF,
					(deviceIdVal32Bit >> 24) & 0xFF
				};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 7);
			}
			break;
		}
	}
}

static void __ATMO_CLOUD_BLE_HandleWifiSubCommand(ATMO_CLOUD_BLE_SubCommand_Wifi_t subCommand, uint8_t *data, unsigned int dataLen)
{
	switch(subCommand)
	{
		case ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_GetStatus:
		{
			ATMO_WIFI_ConnectionStatus_t wifiStatus;
			ATMO_WIFI_GetStatus(0, &wifiStatus);

			uint8_t response[22];
			unsigned int responseLen = 0;

			response[responseLen++] = ATMO_CLOUD_BLE_CommandType_ExtraSettingsWifi;
			response[responseLen++] = ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_GetStatus;
			response[responseLen++] = ATMO_CLOUD_Status_Success;
			response[responseLen++] = (uint8_t)wifiStatus.connectivity;
			response[responseLen++] = (uint8_t)wifiStatus.disconnectReason;
			response[responseLen++] = (uint8_t)wifiStatus.ipType;

			// IP Address
			memcpy(&response[responseLen], wifiStatus.ipAddress, 16);
			responseLen += 16;
			__ATMO_CLOUD_BLE_SendCommandResponse(response, responseLen);
			break;
		}
		case ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_SetSSID:
		{
			ATMO_Value_t atmoValue;
			ATMO_InitValue(&atmoValue);
			ATMO_CreateValueString(&atmoValue, (char *)data);
			ATMO_CLOUD_SetConfig(ATMO_CLOUD_CONFIG(WIFI, SSID), &atmoValue);
			ATMO_FreeValue(&atmoValue);

			uint8_t response[3] = {ATMO_CLOUD_BLE_CommandType_ExtraSettingsWifi, ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_SetSSID, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 3);
			break;
		}
		case ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_SetPassword:
		{
			ATMO_Value_t atmoValue;
			ATMO_InitValue(&atmoValue);
			ATMO_CreateValueString(&atmoValue, (char *)data);
			ATMO_CLOUD_SetConfig(ATMO_CLOUD_CONFIG(WIFI, PASS), &atmoValue);
			ATMO_FreeValue(&atmoValue);

			uint8_t response[3] = {ATMO_CLOUD_BLE_CommandType_ExtraSettingsWifi, ATMO_CLOUD_BLE_SubCommand_WifiExtraSettings_SetPassword, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 3);
			break;
		}
	}
}

void __ATMO_CLOUD_BLE_CharacteristicWrittenCallback(void *value)
{
	
	//We don't have a config to operatate on.
	if(currentBleDriverInstanceConfig == NULL)
	{
		return;
	}
	

	//The BLE Driver returns the current value
	//written as an ATMO_Value_t object.
	ATMO_Value_t *currentValue = (ATMO_Value_t *)value;
	
	//We expect a binary value.
	if(currentValue->type != ATMO_DATATYPE_BINARY || currentValue->size < 1)
	{
		uint8_t response[3] = {0xFF, currentValue->type, 0};
		__ATMO_CLOUD_BLE_SendCommandResponse(response, 3);
		return;
	}
	
	uint8_t *data = (uint8_t *) currentValue->data;

	ATMO_PLATFORM_DebugPrint("[BLE Provision] RX CMD %02X\r\n", data[0]);
	
	switch((ATMO_CLOUD_BLE_CommandType_t)data[0])
	{
		case ATMO_CLOUD_BLE_CommandType_GetIndentityType:
		{
			uint16_t extraSettings = ATMO_CLOUD_GetExtraRequiredSettings();

			uint8_t response[4] = {ATMO_CLOUD_BLE_CommandType_GetIndentityType, ATMO_CLOUD_BLE_IdentityType_Basic,
									extraSettings & 0xFF, (extraSettings >> 8) & 0xFF};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 4);		
			break;
		}
		
		case ATMO_CLOUD_BLE_CommandType_BasicCentralConfirmIdentity:
		{
			//FIXME: We need to actually use the basic certification method.
			//SECURITY: This feature must be complete before launch.
			currentBleDriverInstanceConfig->connectionVerified = true;

			__ATMO_CLOUD_BLE_SendGenericSuccess(ATMO_CLOUD_BLE_CommandType_BasicCentralConfirmIdentity);
			break;
		}
		
		case ATMO_CLOUD_BLE_CommandType_BasicClientConfirmIdentity:
			break;
			
		case ATMO_CLOUD_BLE_CommandType_LockRegistrationInfo:
		{
			if(currentBleDriverInstanceConfig->currentRegistrationInfo.registered == true || currentBleDriverInstanceConfig->connectionVerified == false) {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_LockRegistrationInfo, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
		
				return;
			}
			
			currentBleDriverInstanceConfig->currentRegistrationInfo.registered = true;
			
			ATMO_CLOUD_Status_t saveStatus = ATMO_CLOUD_SetRegistration(&currentBleDriverInstanceConfig->currentRegistrationInfo);

			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_LockRegistrationInfo, saveStatus};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
				
			break;
		}
			
		case ATMO_CLOUD_BLE_CommandType_GetRegistrationInfo:
		{
			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_GetRegistrationInfo, ATMO_CLOUD_Status_Fail};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			break;
		}
			
		case ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUrl:
		{
			
			if(currentBleDriverInstanceConfig->currentRegistrationInfo.registered == true || currentBleDriverInstanceConfig->connectionVerified == false) {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUrl, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
		
				return;
			}
			
			char *url = (char *)&data[1];
			
			ATMO_CLOUD_SetRegistrationInfoUrl(
				&currentBleDriverInstanceConfig->currentRegistrationInfo,
				url);
			
			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUrl, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			
			break;
		}
			
		case ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUuid:
		{
			
			if(currentBleDriverInstanceConfig->currentRegistrationInfo.registered == true || currentBleDriverInstanceConfig->connectionVerified == false) {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUuid, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
		
				return;
			}
			
			uint8_t *uuidData = &data[1];
			
			memcpy(currentBleDriverInstanceConfig->currentRegistrationInfo.uuid, uuidData, 16);
			
			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoUuid, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			
			break;
		}
			
		case ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoToken:
		{
			
			if(currentBleDriverInstanceConfig->currentRegistrationInfo.registered == true || currentBleDriverInstanceConfig->connectionVerified == false) {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoToken, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
		
				return;
			}
			
			char *token = (char *)&data[1];
			
			ATMO_CLOUD_SetRegistrationInfoToken(
				&currentBleDriverInstanceConfig->currentRegistrationInfo,
				token);
			
			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetRegistrationInfoToken, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			
			break;
		}

		case ATMO_CLOUD_BLE_CommandType_ClearRegistrationInfo:
		{
			ATMO_CLOUD_ClearRegistration();

			uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_ClearRegistrationInfo, ATMO_CLOUD_Status_Success};
			__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			break;
		}

		case ATMO_CLOUD_BLE_CommandType_SetDateTimeIsoStr:
		{
			if(currentValue->size == 0)
			{
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetDateTimeIsoStr, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);

				break;

			}
			else
			{
				//FIXME: We should have configuration for which ISO Date instance a CLOUD Driver is using
				char *dateTime = (char *)&data[1];
				
				ATMO_DateTime_SetDateTimeIsoStr(0, dateTime);

				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetDateTimeIsoStr, ATMO_CLOUD_Status_Success};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			}
			break;
		}
		
		case ATMO_CLOUD_BLE_CommandType_UnlockRegistrationInfo:
		{
			if(currentBleDriverInstanceConfig->currentRegistrationInfo.registered == false) {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_UnlockRegistrationInfo, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
				return;
			}
			
			char savedToken[32];
			
			ATMO_CLOUD_GetRegistrationInfoToken(&currentBleDriverInstanceConfig->currentRegistrationInfo, savedToken, 32);
			
			char *token = (char *)&data[1];
			
			if(strncmp(token, savedToken, 32) == 0) {
				currentBleDriverInstanceConfig->currentRegistrationInfo.registered = false;

				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_UnlockRegistrationInfo, ATMO_CLOUD_Status_Success};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
				
				return;
			}
			
			else {
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_UnlockRegistrationInfo, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
		
				return;
			}
				
			break;
		}
		case ATMO_CLOUD_BLE_CommandType_SetDateTimeEpochInt:
		{
			if(currentValue->size != 5)
			{
				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetDateTimeEpochInt, ATMO_CLOUD_Status_Fail};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
				break;

			}
			else
			{
				uint32_t dateEpoch = 0;
				memcpy(&dateEpoch, &data[1], sizeof(uint32_t));
				ATMO_DateTime_SetDateTimeEpoch(0, dateEpoch);

				uint8_t response[2] = {ATMO_CLOUD_BLE_CommandType_SetDateTimeEpochInt, ATMO_CLOUD_Status_Success};
				__ATMO_CLOUD_BLE_SendCommandResponse(response, 2);
			}
			break;
		}
		case ATMO_CLOUD_BLE_CommandType_ExtraSettingsWifi:
		{
			if(currentValue->size < 2)
			{
				__ATMO_CLOUD_BLE_SendGenericFail(ATMO_CLOUD_BLE_CommandType_ExtraSettingsWifi);
				return;
			}

			uint8_t *wifiCommandData = (currentValue->size > 2) ? &data[2] : NULL;
			unsigned int wifiCommandDataSize = (currentValue->size > 2) ? currentValue->size - 2 : 0;
			__ATMO_CLOUD_BLE_HandleWifiSubCommand((ATMO_CLOUD_BLE_SubCommand_Wifi_t)data[1], wifiCommandData, wifiCommandDataSize);
			break;
		}
		case ATMO_CLOUD_BLE_CommandType_ExtraSettingsSigfox:
		{
			if(currentValue->size < 2)
			{
				__ATMO_CLOUD_BLE_SendGenericFail(ATMO_CLOUD_BLE_CommandType_ExtraSettingsSigfox);
				return;
			}

			uint8_t *sfCommandData = (currentValue->size > 2) ? &data[2] : NULL;
			unsigned int sfCommandDataSize = (currentValue->size > 2) ? currentValue->size - 2 : 0;
			__ATMO_CLOUD_BLE_HandleSigfoxSubCommand((ATMO_CLOUD_BLE_SubCommand_Sigfox_t)data[1], sfCommandData, sfCommandDataSize);
			break;
		}
		default:
   			break;
	};
	
}

ATMO_Status_t ATMO_CLOUD_BLE_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber, ATMO_DriverInstanceHandle_t bleInstanceNumber)
{
	
	__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config = (__ATMO_CLOUD_BLE_DriverInstanceConfig_t *)ATMO_Malloc(sizeof(__ATMO_CLOUD_BLE_DriverInstanceConfig_t));
	
	config->connectionVerified = false;
	config->bleInstanceNumber = bleInstanceNumber;
	ATMO_CLOUD_InitRegistrationInfo(&config->currentRegistrationInfo);
	
	ATMO_DriverInstanceData_t *cloudDriverInstanceData = (ATMO_DriverInstanceData_t *)ATMO_Malloc(sizeof(ATMO_DriverInstanceData_t));
	
	cloudDriverInstanceData->name = "Cloud BLE";
	cloudDriverInstanceData->initialized = false;
	cloudDriverInstanceData->instanceNumber = 0;
	cloudDriverInstanceData->argument = (void *)config;
	
	return ATMO_CLOUD_AddDriverInstance( &cloudBleDriverInstance, cloudDriverInstanceData, instanceNumber );
}

ATMO_CLOUD_Status_t ATMO_CLOUD_BLE_Init(ATMO_DriverInstanceData_t *instance)
{
	__ATMO_CLOUD_BLE_DriverInstanceConfig_t *config = (__ATMO_CLOUD_BLE_DriverInstanceConfig_t *)instance->argument;
	
		//FIXME: Really need to make it so that there can be
	// multiple Cloud BLE driver instances which this
	// kind of prevents.
	currentBleDriverInstanceConfig = config;
	
	ATMO_BLE_PeripheralInit(config->bleInstanceNumber);
	
	ATMO_BLE_GATTSAddService(
		config->bleInstanceNumber,
		&config->bleServiceHandle, 
		ATMO_CLOUD_BLE_PRIMARY_SERVICE_UUID);
	
	ATMO_BLE_GATTSAddCharacteristic(
		config->bleInstanceNumber,
		&config->bleCharacteristicHandle, 
		config->bleServiceHandle, 
		ATMO_CLOUD_BLE_PRIMARY_CHARACTERISTIC_UUID, 
		ATMO_BLE_Property_Read | ATMO_BLE_Property_Write | ATMO_BLE_Property_Notify, ATMO_BLE_Permission_Read | ATMO_BLE_Permission_Write, 64);
	
	instance->initialized = true;
	
	ATMO_BLE_GATTSRegisterCharacteristicCallback(
		config->bleInstanceNumber, 
		config->bleCharacteristicHandle, 
		ATMO_BLE_Characteristic_Written,
		__ATMO_CLOUD_BLE_CharacteristicWrittenCallback);
	
	ATMO_CLOUD_BLE_SubDriverSyncRegistration(instance);
	
	ATMO_BLE_RegisterEventCallback(config->bleInstanceNumber, ATMO_BLE_EVENT_Connected, __ATMO_CLOUD_BLE_ConnectedCallback);
	ATMO_BLE_RegisterEventCallback(config->bleInstanceNumber, ATMO_BLE_EVENT_Disconnected, __ATMO_CLOUD_BLE_DisconnectedCallback);
	
	__ATMO_CLOUD_BLE_StartAdvertising(config);
	
	return ATMO_CLOUD_Status_Success;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_BLE_SendEvent(ATMO_DriverInstanceData_t *instance, const char *eventName, ATMO_Value_t *data, uint32_t timeout)
{
	return ATMO_CLOUD_Status_NotSupported;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_BLE_PopCommand(ATMO_DriverInstanceData_t *instance, const char *commandName, ATMO_Value_t *data, uint32_t timeout)
{
	return ATMO_CLOUD_Status_NotSupported;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_BLE_SubDriverSyncRegistration(ATMO_DriverInstanceData_t *instance)
{
	// Load registration or default
	if(ATMO_CLOUD_GetRegistration(&currentBleDriverInstanceConfig->currentRegistrationInfo) != ATMO_CLOUD_Status_Success)
	{
		ATMO_CLOUD_InitRegistrationInfo(&currentBleDriverInstanceConfig->currentRegistrationInfo);
	}

	return ATMO_CLOUD_Status_Success;
}
