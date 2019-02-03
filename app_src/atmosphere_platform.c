#include "atmosphere_platform.h"
#include "./gpio/gpio_mk64f.h"
#include "./i2c/i2c_mk64f.h"
#include "./uart/uart_mk64f.h"
#include "./ble/ble_mk64f.h"
#include "./interval/interval_mk64f.h"
#include "ble_db_mk64f.h"
#include "./cloud/cloud_ble.h"
#include "./datetime/datetime_mk64f.h"
#include "./spi/spi_mk64f.h"
#include "./block/block_mk64f.h"
#include "./filesystem/filesystem_lfs.h"
#include "./pwm/pwm_mk64f.h"
#include "./nfc/nfc_mk64f.h"
#include "../auth/auth_mk64f.h"


#include <stdarg.h>

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* Display */
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "GUI.h"
#include "MULTIPAGE.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "emwin_support.h"


#include "TimersManager.h"

/* graphics */
#include "NXP_logo_white.h"

#include "board.h"
#include "battery.h"
#include "shell.h"

#include "a100x_interface.h"
#include "ble_otap_client.h"
#include "FsciInterface.h"

static bool bleInitialized = false;
static bool gattDbInitialized = false;

extern bool isInitComplete;
extern EventGroupHandle_t executeTickEvent;

#define ATMO_RPK_SENSOR_I2C_BUS (1)

const ATMO_I2C_Peripheral_t atmo_rpk_sensor_i2c_config =
{
	.baudRate = ATMO_I2C_BaudRate_100kHz,
	.operatingMode = ATMO_I2C_OperatingMode_Master
};


/* Define the init structure for the output GPIO Output High pin*/
extern gpio_pin_config_t gpio_output_high_config;

/* Define the init structure for the output GPIO Output Low pin*/
extern gpio_pin_config_t gpio_output_low_config;

static SemaphoreHandle_t abilityExecMutex = NULL;
static SemaphoreHandle_t debugPrintMutex = NULL;

static uint8_t usbSerialManInterface = 0;

enum splash_screen_status_t
{
	SPLASH_INIT,
	SPLASH_NXP,
	SPLASH_ATMO,
	SPLASH_DONE,
};


/* FSCI Interface Configuration structure */
static const gFsciSerialConfig_t mFsciSerials[] =
{
	{
//        .baudrate = gSPI_BaudRate_1000000_c,
//        .interfaceType = gSerialMgrSPIMaster_c,
//        .interfaceChannel = 0,
//        .virtualInterface = 0
		.baudrate = gUARTBaudRate38400_c,
		.interfaceType = gSerialMgrUart_c,
		.interfaceChannel = 4,
		.virtualInterface = 0

	},
#if gHybridApp_d
	{
//        .baudrate = gSPI_BaudRate_1000000_c,
//        .interfaceType = gSerialMgrSPIMaster_c,
//        .interfaceChannel = 0,
//        .virtualInterface = 1
		.baudrate = gUARTBaudRate38400_c,
		.interfaceType = gSerialMgrUart_c,
		.interfaceChannel = 4,
		.virtualInterface = 1

	}
#endif
};

uint32_t splashStatus = SPLASH_INIT;
#define SPLASH_DELAY_MS (1500)

static tmrTimerID_t splashTimerId = gTmrInvalidTimerID_c;

extern const unsigned char _acAtmosphereIoTSplashBlack[];


static void fsciBleRegister( uint32_t fsciInterfaceId )
{
	/* Register Generic FSCI */
	if ( FSCI_RegisterOpGroup( gFSCI_CnfOpcodeGroup_c,
	                           gFsciMonitorMode_c,
	                           BLE_FSCI_RxCb,
	                           NULL,
	                           fsciInterfaceId ) != gFsciSuccess_c )
	{
		panic( 0, ( uint32_t )fsciBleRegister, 0, 0 );
	}

	/* Register L2CAP command handler */
	if ( FSCI_RegisterOpGroup( gFsciBleL2capOpcodeGroup_c,
	                           gFsciMonitorMode_c,
	                           BLE_FSCI_RxCb,
	                           NULL,
	                           fsciInterfaceId ) != gFsciSuccess_c )
	{
		panic( 0, ( uint32_t )fsciBleRegister, 0, 0 );
	}

	/* Register GATT command handler */
	if ( FSCI_RegisterOpGroup( gFsciBleGattOpcodeGroup_c,
	                           gFsciMonitorMode_c,
	                           BLE_FSCI_RxCb,
	                           NULL,
	                           fsciInterfaceId ) != gFsciSuccess_c )
	{
		panic( 0, ( uint32_t )fsciBleRegister, 0, 0 );
	}

	/* Register GATT Database (application) command handler */
	if ( FSCI_RegisterOpGroup( gFsciBleGattDbAppOpcodeGroup_c,
	                           gFsciMonitorMode_c,
	                           BLE_FSCI_RxCb,
	                           NULL,
	                           fsciInterfaceId ) != gFsciSuccess_c )
	{
		panic( 0, ( uint32_t )fsciBleRegister, 0, 0 );
	}

	/* Register GAP command handler */
	if ( FSCI_RegisterOpGroup( gFsciBleGapOpcodeGroup_c,
	                           gFsciMonitorMode_c,
	                           BLE_FSCI_RxCb,
	                           NULL,
	                           fsciInterfaceId ) != gFsciSuccess_c )
	{
		panic( 0, ( uint32_t )fsciBleRegister, 0, 0 );
	}
}

static void fsciThciRegister(uint32_t fsciInterfaceId)
{
    if (FSCI_RegisterOpGroup(gFSCI_IpStackOpGCnf_c,
                             gFsciMonitorMode_c,
                             THCI_RxCb,
                             NULL,
                             fsciInterfaceId) != gFsciSuccess_c)
    {
        panic(0, (uint32_t)fsciThciRegister, 0, 0);
    }
}


static void otaCompleteCallback()
{
	ATMO_PLATFORM_DebugPrint("Saving OTA Complete Flag to SPI FLASH\r\n");
	ATMO_CLOUD_SetOtaFlag(0);
	return;
}


static void ATMO_PLATFORM_InitializeKw41z()
{
	// KW41
	// The sleep used in board.c doesn't seem to be correct
	// So do it here instead
	GPIO_PinInit( BOARD_INITPINS_KW41_RST_GPIO, BOARD_INITPINS_KW41_RST_GPIO_PIN,
	              &gpio_output_low_config ); 			// Put Kw41Z in reset

	ATMO_PLATFORM_DelayMilliseconds( 500 );

	GPIO_WritePinOutput( BOARD_INITPINS_KW41_RST_GPIO, BOARD_INITPINS_KW41_RST_GPIO_PIN, 1 ); // Take kw41z out of reset

	ATMO_PLATFORM_DelayMilliseconds( 500 );
}

void splashScreenCb( void* param )
{
	if(gattDbInitialized || isInitComplete)
	{
		return;
	}

	switch( splashStatus )
	{
		case SPLASH_INIT:
		{
			GUI_BMP_Draw( _acNXP_logo_white, 0, 0 );
			splashStatus = SPLASH_NXP;
			TMR_StartSingleShotTimer( splashTimerId, SPLASH_DELAY_MS, splashScreenCb, NULL );
			break;
		}

		case SPLASH_NXP:
		{
			GUI_BMP_Draw( _acAtmosphereIoTSplashBlack, 0, 0 );
			splashStatus = SPLASH_ATMO;
			TMR_StartSingleShotTimer( splashTimerId, SPLASH_DELAY_MS, splashScreenCb, NULL );
			break;
		}

		case SPLASH_ATMO:
		{
			splashStatus = SPLASH_DONE;
			break;
		}
	}
}

void ATMO_PLATFORM_SendTickEvent()
{
	xEventGroupSetBits(executeTickEvent, 0x1);
}

bool ATMO_PLATFORM_IsInitComplete()
{
	return isInitComplete;
}

void OtapStatusRawCallback(OtapClient_Status_t status)
{
	ATMO_PLATFORM_DebugPrint("OTA Status: %d\r\n", status.type);

	if(status.type == OTAP_CLIENT_STATUS_STARTED)
	{
		ATMO_MK64F_DateTime_SetEnabled(false);
		ATMO_MK64F_BLOCK_SetEnabled(false);
	}
	else if(status.type == OTAP_CLIENT_STATUS_STOPPED)
	{
		ATMO_MK64F_DateTime_SetEnabled(true);
		ATMO_MK64F_BLOCK_SetEnabled(true);
	}
	else if(status.type == OTAP_CLIENT_STATUS_PROGRESS)
	{
		ATMO_MK64F_DateTime_SetEnabled(false);
		ATMO_MK64F_BLOCK_SetEnabled(false);
	}
	else
	{
		ATMO_MK64F_DateTime_SetEnabled(true);
		ATMO_MK64F_BLOCK_SetEnabled(true);
	}
}

void ATMO_PLATFORM_Init()
{

	shell_init( "" );
	FSCI_Init( ( void* ) &mFsciSerials );
    fsciThciRegister(0);
    fsciBleRegister(1);
    Serial_InitInterface(&usbSerialManInterface, gSerialMgrUSB_c, 1);
    Serial_SetBaudRate(usbSerialManInterface, 115200);

	srand( xTaskGetTickCount() );

	abilityExecMutex = xSemaphoreCreateMutex();
	debugPrintMutex = xSemaphoreCreateMutex();

	ATMO_DriverInstanceHandle_t gpioInstanceNumber = 0;
	ATMO_MK64F_GPIO_AddDriverInstance( &gpioInstanceNumber );
	ATMO_GPIO_Init( gpioInstanceNumber );

	ATMO_DriverInstanceHandle_t intervalInstanceNumber = 0;
	ATMO_MK64F_INTERVAL_AddDriverInstance( &intervalInstanceNumber );
	ATMO_INTERVAL_Init( intervalInstanceNumber );

	ATMO_DriverInstanceHandle_t i2cInstanceNumber = 0;
	ATMO_MK64F_I2C_AddDriverInstance( &i2cInstanceNumber );
	ATMO_I2C_Init( i2cInstanceNumber );

	ATMO_DriverInstanceHandle_t pwmInstanceNumber = 0;
	ATMO_MK64F_PWM_AddDriverInstance( &pwmInstanceNumber );
	ATMO_PWM_Init( pwmInstanceNumber );

	ATMO_DriverInstanceHandle_t uartInstanceNumber = 0;
	ATMO_MK64F_UART_AddDriverInstance( &uartInstanceNumber );

	// UART2 and UART 3 are exposed on the docking station
	// UART1 is used for debug console
	//	ATMO_UART_Init(1);
	//	ATMO_UART_Init(2);

	// This is the bus most of the sensors are on
	ATMO_I2C_SetConfiguration( ATMO_RPK_SENSOR_I2C_BUS, &atmo_rpk_sensor_i2c_config );
	ATMO_I2C_SetConfiguration( 0, &atmo_rpk_sensor_i2c_config );

	ATMO_MK64F_Auth_Init();

	CCS811_Connect();
	Touch_Controller_Connect();

	ATMO_BATTERY_MK64F_Init();

	ATMO_DriverInstanceHandle_t spiInstanceNumber = 0;
	ATMO_MK64F_SPI_AddDriverInstance( &spiInstanceNumber );
	ATMO_SPI_Init( 0 ); // RTC SPI
	ATMO_SPI_Init( 1 ); // RTC SPI

	backlight_fct_t backlight_fct;
	backlight_fct.connect_hw = Backlight_Connect;
	backlight_fct.disconnect_hw = Backlight_Disconnect;
	backlight_fct.set_level = Backlight_Set_Level;
	Backlight_Init_Driver( &backlight_fct );
	Backlight_Init_Hw();
	Backlight_SetLevel( BLIGHT_LEVEL_HIGH );

	Display_Connect();

	GUI_SetBkColor( GUI_BLACK );
	GUI_SetColor( GUI_WHITE );
	splashTimerId = TMR_AllocateTimer();
	splashScreenCb( NULL );

	ATMO_DriverInstanceHandle_t datetimeInstanceNumber = 0;
	ATMO_MK64F_DateTime_AddDriverInstance( &datetimeInstanceNumber );
	ATMO_DateTime_Init( datetimeInstanceNumber );

	ATMO_DriverInstanceHandle_t blockInstanceNumber = 0;
	ATMO_MK64F_BLOCK_AddDriverInstance( &blockInstanceNumber );
	ATMO_BLOCK_Init( blockInstanceNumber );

	ATMO_DriverInstanceHandle_t fsInstanceNumber = 0;
	ATMO_LFS_FILESYSTEM_AddDriverInstance( &fsInstanceNumber );
	ATMO_FILESYSTEM_Init(fsInstanceNumber , blockInstanceNumber );

	ATMO_FILESYSTEM_Config_t fsConfig;
	fsConfig.numRetries = 1;
	fsConfig.retryDelayMs = 10;
	ATMO_FILESYSTEM_SetConfiguration(fsInstanceNumber, &fsConfig);

	ATMO_FILESYSTEM_Mount( fsInstanceNumber );

	ATMO_DriverInstanceHandle_t bleInstanceNumber = 0;
	ATMO_MK64F_BLE_AddDriverInstance( &bleInstanceNumber );

	if( ATMO_BLE_PeripheralInit( bleInstanceNumber ) != ATMO_BLE_Status_Success )
	{
		bleInitialized = false;
	}
	else
	{
		bleInitialized = true;
	}

	ATMO_DriverInstanceHandle_t nfcInstanceNumber = 0;
	ATMO_MK64F_NFC_AddDriverInstance(&nfcInstanceNumber, bleInstanceNumber);
	ATMO_NFC_Init(nfcInstanceNumber);

	ATMO_THREAD_VOLANSYS_Init();

	ATMO_CLOUD_InitFilesystemData(fsInstanceNumber);

	bool bleEnabled = true;
	ATMO_BLE_GetEnabled(bleInstanceNumber, &bleEnabled);

	ATMO_PLATFORM_DebugPrint("BLE Enabled: %d\r\n", bleEnabled);

	ATMO_MK64F_BLE_SetEnabledOnBoot(NULL, bleEnabled);

	ATMO_DriverInstanceHandle_t cloudInstanceNumber = 0;
	ATMO_CLOUD_BLE_AddDriverInstance( &cloudInstanceNumber, bleInstanceNumber );
	ATMO_CLOUD_Init( cloudInstanceNumber );
	ATMO_CLOUD_SetExtraRequiredSettings(ATMO_CLOUD_ExtraSettings_BLE);

	ATMO_MK64F_UserButton_Init( 0 );
	ATMO_MK64F_Buzzer_Init();

	rgbled_fct_t rgbLedFct;
	rgbLedFct.connect_hw = Rgb_Led_Connect;
	rgbLedFct.disconnect_hw = Rgb_Led_Disconnect;
	rgbLedFct.set_rgb_colors = Rgb_Led_Set_Colors;
	RGB_Led_Init_Driver( &rgbLedFct );
	RGB_Led_Init_Hw();

	ATMO_UI_Page_Init();

	OtapClient_RegisterCbImageDownloadComplete(otaCompleteCallback);
	ATMO_THREAD_VOLANSYS_RegisterOtaCompleteCb(otaCompleteCallback);
	OtapClient_RegisterOtapStatusCallback(OtapStatusRawCallback);
}

void ATMO_PLATFORM_PostInit()
{
	if( bleInitialized )
	{
		ATMO_MK64F_BLE_PostInit();
		gattDbInitialized = true;

		// Grab auth params
		uint8_t uid[16];
		uint8_t cert[128];

		if( ( ATMO_MK64F_Auth_GetUid( uid ) == 0 ) && ( ATMO_MK64F_Auth_GetCert( cert ) == 0 ) )
		{
			ATMO_PLATFORM_DebugPrint( "Writing auth info\r\n" );
			ATMO_MK64F_BLE_WriteAuthUid( uid );
			ATMO_MK64F_BLE_WriteAuthCert( cert );
		}

	}

	ATMO_UI_Page_PostInit();
	ATMO_UI_Page_DisplayRootPage();
}

void ATMO_PLATFORM_DelayMilliseconds( uint32_t milliseconds )
{
	vTaskDelay( milliseconds / portTICK_PERIOD_MS );
}

void* ATMO_Malloc( uint32_t numBytes )
{
	if(numBytes == 0)
	{
		return NULL;
	}

	void* data = MEM_BufferAlloc( numBytes );

	if( data == NULL )
	{
		return NULL;
	}

	return data;
}

void* ATMO_Calloc( size_t num, size_t size )
{
	void* data = MEM_BufferAlloc( num * size );

	if( data )
	{
		memset( data, 0, num * size );
	}

	return data;
}

void ATMO_Free( void* data )
{
	MEM_BufferFree( data );
}

void ATMO_Lock()
{
	xSemaphoreTake( abilityExecMutex, portMAX_DELAY );
}

void ATMO_Unlock()
{
	xSemaphoreGive( abilityExecMutex );
}

int DbgConsole_Printf(const char *fmt_s, ...)
{
	return 0;
}

void ATMO_PLATFORM_DebugPrint( char const* format, ... )
{
	if(debugPrintMutex == NULL)
	{
		debugPrintMutex = xSemaphoreCreateMutex();
	}
	xSemaphoreTake(debugPrintMutex, portMAX_DELAY);
	va_list ap;
	char* pStr = ( char* )ATMO_Malloc( 100 );

	if( !pStr )
	{
		xSemaphoreGive(debugPrintMutex);
		return;
	}

	va_start( ap, format );
	vsnprintf( pStr, 100, format, ap );

	shell_write( pStr );
	Serial_SyncWrite(usbSerialManInterface, pStr, strlen(pStr) + 1);
	ATMO_Free( pStr );

	xSemaphoreGive(debugPrintMutex);

	return;
}

