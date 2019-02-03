/**
 ******************************************************************************
 * @file    http.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - HTTP header file
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
#ifndef __ATMO_HTTP__H
#define __ATMO_HTTP__H


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
    ATMO_HTTP_Status_Success       = 0x00u,  // Operation was successful
    ATMO_HTTP_Status_Fail          = 0x01u,  // Operation failed
    ATMO_HTTP_Status_Initialized   = 0x02u,  // Peripheral already initialized
    ATMO_HTTP_Status_Invalid       = 0x03u,  // Invalid operation
    ATMO_HTTP_Status_NotSupported  = 0x04u,  // Feature not supported by platform
    ATMO_HTTP_Status_Unspecified   = 0x05u,  // Some other status not defined
} ATMO_HTTP_Status_t;

typedef enum
{
    ATMO_HTTP_GET,
    ATMO_HTTP_POST,
} ATMO_HTTP_Method_t;

/* Exported Structures -------------------------------------------------------*/

/**
 * Single HTTP Header
 */
typedef struct
{
    const char* headerKey;
    const char* headerValue;
} ATMO_HTTP_Header_t;

/**
 * Generic HTTP Transaction structure
 */
typedef struct
{
    const char* url; /**< URL */
    ATMO_HTTP_Method_t method; /**< Get/Post */
    const char *data; /**< Data for POST. Can be NULL. */
    unsigned int dataLen; /**< Length of POST data. */
    ATMO_HTTP_Header_t *headerOverlay; /**< Array of headers to overlay. */
    unsigned int headerOverlayLen; /**< Length of array. */
} ATMO_HTTP_Transaction_t;

/**
 * Generic HTTP Response
 */
typedef struct
{
    unsigned int code; /**< HTTP Response Code */
    uint8_t *data; /**< Response data. User is responsible for freeing this. */
    unsigned int dataLen; /**< Length of response data in bytes */
} ATMO_HTTP_Response_t;

typedef struct
{
    
} ATMO_HTTP_Config_t;

// Some C gore so we can use the struct within itself
typedef struct ATMO_HTTP_DriverInstance_t ATMO_HTTP_DriverInstance_t;

struct ATMO_HTTP_DriverInstance_t{
	ATMO_HTTP_Status_t (*Init)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_HTTP_Status_t (*DeInit)(ATMO_DriverInstanceData_t *instanceData);
	ATMO_HTTP_Status_t (*SetConfiguration)(ATMO_DriverInstanceData_t *instanceData, const ATMO_HTTP_Config_t *config);
    ATMO_HTTP_Status_t (*Perform)(ATMO_DriverInstanceData_t *instanceData, ATMO_HTTP_Transaction_t *transaction, ATMO_HTTP_Response_t *response);
};

/* Exported Function Prototypes -----------------------------------------------*/

/**
 * This routine will add an instance of a driver for HTTP into the HTTP driver handler.
 *
 */
ATMO_Status_t ATMO_HTTP_AddDriverInstance(const ATMO_HTTP_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber);

/**
 * Initialize HTTP Driver
 *
 * @param[in] instance
 *
 * @return status
 */
ATMO_HTTP_Status_t ATMO_HTTP_Init(ATMO_DriverInstanceHandle_t instance);

/**
 * This routine de-initializes the HTTP driver
 *
 * @param[in] instance
 * @return status.
 */
ATMO_HTTP_Status_t ATMO_HTTP_DeInit(ATMO_DriverInstanceHandle_t instance);

/**
 * Set HTTP configuration
 *
 * @param[in] instance
 * @param[in] config
 * @return status
 */
ATMO_HTTP_Status_t ATMO_HTTP_SetConfiguration(ATMO_DriverInstanceHandle_t instance, const ATMO_HTTP_Config_t *config);

/**
 * Perform HTTP Get/Post transaction
 *
 * @param[in] instance
 * @param[in] transaction
 * @param[in/out] response
 * @return status
 */
ATMO_HTTP_Status_t ATMO_HTTP_Perform(ATMO_DriverInstanceHandle_t instance, ATMO_HTTP_Transaction_t *transaction, ATMO_HTTP_Response_t *response);


#ifdef __cplusplus
}
#endif

#endif /* __ATMO_HTTP__H */
