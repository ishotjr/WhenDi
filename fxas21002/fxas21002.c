#include "fxas21002.h"

typedef struct {
    ATMO_FXAS21002_Config_t config;
    bool configured;
} ATMO_FXAS21002_Priv_Config;

static ATMO_FXAS21002_Priv_Config _ATMO_FXAS21002_config = {0};

static bool _ATMO_FXAS21002_Enabled = true;

static ATMO_I2C_Status_t ATMO_FXAS21002_ReadReg(uint8_t regId, uint8_t *data, uint8_t len)
{
	return ATMO_I2C_MasterRead(_ATMO_FXAS21002_config.config.i2cDriverInstance, _ATMO_FXAS21002_config.config.address, &regId, 1, data, len, 1000);
}

static ATMO_I2C_Status_t ATMO_FXAS21002_WriteReg(uint8_t regId, uint8_t data, uint8_t mask)
{
	uint8_t writeByte = data;

	if(mask)
	{
		if(ATMO_FXAS21002_ReadReg(regId, &writeByte, 1) != ATMO_I2C_Status_Success)
		{
			return ATMO_I2C_Status_Fail;
		}

		// Zero out mask
		writeByte = (writeByte & (~mask)) | data;
	}

	return ATMO_I2C_MasterWrite(_ATMO_FXAS21002_config.config.i2cDriverInstance, _ATMO_FXAS21002_config.config.address, &regId, 1, &writeByte, 1, 1000);
}


ATMO_FXAS21002_Status_t ATMO_FXAS21002_Init(ATMO_FXAS21002_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_FXAS21002_SetConfiguration(config);
    }
    else
    {
    	_ATMO_FXAS21002_config.configured = false;
    }

	return ATMO_FXAS21002_Status_Success;
}

ATMO_FXAS21002_Status_t ATMO_FXAS21002_SetEnabled(bool enabled)
{
	_ATMO_FXAS21002_Enabled = enabled;
	return ATMO_FXAS21002_Status_Success;
}

ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetEnabled(bool *enabled)
{
	*enabled = _ATMO_FXAS21002_Enabled;
	return ATMO_FXAS21002_Status_Success;
}

ATMO_FXAS21002_Status_t ATMO_FXAS21002_SetConfiguration(const ATMO_FXAS21002_Config_t *config)
{
    if( config == NULL || !_ATMO_FXAS21002_Enabled)
    {
        return ATMO_FXAS21002_Status_Fail;
    }

    memcpy( &_ATMO_FXAS21002_config.config, config, sizeof(ATMO_FXAS21002_Config_t) );
    _ATMO_FXAS21002_config.configured = true;

    // Check the whoami register
    uint8_t whoAmI = 0;

    if(ATMO_FXAS21002_ReadReg(FXAS21002_WHO_AM_I, &whoAmI, 1) != ATMO_I2C_Status_Success || whoAmI != FXAS21002_WHO_AM_I_WHOAMI_PROD_VALUE)
    {
    	return ATMO_FXAS21002_Status_Fail;
    }

    // Put into standby mode
    if(ATMO_FXAS21002_WriteReg(FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_MODE_STANDBY, FXAS21002_CTRL_REG1_MODE_MASK) != ATMO_I2C_Status_Success)
    {
    	return ATMO_FXAS21002_Status_Fail;
    }


    // Set to 12.5hz sampling rate
    if(ATMO_FXAS21002_WriteReg(FXAS21002_CTRL_REG1, _ATMO_FXAS21002_config.config.samplingRate, FXAS21002_CTRL_REG1_DR_MASK) != ATMO_I2C_Status_Success)
    {
    	return ATMO_FXAS21002_Status_Fail;
    }

    // Put into active mode
    if(ATMO_FXAS21002_WriteReg(FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_MODE_ACTIVE, FXAS21002_CTRL_REG1_MODE_MASK) != ATMO_I2C_Status_Success)
    {
    	return ATMO_FXAS21002_Status_Fail;
    }


    return ATMO_FXAS21002_Status_Success;
}

ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetConfiguration(ATMO_FXAS21002_Config_t *config)
{
    if( config == NULL || !_ATMO_FXAS21002_config.configured )
    {
        return ATMO_FXAS21002_Status_Fail;
    }

    memcpy(config, &_ATMO_FXAS21002_config.config, sizeof(ATMO_FXAS21002_Config_t));

    return ATMO_FXAS21002_Status_Success;
}

ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetSensorData(ATMO_FXAS21002_SensorData_t *data)
{
    if( !_ATMO_FXAS21002_config.configured || !_ATMO_FXAS21002_Enabled)
    {
        return ATMO_FXAS21002_Status_Fail;
    }


	uint8_t rawData[6] = {0};

	if(ATMO_FXAS21002_ReadReg(FXAS21002_OUT_X_MSB, rawData, 6) != ATMO_I2C_Status_Success)
	{
		return ATMO_FXAS21002_Status_Fail;
	}

	// Get scale for conversion
	uint8_t fs = 0;
	if(ATMO_FXAS21002_ReadReg(FXAS21002_CTRL_REG0, &fs, 1) != ATMO_I2C_Status_Success)
	{
		return ATMO_FXAS21002_Status_Fail;
	}

	fs &= FXAS21002_CTRL_REG0_FS_MASK;

	// Multiplier for dps/LSB
	float multiplier = 0.0;

	switch(fs)
	{
		case FXAS21002_CTRL_REG0_FS_DPS2000:
		{
			multiplier = .0625;
			break;
		}
		case FXAS21002_CTRL_REG0_FS_DPS1000:
		{
			multiplier = .03125;
			break;
		}
		case FXAS21002_CTRL_REG0_FS_DPS500:
		{
			multiplier = .015625;
			break;
		}
		case FXAS21002_CTRL_REG0_FS_DPS250:
		{
			multiplier = .0078125;
			break;
		}
	}

	int16_t dataCounts[3] = {0};
	dataCounts[0] = ((int16_t)rawData[0] << 8) | rawData[1];
	dataCounts[1] = ((int16_t)rawData[2] << 8) | rawData[3];
	dataCounts[2] = ((int16_t)rawData[4] << 8) | rawData[5];

	data->X = multiplier * dataCounts[0];
	data->Y = multiplier * dataCounts[1];
	data->Z = multiplier * dataCounts[2];

	return ATMO_FXAS21002_Status_Success;
}
