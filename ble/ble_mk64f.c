#include "ble_mk64f.h"
#include "ble.h"
#include "ble_db_mk64f.h"
#include "ble_mk64f_helpers.h"
#include "atmosphere_platform.h"
#include "nfc.h"

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "SerialManager.h"
#include "MemManager.h"
#include "fsl_os_abstraction.h"
#include "FsciInterface.h"
#include "FunctionLib.h"
#include "ble_sig_defines.h"
#include "cmd_ble.h"
#include "shell.h"
#include "Panic.h"
#include "TimersManager.h"
#include "otap_interface.h"
#include "ble_otap_client.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_rnga.h"

const ATMO_BLE_DriverInstance_t mk64fBleDriverInstance =
{
	ATMO_MK64F_BLE_PeripheralInit,
	ATMO_MK64F_BLE_PeripheralDeInit,
	ATMO_MK64F_BLE_SetEnabled,
	ATMO_MK64F_BLE_GetEnabled,
	ATMO_MK64F_BLE_GetMacAddress,
	ATMO_MK64F_BLE_GAPSetDeviceName,
	ATMO_MK64F_BLE_GAPAdvertisingStart,
	ATMO_MK64F_BLE_GAPAdvertisingStop,
	ATMO_MK64F_BLE_GAPSetAdvertisedServiceUUID,
	ATMO_MK64F_BLE_GAPAdverertisingSetManufacturerData,
	ATMO_MK64F_BLE_GAPScanConfigure,
	ATMO_MK64F_BLE_GAPScanStart,
	ATMO_MK64F_BLE_GAPScanStop,
	ATMO_MK64F_BLE_GAPScanSetData,
	ATMO_MK64F_BLE_GAPPairingCfg,
	ATMO_MK64F_BLE_GAPPair,
	ATMO_MK64F_BLE_GAPConnect,
	ATMO_MK64F_BLE_GAPDisconnect,
	ATMO_MK64F_BLE_GAPPowerLevelConfigure,
	ATMO_MK64F_BLE_GATTSAddService,
	ATMO_MK64F_BLE_GATTSAddCharacteristic,
	ATMO_MK64F_BLE_GATTSGetCharacteristicValue,
	ATMO_MK64F_BLE_GATTSRegisterCharacteristicCallback,
	ATMO_MK64F_BLE_GATTSRegisterCharacteristicAbilityHandle,
	ATMO_MK64F_BLE_GATTSSetCharacteristic,
	ATMO_MK64F_BLE_GATTSWriteDescriptor,
	ATMO_MK64F_BLE_GATTSSendIndicate,
	ATMO_MK64F_BLE_GATTSSendNotify,
	ATMO_MK64F_BLE_GATTCDiscoverService,
	ATMO_MK64F_BLE_GATTCWriteCharacteristic,
	ATMO_MK64F_BLE_GATTCReadCharacteristic,
	ATMO_MK64F_BLE_GATTCEnableNotification,
	ATMO_MK64F_BLE_RegisterEventCallback,
	ATMO_MK64F_BLE_RegisterEventAbilityHandle
};

#define ATMO_MAX_BLE_CHARACTERISTIC_SIZE (256) // Seems like this platform lets you set the characteristic however big you want
#define ATMO_MAX_CHARACTERISTICS (128)

static ATMO_BLE_MacAddress_t _ATMO_MK64F_MacAddress;

static SemaphoreHandle_t findServiceHandleMutex = NULL;
static SemaphoreHandle_t findCharHandleMutex = NULL;
static SemaphoreHandle_t charReadMutex = NULL;
static SemaphoreHandle_t gattDbConfirmMutex = NULL;
static SemaphoreHandle_t cccdMutex = NULL;
static SemaphoreHandle_t deviceAddressMutex = NULL;
static SemaphoreHandle_t bleInitMutex = NULL;

typedef struct
{
	ATMO_BLE_Handle_t handle;
	ATMO_BLE_Handle_t cccdHandle;
	bool initialized;
	bool notificationsEnabled;
	bool indicationsEnabled;
	ATMO_Callback_t callback;
	uint8_t properties;
	unsigned int abilityHandle;
	bool abilityHandlerRegistered;
} ATMO_CharacteristicInfo_t;

static ATMO_CharacteristicInfo_t characteristics[ATMO_MAX_CHARACTERISTICS] = {0};

static uint32_t currentNumCharacteristics = 0;

static uint16_t latestServiceHandle = 0;
static uint16_t latestCharHandle = 0;
static uint16_t latestCccdHandle = 0;

static uint8_t latestCharDataRead[ATMO_MAX_BLE_CHARACTERISTIC_SIZE];
static uint8_t latestCharDataReadSize = 0;

static bool bleInitialized = false;

#define ATMO_BLE_NUM_CALLBACKS_PER_EVENT (50)
static unsigned int _mk64BleEventAbilityHandle[ATMO_BLE_EVENT_NumEvents][ATMO_BLE_NUM_CALLBACKS_PER_EVENT];
static unsigned int _mk64BleEventNumAbilityHandles[ATMO_BLE_EVENT_NumEvents] = {0};

static ATMO_Callback_t _mk64BleEventCallback[ATMO_BLE_EVENT_NumEvents][ATMO_BLE_NUM_CALLBACKS_PER_EVENT];
static unsigned int _mk64BleEventNumCallbacks[ATMO_BLE_EVENT_NumEvents] = {0};

static bool mk64fBleEnabled = true;
static bool mk64BleEnabledOnBoot = true;

static GAPSetAdvertisingDataRequest_t advData;
#define gGapAdvertisingInterval_050ms_c 0x0050
#define gGapAdvertisingInterval_100ms_c 0x00A0
#define gGapAdvertisingInterval_125ms_c 0x00C8
#define gGapAdvertisingInterval_250ms_c 0x0190
#define gGapAdvertisingInterval_500ms_c 0x0320

static uint8_t connectedDeviceId = 0;
static bool currentlyConnected = false;
static bool pairingInProgress = false;
static tmrTimerID_t resetPairingTmr = gTmrInvalidTimerID_c;
static bool blePostInitComplete = false;

static ATMO_BLE_PairingCfg_t _ATMO_MK64F_BLE_PairingConfig = 
{
	.type = ATMO_BLE_PairingType_RandomKey,
	.pairingKey = 0
};

static ATMO_NFC_Record_t atmoProvisioningRecord;

// Needed for OTA
extern ATMO_BLE_Handle_t otapServiceHandle;
extern ATMO_BLE_Handle_t otapCtrlCharHandle;
extern ATMO_BLE_Handle_t otapDataCharHandle;
uint16_t hServiceOTA;
uint16_t hValueControlPointOTA;
uint16_t hValueOTACCCD;
uint16_t hValueDataOTA;

// Advertising data buffers
static uint8_t _ATMO_BLE_AdvServiceUuidBuf[16];
static uint8_t _ATMO_BLE_AdvFlags = 6;
static uint8_t _ATMO_BLE_DeviceNameBuf[20] = {0};
#define _ATMO_BLE_AD_DEVNAME_INDEX (0)
#define _ATMO_BLE_AD_UUID_INDEX (1)
#define _ATMO_BLE_AD_FLAGS_INDEX (2)
#define _ATMO_BLE_AD_NUM_STRUCTURES (3)



static ATMO_BLE_MacAddress_t deviceAddress;

GAPSetAdvertisingParametersRequest_t advParams =
{
	/* minInterval */         gGapAdvertisingInterval_050ms_c /* 1 s */, \
	/* maxInterval */         gGapAdvertisingInterval_100ms_c /* 1 s */, \
	/* advertisingType */     GAPSetAdvertisingParametersRequest_AdvertisingType_gConnectableUndirected_c, \
	/* addressType */         GAPSetAdvertisingParametersRequest_OwnAddressType_gPublic_c, \
	/* directedAddressType */ GAPSetAdvertisingParametersRequest_PeerAddressType_gPublic_c, \
	/* directedAddress */     {0, 0, 0, 0, 0, 0}, \
	/* channelMap */          GAPSetAdvertisingParametersRequest_ChannelMap_gChannel37_c | GAPSetAdvertisingParametersRequest_ChannelMap_gChannel38_c | GAPSetAdvertisingParametersRequest_ChannelMap_gChannel39_c, \
	/* filterPolicy */        GAPSetAdvertisingParametersRequest_FilterPolicy_gProcessAll_c \
};


#define mcEncryptionKeySize_c   16

GAPSetDefaultPairingParametersRequest_t _ATMO_MK64F_BLE_DefaultPairingParameters =
{
	.PairingParametersIncluded = TRUE,
	.PairingParameters = {
		.WithBonding = TRUE,
		.SecurityModeAndLevel = GAPPairRequest_PairingParameters_SecurityModeAndLevel_gMode1Level3_c,
		.MaxEncryptionKeySize = mcEncryptionKeySize_c,
		.LocalIoCapabilities = GAPPairRequest_PairingParameters_LocalIoCapabilities_gIoDisplayOnly_c,
		.OobAvailable = FALSE,
		.CentralKeys = GAPPairRequest_PairingParameters_CentralKeys_gLtk_c,
		.PeripheralKeys = GAPPairRequest_PairingParameters_PeripheralKeys_gLtk_c,
		.LeSecureConnectionSupported = TRUE,
		.UseKeypressNotifications = FALSE
	}
};


static uint32_t getRandomInteger()
{
	uint32_t randomInt = 0;
	RNGA_GetRandomData(RNG, &randomInt, 4);
	return randomInt;
}

/**
 * Register for "characteristic written" events from the kw41z
 */
static bool ATMO_MK64F_BLE_RegisterWrittenCallbacks();

static uint32_t genRandomKey( uint8_t numDigits )
{
	uint32_t num = 0;

	for( uint8_t i = 0; i < numDigits; i++ )
	{
		num *= 10;
		num += getRandomInteger() % 10;
	}

	return num;
}

/**
 * Given a handle, grab the characteristic info
 */
static ATMO_CharacteristicInfo_t* MK64F_GetCharacteristicByHandle( ATMO_BLE_Handle_t handle )
{
	for ( uint32_t i = 0; i < currentNumCharacteristics; i++ )
	{
		if ( characteristics[i].handle == handle )
		{
			return &characteristics[i];
		}
	}

	return NULL;
}

static ATMO_CharacteristicInfo_t* MK64F_GetCharacteristicByCccdHandle( ATMO_BLE_Handle_t handle )
{
	for ( uint32_t i = 0; i < currentNumCharacteristics; i++ )
	{
		if ( characteristics[i].cccdHandle == handle )
		{
			return &characteristics[i];
		}
	}

	return NULL;
}

static void _ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_Event_t event, ATMO_Value_t *value)
{

	for(unsigned int i = 0; i < _mk64BleEventNumCallbacks[event]; i++)
	{
		ATMO_Lock();
		ATMO_AddCallbackExecute( _mk64BleEventCallback[event][i], value );
		ATMO_Unlock();
	}

	for(unsigned int i = 0; i < _mk64BleEventNumAbilityHandles[event]; i++)
	{
		ATMO_Lock();
		ATMO_AddAbilityExecute( _mk64BleEventAbilityHandle[event][i], value );
		ATMO_Unlock();
	}	
}

// #define ATMO_BLE_DEBUG

/**
 * Handler for any events coming from the KW41Z
 */
void BLE_FSCI_RxCb( void* pData, void* param, uint32_t interfaceId )
{
	bleEvtContainer_t container; // this could be allocated instead
	KHC_BLE_RX_MsgHandler( pData, &container, interfaceId );

#ifdef ATMO_BLE_DEBUG
	if ( 1)
	{
		ATMO_PLATFORM_DebugPrint("RX: %02X\r\n", container.id);
	}
#endif

	switch ( container.id )
	{
		case 0x4580:
		{
			xSemaphoreGive( gattDbConfirmMutex );
			break;
		}

		case 0x4582:
		{
			ATMO_PLATFORM_DebugPrint("RX Service Handle: %02X\r\n", container.Data.GATTDBFindServiceHandleIndication.ServiceHandle);
			latestServiceHandle = container.Data.GATTDBFindServiceHandleIndication.ServiceHandle;
			xSemaphoreGive( findServiceHandleMutex );
			break;
		}

		case 0x4583:
		{
			ATMO_PLATFORM_DebugPrint("RX Char Handle: %02X\r\n", container.Data.GATTDBFindCharValueHandleInServiceIndication.CharValueHandle);
			latestCharHandle = container.Data.GATTDBFindCharValueHandleInServiceIndication.CharValueHandle;
			xSemaphoreGive( findCharHandleMutex );
			break;
		}

		case 0x4584:
		{
			ATMO_PLATFORM_DebugPrint( "\r\nGATTDBFindCccdHandleForCharValueHandleIndication" );
			latestCccdHandle = container.Data.GATTDBFindCccdHandleForCharValueHandleIndication.CccdHandle;
			xSemaphoreGive(cccdMutex);
			break;
		}

		case 0x4493:
		{
			GATTServerAttributeWrittenIndication_t* event = &container.Data.GATTServerAttributeWrittenIndication;

			GATTDBWriteAttributeRequest_t req;

			req.Handle = event->AttributeWrittenEvent.Handle;
			req.ValueLength = event->AttributeWrittenEvent.ValueLength;
			req.Value = event->AttributeWrittenEvent.Value;

			GATTDBWriteAttributeRequest( &req, BLE_FSCI_IF );

			GATTServerSendAttributeWrittenStatusRequest_t statusReq;

			statusReq.DeviceId = event->DeviceId;
			statusReq.AttributeHandle = event->AttributeWrittenEvent.Handle;
			statusReq.Status = 0;

			if (event->AttributeWrittenEvent.Handle !=  hValueControlPointOTA)
			{
				GATTServerSendAttributeWrittenStatusRequest( &statusReq, BLE_FSCI_IF );
			}

			ATMO_BLE_Handle_t handle;
			ATMO_BLE_DB_MK64F_Kw41zHandleToAtmo( &event->AttributeWrittenEvent.Handle, &handle );

			ATMO_CharacteristicInfo_t* characteristic = MK64F_GetCharacteristicByHandle( handle );

			if ( characteristic->abilityHandlerRegistered )
			{
				ATMO_Value_t data;
				ATMO_InitValue( &data );
				ATMO_CreateValueBinary( &data, event->AttributeWrittenEvent.Value, event->AttributeWrittenEvent.ValueLength );
				ATMO_Lock();
				ATMO_AddAbilityExecute( characteristic->abilityHandle, &data );
				ATMO_Unlock();
				ATMO_Free(&data);
			}

			if ( characteristic->callback != NULL )
			{
				ATMO_Value_t data;
				ATMO_InitValue( &data );
				ATMO_CreateValueBinary( &data, event->AttributeWrittenEvent.Value, event->AttributeWrittenEvent.ValueLength );
				ATMO_Lock();
				ATMO_AddCallbackExecute( characteristic->callback, &data );
				ATMO_Unlock();
				ATMO_Free(&data);
			}

		    else if (event->AttributeWrittenEvent.Handle ==  hValueControlPointOTA)
		    {
		        BleApp_AttributeWritten(event->DeviceId,
		        		event->AttributeWrittenEvent.Handle,
						event->AttributeWrittenEvent.ValueLength,
						event->AttributeWrittenEvent.Value);
		    }

			MEM_BufferFree( event->AttributeWrittenEvent.Value );


			break;
		}

        case 0x4491:
            BleApp_AttMtuChanged(container.Data.GATTServerMtuChangedIndication.DeviceId,
                                 container.Data.GATTServerMtuChangedIndication.MtuChangedEvent_NewMtu);
            break;

        case 0x4492:
            BleApp_HandleValueConfirmation(container.Data.GATTServerHandleValueConfirmationIndication.DeviceId);
            break;

        case 0x4494:
            ATMO_PLATFORM_DebugPrint("\r\nGATTServerCharacteristicCccdWrittenIndication");
            BleApp_CccdWritten(container.Data.GATTServerCharacteristicCccdWrittenIndication.DeviceId,
                               container.Data.GATTServerCharacteristicCccdWrittenIndication.CharCccdWrittenEvent.Handle);

            ATMO_PLATFORM_DebugPrint("Getting char for CCCD handle %02X\r\n", container.Data.GATTServerCharacteristicCccdWrittenIndication.CharCccdWrittenEvent.Handle);

			ATMO_CharacteristicInfo_t* characteristic = MK64F_GetCharacteristicByCccdHandle( container.Data.GATTServerCharacteristicCccdWrittenIndication.CharCccdWrittenEvent.Handle );

			if(characteristic != NULL)
			{
				characteristic->notificationsEnabled = (container.Data.GATTServerCharacteristicCccdWrittenIndication.CharCccdWrittenEvent.NewCccd & GATTServerCharacteristicCccdWrittenIndication_CharCccdWrittenEvent_NewCccd_gCccdNotification_c);
				characteristic->indicationsEnabled = (container.Data.GATTServerCharacteristicCccdWrittenIndication.CharCccdWrittenEvent.NewCccd & GATTServerCharacteristicCccdWrittenIndication_CharCccdWrittenEvent_NewCccd_gCccdIndication_c);
				ATMO_PLATFORM_DebugPrint("Handle: %02X Notifications: %d Indications: %d\r\n", characteristic->handle, characteristic->notificationsEnabled, characteristic->indicationsEnabled);
			}

            break;

        case 0x4495:
            BleApp_AttributeWrittenWithoutResponse(container.Data.GATTServerAttributeWrittenWithoutResponseIndication.DeviceId,
                            container.Data.GATTServerAttributeWrittenWithoutResponseIndication.AttributeWrittenEvent.Handle,
                            container.Data.GATTServerAttributeWrittenWithoutResponseIndication.AttributeWrittenEvent.ValueLength,
                            container.Data.GATTServerAttributeWrittenWithoutResponseIndication.AttributeWrittenEvent.Value);
            MEM_BufferFree(container.Data.GATTServerAttributeWrittenWithoutResponseIndication.AttributeWrittenEvent.Value);
            break;


        case 0x4794:
        {
        	for(int i = 0; i < 6; i++)
        	{
        		deviceAddress.data[i] = container.Data.GAPGenericEventPublicAddressReadIndication.Address[5 - i];
        	}

            xSemaphoreGive(deviceAddressMutex);
            break;
        }

		case 0x47A4:
		{
			pairingInProgress = true;
			char msg[64];
			sprintf( msg, "BLE Passkey: %d\r\n", container.Data.GAPConnectionEventPasskeyDisplayIndication.PasskeyForDisplay );
			
			ATMO_Value_t atmoKey;
			ATMO_InitValue(&atmoKey);
			ATMO_CreateValueUnsignedInt(&atmoKey, container.Data.GAPConnectionEventPasskeyDisplayIndication.PasskeyForDisplay);
			_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_PairingRequested, &atmoKey);
			ATMO_FreeValue(&atmoKey);
			
			break;
		}

		case 0x47A9:
		{
			pairingInProgress = false;

			if(container.Data.GAPConnectionEventPairingCompleteIndication.PairingStatus == GAPConnectionEventPairingCompleteIndication_PairingStatus_PairingSuccessful)
			{
				_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_PairingSuccess, NULL);
			}
			else
			{
				_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_PairingFailed, NULL);
			}
		}
		case 0x47A1:
		{
			if(pairingInProgress)
			{
				_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_PairingFailed, NULL);
			}
			pairingInProgress = false;
			break;

		}

		case 0x4780:
		{
			xSemaphoreGive(bleInitMutex);
			break;
		}

		case 0x4581:
		{
			memcpy( latestCharDataRead, container.Data.GATTDBReadAttributeIndication.Value,
			        container.Data.GATTDBReadAttributeIndication.ValueLength );
			latestCharDataReadSize = container.Data.GATTDBReadAttributeIndication.ValueLength;
			xSemaphoreGive( charReadMutex );
			break;
		}

		case 0x479D:
		{
			ATMO_PLATFORM_DebugPrint( "GAPConnectionEventConnectedIndication\r\n" );

			uint32_t key = 0;

			if(_ATMO_MK64F_BLE_PairingConfig.type == ATMO_BLE_PairingType_RandomKey)
			{
				key = genRandomKey( 6 );
			}
			else if(_ATMO_MK64F_BLE_PairingConfig.type == ATMO_BLE_PairingType_UserKey)
			{
				key = _ATMO_MK64F_BLE_PairingConfig.pairingKey;
			}

			GAPSetLocalPasskeyRequest_t req_pass = {key};
			GAPSetLocalPasskeyRequest( &req_pass, BLE_FSCI_IF );

			connectedDeviceId = container.Data.GAPConnectionEventConnectedIndication.DeviceId;
			currentlyConnected = true;
			OtapCS_Subscribe(connectedDeviceId);
			BleApp_CccdWritten_lazy();

			_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_Connected, NULL);

			break;
		}

		case 0x479E:
		{
			ATMO_PLATFORM_DebugPrint( "GAPConnectionEventPairingRequestIndication\r\n" );
			break;
		}

		case 0x47AA:
		{
			ATMO_PLATFORM_DebugPrint( "Disconnected\r\n" );
			OtapClient_HandleDisconnectionEvent (container.Data.GAPConnectionEventDisconnectedIndication.DeviceId);
			currentlyConnected = false;

			_ATMO_MK64F_BLE_DispatchEvent(ATMO_BLE_EVENT_Disconnected, NULL);

			break;
		}

		case 0x4496:
		{
			ATMO_PLATFORM_DebugPrint( "\r\nGATTServerErrorIndication " );
			break;
		}
	}
}

static void ATMO_MK64F_BLE_RegisterForWriteNotifications( uint8_t count, uint8_t* handles )
{
	GATTServerRegisterHandlesForWriteNotificationsRequest_t req;

	if ( count == 0 )
	{
		return;
	}

	req.HandleCount = count;
	req.AttributeHandles = handles;

	GATTServerRegisterHandlesForWriteNotificationsRequest( &req, BLE_FSCI_IF );
}

/**
 * Given a UUID, find the handle associated with that service
 *
 * BLOCKING
 *
 * @param[in] uuid
 * @param[out] handle
 *
 * @return true on success, false on error
 */
static bool MK64F_FindServiceHandle( ATMO_UUID_t* uuid, ATMO_BLE_Handle_t* handle )
{
	GATTDBFindServiceHandleRequest_t req;
	req.StartHandle = 0x0001;  // should be 0x0001 on the first call

	if( uuid->type == ATMO_UUID_Type_16_Bit )
	{
		memcpy( req.Uuid.Uuid16Bits, uuid->data, 2 );
		req.UuidType = Uuid16Bits;
	}
	else
	{
		memcpy( req.Uuid.Uuid128Bits, uuid->data, 16 );
		req.UuidType = Uuid128Bits;
	}

	GATTDBFindServiceHandleRequest( &req, BLE_FSCI_IF );

	// Wait a second for the result to come back in
	if ( xSemaphoreTake( findServiceHandleMutex,
	                     1000 / portTICK_PERIOD_MS ) == pdTRUE )
	{
		*handle = latestServiceHandle;
	}
	else
	{
		return false;
	}


	return true;
}

static bool MK64F_FindCharCccdHandle( ATMO_BLE_Handle_t handle, ATMO_BLE_Handle_t* cccdHandle )
{
	GATTDBFindCccdHandleForCharValueHandleRequest_t findcccdReq;
	findcccdReq.CharValueHandle = handle;
	GATTDBFindCccdHandleForCharValueHandleRequest( &findcccdReq, BLE_FSCI_IF );

	if( xSemaphoreTake( cccdMutex, 1000 / portTICK_PERIOD_MS ) != pdTRUE )
	{
		shell_printf( "Didn't get response in time\r\n" );
		return false;
	}

	*cccdHandle = latestCccdHandle;
	return true;
}

bool ATMO_MK64F_BLE_GetCccdHandles()
{
	for( uint32_t i = 0; i < currentNumCharacteristics; i++ )
	{
		if( characteristics[i].properties & ( ATMO_BLE_Property_Notify | ATMO_BLE_Property_Indicate ) )
		{
			ATMO_BLE_Handle_t kw41zHandle;
			ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &characteristics[i].handle, &kw41zHandle );

			if( !MK64F_FindCharCccdHandle( kw41zHandle, &characteristics[i].cccdHandle ) )
			{
				return false;
			}

			shell_printf("CCCD[%d] Atmo Handle %02X Kw41z Handle %02X CCCD Handle: %02X\r\n", i, characteristics[i].handle, kw41zHandle, characteristics[i].cccdHandle);
		}
	}

	return true;
}
/**
 * Given a UUID, find the handle associated with that characteristic
 *
 * BLOCKING
 *
 * @param[in] uuid
 * @param[in] serviceHandle - Handle of parent service
 * @param[out] handle
 *
 * @return true on success, false on error
 */
static bool MK64F_FindCharHandle( ATMO_UUID_t* uuid, ATMO_BLE_Handle_t serviceHandle, ATMO_BLE_Handle_t* handle )
{
	GATTDBFindCharValueHandleInServiceRequest_t req;
	req.ServiceHandle = serviceHandle;

	if( uuid->type == ATMO_UUID_Type_16_Bit )
	{
		memcpy( req.Uuid.Uuid16Bits, uuid->data, 2 );
		req.UuidType = Uuid16Bits;
	}
	else
	{
		memcpy( req.Uuid.Uuid128Bits, uuid->data, 16 );
		req.UuidType = Uuid128Bits;
	}

	GATTDBFindCharValueHandleInServiceRequest( &req, BLE_FSCI_IF );

	// Wait a second for the result to come back in
	if ( xSemaphoreTake( findCharHandleMutex,
	                     1000 / portTICK_PERIOD_MS ) == pdTRUE )
	{
		*handle = latestCharHandle;
	}
	else
	{
		return false;
	}


	return true;
}

bool ATMO_MK64F_BLE_SetAllServiceChanged()
{
	ATMO_MK64F_BLE_SetServiceChanged( 0, latestCharHandle );
	return true;
}

ATMO_Status_t ATMO_MK64F_BLE_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber )
{
	static ATMO_DriverInstanceData_t driverData;
	driverData.name = "MK64F BLE";
	driverData.initialized = false;
	driverData.instanceNumber = 0;
	driverData.argument = NULL;

	return ATMO_BLE_AddDriverInstance( &mk64fBleDriverInstance, &driverData, instanceNumber );
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_PeripheralInit( ATMO_DriverInstanceData_t* instance )
{
	if ( bleInitialized )
	{
		return ATMO_BLE_Status_Success;
	}

	// Initialize mutexes to create sync API
	findServiceHandleMutex = xSemaphoreCreateBinary();
	findCharHandleMutex = xSemaphoreCreateBinary();
	charReadMutex = xSemaphoreCreateBinary();
	gattDbConfirmMutex = xSemaphoreCreateBinary();
	cccdMutex = xSemaphoreCreateBinary();
	deviceAddressMutex = xSemaphoreCreateBinary();
	bleInitMutex = xSemaphoreCreateBinary();

	// Init advdata
	static uint8_t adStructData[sizeof(advData.AdvertisingData.AdStructures[0]) * (_ATMO_BLE_AD_NUM_STRUCTURES)];
	advData.AdvertisingData.AdStructures = (void *)adStructData;
	advData.AdvertisingDataIncluded = true;
	advData.ScanResponseDataIncluded = false;
	advData.AdvertisingData.NbOfAdStructures = _ATMO_BLE_AD_NUM_STRUCTURES;

	ATMO_PLATFORM_DebugPrint( "Initializing BLE\r\n" );

	unsigned int maxMacAddressRetries = 5;

	for(int i = 0; i < maxMacAddressRetries; i++)
	{
		// Try to get the MAC address
		GAPReadPublicDeviceAddressRequest(BLE_FSCI_IF);

		// Wait a second for the result to come back in
		if ( xSemaphoreTake( deviceAddressMutex,
		                     1000 / portTICK_PERIOD_MS ) == pdTRUE )
		{
			bleInitialized = true;
			ATMO_PLATFORM_DebugPrint("Got MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", deviceAddress.data[0],
					deviceAddress.data[1],
					deviceAddress.data[2],
					deviceAddress.data[3],
					deviceAddress.data[4],
					deviceAddress.data[5]);
			ATMO_PLATFORM_DebugPrint("BLE Initialized\r\n");
			memcpy(&_ATMO_MK64F_MacAddress, &deviceAddress, sizeof(deviceAddress));
			break;
		}
		else
		{
			ATMO_PLATFORM_DebugPrint("Failed To Get MAC (Attempt %d/%d)\r\n", i + 1, maxMacAddressRetries);
		}
	}

	GATTServerRegisterCallbackRequest( BLE_FSCI_IF );

	// Initialize virtual DB
	ATMO_BLE_DB_MK64F_Init();
	ATMO_BLE_DB_MK64F_RegisterFuncs( ATMO_MK64F_BLE_PrivAddService, ATMO_MK64F_BLE_PrivAddCharacteristic,
	                                 ATMO_MK64F_BLE_RegisterWrittenCallbacks );

	ATMO_MK64F_BLE_AddAuthService();
	ATMO_MK64F_BLE_AddOtapService();
	ATMO_MK64F_BLE_AddDefaultServices();

	RNGA_Init(RNG);

	
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_SetEnabledOnBoot( ATMO_DriverInstanceData_t *instance, bool enabled )
{
	mk64BleEnabledOnBoot = enabled;
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_SetEnabled( ATMO_DriverInstanceData_t *instance, bool enabled )
{
	// Write to the flash
	ATMO_FILESYSTEM_File_t file;
	if( ATMO_FILESYSTEM_FileOpen(0, &file, "ble_enabled", ATMO_CREAT | ATMO_RDWR | ATMO_TRUNC ) != ATMO_FILESYSTEM_Status_Success )
	{
		return ATMO_BLE_Status_Fail;
	}

	if( ATMO_FILESYSTEM_FileWrite(0, &file, &enabled, sizeof(enabled)) != ATMO_FILESYSTEM_Status_Success )
	{
		ATMO_FILESYSTEM_FileClose(0, &file);
		return ATMO_BLE_Status_Fail;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);

	mk64fBleEnabled = enabled;

	if(!enabled)
	{
		// Disconnect and stop advertising
		if(currentlyConnected)
		{
			GAPDisconnectRequest_t req;
			req.DeviceId = connectedDeviceId;  // the connected peer to disconnect from
			GAPDisconnectRequest(&req, BLE_FSCI_IF);
		}
		
		ATMO_MK64F_BLE_GAPAdvertisingStop(NULL);
		
	}
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GetEnabled( ATMO_DriverInstanceData_t *instance, bool *enabled )
{
	*enabled = true;

	ATMO_FILESYSTEM_File_t file;

	ATMO_FILESYSTEM_Status_t status = ATMO_FILESYSTEM_FileOpen(0, &file, "ble_enabled", ATMO_RDONLY );

	if(status != ATMO_FILESYSTEM_Status_Success)
	{
		return ATMO_BLE_Status_Fail;
	}

	if( ATMO_FILESYSTEM_FileRead(0, &file, enabled, sizeof(enabled)) != ATMO_FILESYSTEM_Status_Success )
	{
		ATMO_PLATFORM_DebugPrint("Read From ble_enabled: %d\r\n", *enabled);
		ATMO_FILESYSTEM_FileClose(0, &file);
		return ATMO_BLE_Status_Fail;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);
	return ATMO_BLE_Status_Success;
}

void ATMO_MK64F_BLE_PostInit()
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return;
	}

	// Register Atmosphere NFC Provisioning Record
	unsigned int recordHandle = 0;
	ATMO_NFC_RegisterStoredRecord(0, &atmoProvisioningRecord, &recordHandle);
	ATMO_NFC_MakeRecordMime(&atmoProvisioningRecord, "application/atmoiotprov", "N/A", strlen("N/A"));

	blePostInitComplete = true;

	ATMO_BLE_DB_MK64F_Flush();
	ATMO_MK64F_BLE_GetCccdHandles();
	ATMO_BLE_AdvertisingParams_t params;
	params.advertisingInterval = 1600;
	params.type = ATMO_BLE_ADV_TYPE_INDIRECT;
	ATMO_BLE_GAPAdvertisingStart(0, &params);
	ATMO_MK64F_BLE_SetAllServiceChanged();

	// OTAP module needs to know the various handles
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z(&otapServiceHandle, &hServiceOTA);
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z(&otapCtrlCharHandle, &hValueControlPointOTA);
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z(&otapDataCharHandle, &hValueDataOTA);

	ATMO_CharacteristicInfo_t *otaCtrlChar = MK64F_GetCharacteristicByHandle(otapCtrlCharHandle);
	if(otaCtrlChar)
	{
		hValueOTACCCD = otaCtrlChar->cccdHandle;
	}
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_PeripheralDeInit( ATMO_DriverInstanceData_t* instance )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GetMacAddress( ATMO_DriverInstanceData_t *instance, ATMO_BLE_MacAddress_t *address )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	memcpy(address, &_ATMO_MK64F_MacAddress, sizeof(_ATMO_MK64F_MacAddress));
	
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPSetDeviceName( ATMO_DriverInstanceData_t* instance, const char* name )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}
	
	if(strlen(name) >= sizeof(_ATMO_BLE_DeviceNameBuf))
	{
		return ATMO_BLE_Status_Fail;
	}

	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_DEVNAME_INDEX].Length = strlen(name);
	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_DEVNAME_INDEX].Type = GAPSetAdvertisingDataRequest_AdvertisingData_AdStructures_Type_gAdShortenedLocalName_c;
	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_DEVNAME_INDEX].Data = _ATMO_BLE_DeviceNameBuf;
	memcpy(_ATMO_BLE_DeviceNameBuf, name, strlen(name));

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPAdvertisingStart( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_AdvertisingParams_t* params )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	GAPSetDefaultPairingParametersRequest( &_ATMO_MK64F_BLE_DefaultPairingParameters, BLE_FSCI_IF );

	// TODO nick set adversiting type
	advParams.MinInterval = params->advertisingInterval;
	advParams.MaxInterval = params->advertisingInterval;


	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_FLAGS_INDEX].Length = 1;
	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_FLAGS_INDEX].Type = GAPSetAdvertisingDataRequest_AdvertisingData_AdStructures_Type_gAdFlags_c;
	advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_FLAGS_INDEX].Data = &_ATMO_BLE_AdvFlags;

#ifdef ATMO_BLE_ADV_DEBUG
		ATMO_MK64F_BLE_PrintAdvData(&advData);
#endif

	GAPSetAdvertisingDataRequest( &advData, BLE_FSCI_IF );

	GAPSetAdvertisingParametersRequest( &advParams, BLE_FSCI_IF );

	if(blePostInitComplete)
	{
		GAPStartAdvertisingRequest( BLE_FSCI_IF );
	}

	char advUuidStr[37] = {0};
	ATMO_UUID_t advServiceUuid;
	advServiceUuid.endianness = ATMO_ENDIAN_Type_Little;
	advServiceUuid.type = ATMO_UUID_Type_128_Bit;
	memcpy(advServiceUuid.data, _ATMO_BLE_AdvServiceUuidBuf, 16);

	if(ATMO_UuidToString(&advServiceUuid, advUuidStr, 37) != ATMO_Status_Success)
	{
		return ATMO_BLE_Status_Fail;
	}

	char advRecord[128];
	sprintf(advRecord, "{\"name\":\"%s\",\"uuid\":\"%s\", \"type\":\"ble\"}", _ATMO_BLE_DeviceNameBuf, advUuidStr);
	ATMO_NFC_MakeRecordMime(&atmoProvisioningRecord, "application/atmoiotprov", advRecord, strlen(advRecord));
	ATMO_NFC_SyncAtmoRecords(0);

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPAdvertisingStop( ATMO_DriverInstanceData_t* instance )
{
	GAPStopAdvertisingRequest(BLE_FSCI_IF);
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPSetAdvertisedServiceUUID( ATMO_DriverInstanceData_t* instance, const char* uuid )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	ATMO_PLATFORM_DebugPrint("Setting advertised UUID: %s\r\n", uuid);

	ATMO_UUID_t uuidBin;

	// Convert string UUID to binary form
	if ( ATMO_StringToUuid( uuid, &uuidBin, ATMO_ENDIAN_Type_Little ) == ATMO_Status_Success )
	{
		advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_UUID_INDEX].Length = 16;
		advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_UUID_INDEX].Type = GAPSetAdvertisingDataRequest_AdvertisingData_AdStructures_Type_gAdIncomplete128bitServiceList_c;
		advData.AdvertisingData.AdStructures[_ATMO_BLE_AD_UUID_INDEX].Data = _ATMO_BLE_AdvServiceUuidBuf;
		memcpy(_ATMO_BLE_AdvServiceUuidBuf, uuidBin.data, 16);

#ifdef ATMO_BLE_ADV_DEBUG
		ATMO_MK64F_BLE_PrintAdvData(&advData);
#endif
	}
	else
	{
		return ATMO_BLE_Status_Fail;
	}

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPAdverertisingSetManufacturerData( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_AdvertisingData_t* data )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	return ATMO_BLE_Status_NotSupported;

}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPScanConfigure( ATMO_DriverInstanceData_t* instance, ATMO_BLE_ScanParams_t* params )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPScanStart( ATMO_DriverInstanceData_t* instance, uint32_t length )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPScanStop( ATMO_DriverInstanceData_t* instance )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPScanSetData( ATMO_DriverInstanceData_t* instance, ATMO_BLE_ScanData_t* data )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPPairingCfg( ATMO_DriverInstanceData_t* instance, ATMO_BLE_PairingCfg_t* config )
{
	GAPSetDefaultPairingParametersRequest_t gPairingParameters =
	{
		.PairingParametersIncluded = TRUE,
		.PairingParameters = {
			.WithBonding = TRUE,
			.SecurityModeAndLevel = GAPPairRequest_PairingParameters_SecurityModeAndLevel_gMode1Level3_c,
			.MaxEncryptionKeySize = mcEncryptionKeySize_c,
			.LocalIoCapabilities = GAPPairRequest_PairingParameters_LocalIoCapabilities_gIoDisplayOnly_c,
			.OobAvailable = FALSE,
			.CentralKeys = GAPPairRequest_PairingParameters_CentralKeys_gLtk_c,
			.PeripheralKeys = GAPPairRequest_PairingParameters_PeripheralKeys_gLtk_c,
			.LeSecureConnectionSupported = TRUE,
			.UseKeypressNotifications = FALSE
		}
	};

	if(config->type == ATMO_BLE_PairingType_JustWorks)
	{
		gPairingParameters.PairingParameters.LocalIoCapabilities = GAPPairRequest_PairingParameters_LocalIoCapabilities_gIoNone_c;
	}
	else if(config->type == ATMO_BLE_PairingType_RandomKey || config->type == ATMO_BLE_PairingType_UserKey)
	{
		gPairingParameters.PairingParameters.LocalIoCapabilities = GAPPairRequest_PairingParameters_LocalIoCapabilities_gIoDisplayOnly_c;
	}
	else
	{
		return ATMO_BLE_Status_Invalid;
	}

	memcpy(&_ATMO_MK64F_BLE_PairingConfig, config, sizeof(ATMO_BLE_PairingCfg_t));

	GAPSetDefaultPairingParametersRequest( &gPairingParameters, BLE_FSCI_IF );
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPPair( ATMO_DriverInstanceData_t* instance )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPConnect( ATMO_DriverInstanceData_t* instance, ATMO_BLE_DeviceID_t scannedDeviceID )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPDisconnect( ATMO_DriverInstanceData_t* instance )
{
	if(currentlyConnected)
	{
		GAPDisconnectRequest_t req;
		req.DeviceId = connectedDeviceId;  // the connected peer to disconnect from
		GAPDisconnectRequest(&req, BLE_FSCI_IF);
	}
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GAPPowerLevelConfigure( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_PowerLevel_t advertisingTxPower,
        ATMO_BLE_PowerLevel_t connectionTxPower )
{
	return ATMO_BLE_Status_NotSupported;
}

bool ATMO_MK64F_BLE_PrivAddService( ATMO_UUID_t* uuid, ATMO_BLE_Handle_t* handle )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	GATTDBDynamicAddPrimaryServiceDeclarationRequest_t req;

	if( uuid->type == ATMO_UUID_Type_16_Bit )
	{
		memcpy( req.Uuid.Uuid16Bits, uuid->data, 2 );
		req.UuidType = Uuid16Bits;
	}
	else
	{
		memcpy( req.Uuid.Uuid128Bits, uuid->data, 16 );
		req.UuidType = Uuid128Bits;
	}

	GATTDBDynamicAddPrimaryServiceDeclarationRequest( &req, BLE_FSCI_IF );
	xSemaphoreTake( gattDbConfirmMutex, 1000 / portTICK_PERIOD_MS );


	// Grab handle
	if ( !MK64F_FindServiceHandle( uuid, handle ) )
	{
		return false;
	}

	return true;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSAddService( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t* handle,
        const char* serviceUUID )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	ATMO_UUID_t uuid;

	if ( ATMO_StringToUuid( serviceUUID, &uuid, ATMO_ENDIAN_Type_Little ) == ATMO_Status_Success )
	{
		// Add service to the virtual database. When initialization is complete, this will be flushed to the KW41Z
		if ( !ATMO_BLE_DB_MK64F_AddService( &uuid, handle ) )
		{
			return ATMO_BLE_Status_Fail;
		}
	}
	else
	{
		return ATMO_BLE_Status_Fail;
	}


	return ATMO_BLE_Status_Success;
}

static bool ATMO_MK64F_BLE_RegisterWrittenCallbacks()
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	// Enable written callbacks on all characteristics
	ATMO_BLE_Handle_t handles[currentNumCharacteristics];
	uint8_t numHandles = 0;

	for ( int i = 0; i < currentNumCharacteristics; i++ )
	{
		if( characteristics[i].properties & ( ATMO_BLE_Property_Write | ATMO_BLE_Property_WriteWithoutResponse ) )
		{
			shell_printf("Properties: %02X\r\n", characteristics[i].properties);
			ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &characteristics[i].handle, &handles[numHandles] );
			shell_printf( "Registering cb for handle %04X\r\n", handles[numHandles] );
			numHandles++;
		}
	}

	ATMO_MK64F_BLE_RegisterForWriteNotifications( numHandles * sizeof( ATMO_BLE_Handle_t ), ( uint8_t* )handles );

	return true;
}

bool ATMO_MK64F_BLE_PrivAddCharacteristic( ATMO_UUID_t* charUuid, ATMO_BLE_Handle_t serviceHandle, uint8_t properties,
        uint8_t permissions, uint32_t maxLen, ATMO_BLE_Handle_t* handle )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	GATTDBDynamicAddCharacteristicDeclarationAndValueRequest_t req;

	if( charUuid->type == ATMO_UUID_Type_16_Bit )
	{
		memcpy( req.Uuid.Uuid16Bits, charUuid->data, 2 );
		req.UuidType = Uuid16Bits;
	}
	else
	{
		memcpy( req.Uuid.Uuid128Bits, charUuid->data, 16 );
		req.UuidType = Uuid128Bits;
	}

	req.CharacteristicProperties = properties;
	req.MaxValueLength = maxLen;
	req.InitialValueLength = maxLen;

	req.InitialValue = ATMO_Malloc( maxLen );

	if ( req.InitialValue == NULL )
	{
		return false;
	}

	memset( req.InitialValue, 0, maxLen );

	req.ValueAccessPermissions = 0;

	if(permissions & ATMO_BLE_Permission_Read)
	{
		req.ValueAccessPermissions |=
		    GATTDBDynamicAddCharacteristicDeclarationAndValueRequest_ValueAccessPermissions_gPermissionFlagReadable_c;
	}
	if(permissions & ATMO_BLE_Permission_Write)
	{
		req.ValueAccessPermissions |=
		    GATTDBDynamicAddCharacteristicDeclarationAndValueRequest_ValueAccessPermissions_gPermissionFlagWritable_c;
	}

	GATTDBDynamicAddCharacteristicDeclarationAndValueRequest( &req, BLE_FSCI_IF );
	xSemaphoreTake( gattDbConfirmMutex, 1000 / portTICK_PERIOD_MS );

	ATMO_PLATFORM_DelayMilliseconds(50);

	if( properties & ( ATMO_BLE_Property_Notify | ATMO_BLE_Property_Indicate ) )
	{
		GATTDBDynamicAddCccdRequest( BLE_FSCI_IF );
	}

	ATMO_Free( req.InitialValue );

	ATMO_PLATFORM_DelayMilliseconds(50);

	// Grab handle from KW41Z
	if ( !MK64F_FindCharHandle( charUuid, serviceHandle, handle ) )
	{
		return false;
	}

	return true;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSAddCharacteristic( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t* handle,
        ATMO_BLE_Handle_t serviceHandle,
        const char* characteristicUUID, uint8_t properties, uint8_t permissions, uint32_t maxLen )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	ATMO_UUID_t uuid;

	if ( ATMO_StringToUuid( characteristicUUID, &uuid, ATMO_ENDIAN_Type_Little ) == ATMO_Status_Success )
	{
		// Add characteristic to virtual DB
		// This will be flushed to the KW41Z post init
		if ( !ATMO_BLE_DB_MK64F_AddCharacteristicToService( &serviceHandle, &uuid, properties, permissions, maxLen, handle ) )
		{
			return ATMO_BLE_Status_Fail;
		}

		characteristics[currentNumCharacteristics].handle = *handle;
		characteristics[currentNumCharacteristics].properties = properties;
		characteristics[currentNumCharacteristics++].initialized = false;
	}
	else
	{
		return ATMO_BLE_Status_Fail;
	}

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSGetCharacteristicValue( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint8_t* valueBuf, uint32_t bufLen, uint32_t* valueLen )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	GATTDBReadAttributeRequest_t req;
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &handle, &req.Handle );
	req.MaxBytes = ATMO_MAX_BLE_CHARACTERISTIC_SIZE;

	GATTDBReadAttributeRequest( &req, BLE_FSCI_IF );

	// Wait a second for the result to come back in
	if ( xSemaphoreTake( charReadMutex,
	                     1000 / portTICK_PERIOD_MS ) == pdTRUE )
	{
		memcpy( valueBuf, latestCharDataRead, latestCharDataReadSize <= bufLen ? latestCharDataReadSize : bufLen );
		*valueLen = latestCharDataReadSize;
	}
	else
	{
		return ATMO_BLE_Status_Fail;
	}

	return ATMO_BLE_Status_Success;

}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSRegisterCharacteristicCallback( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_Handle_t handle,
        ATMO_BLE_Characteristic_Event_t event, ATMO_Callback_t cb )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	if ( event != ATMO_BLE_Characteristic_Written )
	{
		return ATMO_BLE_Status_NotSupported;
	}

	for ( uint32_t i = 0; i < currentNumCharacteristics; i++ )
	{
		if ( characteristics[i].handle == handle )
		{
			characteristics[i].callback = cb;
		}
	}

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSRegisterCharacteristicAbilityHandle( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_Handle_t handle,
        ATMO_BLE_Characteristic_Event_t event, unsigned int abilityHandle )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	if ( event != ATMO_BLE_Characteristic_Written )
	{
		return ATMO_BLE_Status_NotSupported;
	}

	for ( uint32_t i = 0; i < currentNumCharacteristics; i++ )
	{
		if ( characteristics[i].handle == handle )
		{
			characteristics[i].abilityHandle = abilityHandle;
			characteristics[i].abilityHandlerRegistered = true;
		}
	}

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_PrivGATTSSetCharacteristic( ATMO_BLE_Handle_t handle,
        uint16_t length, uint8_t* value,
        ATMO_BLE_CharProperties_t* properties )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	GATTDBWriteAttributeRequest_t req;
	req.Handle = handle;
	req.Value = value;
	req.ValueLength = length;
	GATTDBWriteAttributeRequest( &req, BLE_FSCI_IF );

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSSetCharacteristic( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint16_t length, uint8_t* value,
        ATMO_BLE_CharProperties_t* properties )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	if( length > ATMO_MAX_BLE_CHARACTERISTIC_SIZE || length == 0)
	{
		return ATMO_BLE_Status_Invalid;
	}

	ATMO_BLE_Handle_t kw41zHandle;
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &handle, &kw41zHandle );
	ATMO_CharacteristicInfo_t* characteristic = MK64F_GetCharacteristicByHandle( handle );
	characteristic->initialized = true;

	ATMO_BLE_Status_t retVal = ATMO_MK64F_BLE_PrivGATTSSetCharacteristic( kw41zHandle, length, value, properties );

	if( retVal != ATMO_BLE_Status_Success )
	{
		return retVal;
	}

	// Send notification
	if( characteristic->properties & ATMO_BLE_Property_Notify )
	{
		//shell_printf( "Sending notify on %02X\r\n", kw41zHandle );
		ATMO_MK64F_BLE_GATTSSendNotify( instance, handle, length, value );
	}

	if( characteristic->properties & ATMO_BLE_Property_Indicate )
	{
		ATMO_MK64F_BLE_GATTSSendIndicate( instance, handle, length, value );
	}

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSWriteDescriptor( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint16_t length, uint8_t* value,
        ATMO_BLE_CharProperties_t* properties )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSSendIndicate( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint16_t size, uint8_t* value )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	// It's alright if we're not connected
	if( !currentlyConnected )
	{
		return ATMO_BLE_Status_Success;
	}

	ATMO_CharacteristicInfo_t* characteristic = MK64F_GetCharacteristicByHandle( handle );

	if( !characteristic->indicationsEnabled )
	{
		return ATMO_BLE_Status_Success;
	}

	ATMO_BLE_Handle_t kw41zHandle;
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &handle, &kw41zHandle );

	GATTServerSendIndicationRequest_t req;
	req.DeviceId = connectedDeviceId;
	req.Handle = kw41zHandle;

	GATTServerSendIndicationRequest( &req, BLE_FSCI_IF );
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTSSendNotify( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle, uint16_t size, uint8_t *value )
{
	if((!mk64BleEnabledOnBoot) || (!mk64fBleEnabled))
	{
		return ATMO_BLE_Status_Fail;
	}

	// It's alright if we're not connected
	if( !currentlyConnected )
	{
		return ATMO_BLE_Status_Success;
	}

	ATMO_CharacteristicInfo_t* characteristic = MK64F_GetCharacteristicByHandle( handle );

	if( !characteristic->notificationsEnabled )
	{
		return ATMO_BLE_Status_Success;
	}

	ATMO_BLE_Handle_t kw41zHandle;
	ATMO_BLE_DB_MK64F_AtmoHandleToKw41z( &handle, &kw41zHandle );

	if(value != NULL && size > 0)
	{
		ATMO_MK64F_BLE_PrivGATTSSetCharacteristic( kw41zHandle, size, value, NULL );
	}

	GATTServerSendNotificationRequest_t req;
	req.DeviceId = connectedDeviceId;
	req.Handle = kw41zHandle;

	GATTServerSendNotificationRequest( &req, BLE_FSCI_IF );
	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTCDiscoverService( ATMO_DriverInstanceData_t* instance,
        ATMO_BLE_ServiceHandle_t serviceUUID, ATMO_BLE_ScanResults_t* result )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTCWriteCharacteristic( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint16_t size, uint8_t* value )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTCReadCharacteristic( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        uint16_t size, uint8_t* payload )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_GATTCEnableNotification( ATMO_DriverInstanceData_t* instance, ATMO_BLE_Handle_t handle,
        bool indicate )
{
	return ATMO_BLE_Status_NotSupported;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_RegisterEventCallback( ATMO_DriverInstanceData_t *instance, ATMO_BLE_Event_t event, ATMO_Callback_t cb)
{
	if(_mk64BleEventNumCallbacks[event] >= ATMO_BLE_NUM_CALLBACKS_PER_EVENT)
	{
		return ATMO_BLE_Status_Fail;
	}

	_mk64BleEventCallback[event][_mk64BleEventNumCallbacks[event]++] = cb;

	return ATMO_BLE_Status_Success;
}

ATMO_BLE_Status_t ATMO_MK64F_BLE_RegisterEventAbilityHandle( ATMO_DriverInstanceData_t *instance, ATMO_BLE_Event_t event, unsigned int abilityHandle)
{
	if(_mk64BleEventNumAbilityHandles[event] >= ATMO_BLE_NUM_CALLBACKS_PER_EVENT)
	{
		return ATMO_BLE_Status_Fail;
	}

	_mk64BleEventAbilityHandle[event][_mk64BleEventNumAbilityHandles[event]++] = abilityHandle;

	return ATMO_BLE_Status_Success;
}
