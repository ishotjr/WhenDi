#include "ens210.h"
#include "ens210_internal.h"

typedef struct {
    ATMO_ENS210_Config_t config;
    bool configured;
} ATMO_ENS210_Priv_Config;

static ATMO_ENS210_Priv_Config _ATMO_ENS210_config;

static ATMO_I2C_Peripheral_t _ATMO_ENS210_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static bool _ATMO_ENS210_Enabled = true;

static uint8_t ATMO_ENS210_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_ENS210_config.config.i2cDriverInstance, _ATMO_ENS210_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static uint8_t ATMO_ENS210_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_ENS210_config.config.i2cDriverInstance, _ATMO_ENS210_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

ATMO_ENS210_Status_t ATMO_ENS210_Init(ATMO_ENS210_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_ENS210_SetConfiguration(config);
    }
    else
    {
        _ATMO_ENS210_config.configured = false;
    }

    ens210_IoFunc_t io;
    io.I2C_Read = ATMO_ENS210_I2CRead_Simple;
    io.I2C_Write = ATMO_ENS210_I2CWrite_Simple;
    io.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
    ENS210_Init_Driver(&io);
    ENS210_Init_Hw();

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_SetEnabled(bool enabled)
{
    _ATMO_ENS210_Enabled = enabled;
    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetEnabled(bool *enabled)
{
    *enabled = _ATMO_ENS210_Enabled;
    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_SetConfiguration(const ATMO_ENS210_Config_t *config)
{
    if( config == NULL )
    {
        return ATMO_ENS210_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_ENS210_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_ENS210_Status_Fail;
    }
    memcpy( &_ATMO_ENS210_config.config, config, sizeof(ATMO_ENS210_Config_t) );
    _ATMO_ENS210_config.configured = true;

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetConfiguration(ATMO_ENS210_Config_t *config)
{
    if( config == NULL || !_ATMO_ENS210_config.configured )
    {
        return ATMO_ENS210_Status_Fail;
    }

    memcpy(config, &_ATMO_ENS210_config.config, sizeof(ATMO_ENS210_Config_t));

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetTemperature(int32_t *temperatureCelsius)
{
    if(!_ATMO_ENS210_Enabled || !_ATMO_ENS210_config.configured)
    {
        return ATMO_ENS210_Status_Fail;
    }

    ens210_meas_data_t results;
    if( ENS210_Measure(mode_Tonly, &results) != 0 )
    {
        return ATMO_ENS210_Status_Fail;
    }

    *temperatureCelsius = results.T_Celsius + _ATMO_ENS210_config.config.tempCalibrationOffset;

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetHumidity(int32_t *humidityPct)
{
    if(!_ATMO_ENS210_Enabled)
    {
        return ATMO_ENS210_Status_Fail;
    }

    ens210_meas_data_t results;
    if( ENS210_Measure(mode_Honly, &results) != 0 )
    {
        return ATMO_ENS210_Status_Fail;
    }

    *humidityPct = results.H_Percent;

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetTemperatureFloat(float *temperatureCelsius)
{
    if(!_ATMO_ENS210_Enabled || !_ATMO_ENS210_config.configured)
    {
        return ATMO_ENS210_Status_Fail;
    }

    ens210_meas_data_t results;
    if( ENS210_Measure(mode_Tonly, &results) != 0 )
    {
        return ATMO_ENS210_Status_Fail;
    }

    *temperatureCelsius = (results.T_mCelsius / 1000.0) + _ATMO_ENS210_config.config.tempCalibrationOffset;

    return ATMO_ENS210_Status_Success;
}

ATMO_ENS210_Status_t ATMO_ENS210_GetHumidityFloat(float *humidityPct)
{
    if(!_ATMO_ENS210_Enabled)
    {
        return ATMO_ENS210_Status_Fail;
    }

    ens210_meas_data_t results;
    if( ENS210_Measure(mode_Honly, &results) != 0 )
    {
        return ATMO_ENS210_Status_Fail;
    }

    *humidityPct = results.H_mPercent / 1000.0;

    return ATMO_ENS210_Status_Success;
}