#ifndef _ATMO_FXAS21002_H_
#define _ATMO_FXAS21002_H_

#include "../i2c/i2c.h"
#include "fxas21002_internal.h"

typedef enum {
    ATMO_FXAS21002_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_FXAS21002_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_FXAS21002_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_FXAS21002_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_FXAS21002_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_FXAS21002_Status_t;

typedef struct {
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
    uint8_t samplingRate;
} ATMO_FXAS21002_Config_t;

// X Y Z values in DPS
typedef struct {
	float X;
	float Y;
	float Z;
} ATMO_FXAS21002_SensorData_t;

/**
 * Initialize FXAS21002 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_Init(ATMO_FXAS21002_Config_t *config);

/**
 * Enable/Disable FXAS21002 Driver
 *
 * @param[in] enabled
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of FXAS21002 driver
 *
 * @param[out] enabled
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetEnabled(bool *enabled);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_SetConfiguration(const ATMO_FXAS21002_Config_t *config);

/**
 * Get device configuration
 *
 * @param[out] config
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetConfiguration(ATMO_FXAS21002_Config_t *config);

/**
 * Get XYZ data from sensor
 *
 * @param[out] data
 */
ATMO_FXAS21002_Status_t ATMO_FXAS21002_GetSensorData(ATMO_FXAS21002_SensorData_t *data);

#endif
