#ifndef __ATMO_SX9500_H_
#define __ATMO_SX9500_H_

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"
#include "../gpio/gpio.h"

typedef enum {
    ATMO_SX9500_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_SX9500_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_SX9500_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_SX9500_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_SX9500_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_SX9500_Status_t;

typedef struct {
    uint16_t address;
    bool interruptEnabled;
    ATMO_GPIO_Device_Pin_t interruptPin;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
    ATMO_DriverInstanceHandle_t gpioDriverInstance;
} ATMO_SX9500_Config_t;

typedef struct {
	bool downPressed;
	bool upPressed;
	bool leftPressed;
	bool rightPressed;
} SX9500_TouchState_t;

typedef enum {
	SX9500_Touched_Any = 0,
	SX9500_Touched_Down,
	SX9500_Touched_Right,
	SX9500_Touched_Up,
	SX9500_Touched_Left,
	SX9500_Touched_NumEvents,
} SX9500_TouchEvent_t;


/**
 * Initialize SX9500 Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_SX9500_Status_t ATMO_SX9500_Init(ATMO_SX9500_Config_t *config);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_SX9500_Status_t ATMO_SX9500_SetConfiguration(const ATMO_SX9500_Config_t *config);

/**
 * Get device configuration
 *
 * @param[out] config
 */
ATMO_SX9500_Status_t ATMO_SX9500_GetConfiguration(ATMO_SX9500_Config_t *config);

/**
 * Get current touch sensor state
 *
 * @param[out] touchState
 */
ATMO_SX9500_Status_t ATMO_SX9500_GetTouchState(SX9500_TouchState_t *touchState);

/**
 * Register ability handle for touched event
 *
 * @param[in] abilityHandle
 */
ATMO_SX9500_Status_t ATMO_SX9500_RegisterTouchedAbilityHandle(SX9500_TouchEvent_t event, unsigned int abilityHandle);


/**
 * Register ability handle for touched event
 *
 * @param[in] abilityHandle
 */
ATMO_SX9500_Status_t ATMO_SX9500_RegisterTouchedCallback(SX9500_TouchEvent_t event, ATMO_Callback_t cb);


#endif

