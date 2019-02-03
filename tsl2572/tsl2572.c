#include "tsl2572.h"
#include "tsl2572_internal.h"
#include "../app_src/atmosphere_platform.h"

typedef struct {
    ATMO_TSL2572_Config_t config;
    bool configured;
} ATMO_TSL2572_Priv_Config;

static ATMO_TSL2572_Priv_Config _ATMO_TSL2572_config;

static ATMO_I2C_Peripheral_t _ATMO_TSL2572_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static bool tsl2572Enabled = true;

static uint8_t _ATMO_TSL2572_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_TSL2572_config.config.i2cDriverInstance, _ATMO_TSL2572_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static uint8_t _ATMO_TSL2572_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_TSL2572_config.config.i2cDriverInstance, _ATMO_TSL2572_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}
ATMO_TSL2572_Status_t ATMO_TSL2572_Init(ATMO_TSL2572_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_TSL2572_SetConfiguration(config);
    }
    else
    {
    	_ATMO_TSL2572_config.configured = false;
    }

    tsl2572_IoFunc_t io;
    io.I2C_Read = _ATMO_TSL2572_I2CRead_Simple;
    io.I2C_Write = _ATMO_TSL2572_I2CWrite_Simple;
    io.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
    TSL2572_Init_Driver(&io);
    TSL2572_Init_HW();

    return ATMO_TSL2572_Status_Success;
}

ATMO_TSL2572_Status_t ATMO_TSL2572_SetEnabled(bool enabled)
{
    tsl2572Enabled = enabled;
    return ATMO_TSL2572_Status_Success;
}

ATMO_TSL2572_Status_t ATMO_TSL2572_GetEnabled(bool *enabled)
{
    *enabled = tsl2572Enabled;
    return ATMO_TSL2572_Status_Success;
}

ATMO_TSL2572_Status_t ATMO_TSL2572_SetConfiguration(const ATMO_TSL2572_Config_t *config)
{
    if( config == NULL || !tsl2572Enabled)
    {
        return ATMO_TSL2572_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_TSL2572_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_TSL2572_Status_Fail;
    }

    memcpy( &_ATMO_TSL2572_config.config, config, sizeof(ATMO_TSL2572_Config_t) );
    _ATMO_TSL2572_config.configured = true;

    return ATMO_TSL2572_Status_Success;
}

ATMO_TSL2572_Status_t ATMO_TSL2572_GetConfiguration(ATMO_TSL2572_Config_t *config)
{
    if( config == NULL || !_ATMO_TSL2572_config.configured )
    {
        return ATMO_TSL2572_Status_Fail;
    }

    memcpy(config, &_ATMO_TSL2572_config.config, sizeof(ATMO_TSL2572_Config_t));

    return ATMO_TSL2572_Status_Success;
}

ATMO_TSL2572_Status_t ATMO_TSL2572_GetAmbientLight(float *lightLux)
{
    if(!tsl2572Enabled)
    {
        return ATMO_TSL2572_Status_Fail;
    }
    
	if( TSL2572_ReadAmbientLight(lightLux) != 0 )
	{
		return ATMO_TSL2572_Status_Fail;
	}

	return ATMO_TSL2572_Status_Success;
}



