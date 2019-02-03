#include "fxos8700.h"
#include "fxos8700_internal.h"

typedef struct {
    ATMO_FXOS8700_Config_t config;

    unsigned int motionDetectedAbilityHandle;
    ATMO_Callback_t motionDetectedCallback;
    bool motionDetectedAbilityHandleRegistered;

	unsigned int freefallDetectedAbilityHandle;
    ATMO_Callback_t freefallDetectedCallback;
    bool freefallDetectedAbilityHandleRegistered;

    unsigned int tapDetectedAbilityHandle;
    ATMO_Callback_t tapDetectedCallback;
    bool tapDetectedAbilityHandleRegistered;

    unsigned int doubleTapDetectedAbilityHandle;
    ATMO_Callback_t doubleTapDetectedCallback;
    bool doubleTapDetectedAbilityHandleRegistered;

    bool configured;
} ATMO_FXOS8700_Priv_Config;

static bool fxos8700Enabled = true;

static ATMO_FXOS8700_Priv_Config _ATMO_FXOS8700_config = {0};

static ATMO_I2C_Peripheral_t _ATMO_FXOS8700_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static fxos8700_handle_t handle;

static uint8_t ATMO_FXOS8700_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_FXOS8700_config.config.i2cDriverInstance, _ATMO_FXOS8700_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static uint8_t ATMO_FXOS8700_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_FXOS8700_config.config.i2cDriverInstance, _ATMO_FXOS8700_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static void ATMO_FXOS8700_INT_DoAbilityOrCallback(bool abilityRegistered, unsigned int abilityHandle, ATMO_Callback_t cb, void *data, uint32_t dataLen)
{
	if(!fxos8700Enabled)
	{
		return;
	}

	ATMO_Value_t value;
	ATMO_InitValue(&value);
	if(data != NULL)
	{
		ATMO_CreateValueBinary(&value, data, dataLen);
	}

	if(abilityRegistered)
	{
		ATMO_AddAbilityExecute(abilityHandle, &value);
	}

	if(cb != NULL)
	{
		ATMO_AddCallbackExecute(cb, &value);
	}

	ATMO_FreeValue(&value);
}

static void ATMO_FXOS8700_INT_Callback(void *data)
{
	// Read int src
	uint8_t src;
	if( FXOS8700_ReadReg(&handle, FXOS8700_INT_SOURCE_REG, &src, 1) == FXOS8700_Status_Success )
	{
		uint8_t tmp;
		if(src & FXOS8700_SRC_DRDY_MASK)
		{
			// Clear it by reading accelerometer data
			fxos8700_data_t sensorData;
			FXOS8700_ReadSensorData(&handle, &sensorData);
			//ATMO_FXOS8700_INT_DoAbilityOrCallback(_src, &sensorData, sizeof(sensorData));
		}

		if(src & FXOS8700_SRC_FF_MT_MASK)
		{
			// Clear it by reading ffmt register
			FXOS8700_ReadReg(&handle, FXOS8700_FF_MT_SRC_REG, &tmp, 1);
			ATMO_FXOS8700_INT_DoAbilityOrCallback(_ATMO_FXOS8700_config.freefallDetectedAbilityHandleRegistered,
					_ATMO_FXOS8700_config.freefallDetectedAbilityHandle, _ATMO_FXOS8700_config.freefallDetectedCallback, NULL, 0);
		}

		if(src & FXOS8700_SRC_TRANS_MASK)
		{
			// Clear it by reading trans register
			FXOS8700_ReadReg(&handle, FXOS8700_TRANSIENT_SRC_REG, &tmp, 1);
			ATMO_FXOS8700_INT_DoAbilityOrCallback(_ATMO_FXOS8700_config.motionDetectedAbilityHandleRegistered,
			_ATMO_FXOS8700_config.motionDetectedAbilityHandle, _ATMO_FXOS8700_config.motionDetectedCallback, NULL, 0);	
		}

		if(src & FXOS8700_SRC_PULSE_MASK)
		{
			// Clear it by reading PULSE_SRC reg
			bool tapDetected, doubleTap = false;
			ATMO_FXOS8700_IsTapDetected(&tapDetected, &doubleTap);

			ATMO_FXOS8700_INT_DoAbilityOrCallback(_ATMO_FXOS8700_config.tapDetectedAbilityHandleRegistered,
					_ATMO_FXOS8700_config.tapDetectedAbilityHandle, _ATMO_FXOS8700_config.tapDetectedCallback, NULL, 0);

			if(doubleTap)
			{
				ATMO_FXOS8700_INT_DoAbilityOrCallback(_ATMO_FXOS8700_config.doubleTapDetectedAbilityHandleRegistered,
						_ATMO_FXOS8700_config.doubleTapDetectedAbilityHandle, _ATMO_FXOS8700_config.doubleTapDetectedCallback, NULL, 0);
			}
		}
	}
}

static ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableInterrupts()
{
	// Cannot enable both motion and freefall detection
	if(_ATMO_FXOS8700_config.config.freefallEnabled && _ATMO_FXOS8700_config.config.motionEnabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	fxos8700_status_t status = FXOS8700_Status_Success;

	if(_ATMO_FXOS8700_config.config.freefallEnabled)
	{
		status = FXOS8700_FreefallDetect_Init(&handle);
	}
	else
	{
		status = FXOS8700_MotionDetectFreefall_DeInit(&handle);
	}

	if(_ATMO_FXOS8700_config.config.motionEnabled && status == FXOS8700_Status_Success)
	{
		status = FXOS8700_TransientDetect_Init(&handle);
	}
	else
	{
		status = FXOS8700_TransientDetect_DeInit(&handle);
	}

	if(_ATMO_FXOS8700_config.config.tapDetectionEnabled && status == FXOS8700_Status_Success)
	{
		status = FXOS8700_TapDetect_Init(&handle);
	}
	else
	{
		status = FXOS8700_TapDetect_DeInit(&handle);
	}

	return status == FXOS8700_Status_Success ? ATMO_FXOS8700_Status_Success : ATMO_FXOS8700_Status_Fail;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_Init(ATMO_FXOS8700_Config_t *config)
{
	handle.I2C_Read = ATMO_FXOS8700_I2CRead_Simple;
	handle.I2C_Write = ATMO_FXOS8700_I2CWrite_Simple;
	handle.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;

    // Did the user supply a configuration?
    if( config )
    {
        ATMO_FXOS8700_SetConfiguration(config);
    }
    else
    {
    	_ATMO_FXOS8700_config.configured = false;
    }

	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetEnabled(bool enabled)
{
	fxos8700Enabled = enabled;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetEnabled(bool *enabled)
{
	*enabled = fxos8700Enabled;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetConfiguration(const ATMO_FXOS8700_Config_t *config)
{
    if( config == NULL || !fxos8700Enabled)
    {
        return ATMO_FXOS8700_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_FXOS8700_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_FXOS8700_Status_Fail;
    }

    memcpy( &_ATMO_FXOS8700_config.config, config, sizeof(ATMO_FXOS8700_Config_t) );
    _ATMO_FXOS8700_config.configured = true;

	fxos8700_status_t status = FXOS8700_Init(&handle);

	if(status != FXOS8700_Status_Success)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

    // Register interrupt
    if(config->int1En)
    {
    	ATMO_GPIO_RegisterInterruptCallback(config->gpioDriverInstance, config->int1Pin, ATMO_GPIO_InterruptTrigger_FallingEdge, ATMO_FXOS8700_INT_Callback);
    }
    if(config->int2En)
    {
    	ATMO_GPIO_RegisterInterruptCallback(config->gpioDriverInstance, config->int2Pin, ATMO_GPIO_InterruptTrigger_FallingEdge, ATMO_FXOS8700_INT_Callback);
    }


    return ATMO_FXOS8700_EnableInterrupts();
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetConfiguration(ATMO_FXOS8700_Config_t *config)
{
    if( config == NULL || !_ATMO_FXOS8700_config.configured )
    {
        return ATMO_FXOS8700_Status_Fail;
    }

    memcpy(config, &_ATMO_FXOS8700_config.config, sizeof(ATMO_FXOS8700_Config_t));

    return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetAccelData(ATMO_3dFloatVector_t *data)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	fxos8700_data_t rawData;
	if(FXOS8700_ReadSensorData(&handle, &rawData) != FXOS8700_Status_Success)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	float dataScale = 0;
	uint8_t sensorRange = 0;

	    /* Get sensor range */
    if (FXOS8700_ReadReg(&handle, FXOS8700_XYZ_DATA_CFG_REG, &sensorRange, 1) != FXOS8700_Status_Success)
    {
        return ATMO_FXOS8700_Status_Fail;
    }
    if(sensorRange == 0x00)
    {
    	/* 0.244 mg/LSB */
        dataScale = 0.000244;
    }
    else if(sensorRange == 0x01)
    {
    	/* 0.488 mg/LSB */
        dataScale = 0.000488;
    }
    else if(sensorRange == 0x10)
    {
    	/* 0.976 mg/LSB */
        dataScale = 0.000976;
    }

	data->x =  ((float)((int16_t)(((rawData.accelXMSB*256) + (rawData.accelXLSB)))>> 2));
	data->y =  ((float)((int16_t)(((rawData.accelYMSB*256) + (rawData.accelYLSB)))>> 2));
	data->z =  ((float)((int16_t)(((rawData.accelZMSB*256) + (rawData.accelZLSB)))>> 2));
	data->x *= dataScale;
	data->y *= dataScale;
	data->z *= dataScale;

	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetMagData(ATMO_3dFloatVector_t *data)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	fxos8700_data_t rawData;
	if(FXOS8700_ReadSensorData(&handle, &rawData) != FXOS8700_Status_Success)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	float dataScale = 0;
	uint8_t sensorRange = 0;

	    /* Get sensor range */
    if (FXOS8700_ReadReg(&handle, FXOS8700_XYZ_DATA_CFG_REG, &sensorRange, 1) != FXOS8700_Status_Success)
    {
        return ATMO_FXOS8700_Status_Fail;
    }

	data->x =  (float)((int16_t)((rawData.magXMSB*256) + (rawData.magXLSB))) * 0.1;
	data->y =  (float)((int16_t)((rawData.magYMSB*256) + (rawData.magYLSB))) * 0.1;
	data->z =  (float)((int16_t)((rawData.magZMSB*256) + (rawData.magZLSB))) * 0.1;

	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetFreefallDetectedAbilityHandle(unsigned int abilityHandle)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	_ATMO_FXOS8700_config.freefallDetectedAbilityHandle = abilityHandle;
	_ATMO_FXOS8700_config.freefallDetectedAbilityHandleRegistered = true;
	return ATMO_FXOS8700_Status_Success;

}
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetFreefallDetectedCallback(ATMO_Callback_t cb)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.freefallDetectedCallback = cb;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetMotionDetectedAbilityHandle(unsigned int abilityHandle)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}

	_ATMO_FXOS8700_config.motionDetectedAbilityHandle = abilityHandle;
	_ATMO_FXOS8700_config.motionDetectedAbilityHandleRegistered = true;
	return ATMO_FXOS8700_Status_Success;

}
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetMotionDetectedCallback(ATMO_Callback_t cb)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.motionDetectedCallback = cb;
	return ATMO_FXOS8700_Status_Success;
}

/**
 * Tap detection
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_IsTapDetected(bool *isTapDetected, bool *isDoubleTap)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
    uint8_t tmp[1];
    *isDoubleTap = false;

    if(FXOS8700_ReadReg(&handle, FXOS8700_PULSE_SRC_REG, tmp, 1) != FXOS8700_Status_Success)
    {
        return ATMO_FXOS8700_Status_Fail;
    }


    if ((tmp[0] & FXOS8700_PEA_MASK) == FXOS8700_PEA_MASK)
    {
        /* Check to see if it was a double tap */
        if ((tmp[0] & FXOS8700_DPE_MASK) == FXOS8700_DPE_MASK)
        {
            *isDoubleTap = true;
        }

        *isTapDetected = true;
    }
    else
    {
    	*isTapDetected = false;
    }

    return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetTapDetectedAbilityHandle(unsigned int abilityHandle)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.tapDetectedAbilityHandle = abilityHandle;
	_ATMO_FXOS8700_config.tapDetectedAbilityHandleRegistered = true;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetTapDetectedCallback(ATMO_Callback_t cb)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.tapDetectedCallback = cb;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetDoubleTapDetectedAbilityHandle(unsigned int abilityHandle)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.doubleTapDetectedAbilityHandle = abilityHandle;
	_ATMO_FXOS8700_config.doubleTapDetectedAbilityHandleRegistered = true;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetDoubleTapDetectedCallback(ATMO_Callback_t cb)
{
	if(!fxos8700Enabled)
	{
		return ATMO_FXOS8700_Status_Fail;
	}
	_ATMO_FXOS8700_config.doubleTapDetectedCallback = cb;
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableFreefallDetection()
{
	_ATMO_FXOS8700_config.config.freefallEnabled = true;
	_ATMO_FXOS8700_config.config.motionEnabled = false;
	_ATMO_FXOS8700_config.config.tapDetectionEnabled = false;
	ATMO_FXOS8700_EnableInterrupts();
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableMotionDetection()
{
	_ATMO_FXOS8700_config.config.freefallEnabled = false;
	_ATMO_FXOS8700_config.config.motionEnabled = true;
	_ATMO_FXOS8700_config.config.tapDetectionEnabled = false;
	ATMO_FXOS8700_EnableInterrupts();
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableTapDetection()
{
	_ATMO_FXOS8700_config.config.freefallEnabled = false;
	_ATMO_FXOS8700_config.config.motionEnabled = false;
	_ATMO_FXOS8700_config.config.tapDetectionEnabled = true;
	ATMO_FXOS8700_EnableInterrupts();
	return ATMO_FXOS8700_Status_Success;
}

ATMO_FXOS8700_Status_t ATMO_FXOS8700_DisableAllDetection()
{
	_ATMO_FXOS8700_config.config.freefallEnabled = false;
	_ATMO_FXOS8700_config.config.motionEnabled = false;
	_ATMO_FXOS8700_config.config.tapDetectionEnabled = false;
	ATMO_FXOS8700_EnableInterrupts();
	return ATMO_FXOS8700_Status_Success;
}


