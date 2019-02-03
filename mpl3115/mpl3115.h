#ifndef __ATMO_MPL3115_H_
#define __ATMO_MPL3115_H_

#include "mpl3115_internal.h"
#include "../i2c/i2c.h"

typedef enum {
    ATMO_MPL3115_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_MPL3115_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_MPL3115_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_MPL3115_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_MPL3115_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_MPL3115_Status_t;

typedef struct {
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
	settingsMPL_t MPLsettings;
} ATMO_MPL3115_Config_t;


/**
 * Initialize MPL3115 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_Init(ATMO_MPL3115_Config_t *config);

/**
 * Enable/Disable MPL3115 Driver
 *
 * @param[in] enabled
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of MPL3115 driver
 *
 * @param[out] enabled
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_GetEnabled(bool *enabled);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_SetConfiguration(const ATMO_MPL3115_Config_t *config);

/**
 * Get device configuration
 * 
 * @param[out] config
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_GetConfiguration(ATMO_MPL3115_Config_t *config);

/**
 * Get Barometeric Pressure in Pascals
 * 
 * @param[out] pressurePascals
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_GetPressure(uint32_t *pressurePascals);

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperature
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_GetTemperature(int32_t *temperatureCelsius);

/**
 * Get Altitude in meters
 * 
 * @param[out] altitudeMeters
 */
ATMO_MPL3115_Status_t ATMO_MPL3115_GetAltitude(uint32_t *altitudeMeters);

#endif
