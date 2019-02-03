#ifndef __ATMO_TSL2572_H_
#define __ATMO_TSL2572_H_


#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"

typedef enum {
    ATMO_TSL2572_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_TSL2572_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_TSL2572_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_TSL2572_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_TSL2572_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_TSL2572_Status_t;

typedef struct {
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
} ATMO_TSL2572_Config_t;


/**
 * Initialize TSL2572 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_Init(ATMO_TSL2572_Config_t *config);

/**
 * Enable/Disable TSL2572 Driver
 *
 * @param[in] enabled
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of TSL2572 driver
 *
 * @param[out] enabled
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_GetEnabled(bool *enabled);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_SetConfiguration(const ATMO_TSL2572_Config_t *config);

/**
 * Get device configuration
 *
 * @param[out] config
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_GetConfiguration(ATMO_TSL2572_Config_t *config);

/**
 * Get ambient light intensity in lux
 *
 * @param[out] lightLux
 */
ATMO_TSL2572_Status_t ATMO_TSL2572_GetAmbientLight(float *lightLux);


#endif
