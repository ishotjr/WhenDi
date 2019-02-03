/**
 ******************************************************************************
 * @file    http.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - Core HTTP Driver
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

#include "http.h"

#define NUMBER_OF_HTTP_DRIVER_INSTANCES 1

ATMO_DriverInstanceHandle_t numberOfHTTPDriverInstance = 0;

const ATMO_HTTP_DriverInstance_t *httpInstances[NUMBER_OF_HTTP_DRIVER_INSTANCES];
ATMO_DriverInstanceData_t *httpInstancesData[NUMBER_OF_HTTP_DRIVER_INSTANCES];

ATMO_Status_t ATMO_HTTP_AddDriverInstance(const ATMO_HTTP_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber)
{
	if( !( numberOfHTTPDriverInstance < NUMBER_OF_HTTP_DRIVER_INSTANCES ) )
	{
		return ATMO_Status_OutOfMemory;
	}

	httpInstances[numberOfHTTPDriverInstance] = driverInstance;
	httpInstancesData[numberOfHTTPDriverInstance] = driverInstanceData;
	*instanceNumber = numberOfHTTPDriverInstance;

	numberOfHTTPDriverInstance++;

	return ATMO_Status_Success;
}

ATMO_HTTP_Status_t ATMO_HTTP_Init( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfHTTPDriverInstance ) )
	{
		return ATMO_HTTP_Status_Invalid;
	}

	return httpInstances[instance]->Init( httpInstancesData[instance]);
}

ATMO_HTTP_Status_t ATMO_HTTP_DeInit( ATMO_DriverInstanceHandle_t instance )
{
	if( !( instance < numberOfHTTPDriverInstance ) )
	{
		return ATMO_HTTP_Status_Invalid;
	}

	return httpInstances[instance]->DeInit( httpInstancesData[instance] );
}


ATMO_HTTP_Status_t ATMO_HTTP_SetConfiguration(ATMO_DriverInstanceHandle_t instance, const ATMO_HTTP_Config_t *config)
{
	if( !( instance < numberOfHTTPDriverInstance ) )
	{
		return ATMO_HTTP_Status_Invalid;
	}

	return httpInstances[instance]->SetConfiguration( httpInstancesData[instance], config );
}

ATMO_HTTP_Status_t ATMO_HTTP_Perform(ATMO_DriverInstanceHandle_t instance, ATMO_HTTP_Transaction_t *transaction, ATMO_HTTP_Response_t *response)
{
 	if( !( instance < numberOfHTTPDriverInstance ) )
	{
		return ATMO_HTTP_Status_Invalid;
	}

	return httpInstances[instance]->Perform( httpInstancesData[instance], transaction, response );   
}




