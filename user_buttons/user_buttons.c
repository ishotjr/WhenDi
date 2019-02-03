#include "user_buttons.h"
#include "pin_mux.h"
#include "atmosphere_platform.h"
#include "fsl_os_abstraction.h"

static unsigned int switchAbilityHandles[4];
static ATMO_Callback_t switchCallbacks[4] = {NULL, NULL, NULL, NULL};
static bool switchAbilityHandlesInitialized[4] = {false, false, false, false};

static void ATMO_MK64F_SW_Pushed(uint32_t swNum)
{

	if(switchAbilityHandlesInitialized[swNum])
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		ATMO_AddAbilityExecute( switchAbilityHandles[swNum], &val );
	}

	if(switchCallbacks[swNum] != NULL)
	{
		ATMO_Value_t val;
		ATMO_InitValue(&val);
		switchCallbacks[swNum](&val);
		ATMO_Free(&val);
	}

}

static void ATMO_MK64F_Callback_SW1_Pushed(void *in)
{
	if(ATMO_MK64F_GPIO_Read(NULL, BOARD_INITPINS_USER_SW1_PIN_NAME) == ATMO_GPIO_PinState_Low)
	{
		ATMO_MK64F_SW_Pushed(0);
	}
}

static void ATMO_MK64F_Callback_SW2_Pushed(void *in)
{
	if(ATMO_MK64F_GPIO_Read(NULL, BOARD_INITPINS_USER_SW2_PIN_NAME) == ATMO_GPIO_PinState_Low)
	{
		ATMO_MK64F_SW_Pushed(1);
	}
}

static void ATMO_MK64F_Callback_SW3_Pushed(void *in)
{
	if(ATMO_MK64F_GPIO_Read(NULL, BOARD_INITPINS_USER_SW3_PIN_NAME) == ATMO_GPIO_PinState_Low)
	{
		ATMO_MK64F_SW_Pushed(2);
	}
}

static void ATMO_MK64F_Callback_SW4_Pushed(void *in)
{
	if(ATMO_MK64F_GPIO_Read(NULL, BOARD_INITPINS_USER_SW4_PIN_NAME) == ATMO_GPIO_PinState_Low)
	{
		ATMO_MK64F_SW_Pushed(3);
	}
}

bool ATMO_MK64F_UserButton_Init(ATMO_DriverInstanceHandle_t gpioDriver)
{
	// Initialize buttons
	ATMO_GPIO_Config_t gpioConfig;
	gpioConfig.pinMode = ATMO_GPIO_PinMode_Input_PullUp;

	ATMO_GPIO_SetPinConfiguration(gpioDriver, BOARD_INITPINS_USER_SW1_PIN_NAME, &gpioConfig);
	ATMO_GPIO_RegisterInterruptCallback(gpioDriver, BOARD_INITPINS_USER_SW1_PIN_NAME, ATMO_GPIO_InterruptTrigger_BothEdges, ATMO_MK64F_Callback_SW1_Pushed);
	ATMO_GPIO_SetPinConfiguration(gpioDriver, BOARD_INITPINS_USER_SW2_PIN_NAME, &gpioConfig);
	ATMO_GPIO_RegisterInterruptCallback(gpioDriver, BOARD_INITPINS_USER_SW2_PIN_NAME, ATMO_GPIO_InterruptTrigger_BothEdges, ATMO_MK64F_Callback_SW2_Pushed);
	ATMO_GPIO_SetPinConfiguration(gpioDriver, BOARD_INITPINS_USER_SW3_PIN_NAME, &gpioConfig);
	ATMO_GPIO_RegisterInterruptCallback(gpioDriver, BOARD_INITPINS_USER_SW3_PIN_NAME, ATMO_GPIO_InterruptTrigger_BothEdges, ATMO_MK64F_Callback_SW3_Pushed);
	ATMO_GPIO_SetPinConfiguration(gpioDriver, BOARD_INITPINS_USER_SW4_PIN_NAME, &gpioConfig);
	ATMO_GPIO_RegisterInterruptCallback(gpioDriver, BOARD_INITPINS_USER_SW4_PIN_NAME, ATMO_GPIO_InterruptTrigger_BothEdges, ATMO_MK64F_Callback_SW4_Pushed);

	return true;
}

bool ATMO_MK64F_UserButton_RegisterCallback(ATMO_MK64F_UserButton_t type, ATMO_Callback_t cb)
{
	switch(type)
	{
		case ATMO_MK64F_SW1_Pushed:
		{
			switchCallbacks[0] = cb;
			break;
		}
		case ATMO_MK64F_SW2_Pushed:
		{
			switchCallbacks[1] = cb;
			break;
		}
		case ATMO_MK64F_SW3_Pushed:
		{
			switchCallbacks[2] = cb;
			break;
		}
		case ATMO_MK64F_SW4_Pushed:
		{
			switchCallbacks[3] = cb;
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}

bool ATMO_MK64F_UserButton_RegisterAbilityHandle(ATMO_MK64F_UserButton_t type, unsigned int abilityHandle)
{
	switch(type)
	{
		case ATMO_MK64F_SW1_Pushed:
		{
			switchAbilityHandles[0] = abilityHandle;
			switchAbilityHandlesInitialized[0] = true;
			break;
		}
		case ATMO_MK64F_SW2_Pushed:
		{
			switchAbilityHandles[1] = abilityHandle;
			switchAbilityHandlesInitialized[1] = true;
			break;
		}
		case ATMO_MK64F_SW3_Pushed:
		{
			switchAbilityHandles[2] = abilityHandle;
			switchAbilityHandlesInitialized[2] = true;
			break;
		}
		case ATMO_MK64F_SW4_Pushed:
		{
			switchAbilityHandles[3] = abilityHandle;
			switchAbilityHandlesInitialized[3] = true;
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}
