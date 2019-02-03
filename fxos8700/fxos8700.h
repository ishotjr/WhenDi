#ifndef _ATMO_FXOS8700_H_
#define _ATMO_FXOS8700_H_

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"
#include "../gpio/gpio.h"

typedef enum {
    ATMO_FXOS8700_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_FXOS8700_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_FXOS8700_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_FXOS8700_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_FXOS8700_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_FXOS8700_Status_t;

typedef struct {
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
    ATMO_DriverInstanceHandle_t gpioDriverInstance;
    bool int1En;
    ATMO_GPIO_Device_Pin_t int1Pin;
    bool int2En;
    ATMO_GPIO_Device_Pin_t int2Pin;
    bool motionEnabled;
    bool freefallEnabled;
    bool tapDetectionEnabled;

} ATMO_FXOS8700_Config_t;

typedef enum {
	FXOS8700_NoDetect,
	FXOS8700_FreefallDetect,
	FXOS8700_MotionDetect,
    FXOS8700_TapDetect,
} FXOS8700_Detect_t;

/**
 * Initialize FXOS8700 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_Init(ATMO_FXOS8700_Config_t *config);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetConfiguration(const ATMO_FXOS8700_Config_t *config);

/**
 * Enable/Disable FXOS8700 Driver
 *
 * @param[in] enabled
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetEnabled(bool enabled);

/**
 * Get enabled/disabled status of FXOS8700 driver
 *
 * @param[out] enabled
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetEnabled(bool *enabled);

/**
 * Get device configuration
 *
 * @param[out] config
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetConfiguration(ATMO_FXOS8700_Config_t *config);

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetAccelData(ATMO_3dFloatVector_t *data);

ATMO_FXOS8700_Status_t ATMO_FXOS8700_GetMagData(ATMO_3dFloatVector_t *data);

/**
 * Motion/Freefall detection
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetFreefallDetectedAbilityHandle(unsigned int abilityHandle);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetFreefallDetectedCallback(ATMO_Callback_t cb);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetMotionDetectedAbilityHandle(unsigned int abilityHandle);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetMotionDetectedCallback(ATMO_Callback_t cb);

/**
 * Tap detection
 */
ATMO_FXOS8700_Status_t ATMO_FXOS8700_IsTapDetected(bool *isTapDetected, bool *isDoubleTap);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetTapDetectedAbilityHandle(unsigned int abilityHandle);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetTapDetectedCallback(ATMO_Callback_t cb);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetDoubleTapDetectedAbilityHandle(unsigned int abilityHandle);
ATMO_FXOS8700_Status_t ATMO_FXOS8700_SetDoubleTapDetectedCallback(ATMO_Callback_t cb);

ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableFreefallDetection();
ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableMotionDetection();
ATMO_FXOS8700_Status_t ATMO_FXOS8700_EnableTapDetection();
ATMO_FXOS8700_Status_t ATMO_FXOS8700_DisableAllDetection();

#endif
