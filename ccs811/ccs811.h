#ifndef _ATMO_CCS811_H_
#define _ATMO_CCS811_H_

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"


typedef enum {
    ATMO_CCS811_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_CCS811_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_CCS811_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_CCS811_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_CCS811_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_CCS811_Status_t;

typedef enum {
    ATMO_CCS811_OperatingMode_0 = 0,
    ATMO_CCS811_OperatingMode_1 = 1,
    ATMO_CCS811_OperatingMode_2 = 2,
    ATMO_CCS811_OperatingMode_3 = 3,
    ATMO_CCS811_OperatingMode_4 = 4
} ATMO_CCS811_OperatingMode_t;

typedef struct {
	//Mode 0 = Idle
	//Mode 1 = read every 1s
	//Mode 2 = every 10s
	//Mode 3 = every 60s
	//Mode 4 = RAW mode
	uint8_t operatingMode;
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
} ATMO_CCS811_Config_t;


/**
 * Initialize CCS811 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_CCS811_Status_t ATMO_CCS811_Init(ATMO_CCS811_Config_t *config);

/**
 * Enable/Disable CCS811 Driver
 *
 * @param[in] enabled
 */
ATMO_CCS811_Status_t ATMO_CCS811_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of CCS811 driver
 *
 * @param[out] enabled
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetEnabled(bool *enabled);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_CCS811_Status_t ATMO_CCS811_SetConfiguration(const ATMO_CCS811_Config_t *config);

/**
 * Get device configuration
 *
 * @param[out] config
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetConfiguration(ATMO_CCS811_Config_t *config);

/**
 * Set environmental data for more accurate readings
 *
 * @param[in] humidity - Pct from 0.0 to 100.0
 * @param[in] temperature - Degrees C
 */
ATMO_CCS811_Status_t ATMO_CCS811_SetEnvironmentalData(float humidity, float temperature);

/**
 * Get Total Volatile Organic Compounds (TVOC) Reading
 *
 * @param[out] tvoc - TVOC reading in PPB
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetTVOC(uint16_t *tvoc);

/**
 * Get total CO2 Reading in ppm
 *
 * @param[out] CO2
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetCO2(uint16_t *co2);

/**
 * Get air quality in percentage based on TVOC reading
 *
 * @param[out] airQuality
 */
ATMO_CCS811_Status_t ATMO_CCS811_GetAirQuality(float *airQuality);


#endif
