/**
 ******************************************************************************
 * @file    i2c.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - Core I2C Driver
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

#include "i2c.h"

#define NUMBER_OF_I2C_DRIVER_INSTANCES 4

ATMO_DriverInstanceHandle_t numberOfI2CDriverInstance = 0;

const ATMO_I2C_DriverInstance_t *i2cInstances[NUMBER_OF_I2C_DRIVER_INSTANCES];
ATMO_DriverInstanceData_t *i2cInstancesData[NUMBER_OF_I2C_DRIVER_INSTANCES];

ATMO_Status_t ATMO_I2C_AddDriverInstance(const ATMO_I2C_DriverInstance_t *driverInstance, ATMO_DriverInstanceData_t *driverInstanceData, ATMO_DriverInstanceHandle_t *instanceNumber)
{
	if(!(numberOfI2CDriverInstance < NUMBER_OF_I2C_DRIVER_INSTANCES)) {
		return ATMO_Status_OutOfMemory;
	}
	
	i2cInstances[numberOfI2CDriverInstance] = driverInstance;
	i2cInstancesData[numberOfI2CDriverInstance] = driverInstanceData;
	*instanceNumber = numberOfI2CDriverInstance;
	
	numberOfI2CDriverInstance++;
	
	return ATMO_Status_Success;
}

ATMO_I2C_Status_t ATMO_I2C_SetDeviceHandle(ATMO_DriverInstanceHandle_t handle, uint16_t address, ATMO_I2C_DeviceHandle_t *deviceHandle)
{
	if(deviceHandle == NULL) {
		return ATMO_I2C_Status_Fail;
	}
	
	deviceHandle->handle = handle;
	deviceHandle->address = address;
	deviceHandle->type = ATMO_DRIVERTYPE_I2C;
	
	return ATMO_I2C_Status_Success;
}

ATMO_I2C_Status_t ATMO_I2C_Init(ATMO_DriverInstanceHandle_t instance)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->Init(i2cInstancesData[instance]);
}

ATMO_I2C_Status_t ATMO_I2C_DeInit(ATMO_DriverInstanceHandle_t instance)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->DeInit(i2cInstancesData[instance]);
}

ATMO_I2C_Status_t ATMO_I2C_SetConfiguration(ATMO_DriverInstanceHandle_t instance, const ATMO_I2C_Peripheral_t *config)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->SetConfiguration(i2cInstancesData[instance], config);
}

ATMO_I2C_Status_t ATMO_I2C_GetConfiguration(ATMO_DriverInstanceHandle_t instance, ATMO_I2C_Peripheral_t *config)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->GetConfiguration(i2cInstancesData[instance], config);
}

ATMO_I2C_Status_t ATMO_I2C_MasterRegisterCallback(ATMO_DriverInstanceHandle_t instance, ATMO_I2C_Master_Callback_t cbFunc)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterRegisterCallback(i2cInstancesData[instance], cbFunc);
}

ATMO_I2C_Status_t ATMO_I2C_SlaveRegisterCallback(ATMO_DriverInstanceHandle_t instance, ATMO_I2C_Slave_Callback_t cbFunc)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->SlaveRegisterCallback(i2cInstancesData[instance], cbFunc);
}

ATMO_I2C_Status_t ATMO_I2C_MasterWrite(ATMO_DriverInstanceHandle_t instance, uint16_t slaveAddress, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes, uint16_t timeout_ms)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterWrite(i2cInstancesData[instance], slaveAddress, cmdBytes, numCmdBytes, writeBytes, numWriteBytes, timeout_ms);
}

ATMO_I2C_Status_t ATMO_I2C_MasterWriteNonBlocking(ATMO_DriverInstanceHandle_t instance, uint16_t slaveAddress, const uint8_t* cmdBytes, uint16_t numCmdBytes, const uint8_t* writeBytes, uint16_t numWriteBytes)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterWriteNonBlocking(i2cInstancesData[instance], slaveAddress, cmdBytes, numCmdBytes, writeBytes, numWriteBytes);
}

ATMO_I2C_Status_t ATMO_I2C_MasterRead(ATMO_DriverInstanceHandle_t instance, uint16_t slaveAddress, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes, uint16_t timeout_ms)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterRead(i2cInstancesData[instance], slaveAddress, cmdBytes, numCmdBytes, readBytes, numReadBytes, timeout_ms);
}


ATMO_I2C_Status_t ATMO_I2C_MasterReadNonBlocking(ATMO_DriverInstanceHandle_t instance, uint16_t slaveAddress, const uint8_t *cmdBytes, uint16_t numCmdBytes, uint8_t *readBytes, uint16_t numReadBytes)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterReadNonBlocking(i2cInstancesData[instance], slaveAddress, cmdBytes, numCmdBytes, readBytes, numReadBytes);
}

ATMO_I2C_Status_t ATMO_I2C_MasterGetStatus(ATMO_DriverInstanceHandle_t instance, uint16_t *bytesRemaining)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->MasterGetStatus(i2cInstancesData[instance], bytesRemaining);
}

ATMO_I2C_Status_t ATMO_I2C_SlaveWaitForReceive(ATMO_DriverInstanceHandle_t instance)
{
	if(!(instance < numberOfI2CDriverInstance)) {
		return ATMO_I2C_Status_Invalid;
	}
	
	return i2cInstances[instance]->SlaveWaitForReceive(i2cInstancesData[instance]);
}
