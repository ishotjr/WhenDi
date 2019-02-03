#include "thread_volansys.h"

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "SerialManager.h"
#include "MemManager.h"
#include "fsl_os_abstraction.h"
#include "FsciInterface.h"
#include "FunctionLib.h"
#include "cmd_threadip.h"
#include "shell.h"
#include "Panic.h"
#include "TimersManager.h"
#include "battery.h"
#include "buzzer.h"
#include "network_utils.h"
#include "coap.h"
#include "backlight.h"
#include "a100x_interface.h"
#include "rgb_led.h"

#define gFSCI_IpStackOpGCnf_c                   0xCFU
/*! FSCI utility Confirmations/Indications              */
#define gFSCI_CnfOpcodeGroup_c                  0xA4

/* GET REQUESTS FROM GATEWAY */
#define APP_CO2_URI_PATH                        "/co2"
#define APP_LIGHT_URI_PATH                      "/light"
#define APP_HUMID_URI_PATH                      "/humid"
#define APP_TEMP_URI_PATH                       "/temp"
#define APP_PRES_URI_PATH                       "/pres"
#define APP_ROTSPEED_URI_PATH                   "/rot_speed"
#define APP_ACCEL_URI_PATH                      "/accel"
#define APP_MAGFIELD_URI_PATH                   "/mag_field"
#define APP_BATTERY_URI_PATH                    "/bat"

/* GET / POST REQUESTS FROM GATEWAY */
#define APP_BUZZER_URI_PATH                     "/buzzer"
#define APP_BACKLIGHT_URI_PATH                  "/backlight"
#define APP_RGBLED_URI_PATH                     "/rgb_led"
#define APP_DATE_URI_PATH                       "/date"
#define APP_TIME_URI_PATH                       "/time"

/* RPK PUSHING COAP MESSAGES AT SPECIFIC EVENTS */
#define APP_BUTTON_URI_PATH                     "/button"
#define APP_TOUCH_URI_PATH                      "/touch"

/* Authentication URIs */
#define APP_AUTH_REQ_URI_PATH                   "/auth_req"
#define APP_AUTH_CHL_URI_PATH                   "/auth_chl"
#define APP_AUTH_RES_URI_PATH                   "/auth_res"
#define APP_AUTH_UID_RAW_LEN					16
#define APP_AUTH_CERT_RAW_LEN					128
#define APP_AUTH_CHL_RSP_RAW_LEN				44
#define APP_AUTH_UID_ENC_LEN					24
#define APP_AUTH_CERT_ENC_LEN					172
#define APP_AUTH_CHL_RSP_ENC_LEN				60

/* JSON reply formats below */
#define APP_CO2_JSON_FORMAT                     "{\"uri\":\"/co2_get\",\"data\":[{\"co2\":\"%d\"}]}"
#define APP_LIGHT_JSON_FORMAT                   "{\"uri\":\"/light_get\",\"data\":[{\"light\":\"%.2f\"}]}"
#define APP_HUMID_JSON_FORMAT                   "{\"uri\":\"/humid_get\",\"data\":[{\"humid\":\"%.2f\"}]}"
#define APP_TEMP_JSON_FORMAT                    "{\"uri\":\"/temp_get\",\"data\":[{\"temp\":\"%.2f\"}]}"
#define APP_PRES_JSON_FORMAT                    "{\"uri\":\"/pres_get\",\"data\":[{\"pres\":\"%d\"}]}"
#define APP_ROTSPEED_JSON_FORMAT                "{\"uri\":\"/rot_speed_get\",\"data\":[{\"X\":\"%d\",\"Y\":\"%d\",\"Z\":\"%d\"}]}"
#define APP_ACCEL_JSON_FORMAT                   "{\"uri\":\"/accel_get\",\"data\":[{\"X\":\"%.6f\",\"Y\":\"%.6f\",\"Z\":\"%.6f\"}]}"
#define APP_MAGFIELD_JSON_FORMAT                "{\"uri\":\"/mag_field_get\",\"data\":[{\"X\":\"%.1f\",\"Y\":\"%.1f\",\"Z\":\"%.1f\"}]}"
#define APP_BATTERY_JSON_FORMAT                 "{\"uri\":\"/bat_get\",\"data\":[{\"bat_per\":\"%d\",\"bat_charge\":\"%d\"}]}"

#define APP_BUZZER_GET_JSON_FORMAT              "{\"uri\":\"/buzzer_get\",\"data\":[{\"buzzer\":\"%d\"}]}"
#define APP_BUZZER_POST_JSON_FORMAT             "{\"uri\":\"/buzzer_post\",\"data\":[{\"buzzer\":\"%d\"}]}"
#define APP_BACKLIGHT_GET_JSON_FORMAT           "{\"uri\":\"/backlight_get\",\"data\":[{\"backlight\":\"%d\"}]}"
#define APP_BACKLIGHT_POST_JSON_FORMAT          "{\"uri\":\"/backlight_post\",\"data\":[{\"backlight\":\"%d\"}]}"
#define APP_RGBLED_GET_JSON_FORMAT              "{\"uri\":\"/rgb_led_get\",\"data\":[{\"brightness\":\"%d\",\"color\":\"%d\"}]}"
#define APP_RGBLED_POST_JSON_FORMAT             "{\"uri\":\"/rgb_led_post\",\"data\":[{\"brightness\":\"%d\",\"color\":\"%d\"}]}"

#define APP_AUTH_REQ_JSON_FORMAT                "{\"uri\":\"/auth_req\",\"data\":[{\"uid\":\"%s\",\"cert\":\"%s\"}]}"
#define APP_AUTH_CHL_JSON_FORMAT                "{\"uri\":\"/auth_chl\",\"data\":[{\"chal\":\"%s\"}]}"
#define APP_AUTH_RES_JSON_FORMAT                "{\"uri\":\"/auth_res\",\"data\":[{\"res\":\"%s\"}]}"

#define JSON_BUFF_SIZE  256

#define THR_FSCI_IF (0)
#define THREAD_INSTANCE_ID (0)

static uint16_t ATMO_THREAD_VOLANSYS_airQualityTVOC = 0;
static uint16_t ATMO_THREAD_VOLANSYS_airQualityCO2 = 0;
static float ATMO_THREAD_VOLANSYS_ambientLight = 0;
static float ATMO_THREAD_VOLANSYS_humidity = 0;
static float ATMO_THREAD_VOLANSYS_temperature = 0;
static int32_t ATMO_THREAD_VOLANSYS_pressure = 0;
static int16_t ATMO_THREAD_VOLANSYS_rotSpeed[3] = {0};
static float ATMO_THREAD_VOLANSYS_accel[3] = {0};
static float ATMO_THREAD_VOLANSYS_magField[3] = {0};

static bool backlightAbilityHandleRegistered = false;
static unsigned int backlightAbilityHandle = 0;

static bool rgbLedAbilityHandleRegistered = false;
static unsigned int rgbLedAbilityHandle = 0;

static bool buzzerAbilityHandleRegistered = false;
static unsigned int buzzerAbilityHandle = 0;

static bool ATMO_THREAD_VOLANSYS_enabled = true;

static uint8_t mCoapInstId = THR_ALL_FFs8;

static uint8_t lastBattLevel = 100;
static uint8_t lastChargeStatus = 1;

#define ATMO_THREAD_NUM_CONNECTION_CALLBACKS (10)

static unsigned int numThreadConnectedCallbacks = 0;
static ATMO_Callback_t cbConnected[ATMO_THREAD_NUM_CONNECTION_CALLBACKS] = {NULL};

static unsigned int numThreadDisconnectedCallbacks = 0;
static ATMO_Callback_t cbDisconnected[ATMO_THREAD_NUM_CONNECTION_CALLBACKS] = {NULL};

static bool currentlyJoining = false;

static ThreadOtaCompleteCb_t otaCompleteCb = NULL;

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetTemperature(float temperatureC)
{
	ATMO_THREAD_VOLANSYS_temperature = (float)temperatureC;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetHumidity(float humidity)
{
	ATMO_THREAD_VOLANSYS_humidity = (float)humidity;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAmbientLight(uint32_t ambientLight)
{
	ATMO_THREAD_VOLANSYS_ambientLight = (float)ambientLight;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetPressure(uint32_t pressureHpa)
{
	ATMO_THREAD_VOLANSYS_pressure = (float)pressureHpa;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAirQualityTvoc(uint32_t airQuality)
{
	ATMO_THREAD_VOLANSYS_airQualityTVOC = (uint16_t)airQuality;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAirQualityCO2(uint32_t airQuality)
{
	ATMO_THREAD_VOLANSYS_airQualityCO2 = (uint16_t)airQuality;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetRotspeedX(float value)
{
	ATMO_THREAD_VOLANSYS_rotSpeed[0] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetRotspeedY(float value)
{
	ATMO_THREAD_VOLANSYS_rotSpeed[1] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetRotspeedZ(float value)
{
	ATMO_THREAD_VOLANSYS_rotSpeed[2] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAccelX(float value)
{
	ATMO_THREAD_VOLANSYS_accel[0] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAccelY(float value)
{
	ATMO_THREAD_VOLANSYS_accel[1] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetAccelZ(float value)
{
	ATMO_THREAD_VOLANSYS_accel[2] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetMagX(float value)
{
	ATMO_THREAD_VOLANSYS_magField[0] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetMagY(float value)
{
	ATMO_THREAD_VOLANSYS_magField[1] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_SetMagZ(float value)
{
	ATMO_THREAD_VOLANSYS_magField[2] = value;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

void ATMO_THREAD_BatteryStateChangeCb(uint8_t chargeLevel, uint8_t chargeStatus)
{
	lastBattLevel = chargeLevel;
	lastChargeStatus = chargeStatus;
}	

static void ATMO_THREAD_VOLANSYS_CoapSend
(
    void *param
)
{
	if(!ATMO_THREAD_VOLANSYS_enabled)
	{
		return;
	}
    uint8_t dummyCoapSession[4] = {0};
    NWKU_CoapSendRequest_t *coapSendReq = (NWKU_CoapSendRequest_t *)param;

    if (!FLib_MemCmp(dummyCoapSession, coapSendReq->coapSession, 4) ||
        mCoapInstId != THR_ALL_FFs8)
    {
        NWKU_CoapSendRequest(coapSendReq, THR_FSCI_IF);
    }
    else
    {
        ATMO_PLATFORM_DebugPrint("CoAP instance could not be created! Please try again..\r\n");
    }

    MEM_BufferFree(coapSendReq->Payload);
    MEM_BufferFree(coapSendReq);
}

static int COAP_Reply(NWKU_CoapMsgReceivedIndication_t *indication)
{
	if(!ATMO_THREAD_VOLANSYS_enabled)
	{
		return;
	}
	
	  command_ret_t ret = CMD_RET_SUCCESS;
	    uint8_t *pCoapPayload = NULL;
	    uint8_t coapPayloadSize = 0;
	    uint8_t *currentPayloadData;
	    char jsonString[JSON_BUFF_SIZE] = {0};

	    NWKU_CoapSendRequest_t *pCoapSendReq = MEM_BufferAlloc(sizeof(NWKU_CoapSendRequest_t));

	    if (!pCoapSendReq)
	    {
	        ATMO_PLATFORM_DebugPrint("Insufficient memory to create the FSCI request!\n\r");
	        return CMD_RET_SUCCESS;
	    }

	    FLib_MemSet(pCoapSendReq, 0, sizeof(NWKU_CoapSendRequest_t));
	    pCoapSendReq->InstanceID = THREAD_INSTANCE_ID;
	    pCoapSendReq->UDPPort = COAP_DEFAULT_PORT;

	    pCoapSendReq->RequestType = gCoapAcknowledgement_c;
	    pCoapSendReq->MessageType = gContent_c;

	    /* Get destination address */
	    FLib_MemCpy(pCoapSendReq->DestinationIpAddress, indication->RemoteIpAddress, sizeof(indication->RemoteIpAddress));

	    /* Get URI path */
	    FLib_MemSet(pCoapSendReq->URIpath, 0, sizeof(pCoapSendReq->URIpath));
	    FLib_MemCpy(pCoapSendReq->URIpath, indication->URIpath, FLib_StrLen(indication->URIpath));

	    /* Get the session */
	    FLib_MemCpy(pCoapSendReq->coapSession, indication->coapSession, 4);

	    /* Depending on the URI path, fill the requested payload here */
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_CO2_URI_PATH, sizeof(APP_CO2_URI_PATH) - 1)) {
	        coapPayloadSize = sizeof(uint16_t);

	        sprintf(jsonString, APP_CO2_JSON_FORMAT, ATMO_THREAD_VOLANSYS_airQualityCO2);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_LIGHT_URI_PATH, sizeof(APP_LIGHT_URI_PATH) - 1)) {
	        coapPayloadSize = sizeof(float);

	        sprintf(jsonString, APP_LIGHT_JSON_FORMAT, ATMO_THREAD_VOLANSYS_ambientLight);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_HUMID_URI_PATH, sizeof(APP_HUMID_URI_PATH) - 1)) {
	        coapPayloadSize = sizeof(float);

	        sprintf(jsonString, APP_HUMID_JSON_FORMAT, ATMO_THREAD_VOLANSYS_humidity);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_TEMP_URI_PATH, sizeof(APP_TEMP_URI_PATH) - 1)) {
	        coapPayloadSize = sizeof(float);

	        sprintf(jsonString, APP_TEMP_JSON_FORMAT, ATMO_THREAD_VOLANSYS_temperature);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_PRES_URI_PATH, sizeof(APP_PRES_URI_PATH) - 1)) {
	        coapPayloadSize = sizeof(int32_t);

	        sprintf(jsonString, APP_PRES_JSON_FORMAT, ATMO_THREAD_VOLANSYS_pressure);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_ROTSPEED_URI_PATH, sizeof(APP_ROTSPEED_URI_PATH) - 1)) {
	        coapPayloadSize = 3 * sizeof(int16_t);

	        sprintf(jsonString, APP_ROTSPEED_JSON_FORMAT, ATMO_THREAD_VOLANSYS_rotSpeed[0], ATMO_THREAD_VOLANSYS_rotSpeed[1], ATMO_THREAD_VOLANSYS_rotSpeed[2]);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_ACCEL_URI_PATH, sizeof(APP_ACCEL_URI_PATH) - 1)) {
	        coapPayloadSize = 3 * sizeof(float);

	        sprintf(jsonString, APP_ACCEL_JSON_FORMAT, ATMO_THREAD_VOLANSYS_accel[0], ATMO_THREAD_VOLANSYS_accel[1], ATMO_THREAD_VOLANSYS_accel[2]);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_MAGFIELD_URI_PATH, sizeof(APP_MAGFIELD_URI_PATH) - 1)) {
	        coapPayloadSize = 3 * sizeof(float);

	        sprintf(jsonString, APP_MAGFIELD_JSON_FORMAT, ATMO_THREAD_VOLANSYS_magField[0], ATMO_THREAD_VOLANSYS_magField[1], ATMO_THREAD_VOLANSYS_magField[2]);
	        coapPayloadSize = strlen(jsonString) + 1;
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_BATTERY_URI_PATH, sizeof(APP_BATTERY_URI_PATH) - 1)) {

			sprintf(jsonString, APP_BATTERY_JSON_FORMAT, lastBattLevel, lastChargeStatus);
			coapPayloadSize = strlen(jsonString) + 1;

	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_BUZZER_URI_PATH, sizeof(APP_BUZZER_URI_PATH) - 1)) {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_GET ==
	                                indication->MessageType)
	        {
	            sprintf(jsonString, APP_BUZZER_GET_JSON_FORMAT, ATMO_MK64F_Buzzer_GetState());
	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	        else
	        if (NWKU_CoapMsgReceivedIndication_MessageType_POST ==
	                                indication->MessageType)
	        {
	            if (FLib_MemCmp(indication->Payload, "data:0", sizeof("data:0") - 1))
	            {
	            	ATMO_MK64F_Buzzer_Off();
	            }
	            else
	            if (FLib_MemCmp(indication->Payload, "data:1", sizeof("data:1") - 1))
	            {
	            	ATMO_MK64F_Buzzer_On();
	            }
	            sprintf(jsonString, APP_BUZZER_POST_JSON_FORMAT, ATMO_MK64F_Buzzer_GetState());
	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_BACKLIGHT_URI_PATH, sizeof(APP_BACKLIGHT_URI_PATH) - 1)) {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_GET ==
	                                indication->MessageType)
	        {
	        	backlight_level_t backlight_level;
	        	if (Backlight_GetLevel(&backlight_level))
	        	{
	        		ATMO_PLATFORM_DebugPrint("Error while reading from backlight!\n\r");
	        	}
	        	else
	        	{
					sprintf(jsonString, APP_BACKLIGHT_GET_JSON_FORMAT, backlight_level);
					coapPayloadSize = strlen(jsonString) + 1;
	        	}
	        }
	        else
	        if (NWKU_CoapMsgReceivedIndication_MessageType_POST ==
	                                indication->MessageType)
	        {
	        	_Bool noError = true;
	        	backlight_level_t backlight_level;
	            if (FLib_MemCmp(indication->Payload, "data:0", sizeof("data:0") - 1))
	            {
	            	backlight_level = BLIGHT_LEVEL_OFF;
	            }
	            else if (FLib_MemCmp(indication->Payload, "data:1", sizeof("data:1") - 1))
	            {
	            	backlight_level = BLIGHT_LEVEL_LOW;
	            }
	            else if (FLib_MemCmp(indication->Payload, "data:2", sizeof("data:2") - 1))
	            {
	            	backlight_level = BLIGHT_LEVEL_MEDIUM;
	            }
	            else if (FLib_MemCmp(indication->Payload, "data:3", sizeof("data:3") - 1))
	            {
	            	backlight_level = BLIGHT_LEVEL_HIGH;
	            }
	            else
	            {
	            	noError = false;
	            }

	            if(noError)
	            {
		        	
					Backlight_SetLevel(backlight_level);
		        	sprintf(jsonString, APP_BACKLIGHT_POST_JSON_FORMAT, backlight_level);
		        	coapPayloadSize = strlen(jsonString) + 1;
	            }
	            else
	            {
	                ATMO_PLATFORM_DebugPrint("Error while writing to backlight!\n\r");
	            }
	        }
	    }
	    else
	    if (FLib_MemCmp(pCoapSendReq->URIpath, APP_RGBLED_URI_PATH, sizeof(APP_RGBLED_URI_PATH) - 1)) {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_GET ==
	                                indication->MessageType)
	        {
	            uint8_t brightness_state = 0;
	            uint8_t color_state = 0;
				RGB_Led_Get_State(&brightness_state, &color_state);

	            sprintf(jsonString, APP_RGBLED_GET_JSON_FORMAT, brightness_state, color_state);
	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	        else
	        if (NWKU_CoapMsgReceivedIndication_MessageType_POST ==
	                                indication->MessageType)
	        {
	            _Bool noError = true;
	            uint8_t brightness_state;
	            uint8_t color_state;
	            currentPayloadData = indication->Payload;
	            if (FLib_MemCmp(currentPayloadData, "brightness:", sizeof("brightness:") - 1))
	            {
	                /* brightness */
	                currentPayloadData += sizeof("brightness:") - 1;
	                if (FLib_MemCmp(currentPayloadData, "0", 1))
	                {
	                    /**/
	                    currentPayloadData += 1;
	                    brightness_state = 0;
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "1", 1))
	                {
	                    /**/
	                    currentPayloadData += 1;
	                    brightness_state = 1;
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "2", 1))
	                {
	                    /**/
	                    currentPayloadData += 1;
	                    brightness_state = 2;
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "3", 1))
	                {
	                    /**/
	                    currentPayloadData += 1;
	                    brightness_state = 3;
	                }
	                else
	                {
	                    noError = false;
	                }
	            }
	            else
	            {
	                noError = false;
	            }

	            if (noError && (FLib_MemCmp(currentPayloadData, ",color:", sizeof(",color:") - 1)))
	            {
	                /* color */
	                currentPayloadData += sizeof(",color:") - 1;
	                if (FLib_MemCmp(currentPayloadData, "0", 1))
	                {
	                    /**/
	                    color_state = 0; /* green */
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "1", 1))
	                {
	                    /**/
	                    color_state = 1; /* red */
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "2", 1))
	                {
	                    /**/
	                    color_state = 2; /* blue */
	                }
	                else
	                if (FLib_MemCmp(currentPayloadData, "3", 1))
	                {
	                    /**/
	                    color_state = 3; /* white */
	                }
	                else
	                {
	                    noError = false;
	                }
	            }
	            else
	            {
	                noError = false;
	            }

	            if (noError)
	            {
	                /* set brightness & color */
	                if (RGB_Led_Set_State(brightness_state, color_state) == 0)
	                {
	                    sprintf(jsonString, APP_RGBLED_POST_JSON_FORMAT, brightness_state, color_state);
	                    coapPayloadSize = strlen(jsonString) + 1;
	                }
	                else
	                {
	                	noError = false;
	                }
	            }

	            if (!noError)
	            {
	                ATMO_PLATFORM_DebugPrint("Error while writing to RGB LED!\n\r");
	            }
	        }
	    }
	    else if (FLib_MemCmp(pCoapSendReq->URIpath, APP_AUTH_REQ_URI_PATH, sizeof(APP_AUTH_REQ_URI_PATH) - 1))
	    {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_GET == indication->MessageType)
	        {
	        	uint8_t uid[APP_AUTH_UID_RAW_LEN] = {0};

	        	if (ATMO_MK64F_Auth_GetUid(uid))
	        	{
	        		ATMO_PLATFORM_DebugPrint("Error while writing retrieving UID!\n\r");
	        	}

	        	uint8_t cert[APP_AUTH_CERT_RAW_LEN] = {0};

	        	if (ATMO_MK64F_Auth_GetCert(cert))
	        	{
	        		ATMO_PLATFORM_DebugPrint("Error while writing retrieving Certificate!\n\r");
	        	}

	        	// Encode UID
	        	uint8_t b64_UID[APP_AUTH_UID_ENC_LEN + 1] = {0};
	        	RPK_Base64_Encode(uid, APP_AUTH_UID_RAW_LEN, b64_UID, APP_AUTH_UID_ENC_LEN);

	        	// Encode Certificate
	        	uint8_t b64_CERT[APP_AUTH_CERT_ENC_LEN + 1] = {0};
	        	RPK_Base64_Encode(cert, APP_AUTH_CERT_RAW_LEN, b64_CERT, APP_AUTH_CERT_ENC_LEN);

	        	ATMO_PLATFORM_DebugPrint("Sending /auth_req payload!\n\r");

	            sprintf(jsonString, APP_AUTH_REQ_JSON_FORMAT, (const char *)b64_UID, (const char*)b64_CERT);
	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	    }
	    else if (FLib_MemCmp(pCoapSendReq->URIpath, APP_AUTH_CHL_URI_PATH, sizeof(APP_AUTH_CHL_URI_PATH) - 1))
	    {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_POST == indication->MessageType)
	        {
	        	uint8_t chlState = 0;
	            currentPayloadData = (indication->Payload + sizeof("{\"uri\":\"/auth_chl\",\"data\":[{\"") - 1);
	            if (FLib_MemCmp(currentPayloadData, "chal\":\"", sizeof("chal\":\"") - 1))
	            {
	            	currentPayloadData += sizeof("chal\":\"") - 1;

	            	// Copy payload data to buffer
	            	uint8_t b64_CHALLENGE[APP_AUTH_CHL_RSP_ENC_LEN + 1] = {0};
	            	FLib_MemCpy(b64_CHALLENGE, currentPayloadData, APP_AUTH_CHL_RSP_ENC_LEN);


	            	// Decode challenge data
	            	uint8_t challenge[APP_AUTH_CHL_RSP_RAW_LEN] = {0};
	            	RPK_Base64_Decode(b64_CHALLENGE, APP_AUTH_CHL_RSP_ENC_LEN, challenge, APP_AUTH_CHL_RSP_RAW_LEN);


	            	// Send Payload data to A1006
	            	if (ATMO_MK64F_Auth_SetChallenge(challenge))
	            	{
	            		chlState = 2;
	            	}
	            }
	            else
	            {
	            	chlState = 1;
	            }

	            if (1 == chlState)
	            {
	            	sprintf(jsonString, APP_AUTH_CHL_JSON_FORMAT, "INVALID");
	            }
	            else if (2 == chlState)
	            {
	            	sprintf(jsonString, APP_AUTH_CHL_JSON_FORMAT, "ERROR");
	            }
	            else
	            {
	            	sprintf(jsonString, APP_AUTH_CHL_JSON_FORMAT, "OK");
	            }

	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	    }
	    else if (FLib_MemCmp(pCoapSendReq->URIpath, APP_AUTH_RES_URI_PATH, sizeof(APP_AUTH_RES_URI_PATH) - 1))
	    {
	        if (NWKU_CoapMsgReceivedIndication_MessageType_GET == indication->MessageType)
	        {
	        	// TODO: Get A1006 Response

	        	uint8_t response[APP_AUTH_CHL_RSP_RAW_LEN] = {0};

	        	if (ATMO_MK64F_Auth_GetResponse(response))
	        	{
	        		ATMO_PLATFORM_DebugPrint("Error while writing retrieving challenge response!\n\r");
	        	}

	        	// Encode response
	        	uint8_t b64_RESPONSE[APP_AUTH_CHL_RSP_ENC_LEN + 1] = {0};
	        	RPK_Base64_Encode(response, APP_AUTH_CHL_RSP_RAW_LEN, b64_RESPONSE, APP_AUTH_CHL_RSP_ENC_LEN);

	            sprintf(jsonString, APP_AUTH_RES_JSON_FORMAT, (const char*)b64_RESPONSE);
	            coapPayloadSize = strlen(jsonString) + 1;
	        }
	    }
	    if (coapPayloadSize)
	    {
	        pCoapPayload = MEM_BufferAlloc(coapPayloadSize);
	        FLib_MemCpy(pCoapPayload, jsonString, coapPayloadSize);
	    }

	    pCoapSendReq->PayloadLength = coapPayloadSize;
	    pCoapSendReq->Payload = (char *)pCoapPayload;

	    /* We don't need to request for a COAP instance creation */
	    ATMO_THREAD_VOLANSYS_CoapSend((void *)pCoapSendReq);

	    return ret;
}

void THCI_RxCb
(
    void *pData,
    void *param,
    uint32_t interfaceId
)
{
    thrEvtContainer_t container; // this could be allocated instead
    KHC_ThreadIP_RX_MsgHandler(pData, &container, interfaceId);

#ifdef ATMO_THREAD_DEBUG
    ATMO_PLATFORM_DebugPrint("Thread RX: %X\r\n", container.id);
#endif

    switch(container.id)
    {
		case 0xCF22:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR CPU Reset Indication\r\n");
#endif
			currentlyJoining = false;
			break;
		}
		case 0xCF54:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR_EventGeneralConfirm\r\n");
#endif
			break;
		}
		case 0xCF18:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR SetAttrConfirm\r\n");
#endif
			break;
		}
		case 0xCF1C:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR JoinConfirm\r\n");
#endif
			currentlyJoining = false;
			break;
		}
		case 0xCFD1:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("OTA Complete, resetting MCU");
#endif
			if(otaCompleteCb)
			{
				otaCompleteCb();
			}
			ResetMCU();
			break;
		}
		case 0xCF55:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR_EventNwkCommissioningIndication\r\n");
#endif
			break;
		}
		case 0xCF52:
		{
#ifdef ATMO_THREAD_DEBUG
			ATMO_PLATFORM_DebugPrint("THR_EventNwkJoinConfirm\r\n");
#endif

			currentlyJoining = false;

            switch (container.Data.THR_EventNwkJoinConfirm.EventStatus)
            {
                case THR_EventNwkJoinConfirm_EventStatus_Attaching:
                    ATMO_PLATFORM_DebugPrint("THR Attaching...\r\n");
                    break;

                case THR_EventNwkJoinConfirm_EventStatus_JoinSuccess:
                	ATMO_PLATFORM_DebugPrint("Join success\r\n");
					for(unsigned int i = 0; i < numThreadConnectedCallbacks; i++)
					{
						cbConnected[i](NULL);
					}
                    break;

                case THR_EventNwkJoinConfirm_EventStatus_JoinFailed:
                	ATMO_PLATFORM_DebugPrint("Join failed\r\n");
                    break;

                default:
#ifdef ATMO_THREAD_DEBUG
                	ATMO_PLATFORM_DebugPrint("Unrecognized status 0x%04X", container.Data.THR_EventNwkJoinConfirm.EventStatus);
#endif
					break;
            }
			break;
		}
        case 0xCF92:
            switch (container.Data.NWKU_CoapMsgReceivedIndication.Status)
            {
                case NWKU_CoapMsgReceivedIndication_Status_Success:
                    if (NWKU_CoapMsgReceivedIndication_MessageType_GET ==
                        container.Data.NWKU_CoapMsgReceivedIndication.MessageType ||
                        NWKU_CoapMsgReceivedIndication_MessageType_POST ==
                        container.Data.NWKU_CoapMsgReceivedIndication.MessageType)
                    {
                    	COAP_Reply(&container.Data.NWKU_CoapMsgReceivedIndication);
                    }
                    //SHELL_PrintCoapMsg(&(container->Data.NWKU_CoapMsgReceivedIndication));
                    break;

                case NWKU_CoapMsgReceivedIndication_Status_Failed:
                    break;

                case NWKU_CoapMsgReceivedIndication_Status_Duplicate:
                    break;

                default:
                    break;
            }

            MEM_BufferFree(container.Data.NWKU_CoapMsgReceivedIndication.Payload);

            break;


    }

    //SHELL_ThrEventNotify(&container);
}

static void ATMO_THREAD_VOLANSYS_SetChannel(uint8_t channel)
{
    THR_SetAttrRequest_t req;
    req.InstanceId = THREAD_INSTANCE_ID;
    req.AttributeId = THR_SetAttrRequest_AttributeId_Channel;
    req.Index = 0;
    req.AttributeValue.Channel.AttrSize = 1;
    req.AttributeValue.Channel.Value = channel;
    THR_SetAttrRequest(&req, THR_FSCI_IF);
}

void ATMO_THREAD_VOLANSYS_RegisterOtaCompleteCb(ThreadOtaCompleteCb_t cb)
{
	otaCompleteCb = cb;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_Init()
{
	ATMO_THREAD_VOLANSYS_GetEnabled(&ATMO_THREAD_VOLANSYS_enabled);

	ATMO_PLATFORM_DebugPrint("Thread Enabled: %d\r\n", ATMO_THREAD_VOLANSYS_enabled);

	if(!ATMO_THREAD_VOLANSYS_enabled)
	{
		return ATMO_THREAD_VOLANSYS_Status_Success;
	}

	ATMO_BATTERY_MK64F_RegisterStateChangeCallback(ATMO_THREAD_BatteryStateChangeCb);
	ATMO_BATTERY_MK64F_ForceUpdate();

	ATMO_PLATFORM_DelayMilliseconds(100);

	ATMO_THREAD_VOLANSYS_TryJoin();

	return ATMO_THREAD_VOLANSYS_Status_Success;
}

void ATMO_THREAD_VOLANSYS_GetEnabled(bool *enabled)
{
	*enabled = false;
	// Write to the flash
	ATMO_FILESYSTEM_File_t file;
	if( ATMO_FILESYSTEM_FileOpen(0, &file, "thread_enabled", ATMO_RDONLY ) != ATMO_FILESYSTEM_Status_Success )
	{
		return ATMO_THREAD_VOLANSYS_Status_Fail;
	}

	if( ATMO_FILESYSTEM_FileRead(0, &file, enabled, sizeof(enabled)) != ATMO_FILESYSTEM_Status_Success )
	{
		ATMO_FILESYSTEM_FileClose(0, &file);
		return ATMO_THREAD_VOLANSYS_Status_Fail;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

void ATMO_THREAD_VOLANSYS_SetEnabled(bool enabled)
{
	// Write to the flash
	ATMO_FILESYSTEM_File_t file;
	if( ATMO_FILESYSTEM_FileOpen(0, &file, "thread_enabled", ATMO_CREAT | ATMO_RDWR | ATMO_TRUNC ) != ATMO_FILESYSTEM_Status_Success )
	{
		return;
	}

	if( ATMO_FILESYSTEM_FileWrite(0, &file, &enabled, sizeof(enabled)) != ATMO_FILESYSTEM_Status_Success )
	{
		ATMO_FILESYSTEM_FileClose(0, &file);
		return;
	}

	ATMO_FILESYSTEM_FileClose(0, &file);

	ATMO_THREAD_VOLANSYS_enabled = enabled;

	if(!enabled)
	{
		ATMO_PLATFORM_DebugPrint("Sending disconnect request\r\n");
		THR_DisconnectRequest_t req;
		req.InstanceID = THREAD_INSTANCE_ID;
		THR_DisconnectRequest(&req, THR_FSCI_IF);
		ATMO_PLATFORM_DelayMilliseconds(100);
	}
	return;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_TryJoin()
{
	if(!ATMO_THREAD_VOLANSYS_enabled)
	{
		return ATMO_THREAD_VOLANSYS_Status_Success;
	}

	if(currentlyJoining)
	{
		return ATMO_THREAD_VOLANSYS_Status_Success;
	}

	currentlyJoining = true;

	ATMO_PLATFORM_DebugPrint("Setting channel to 15\r\n");
	ATMO_THREAD_VOLANSYS_SetChannel(15);
	ATMO_PLATFORM_DelayMilliseconds(100);

	// Try to join
	ATMO_PLATFORM_DebugPrint("Joining network\r\n");
	THR_JoinRequest_t req;
    req.InstanceID = THREAD_INSTANCE_ID;
    req.discoveryMethod = THR_JoinRequest_discoveryMethod_gUseThreadDiscovery_c;

    if (THR_JoinRequest(&req, THR_FSCI_IF) != MEM_SUCCESS_c)
    {
        ATMO_PLATFORM_DebugPrint("No memory!\n\r");
		return ATMO_THREAD_VOLANSYS_Status_Fail;
    }

	ATMO_PLATFORM_DelayMilliseconds(100);

	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_RegisterCallbackConnected(ATMO_Callback_t cb)
{
	if(numThreadConnectedCallbacks >= ATMO_THREAD_NUM_CONNECTION_CALLBACKS)
	{
		return ATMO_THREAD_VOLANSYS_Status_Fail;
	}
	cbConnected[numThreadConnectedCallbacks++] = cb;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}

ATMO_THREAD_VOLANSYS_Status_t ATMO_THREAD_VOLANSYS_RegisterCallbackDisconnected(ATMO_Callback_t cb)
{
	if(numThreadDisconnectedCallbacks >= ATMO_THREAD_NUM_CONNECTION_CALLBACKS)
	{
		return ATMO_THREAD_VOLANSYS_Status_Fail;
	}
	cbDisconnected[numThreadDisconnectedCallbacks++] = cb;
	return ATMO_THREAD_VOLANSYS_Status_Success;
}


