#include "ccs811.h"
#include "ccs811_internal.h"
#include "../app_src/atmosphere_platform.h"

#define CCS811_TVOC_MAX_PPB (1187)


typedef struct {
    ATMO_CCS811_Config_t config;
    bool configured;
} ATMO_CCS811_Priv_Config;

static ATMO_CCS811_Priv_Config _ATMO_CCS811_config;

static ATMO_I2C_Peripheral_t i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static bool _ATMO_CCS811_Enabled = true;

static uint8_t ATMO_CCS811_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_CCS811_config.config.i2cDriverInstance, _ATMO_CCS811_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static uint8_t ATMO_CCS811_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_CCS811_config.config.i2cDriverInstance, _ATMO_CCS811_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

ATMO_CCS811_Status_t ATMO_CCS811_Init(ATMO_CCS811_Config_t *config)
{

    CCS811_fct_t io;
    io.I2C_Read = ATMO_CCS811_I2CRead_Simple;
    io.I2C_Write = ATMO_CCS811_I2CWrite_Simple;
    io.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
    CCS811_Init_Driver(&io);

    // Did the user supply a configuration?
    if( config )
    {
        ATMO_CCS811_SetConfiguration(config);
    }
    else
    {
        _ATMO_CCS811_config.configured = false;
    }

    if( CCS811_Init_Hw() != CCS811_SUCCESS )
    {
    	return ATMO_CCS811_Status_Fail;
    }

    // The configuration has to be set before the Init_HW routine is called
    // But the Init_HW routine resets the device
    // So, we need to just set the configuration one more time after this reset
    if( config )
    {
        ATMO_CCS811_SetConfiguration(config);
    }

    return ATMO_CCS811_Status_Success;
}

ATMO_CCS811_Status_t ATMO_CCS811_SetEnabled(bool enabled)
{
    _ATMO_CCS811_Enabled = enabled;
    return ATMO_CCS811_Status_Success;
}

ATMO_CCS811_Status_t ATMO_CCS811_GetEnabled(bool *enabled)
{
    *enabled = _ATMO_CCS811_Enabled;
    return ATMO_CCS811_Status_Success;
}

ATMO_CCS811_Status_t ATMO_CCS811_SetConfiguration(const ATMO_CCS811_Config_t *config)
{
    if( config == NULL )
    {
        return ATMO_CCS811_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_CCS811_Status_Fail;
    }
    memcpy( &_ATMO_CCS811_config.config, config, sizeof(ATMO_CCS811_Config_t) );
    _ATMO_CCS811_config.configured = true;

    if( CCS811_setDriveMode(config->operatingMode) != CCS811_SUCCESS )
    {
    	return ATMO_CCS811_Status_Fail;
    }

    return ATMO_CCS811_Status_Success;
}

ATMO_CCS811_Status_t ATMO_CCS811_GetConfiguration(ATMO_CCS811_Config_t *config)
{
    if( config == NULL || !_ATMO_CCS811_config.configured )
    {
        return ATMO_CCS811_Status_Fail;
    }

    memcpy(config, &_ATMO_CCS811_config.config, sizeof(ATMO_CCS811_Config_t));

    return ATMO_CCS811_Status_Success;
}


ATMO_CCS811_Status_t ATMO_CCS811_GetTVOC(uint16_t *tvoc)
{
    if(!_ATMO_CCS811_Enabled)
    {
        return ATMO_CCS811_Status_Fail;
    }

	if( CCS811_readAlgorithmResults() != CCS811_SUCCESS )
	{
		return ATMO_CCS811_Status_Fail;
	}

	*tvoc = CCS811_getTVOC();

	return ATMO_CCS811_Status_Success;
}

ATMO_CCS811_Status_t ATMO_CCS811_GetCO2(uint16_t *co2)
{
    if(!_ATMO_CCS811_Enabled)
    {
        return ATMO_CCS811_Status_Fail;
    }

	if( CCS811_readAlgorithmResults() != CCS811_SUCCESS )
	{
		return ATMO_CCS811_Status_Fail;
	}

	*co2 = CCS811_getCO2();

	return ATMO_CCS811_Status_Success;
}

/**
 * Get air quality in percentage based on TVOC reading
 *
 * @param[out] airQuality
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetAirQuality(float *airQuality)
{
    if(!_ATMO_CCS811_Enabled)
    {
        return ATMO_CCS811_Status_Fail;
    }
    
	if( CCS811_readAlgorithmResults() != CCS811_SUCCESS )
	{
		return ATMO_CCS811_Status_Fail;
	}

	*airQuality = (CCS811_getTVOC() * 100.0) / CCS811_TVOC_MAX_PPB;

	return ATMO_CCS811_Status_Success;
}
