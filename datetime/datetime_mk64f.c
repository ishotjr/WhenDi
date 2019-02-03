#include "datetime_mk64f.h"
#include "pcf2123.h"
#include "atmosphere_platform.h"
#include "spi.h"
#include "pin_mux.h"
#include "time.h"
#include "fsl_dspi.h"
#include "spi_bus_share.h"

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define DATETIME_NUM_MIN_CALLBACKS (10)

ATMO_DateTime_DriverInstance_t mk64fDateTimeDriverInstance = {
	ATMO_MK64F_DateTime_Init,
	ATMO_MK64F_DateTime_DeInit,
	ATMO_MK64F_DateTime_SetConfiguration,
	ATMO_MK64F_DateTime_GetDateTime,
	ATMO_MK64F_DateTime_GetDateTimeEpoch,
	ATMO_MK64F_DateTime_GetDateTimeIsoStr,
	ATMO_MK64F_DateTime_SetDateTime,
	ATMO_MK64F_DateTime_SetDateTimeEpoch,
	ATMO_MK64F_DateTime_SetDateTimeIsoStr
};

static uint32_t currentNumCallbacks = 0;
static ATMO_Callback_t minCb[DATETIME_NUM_MIN_CALLBACKS] = {NULL};
static bool initialTimeSet = false;
static bool ATMO_MK64F_DATETIME_enabled = true;

static SemaphoreHandle_t datetimeEnableMutex = NULL;

#define RTC_CS_PIN (PTB9)

static ATMO_SPI_Device_t spiConfig = {
		false,
		true,
		false,
		5000000,
		ATMO_SPI_ClockMode_0,
		true
};


static uint8_t ATMO_MK64F_DateTime_SpiWrite(uint8_t *buf, uint32_t size)
{
	SPI_Bus_Share_Get_Access();

	if( ATMO_SPI_MasterWrite(1, RTC_CS_PIN, NULL, 0, buf, size, 1000) != ATMO_SPI_Status_Success)
	{
		ATMO_PLATFORM_DebugPrint("[DateTime]SPI Write Error\r\n");
		SPI_Bus_Share_Release_Access();
		return 1;
	}

	SPI_Bus_Share_Release_Access();


	return 0;
}

static uint8_t ATMO_MK64F_DateTime_SpiRead(uint8_t *writeBuf, uint8_t *readBuf, uint32_t size)
{

	uint8_t tempWrBuf[size + 1];
	memset(tempWrBuf, 0, size + 1);
	memset(readBuf, 0, size + 1);
	tempWrBuf[0] = writeBuf[0];
	
	SPI_Bus_Share_Get_Access();

	if( ATMO_SPI_MasterRead(1, RTC_CS_PIN, tempWrBuf, 1, readBuf, size, 1000) != ATMO_SPI_Status_Success)
	{
		ATMO_PLATFORM_DebugPrint("[DateTime]SPI Read Error\r\n");
		SPI_Bus_Share_Release_Access();
		return 1;
	}

	SPI_Bus_Share_Release_Access();


	return 0;
}

static void ATMO_MK64F_DateTime_MinCb(void *data)
{
	PCF2123_ClearMinSecInterruptFlag();
	PCF2123_SetMinSecInterrupt(true, false, false);

	for(uint32_t i = 0; i < currentNumCallbacks; i++)
	{
		minCb[i](NULL);
	}
}

ATMO_Status_t ATMO_MK64F_DateTime_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber)
{
	ATMO_DriverInstanceData_t *driver = ATMO_Malloc(sizeof(ATMO_DriverInstanceData_t));
	driver->name = "MK64F Date Time";
	driver->initialized = false;
	driver->instanceNumber = *instanceNumber;
	driver->argument = NULL;

	return ATMO_DateTime_AddDriverInstance(&mk64fDateTimeDriverInstance, driver, instanceNumber);
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_Init(ATMO_DriverInstanceData_t *instance)
{
	ATMO_SPI_MasterSetDeviceConfiguration(1, RTC_CS_PIN, &spiConfig);

	pcf2123_IoFunc_t io;
	io.SPI_Write = ATMO_MK64F_DateTime_SpiWrite;
	io.SPI_Read = ATMO_MK64F_DateTime_SpiRead;
	io.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;

	settingsPCF_t settings;
	settings.MinInterrupt = true; // Interrupt when minute changes
	settings.PulseInterrupt = false;
	settings.SecInterrupt = false;
	settings.Softreset = true;
	settings.clockOutputFreq = clkout_High_Z;
	settings.days = 1;
	settings.hours = 2;
	settings.minutes = 3;
	settings.mode12_24 = PCF2123_MODE_24HOURS;
	settings.months = 4;
	settings.seconds = 14;
	settings.weekdays = Tuesday;
	settings.years = 18;

	ATMO_GPIO_Config_t intPinConfig;
	intPinConfig.pinMode = ATMO_GPIO_PinMode_Input_PullUp;
	ATMO_GPIO_SetPinConfiguration(0, BOARD_INITPINS_RTC_INT_PIN_NAME, &intPinConfig);

	datetimeEnableMutex = xSemaphoreCreateMutex();

	ATMO_GPIO_RegisterInterruptCallback(0, BOARD_INITPINS_RTC_INT_PIN_NAME, ATMO_GPIO_InterruptTrigger_FallingEdge, ATMO_MK64F_DateTime_MinCb);

	// Initialize RTC
	PCF2123_Init_Driver(&io);

	if( PCF2123_Init_Hw(&settings, false) != PCF2123_SUCCESS )
	{
		return ATMO_DateTime_Status_Fail;
	}

	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_DeInit(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetConfiguration(ATMO_DriverInstanceData_t *instance, const ATMO_DateTime_Config_t *config)
{
	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTime(ATMO_DriverInstanceData_t *instance, ATMO_DateTime_Time_t *datetime)
{
	if(!initialTimeSet || !ATMO_MK64F_DATETIME_enabled)
	{
		memset(datetime, 0, sizeof(ATMO_DateTime_Time_t));
		return ATMO_DateTime_Status_Success;
	}

	settingsPCF_t pcfSettings;

	if( PCF2123_GetDateTime(&pcfSettings) != PCF2123_SUCCESS )
	{
		return ATMO_DateTime_Status_Fail;
	}

	datetime->days = (((pcfSettings.days >> 4) & 0x3) * 10) + (pcfSettings.days & 0xF);
	datetime->hours = (((pcfSettings.hours >> 4) & 0x3) * 10) + (pcfSettings.hours & 0xF);
	datetime->minutes = (((pcfSettings.minutes >> 4) & 0x7) * 10) + (pcfSettings.minutes & 0xF);
	datetime->month = (((pcfSettings.months >> 4) & 0x1) * 10) + (pcfSettings.months & 0xF);
	datetime->seconds = (((pcfSettings.seconds >> 4) & 0x7) * 10) + (pcfSettings.seconds & 0xF);
	datetime->weekday = pcfSettings.weekdays;
	datetime->years = (((pcfSettings.years >> 4) & 0xF) * 10) + (pcfSettings.years & 0xF);

	return ATMO_DateTime_Status_Success;
}

static ATMO_DateTime_Time_t ATMO_MK64F_DateTime_TmToAtmoTime(struct tm *time)
{
	ATMO_DateTime_Time_t atmoTime;
	atmoTime.days = time->tm_mday;
	atmoTime.hours = time->tm_hour;
	atmoTime.minutes = time->tm_min;
	atmoTime.month = time->tm_mon + 1;
	atmoTime.seconds = time->tm_sec;
	atmoTime.weekday = time->tm_wday;
	atmoTime.years = time->tm_year - 100;
	return atmoTime;

}


struct tm ATMO_MK64F_DateTime_AtmoTimeToTm(ATMO_DateTime_Time_t *atmoTime)
{
	struct tm time;
	time.tm_mday = atmoTime->days;
	time.tm_isdst = 0;
	time.tm_hour = atmoTime->hours;
	time.tm_min = atmoTime->minutes;
	time.tm_mon = atmoTime->month - 1;
	time.tm_sec = atmoTime->seconds;
	time.tm_wday = atmoTime->weekday;
	time.tm_yday = 0;
	time.tm_year = atmoTime->years + 100;

	return time;

}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTimeEpoch(ATMO_DriverInstanceData_t *instance, uint32_t *datetime)
{
	ATMO_DateTime_Time_t atmoTime;

	if( ATMO_MK64F_DateTime_GetDateTime(instance, &atmoTime) != ATMO_DateTime_Status_Success )
	{
		return ATMO_DateTime_Status_Fail;
	}

	struct tm time = ATMO_MK64F_DateTime_AtmoTimeToTm(&atmoTime);
	*datetime = mktime(&time);
	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTimeIsoStr(ATMO_DriverInstanceData_t *instance, char *datetime, uint32_t bufferLen)
{
	ATMO_DateTime_Time_t atmoTime;

	if( ATMO_MK64F_DateTime_GetDateTime(instance, &atmoTime) != ATMO_DateTime_Status_Success )
	{
		return ATMO_DateTime_Status_Fail;
	}

	struct tm time = ATMO_MK64F_DateTime_AtmoTimeToTm(&atmoTime);

	strftime(datetime, bufferLen, "%Y-%m-%dT%H:%M:%S", &time);
	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTime(ATMO_DriverInstanceData_t *instance, ATMO_DateTime_Time_t *datetime)
{
	if(!ATMO_MK64F_DATETIME_enabled)
	{
		return ATMO_DateTime_Status_Success;
	}

	settingsPCF_t pcfSettings;

	pcfSettings.days = (datetime->days / 10) << 4 | (datetime->days % 10);
	pcfSettings.hours = (datetime->hours / 10) << 4 | (datetime->hours % 10);
	pcfSettings.minutes = (datetime->minutes / 10) << 4 | (datetime->minutes % 10);
	pcfSettings.months = (datetime->month / 10) << 4 | (datetime->month % 10);
	pcfSettings.seconds = (datetime->seconds / 10) << 4 | (datetime->seconds % 10);
	pcfSettings.weekdays = datetime->weekday;
	pcfSettings.years = (datetime->years / 10) << 4 | (datetime->years % 10);

	if( PCF2123_SetDateTime(&pcfSettings) != PCF2123_SUCCESS )
	{
		return ATMO_DateTime_Status_Fail;
	}

	initialTimeSet =  true;

	for(uint32_t i = 0; i < currentNumCallbacks; i++)
	{
		minCb[i](NULL);
	}

	return ATMO_DateTime_Status_Success;
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTimeEpoch(ATMO_DriverInstanceData_t *instance, uint32_t datetime)
{
	char str[32];
	sprintf(str, "%d", datetime);
	struct tm time;

	strptime(str, "%s", &time);

	ATMO_DateTime_Time_t atmoTime = ATMO_MK64F_DateTime_TmToAtmoTime(&time);
	return ATMO_MK64F_DateTime_SetDateTime(instance, &atmoTime);
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTimeIsoStr(ATMO_DriverInstanceData_t *instance, const char* datetime)
{
	struct tm time;

	strptime(datetime, "%Y-%m-%dT%H:%M:%S", &time);

	ATMO_DateTime_Time_t atmoTime = ATMO_MK64F_DateTime_TmToAtmoTime(&time);
	return ATMO_MK64F_DateTime_SetDateTime(instance, &atmoTime);
}

ATMO_DateTime_Status_t ATMO_MK64F_DateTime_RegisterMinChangeCb(ATMO_Callback_t callback)
{
	if(currentNumCallbacks >= DATETIME_NUM_MIN_CALLBACKS)
	{
		return ATMO_DateTime_Status_Fail;
	}

	minCb[currentNumCallbacks++] = callback;
	
	return ATMO_DateTime_Status_Success;
}

void ATMO_MK64F_DateTime_SetEnabled(bool enabled)
{
	xSemaphoreTake( datetimeEnableMutex, portMAX_DELAY );
	if( enabled && (!ATMO_MK64F_DATETIME_enabled))
	{
		PCF2123_SetMinSecInterrupt(true, false, false);
	}
	else if((!enabled) && (ATMO_MK64F_DATETIME_enabled))
	{
		PCF2123_SetMinSecInterrupt(false, false, false);
	}
	ATMO_MK64F_DATETIME_enabled = enabled;
	xSemaphoreGive( datetimeEnableMutex );
}


