#ifndef _ATMO_ENS210_H_
#define _ATMO_ENS210_H_

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"

typedef enum {
    ATMO_ENS210_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_ENS210_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_ENS210_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_ENS210_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_ENS210_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_ENS210_Status_t;

typedef struct {
    uint16_t address;
    int16_t tempCalibrationOffset;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
} ATMO_ENS210_Config_t;

/**
 * Initialize ENS210 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_ENS210_Status_t ATMO_ENS210_Init(ATMO_ENS210_Config_t *config);

/**
 * Enable/Disable ENS210 Driver
 *
 * @param[in] enabled
 */
ATMO_ENS210_Status_t ATMO_ENS210_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of ENS210 driver
 *
 * @param[out] enabled
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetEnabled(bool *enabled);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_ENS210_Status_t ATMO_ENS210_SetConfiguration(const ATMO_ENS210_Config_t *config);

/**
 * Get device configuration
 * 
 * @param[out] config
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetConfiguration(ATMO_ENS210_Config_t *config);

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperatureCelsius
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetTemperature(int32_t *temperatureCelsius);

/**
 * Get Humidity
 * 
 * @param[out] humidityPct
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetHumidity(int32_t *humidityPct);

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperatureCelsius
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetTemperatureFloat(float *temperatureCelsius);

/**
 * Get Humidity
 * 
 * @param[out] humidityPct
 */
ATMO_ENS210_Status_t ATMO_ENS210_GetHumidityFloat(float *humidityPct);

#endif
