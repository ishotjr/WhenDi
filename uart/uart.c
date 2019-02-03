/**
 * 
 ******************************************************************************
 * @file    uart.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - Core UART Driver
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

#include "uart.h"
#include "../app_src/atmosphere_platform.h"

#define NUMBER_OF_UART_DRIVER_INSTANCES 4
ATMO_DriverInstanceHandle_t numberOfUARTDriverInstance = 0;

const ATMO_UART_DriverInstance_t *uartInstances[NUMBER_OF_UART_DRIVER_INSTANCES];
ATMO_DriverInstanceData_t *uartInstancesData[NUMBER_OF_UART_DRIVER_INSTANCES];

ATMO_Status_t ATMO_UART_AddDriverInstance(const ATMO_UART_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber)
{
	if(!(numberOfUARTDriverInstance < NUMBER_OF_UART_DRIVER_INSTANCES)) {
		return ATMO_Status_OutOfMemory;
	}
	
	uartInstances[numberOfUARTDriverInstance] = driverInstance;
	uartInstancesData[numberOfUARTDriverInstance] = driverInstanceData;
	*instanceNumber = numberOfUARTDriverInstance;
	
	numberOfUARTDriverInstance++;
	
	return ATMO_Status_Success;
}

ATMO_UART_Status_t ATMO_UART_Init(ATMO_DriverInstanceHandle_t instance)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->Init(uartInstancesData[instance]);
}

ATMO_UART_Status_t ATMO_UART_DeInit(ATMO_DriverInstanceHandle_t instance)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->DeInit(uartInstancesData[instance]);
}

ATMO_UART_Status_t ATMO_UART_SetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_UART_Peripheral_t *config)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->SetConfiguration(uartInstancesData[instance], config);
}

ATMO_UART_Status_t ATMO_UART_GetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_UART_Peripheral_t *config)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->GetConfiguration(uartInstancesData[instance], config);
}

ATMO_UART_Status_t ATMO_UART_WriteBlocking(ATMO_DriverInstanceHandle_t instance, const char *buffer, uint32_t length, uint32_t *numBytesSent, uint16_t timeoutMs)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->WriteBlocking(uartInstancesData[instance], buffer, length, numBytesSent, timeoutMs);
}

ATMO_UART_Status_t ATMO_UART_ReadBlocking(ATMO_DriverInstanceHandle_t instance, char *buffer, uint32_t length, uint32_t *numBytesReceived, uint16_t timeoutMs)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->ReadBlocking(uartInstancesData[instance], buffer, length, numBytesReceived, timeoutMs);
}

ATMO_UART_Status_t ATMO_UART_FlushTx(ATMO_DriverInstanceHandle_t instance)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->FlushTx(uartInstancesData[instance]);
}

ATMO_UART_Status_t ATMO_UART_FlushRx(ATMO_DriverInstanceHandle_t instance)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->FlushRx(uartInstancesData[instance]);
}

ATMO_UART_Status_t ATMO_UART_RegisterRxAbilityHandle(ATMO_DriverInstanceHandle_t instance, unsigned int abilityHandle)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->RegisterRxAbilityHandle(uartInstancesData[instance], abilityHandle);
}

ATMO_UART_Status_t ATMO_UART_RegisterRxCbFunc(ATMO_DriverInstanceHandle_t instance, ATMO_Callback_t cbFunc)
{
    if( instance >= NUMBER_OF_UART_DRIVER_INSTANCES )
    {
        return ATMO_UART_Status_Invalid;
    }

    return uartInstances[instance]->RegisterRxCbFunc(uartInstancesData[instance], cbFunc);
}

static inline bool ATMO_UART_IsWhiteSpace(char c)
{
	return(c == ' ' || c == '\r' || c == '\n');
}

ATMO_UART_Status_t ATMO_UART_ProcessRxData(ATMO_UART_InternalConfig_t *config, uint8_t *rxData, unsigned int rxDataLen, uint8_t *buffer, unsigned int bufferLen, unsigned int *currentBufferFill)
{
    bool sendBuff = false;
    bool noRoomForData = false;

    // Must be string data with rx buffer enabled
    if(!config->config.isBinaryData && config->config.rxBuffer)
    {
        // Is there room for the current data buffer?
        if((*currentBufferFill) + rxDataLen > (bufferLen - 2))
        {
            noRoomForData = true;
        }
        else
        {
            // There's room, so copy rx data into buffer and NULL terminate
            memcpy(&buffer[(*currentBufferFill)], rxData, rxDataLen);
            *currentBufferFill += rxDataLen;
            buffer[(*currentBufferFill)] = 0;
        }

        // Does string match regex?
        if(re_match((char *)config->config.splitRegex, (char *)buffer) != -1)
        {
            sendBuff = true;
        }
        
        // If there was no room for the data
        // and the data doesn't match the regex (assume garbage data)
        if( noRoomForData && !sendBuff && (rxDataLen < (bufferLen - 1)) )
        {
            // Wipe buffer and start at beginning
            *currentBufferFill = 0;
            memcpy( &buffer[*currentBufferFill], rxData, rxDataLen );
            *currentBufferFill += rxDataLen;
            buffer[*currentBufferFill] = 0;

            // Does string match regex?
            if(re_match((char *)config->config.splitRegex, (char *)buffer) != -1)
            {
                sendBuff = true;
            }
        }
    }

    if(sendBuff || (config->config.isBinaryData) || (!config->config.rxBuffer))
    {
        // Trim the end of the buffer
        if(config->config.rxTrim && (!config->config.isBinaryData) && (config->config.rxBuffer))
        {
            char *end = (char *)&buffer[(*currentBufferFill) - 1];

            while((end != buffer) && (ATMO_UART_IsWhiteSpace(*end)))
            {
                *end = 0;
                (*currentBufferFill)--;
                end--;
            }
        }

        // Null terminate buffer
        if(!config->config.isBinaryData && config->config.rxBuffer)
        {
            buffer[(*currentBufferFill)] = 0;
        }
        if(config->cb != NULL)
        {
            ATMO_Value_t byteVal;
            ATMO_InitValue(&byteVal);
            if(!config->config.isBinaryData && config->config.rxBuffer)
            {
                ATMO_CreateValueString(&byteVal, (char *)buffer);
            }
            else
            {
                ATMO_CreateValueBinary(&byteVal, rxData, rxDataLen);
            }
            ATMO_Lock();
            ATMO_AddCallbackExecute( config->cb, &byteVal );
            ATMO_Unlock();
        }
        if(config->abilityHandleRegistered)
        {
            ATMO_Value_t byteVal;
            ATMO_InitValue(&byteVal);
            if(!config->config.isBinaryData && config->config.rxBuffer)
            {
                ATMO_CreateValueString(&byteVal, (char *)buffer);
            }
            else
            {
                ATMO_CreateValueBinary(&byteVal, rxData, rxDataLen);
            }

            ATMO_Lock();
            ATMO_AddAbilityExecute( config->abilityHandle, &byteVal );
            ATMO_Unlock();
        }

        *currentBufferFill = 0;

    }

    return ATMO_UART_Status_Success;
}