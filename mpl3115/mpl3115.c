#include "mpl3115.h"
#include "../app_src/atmosphere_platform.h"

typedef struct {
    ATMO_MPL3115_Config_t config;
    bool configured;
} ATMO_MPL3115_Priv_Config;

static ATMO_MPL3115_Priv_Config _ATMO_MPL3115_config;

static ATMO_I2C_Peripheral_t _ATMO_MPL3115_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static bool mpl3115Enabled = true;

static uint8_t ATMO_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_MPL3115_config.config.i2cDriverInstance, _ATMO_MPL3115_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? MPL_SUCCESS : MPL_ERROR;
}

static uint8_t ATMO_MPL3115_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_MPL3115_config.config.i2cDriverInstance, _ATMO_MPL3115_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? MPL_SUCCESS : MPL_ERROR;
}
ATMO_MPL3115_Status_t ATMO_MPL3115_Init(ATMO_MPL3115_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_MPL3115_SetConfiguration(config);
    }
    else
    {
    	_ATMO_MPL3115_config.configured = false;
    }

    mpl3115_IoFunc_t io;
    io.I2C_Read = ATMO_I2CRead_Simple;
    io.I2C_Write = ATMO_MPL3115_I2CWrite_Simple;
    io.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
    MPL3115_Init_Driver(&io);
    MPL3115_Init_Hw(&config->MPLsettings);

    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_SetEnabled(bool enabled)
{
    mpl3115Enabled = enabled;
    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_GetEnabled(bool *enabled)
{
    *enabled = mpl3115Enabled;
    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_SetConfiguration(const ATMO_MPL3115_Config_t *config)
{
    if( config == NULL || !mpl3115Enabled)
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_MPL3115_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    memcpy( &_ATMO_MPL3115_config.config, config, sizeof(ATMO_MPL3115_Config_t) );
    _ATMO_MPL3115_config.configured = true;

    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_GetConfiguration(ATMO_MPL3115_Config_t *config)
{
    if( config == NULL || !_ATMO_MPL3115_config.configured )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    memcpy(config, &_ATMO_MPL3115_config.config, sizeof(ATMO_MPL3115_Config_t));

    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_GetPressure(uint32_t *pressurePascals)
{
    if(!mpl3115Enabled)
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( MPL_ToggleOneShot() != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( MPL_ReadRawData(MPL_MODE_PRESSURE, (int32_t *)pressurePascals) != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    *pressurePascals = (*pressurePascals) / 4;

    return ATMO_MPL3115_Status_Success;
        
}

ATMO_MPL3115_Status_t ATMO_MPL3115_GetTemperature(int32_t *temperatureCelsius)
{
    if(!mpl3115Enabled)
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( MPL_ToggleOneShot() != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( MPL_ReadRawData(MPL_MODE_TEMPERATURE, (int32_t *)temperatureCelsius) != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    *temperatureCelsius = (*temperatureCelsius) / 16;

    return ATMO_MPL3115_Status_Success;
}

ATMO_MPL3115_Status_t ATMO_MPL3115_GetAltitude(uint32_t *altitudeMeters)
{
    if(!mpl3115Enabled)
    {
        return ATMO_MPL3115_Status_Fail;
    }
    
    if( MPL_ToggleOneShot() != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    if( MPL_ReadRawData(MPL_MODE_PRESSURE, (int32_t *)altitudeMeters) != MPL_SUCCESS )
    {
        return ATMO_MPL3115_Status_Fail;
    }

    *altitudeMeters = (*altitudeMeters) / 16;

    return ATMO_MPL3115_Status_Success; 
}




