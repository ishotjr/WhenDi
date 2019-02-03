#include "sx9500.h"
#include "sx9500_internal.h"
#include "../app_src/atmosphere_platform.h"

typedef struct {
    ATMO_SX9500_Config_t config;
    unsigned int abilityHandle[SX9500_Touched_NumEvents];
	bool abilityHandleRegistered[SX9500_Touched_NumEvents];
    ATMO_Callback_t callback[SX9500_Touched_NumEvents];
    bool configured;
} ATMO_SX9500_Priv_Config;

static ATMO_SX9500_Priv_Config _ATMO_SX9500_config = {0};

static ATMO_I2C_Peripheral_t _ATMO_SX9500_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

static uint8_t _ATMO_SX9500_I2CRead_Simple(uint8_t *writeBuf, uint32_t writeSize, uint8_t *readBuf, uint32_t readSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead(_ATMO_SX9500_config.config.i2cDriverInstance, _ATMO_SX9500_config.config.address, writeBuf, writeSize, readBuf, readSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static uint8_t _ATMO_SX9500_I2CWrite_Simple(uint8_t *writeBuf, uint32_t writeSize)
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_ATMO_SX9500_config.config.i2cDriverInstance, _ATMO_SX9500_config.config.address, NULL, 0, writeBuf, writeSize, 1000);
	return (status == ATMO_I2C_Status_Success) ? 0 : 1;
}

static bool ATMO_SX9500_TouchStateToIndex(SX9500_TouchState_t *touchState, uint32_t *index)
{
	bool anyPressed = false;

	if(touchState->downPressed)
	{
		anyPressed = true;
		*index = SX9500_Touched_Down;
	}
	if(touchState->upPressed)
	{
		anyPressed = true;
		*index = SX9500_Touched_Up;
	}
	if(touchState->rightPressed)
	{
		anyPressed = true;
		*index = SX9500_Touched_Right;
	}
	if(touchState->leftPressed)
	{
		anyPressed = true;
		*index = SX9500_Touched_Left;
	}

	return anyPressed;
}

static void ATMO_SX9500_Callback(void *data)
{
    RegIrqSrc_t irq;
    if (SX9500_Read_Irq(&irq.octet) == SX9500_SUCCESS)
    {
        if (irq.bits.close || irq.bits.far)
        {
        	SX9500_TouchState_t touchState;
        	if(ATMO_SX9500_GetTouchState(&touchState) == ATMO_SX9500_Status_Success)
        	{
        		uint32_t index = 0;
        		if(ATMO_SX9500_TouchStateToIndex(&touchState, &index))
        		{
        			if(_ATMO_SX9500_config.abilityHandleRegistered[SX9500_Touched_Any] )
        			{
        				ATMO_Value_t val;
        				ATMO_InitValue(&val);
        				ATMO_CreateValueBinary(&val, &touchState, sizeof(touchState));
        				ATMO_AddAbilityExecute(_ATMO_SX9500_config.abilityHandle[SX9500_Touched_Any], &val);
        			}

        			if(_ATMO_SX9500_config.abilityHandleRegistered[index] )
        			{
        				ATMO_Value_t val;
        				ATMO_InitValue(&val);
        				ATMO_CreateValueBinary(&val, &touchState, sizeof(touchState));
        				ATMO_AddAbilityExecute(_ATMO_SX9500_config.abilityHandle[index], &val);
        			}

        			if(_ATMO_SX9500_config.callback[SX9500_Touched_Any] != NULL)
        			{
        				ATMO_Value_t val;
        				ATMO_InitValue(&val);
        				ATMO_CreateValueBinary(&val, &touchState, sizeof(touchState));
        				_ATMO_SX9500_config.callback[SX9500_Touched_Any](&val);
        				ATMO_FreeValue(&val);
        			}
        			if(_ATMO_SX9500_config.callback[index] != NULL)
        			{
        				ATMO_Value_t val;
        				ATMO_InitValue(&val);
        				ATMO_CreateValueBinary(&val, &touchState, sizeof(touchState));
        				_ATMO_SX9500_config.callback[index](&val);
        				ATMO_FreeValue(&val);
        			}
        		}
        	}
        	else
        	{
        		return ATMO_SX9500_Status_Fail;
        	}
        }
    }
    else
    {
    	return ATMO_SX9500_Status_Fail;
    }

    return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_Init(ATMO_SX9500_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_SX9500_SetConfiguration(config);
    }
    else
    {
    	_ATMO_SX9500_config.configured = false;
    }

	sx9500_fct_t fct;
	fct.I2C_Read = _ATMO_SX9500_I2CRead_Simple;
	fct.I2C_Write = _ATMO_SX9500_I2CWrite_Simple;
	fct.WaitMs = ATMO_PLATFORM_DelayMilliseconds;
	SX9500_Init_Driver(&fct);
	SX9500_Init_Hw();

    return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_SetConfiguration(const ATMO_SX9500_Config_t *config)
{
    if( config == NULL )
    {
        return ATMO_SX9500_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_SX9500_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_SX9500_Status_Fail;
    }

    // Register interrupt
    if(config->interruptEnabled)
    {
    	ATMO_GPIO_RegisterInterruptCallback(config->gpioDriverInstance, config->interruptPin, ATMO_GPIO_InterruptTrigger_FallingEdge, ATMO_SX9500_Callback);
    }

    memcpy( &_ATMO_SX9500_config.config, config, sizeof(ATMO_SX9500_Config_t) );
    _ATMO_SX9500_config.configured = true;

    return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_GetConfiguration(ATMO_SX9500_Config_t *config)
{
    if( config == NULL || !_ATMO_SX9500_config.configured )
    {
        return ATMO_SX9500_Status_Fail;
    }

    memcpy(config, &_ATMO_SX9500_config.config, sizeof(ATMO_SX9500_Config_t));

    return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_GetTouchState(SX9500_TouchState_t *touchState)
{
	memset(touchState, 0, sizeof(SX9500_TouchState_t));

	RegStat_t dir;
	if( SX9500_Read_Proximity_Sensors(&dir.octet) != SX9500_SUCCESS )
	{
		return ATMO_SX9500_Status_Fail;
	}

	if(dir.octet > 0)
	{
		if(dir.bits.proxstat0)
		{
			touchState->downPressed = true;
		}
		if(dir.bits.proxstat1)
		{
			touchState->rightPressed = true;
		}
		if(dir.bits.proxstat2)
		{
			touchState->upPressed = true;
		}
		if(dir.bits.proxstat3)
		{
			touchState->leftPressed = true;
		}
	}

	return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_RegisterTouchedAbilityHandle(SX9500_TouchEvent_t event, unsigned int abilityHandle)
{
	_ATMO_SX9500_config.abilityHandle[event] = abilityHandle;
	_ATMO_SX9500_config.abilityHandleRegistered[event] = true;
	return ATMO_SX9500_Status_Success;
}

ATMO_SX9500_Status_t ATMO_SX9500_RegisterTouchedCallback(SX9500_TouchEvent_t event, ATMO_Callback_t cb)
{
	_ATMO_SX9500_config.callback[event] = cb;
	return ATMO_SX9500_Status_Success;
}


