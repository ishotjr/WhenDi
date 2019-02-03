/**
 ******************************************************************************
 * @file    wifi.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - WIFI header file
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

 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_WIFI__H
#define __ATMO_WIFI__H


/* Includes ------------------------------------------------------------------*/
#include "../atmo/core.h"

#ifdef __cplusplus
	extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

typedef enum
{
    ATMO_WIFI_Status_Success       = 0x00u,  // Operation was successful
    ATMO_WIFI_Status_Fail          = 0x01u,  // Operation failed
    ATMO_WIFI_Status_Initialized   = 0x02u,  // Peripheral already initialized
    ATMO_WIFI_Status_Invalid       = 0x03u,  // Invalid operation
    ATMO_WIFI_Status_NotSupported  = 0x04u,  // Feature not supported by platform
    ATMO_WIFI_Status_Unspecified   = 0x05u,  // Some other status not defined
} ATMO_WIFI_Status_t;

/**
 * @brief Borrowed from ESP32 and renumbered
 */
typedef enum {
    ATMO_WIFI_DISC_REASON_UNSPECIFIED              = 1,
    ATMO_WIFI_DISC_REASON_AUTH_EXPIRE              = 2,
    ATMO_WIFI_DISC_REASON_AUTH_LEAVE               = 3,
    ATMO_WIFI_DISC_REASON_ASSOC_EXPIRE             = 4,
    ATMO_WIFI_DISC_REASON_ASSOC_TOOMANY            = 5,
    ATMO_WIFI_DISC_REASON_NOT_AUTHED               = 6,
    ATMO_WIFI_DISC_REASON_NOT_ASSOCED              = 7,
    ATMO_WIFI_DISC_REASON_ASSOC_LEAVE              = 8,
    ATMO_WIFI_DISC_REASON_ASSOC_NOT_AUTHED         = 9,
    ATMO_WIFI_DISC_REASON_DISASSOC_PWRCAP_BAD      = 10,
    ATMO_WIFI_DISC_REASON_DISASSOC_SUPCHAN_BAD     = 11,
    ATMO_WIFI_DISC_REASON_IE_INVALID               = 13,
    ATMO_WIFI_DISC_REASON_MIC_FAILURE              = 14,
    ATMO_WIFI_DISC_REASON_4WAY_HANDSHAKE_TIMEOUT   = 15,
    ATMO_WIFI_DISC_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    ATMO_WIFI_DISC_REASON_IE_IN_4WAY_DIFFERS       = 17,
    ATMO_WIFI_DISC_REASON_GROUP_CIPHER_INVALID     = 18,
    ATMO_WIFI_DISC_REASON_PAIRWISE_CIPHER_INVALID  = 19,
    ATMO_WIFI_DISC_REASON_AKMP_INVALID             = 20,
    ATMO_WIFI_DISC_REASON_UNSUPP_RSN_IE_VERSION    = 21,
    ATMO_WIFI_DISC_REASON_INVALID_RSN_IE_CAP       = 22,
    ATMO_WIFI_DISC_REASON_802_1X_AUTH_FAILED       = 23,
    ATMO_WIFI_DISC_REASON_CIPHER_SUITE_REJECTED    = 24,

    ATMO_WIFI_DISC_REASON_BEACON_TIMEOUT           = 200,
    ATMO_WIFI_DISC_REASON_NO_AP_FOUND              = 201,
    ATMO_WIFI_DISC_REASON_AUTH_FAIL                = 202,
    ATMO_WIFI_DISC_REASON_ASSOC_FAIL               = 203,
    ATMO_WIFI_DISC_REASON_HANDSHAKE_TIMEOUT        = 204,
} ATMO_WIFI_DisconnectReason_t;

typedef enum
{
    ATMO_WIFI_MODE_AP,
    ATMO_WIFI_MODE_STA,
    ATMO_WIFI_MODE_AP_STA
} ATMO_WIFI_Mode_t;

typedef enum
{
    ATMO_WIFI_CONNECTED = 0, /**< Connected to access point in STA mode, or client connected in AP mode. */
    ATMO_WIFI_DISCONNECTED, /**< Disconnected from access point in STA mode, or client disconnected in AP mode. */
    ATMO_WIFI_NUM_EVENTS,
} ATMO_WIFI_Event_t;

typedef enum
{
    ATMO_WIFI_CONNECTIVITY_CONNECTED,
    ATMO_WIFI_CONNECTIVITY_CONNECTING,
    ATMO_WIFI_CONNECTIVITY_DISCONNECTED,
} ATMO_WIFI_ConnectivityStatus_t;

typedef enum 
{
    ATMO_WIFI_IPV4,
    ATMO_WIFI_IPV6,
} ATMO_WIFI_IpType_t;

typedef struct {
    ATMO_WIFI_ConnectivityStatus_t connectivity;
    ATMO_WIFI_DisconnectReason_t disconnectReason;
    ATMO_WIFI_IpType_t ipType;
    uint8_t ipAddress[16]; // To store possible ipv6
} ATMO_WIFI_ConnectionStatus_t;

/* Exported Structures -------------------------------------------------------*/

typedef struct
{
    const char* ssid;
    const char* password;
    ATMO_WIFI_Mode_t mode;
} ATMO_WIFI_Config_t;


// Some C gore so we can use the struct within itself
typedef struct ATMO_WIFI_DriverInstance_t ATMO_WIFI_DriverInstance_t;

struct ATMO_WIFI_DriverInstance_t{
	ATMO_WIFI_Status_t (*Init)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_WIFI_Status_t (*DeInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_WIFI_Status_t (*SetConfiguration)(ATMO_DriverInstanceData_t *instanceData, const ATMO_WIFI_Config_t *config);
    ATMO_WIFI_Status_t (*Start)(ATMO_DriverInstanceData_t *instanceData);
    ATMO_WIFI_Status_t (*Stop)(ATMO_DriverInstanceData_t *instanceData);
    ATMO_WIFI_Status_t (*GetIpAddress)(ATMO_DriverInstanceData_t *instanceData, char *ipBuf, uint32_t bufLen);
    ATMO_WIFI_Status_t (*RegisterCallback)(ATMO_DriverInstanceData_t *instanceData, ATMO_WIFI_Event_t event, ATMO_Callback_t cb);
    ATMO_WIFI_Status_t (*RegisterAbility)(ATMO_DriverInstanceData_t *instanceData, ATMO_WIFI_Event_t event, unsigned int abilityHandle);
    ATMO_WIFI_Status_t (*GetStatus)(ATMO_DriverInstanceData_t *instanceData, ATMO_WIFI_ConnectionStatus_t *status);
};

/* Exported Function Prototypes -----------------------------------------------*/

/**
 * This routine will add an instance of a driver for WIFI into the WIFI driver handler.
 *
 */
ATMO_Status_t ATMO_WIFI_AddDriverInstance(const ATMO_WIFI_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber);

/**
 * Initialize WIFI Driver
 *
 * @param[in] instance
 *
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_Init(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine de-initializes the WIFI driver
 *
 * @param[in] instance
 * @return status.
 */
ATMO_WIFI_Status_t ATMO_WIFI_DeInit(ATMO_DriverInstanceHandle_t instance);

/**
 * Set wifi driver enabled/disabled
 *
 * @param[in] instance
 * @param[in] enabled
 *
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_SetEnabled(ATMO_DriverInstanceHandle_t instance, bool enabled);

/**
 * Get wifi driver enabled/disabled status
 *
 * @param[in] instance
 * @param[out] enabled
 *
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_GetEnabled(ATMO_DriverInstanceHandle_t instance, bool *enabled);

/**
 * Set WIFI configuration
 *
 * @param[in] instance
 * @param[in] config
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_SetConfiguration(ATMO_DriverInstanceHandle_t instance, const ATMO_WIFI_Config_t *config);

/**
 * Start WIFI in chosen mode.
 *
 * @param[in] instance
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_Start(ATMO_DriverInstanceHandle_t instance);

/**
 * Stop WIFI in chosen mode.
 *
 * @param[in] instance
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_Stop(ATMO_DriverInstanceHandle_t instance);


/**
 * Get the current IP address. Only applicable if set to STA Mode.
 * 
 * @param[in] instance
 * @param[out] ipBuf - user provided buffer for IP string
 * @param[in] bufLen - length of ipBuf in bytes
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_GetIpAddress(ATMO_DriverInstanceHandle_t instance, char *ipBuf, uint32_t bufLen);

/**
 * Register callback to execute on WIFI event
 * 
 * @param[in] instance
 * @param[in] event
 * @param[in] cb
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_RegisterCallback(ATMO_DriverInstanceHandle_t instance, ATMO_WIFI_Event_t event, ATMO_Callback_t cb);

/**
 * Register ability to execute on WIFI event
 * 
 * @param[in] instance
 * @param[in] event
 * @param[in] abilityHandle
 * @return status
 */
ATMO_WIFI_Status_t ATMO_WIFI_RegisterAbility(ATMO_DriverInstanceHandle_t instance, ATMO_WIFI_Event_t event, unsigned int abilityHandle);

/**
 * @brief Get connectivity status for wifi subdriver
 * 
 * @param instance 
 * @param status 
 * @return ATMO_WIFI_ConnectionStatus_t 
 */
ATMO_WIFI_Status_t ATMO_WIFI_GetStatus(ATMO_DriverInstanceHandle_t instance, ATMO_WIFI_ConnectionStatus_t *status);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_WIFI__H */
