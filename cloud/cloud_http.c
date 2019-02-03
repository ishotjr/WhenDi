/**
 ******************************************************************************
 * @file    cloud_http.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - CLOUD over HTTP Driver
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
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIAHTTP
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "cloud_http.h"
#include "cloud_config.h"
#include "../app_src/atmosphere_platform.h"
#include "../app_src/atmosphere_globals.h"
#include "../app_src/atmosphere_platform.h"
#include "../datetime/datetime.h"
#include <ctype.h>

const ATMO_CLOUD_DriverInstance_t cloudWifiDriverInstance =
{
	ATMO_CLOUD_HTTP_Init,
	ATMO_CLOUD_HTTP_SubDriverSyncRegistration,
	ATMO_CLOUD_HTTP_SendEvent,
	ATMO_CLOUD_HTTP_PopCommand
};

typedef struct
{
	ATMO_DriverInstanceHandle_t httpInstanceNumber;
	ATMO_CLOUD_RegistrationInfo_t currentRegistrationInfo;
} __ATMO_CLOUD_HTTP_DriverInstanceConfig_t;

static __ATMO_CLOUD_HTTP_DriverInstanceConfig_t* currentHttpDriverInstanceConfig = NULL;

static char rfc3986[256] = {0};

static void __ATMO_CLOUD_HTTP_UrlEncodeTableInit()
{

	int i;

	for ( i = 0; i < 256; i++ )
	{

		rfc3986[i] = isalnum( i ) || i == '~' || i == '-' || i == '.' || i == '_' ? i : 0;
	}
}

/**
 *
uuid	"b0d8114a-fbdf-4c0b-a591-8f100c09bba5"
token	"0.85CWCRJ8FQ7"
*/

static char* __ATMO_CLOUD_HTTP_UrlEncode( char* s, char* enc, unsigned int maxLen )
{

	unsigned int currentIndex = 0;

	for ( ; *s; s++ )
	{
		if ( rfc3986[( int )*s] )
		{
			sprintf( enc, "%c", rfc3986[( int )*s] );
		}
		else
		{
			sprintf( enc, "%%%02X", *s );
		}

		while ( *++enc && currentIndex < maxLen )
		{
			currentIndex++;
		}

		if( currentIndex >= maxLen )
		{
			break;
		}
	}

	return( enc );
}


ATMO_Status_t ATMO_CLOUD_HTTP_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber,
        ATMO_DriverInstanceHandle_t httpInstanceNumber )
{

	__ATMO_CLOUD_HTTP_DriverInstanceConfig_t* config = ( __ATMO_CLOUD_HTTP_DriverInstanceConfig_t* )ATMO_Malloc( sizeof(
	            __ATMO_CLOUD_HTTP_DriverInstanceConfig_t ) );

	config->httpInstanceNumber = httpInstanceNumber;
	ATMO_CLOUD_InitRegistrationInfo( &config->currentRegistrationInfo );

	ATMO_DriverInstanceData_t* cloudDriverInstanceData = ( ATMO_DriverInstanceData_t* )ATMO_Malloc( sizeof(
	            ATMO_DriverInstanceData_t ) );

	cloudDriverInstanceData->name = "Cloud HTTP";
	cloudDriverInstanceData->initialized = false;
	cloudDriverInstanceData->instanceNumber = 0;
	cloudDriverInstanceData->argument = ( void* )config;

	return ATMO_CLOUD_AddDriverInstance( &cloudWifiDriverInstance, cloudDriverInstanceData, instanceNumber );
}

ATMO_CLOUD_Status_t ATMO_CLOUD_HTTP_Init( ATMO_DriverInstanceData_t* instance )
{
	__ATMO_CLOUD_HTTP_DriverInstanceConfig_t* config = ( __ATMO_CLOUD_HTTP_DriverInstanceConfig_t* )instance->argument;
	currentHttpDriverInstanceConfig = config;

	instance->initialized = true;

	ATMO_CLOUD_HTTP_SubDriverSyncRegistration( instance );

	__ATMO_CLOUD_HTTP_UrlEncodeTableInit();

	return ATMO_CLOUD_Status_Success;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_HTTP_SendEvent( ATMO_DriverInstanceData_t* instance, const char* eventName, ATMO_Value_t* data,
        uint32_t timeout )
{
	if( !currentHttpDriverInstanceConfig->currentRegistrationInfo.registered )
	{
#ifdef ATMO_HTTP_DEBUG
		ATMO_PLATFORM_DebugPrint( "Not registered\r\n" );
#endif
		return ATMO_CLOUD_Status_Fail;
	}

	// Get data as a string
	ATMO_Value_t dataStr;
	ATMO_InitValue( &dataStr );
	ATMO_CreateValueConverted( &dataStr, ATMO_DATATYPE_STRING, data );

	unsigned int encodedDataLen = strlen( dataStr.data ) * 2;
	char* dataEncoded = ATMO_Malloc( encodedDataLen );
	__ATMO_CLOUD_HTTP_UrlEncode( dataStr.data, dataEncoded, encodedDataLen );
	ATMO_FreeValue( &dataStr );

	// Get UUID str
	char uuid[40];
	ATMO_CLOUD_GetRegistrationInfoUuid( &currentHttpDriverInstanceConfig->currentRegistrationInfo, uuid, sizeof( uuid ) );

	// Build URL
	char* url = ATMO_Malloc( strlen( currentHttpDriverInstanceConfig->currentRegistrationInfo.url ) + strlen( uuid ) +
	                         strlen( eventName ) + strlen( dataEncoded ) + 16 );
	sprintf( url, "%s/thing/%s/event/%s/%s", currentHttpDriverInstanceConfig->currentRegistrationInfo.url, uuid, eventName,
	         dataEncoded );

	ATMO_HTTP_Header_t header;
	header.headerKey = "cloud";
	header.headerValue = currentHttpDriverInstanceConfig->currentRegistrationInfo.token;

#ifdef ATMO_HTTP_DEBUG
	ATMO_PLATFORM_DebugPrint("URL: %s\r\n", url);
	ATMO_PLATFORM_DebugPrint("Token: %s\r\n",  currentHttpDriverInstanceConfig->currentRegistrationInfo.token);
#endif

	ATMO_HTTP_Transaction_t trans;
	trans.url = url;
	trans.method = ATMO_HTTP_GET;
	trans.data = NULL;
	trans.dataLen = 0;
	trans.headerOverlay = &header;
	trans.headerOverlayLen = 1;

	ATMO_HTTP_Response_t resp;

	if( ATMO_HTTP_Perform( currentHttpDriverInstanceConfig->httpInstanceNumber, &trans, &resp ) != ATMO_HTTP_Status_Success )
	{
#ifdef ATMO_HTTP_DEBUG
		ATMO_PLATFORM_DebugPrint("HTTP Error\r\n");
#endif
		return ATMO_CLOUD_Status_Fail;
	}

#ifdef ATMO_HTTP_DEBUG
	ATMO_PLATFORM_DebugPrint("CLOUD EVENT: %d\r\n", resp.code);
#endif
	ATMO_CLOUD_Status_t status = ATMO_CLOUD_Status_Success;

	if( resp.code != 200 )
	{
		status = ATMO_CLOUD_Status_Fail;
	}

	ATMO_Free( resp.data );
	ATMO_Free( dataEncoded );
	ATMO_Free( url );
	return status;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_HTTP_PopCommand( ATMO_DriverInstanceData_t* instance, const char* commandName,
        ATMO_Value_t* data, uint32_t timeout )
{
	if( !currentHttpDriverInstanceConfig->currentRegistrationInfo.registered )
	{
		return ATMO_CLOUD_Status_Fail;
	}

	// Get UUID str
	char uuid[40];
	ATMO_CLOUD_GetRegistrationInfoUuid( &currentHttpDriverInstanceConfig->currentRegistrationInfo, uuid, sizeof( uuid ) );

	char* url = ATMO_Malloc( strlen( currentHttpDriverInstanceConfig->currentRegistrationInfo.url ) + strlen( uuid ) +
	                         strlen( commandName ) + 21 );

	sprintf( url, "%s/thing/%s/command/%s/pop", currentHttpDriverInstanceConfig->currentRegistrationInfo.url, uuid, commandName );

#ifdef ATMO_HTTP_DEBUG
	ATMO_PLATFORM_DebugPrint( "Sending to url: %s\r\n", url );
#endif

	ATMO_HTTP_Header_t header;
	header.headerKey = "cloud";
	header.headerValue = currentHttpDriverInstanceConfig->currentRegistrationInfo.token;

	ATMO_HTTP_Transaction_t trans;
	trans.url = url;
	trans.method = ATMO_HTTP_GET;
	trans.data = NULL;
	trans.dataLen = 0;
	trans.headerOverlay = &header;
	trans.headerOverlayLen = 1;

	ATMO_HTTP_Response_t resp;

	ATMO_CLOUD_Status_t status = ATMO_CLOUD_Status_Success;

	if( ATMO_HTTP_Perform( currentHttpDriverInstanceConfig->httpInstanceNumber, &trans, &resp ) != ATMO_HTTP_Status_Success )
	{
		return ATMO_CLOUD_Status_Fail;
	}

#ifdef ATMO_HTTP_DEBUG
	ATMO_PLATFORM_DebugPrint("Resp Code: %d\r\nResp Data: %s\r\n", resp.code, (char *)resp.data);
#endif

	if( resp.code != 200 )
	{
		status = ATMO_CLOUD_Status_Fail;
	}
	else
	{
		ATMO_InitValue( data );

		// Simple JSON parsing (should incorporate robust json lib at some point)
		char* start = strstr( ( char* )resp.data, "command\":" );

		if( start )
		{
			start += strlen( "command\":" );

			// Trim if string
			if( *start == '\"' )
			{
				start++;
			}

			char* end = ( char* )&resp.data[strlen( ( char* )resp.data ) - 1];

			if( *end == '}' )
			{
				end--;

				if( *end == '\"' )
				{
					end--;
				}

				unsigned int len = ( unsigned int )( end - start + 1 );
				char* cmdDataStr = ATMO_Malloc( len + 1 );
				cmdDataStr[len] = 0;
				memcpy( cmdDataStr, start, len );
				ATMO_CreateValueString( data,  cmdDataStr );
				ATMO_Free( cmdDataStr );
#ifdef ATMO_HTTP_DEBUG
				ATMO_PLATFORM_DebugPrint("Final Data: %s\r\n", data->data);
#endif
			}
			else
			{
				status = ATMO_CLOUD_Status_Fail; // Invalid returned data from cloud
			}
		}
		else
		{
			if( strcmp( ( char* )resp.data, "null" ) == 0 )
			{
				status = ATMO_CLOUD_Status_NoData;
			}
			else
			{
				status = ATMO_CLOUD_Status_Fail; // Invalid returned data from cloud
			}
		}
	}

	ATMO_Free( resp.data );
	ATMO_Free( url );

	return status;
}

ATMO_CLOUD_Status_t ATMO_CLOUD_HTTP_SubDriverSyncRegistration( ATMO_DriverInstanceData_t* instance )
{
	// Load registration or default
	if( ATMO_CLOUD_GetRegistration( &currentHttpDriverInstanceConfig->currentRegistrationInfo ) != ATMO_CLOUD_Status_Success )
	{
		ATMO_CLOUD_InitRegistrationInfo( &currentHttpDriverInstanceConfig->currentRegistrationInfo );
	}

	ATMO_PLATFORM_DebugPrint( "Registered: %d\r\n", currentHttpDriverInstanceConfig->currentRegistrationInfo.registered );

	if( currentHttpDriverInstanceConfig->currentRegistrationInfo.registered )
	{
		ATMO_PLATFORM_DebugPrint( "URL: %s\r\n", currentHttpDriverInstanceConfig->currentRegistrationInfo.url );
		ATMO_PLATFORM_DebugPrint( "Token: %s\r\n", currentHttpDriverInstanceConfig->currentRegistrationInfo.token );
	}

	return ATMO_CLOUD_Status_Success;
}
