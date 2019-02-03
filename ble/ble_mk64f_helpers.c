#include "ble_mk64f_helpers.h"
#include "a100x_interface.h"
#include "atmosphere_platform.h"

#define gcGapMaxAdStructures                (5)

static ATMO_BLE_Handle_t gattHandle = 0;
static ATMO_BLE_Handle_t serviceChangedHandle = 0;

static uint8_t DEVICE_NAME[]                 = "Atmosphere Project";

#define AUTH_SERVICE_UUID  "aa386520826cc0cdaccf40096d5876de"
#define AUTH_UID_CHAR_UUID  "aa386521826cc0cdaccf40096d5876de"
#define AUTH_CERT_CHAR_UUID "aa386522826cc0cdaccf40096d5876de"
#define AUTH_CHAL_CHAR_UUID "aa386523826cc0cdaccf40096d5876de"
#define AUTH_RESP_CHAR_UUID "aa386524826cc0cdaccf40096d5876de"


#define OTAP_SERVICE_UUID "01ff5550ba5ef4ee5ca1eb1e5e4b1ce0"
#define OTAP_CTRL_CHAR_UUID "01ff5551ba5ef4ee5ca1eb1e5e4b1ce0"
#define OTAP_DATA_CHAR_UUID "01ff5552ba5ef4ee5ca1eb1e5e4b1ce0"

static ATMO_BLE_Handle_t authServiceHandle = 0;
static ATMO_BLE_Handle_t authUidCharHandle = 0;
static ATMO_BLE_Handle_t authCertCharHandle = 0;
static ATMO_BLE_Handle_t authChalCharHandle = 0;
static ATMO_BLE_Handle_t authRespCharHandle = 0;

ATMO_BLE_Handle_t otapServiceHandle = 0;
ATMO_BLE_Handle_t otapCtrlCharHandle = 0;
ATMO_BLE_Handle_t otapDataCharHandle = 0;

static bool ATMO_MK64F_BLE_AddGattService()
{
	ATMO_UUID_t gattUuid;
	ATMO_StringToUuid("1801", &gattUuid, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddService(&gattUuid, &gattHandle);

	ATMO_UUID_t serviceChanged;
	uint32_t serviceChangedVal = 0;
	ATMO_StringToUuid("2A05", &serviceChanged, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddCharacteristic( &serviceChanged, gattHandle, ATMO_BLE_Property_Read | ATMO_BLE_Property_Notify, ATMO_BLE_Permission_Read | ATMO_BLE_Permission_Write, 4, &serviceChangedHandle );
	ATMO_MK64F_BLE_PrivGATTSSetCharacteristic(serviceChangedHandle, sizeof(serviceChangedVal), (uint8_t *)&serviceChangedVal, NULL);

	return true;
}

static bool ATMO_MK64F_BLE_AddGapService()
{
	ATMO_UUID_t gapUuid;
	ATMO_BLE_Handle_t gapHandle;
	ATMO_StringToUuid("1800", &gapUuid, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddService(&gapUuid, &gapHandle);

	ATMO_UUID_t deviceName;
	ATMO_BLE_Handle_t deviceNameHandle;
	ATMO_StringToUuid("2A00", &deviceName, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddCharacteristic( &deviceName, gapHandle, ATMO_BLE_Property_Read, ATMO_BLE_Permission_Read, 64, &deviceNameHandle );
	ATMO_MK64F_BLE_PrivGATTSSetCharacteristic(deviceNameHandle, sizeof(DEVICE_NAME), (uint8_t *)DEVICE_NAME, NULL);

	ATMO_UUID_t deviceAppearance;
	ATMO_BLE_Handle_t deviceAppearanceHandle;
	uint16_t deviceAppearanceVal = 0; // Unknown
	ATMO_StringToUuid("2A01", &deviceAppearance, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddCharacteristic( &deviceAppearance, gapHandle, ATMO_BLE_Property_Read, ATMO_BLE_Permission_Read, 2, &deviceAppearanceHandle );
	ATMO_MK64F_BLE_PrivGATTSSetCharacteristic(deviceAppearanceHandle, sizeof(deviceAppearanceVal), (uint8_t *)&deviceAppearanceVal, NULL);

	ATMO_UUID_t conParams;
	ATMO_BLE_Handle_t conParamsHandle;
	uint8_t conParamsVal[] = {0x0A, 0x00, 0x10, 0x00, 0x64, 0x00, 0xE2, 0x04};
	ATMO_StringToUuid("2A04", &conParams, ATMO_ENDIAN_Type_Little);
	ATMO_MK64F_BLE_PrivAddCharacteristic( &conParams, gapHandle, ATMO_BLE_Property_Read, ATMO_BLE_Permission_Read, sizeof(conParamsVal), &conParamsHandle );
	ATMO_MK64F_BLE_PrivGATTSSetCharacteristic(conParamsHandle, sizeof(conParamsVal), conParamsVal, NULL);

	return true;

}

static void authChalWrittenCallback(void *data)
{
	shell_printf("Callback\r\n");
	ATMO_Value_t *atmoVal = (ATMO_Value_t *)data;
	if(atmoVal->size == 44)
	{
		uint8_t resValue[44];
		ATMO_PLATFORM_DebugPrint("Authenticating...\r\n");
		if( ATMO_MK64F_Auth_Authenticate(atmoVal->data, resValue) == 0 )
		{
			ATMO_MK64F_BLE_GATTSSetCharacteristic(NULL, authRespCharHandle, 44, resValue, NULL);
		}
	}
}

bool ATMO_MK64F_BLE_AddAuthService()
{
	ATMO_MK64F_BLE_GATTSAddService(NULL, &authServiceHandle, AUTH_SERVICE_UUID);

	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &authUidCharHandle, authServiceHandle, AUTH_UID_CHAR_UUID, ATMO_BLE_Property_Read, ATMO_BLE_Permission_Read, 16);
	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &authCertCharHandle, authServiceHandle, AUTH_CERT_CHAR_UUID, ATMO_BLE_Property_Read, ATMO_BLE_Permission_Read, 128);
	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &authChalCharHandle, authServiceHandle, AUTH_CHAL_CHAR_UUID, ATMO_BLE_Property_Write, ATMO_BLE_Permission_Write, 44);
	ATMO_MK64F_BLE_GATTSRegisterCharacteristicCallback(NULL, authChalCharHandle, ATMO_BLE_Characteristic_Written, authChalWrittenCallback);
	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &authRespCharHandle, authServiceHandle, AUTH_RESP_CHAR_UUID, ATMO_BLE_Property_Read | ATMO_BLE_Property_Notify, ATMO_BLE_Permission_Read, 44);

	return true;
}

bool ATMO_MK64F_BLE_AddOtapService()
{
	ATMO_MK64F_BLE_GATTSAddService(NULL, &otapServiceHandle, OTAP_SERVICE_UUID);
	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &otapCtrlCharHandle, otapServiceHandle, OTAP_CTRL_CHAR_UUID, ATMO_BLE_Property_Write | ATMO_BLE_Property_Indicate, ATMO_BLE_Permission_Write, 128);
	ATMO_MK64F_BLE_GATTSAddCharacteristic(NULL, &otapDataCharHandle, otapServiceHandle, OTAP_DATA_CHAR_UUID, ATMO_BLE_Property_WriteWithoutResponse, ATMO_BLE_Permission_Write,256);

	// For the OTAP service, the callbacks are handled in a lower layer right in the FSCI callback
	// This is because we need access to the device ID and other BLE specific info the ATMO layer doesn't pass
	return true;
}

bool ATMO_MK64F_BLE_SetServiceChanged(ATMO_BLE_Handle_t startHandle, ATMO_BLE_Handle_t endHandle)
{
	uint16_t handles[2] = {startHandle, endHandle};
	ATMO_MK64F_BLE_PrivGATTSSetCharacteristic(serviceChangedHandle, sizeof(handles), (uint8_t *)handles, NULL);
	return true;
}

bool ATMO_MK64F_BLE_AddDefaultServices()
{
	ATMO_MK64F_BLE_AddGapService();
	ATMO_MK64F_BLE_AddGattService();
}

bool ATMO_MK64F_BLE_WriteAuthUid(uint8_t *uid)
{
	ATMO_MK64F_BLE_GATTSSetCharacteristic(NULL,authUidCharHandle, 16, uid, NULL);
	return true;
}

bool ATMO_MK64F_BLE_WriteAuthCert(uint8_t *cert)
{
	ATMO_MK64F_BLE_GATTSSetCharacteristic(NULL,authCertCharHandle, 128, cert, NULL);
	return true;
}

bool ATMO_MK64F_BLE_IsAsciiPrintable(uint8_t c)
{
	return (c >= 33) && (c <= 126);
}

void ATMO_MK64F_BLE_PrintAdvData(GAPSetAdvertisingDataRequest_t *pAdvData)
{
	ATMO_PLATFORM_DebugPrint("\r\n\r\nADV DATA\r\n=======\r\n");
	for(int i = 0; i < pAdvData->AdvertisingData.NbOfAdStructures; i++)
	{
		ATMO_PLATFORM_DebugPrint("\r\nType: %02X\r\n", pAdvData->AdvertisingData.AdStructures[i].Type);
		ATMO_PLATFORM_DebugPrint("Length: %02X\r\n", pAdvData->AdvertisingData.AdStructures[i].Length);
		for(int j = 0; j < pAdvData->AdvertisingData.AdStructures[i].Length; j++)
		{
			uint8_t c = pAdvData->AdvertisingData.AdStructures[i].Data[j];
			if(ATMO_MK64F_BLE_IsAsciiPrintable(c))
			{
				ATMO_PLATFORM_DebugPrint("Data[%02d]: %02X (%c)\r\n", j, c, c);
			}
			else
			{
				ATMO_PLATFORM_DebugPrint("Data[%02d]: %02X\r\n", j, c);
			}
		}
	}
	ATMO_PLATFORM_DebugPrint("\r\n=======\r\n\r\n");
}



uint32_t ATMO_MK64F_BLE_AsciiToHex( uint8_t *pString, uint32_t strLen )
{
	uint32_t length = strLen;
	uint32_t retValue = 0, hexDig = 0;
	bool_t validChar;

	while ( length && pString )
	{
		hexDig = 0;
		validChar = FALSE;

		if ( *pString >= '0' && *pString <= '9' )
		{
			hexDig = *pString - '0';
			validChar = TRUE;
		}

		if ( *pString >= 'a' && *pString <= 'f' )
		{
			hexDig = *pString - 'a' + 10;
			validChar = TRUE;
		}

		if ( *pString >= 'A' && *pString <= 'F' )
		{
			hexDig = *pString - 'A' + 10;
			validChar = TRUE;
		}

		if ( validChar == TRUE )
		{
			retValue = ( retValue << 4 ) ^ hexDig;
		}

		pString++;
		length--;
	}

	return retValue;
}
