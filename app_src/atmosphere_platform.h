#ifndef ATMO_PLATFORM_H
#define ATMO_PLATFORM_H

#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "./atmosphere_typedefs.h"
#include "./atmosphere_globals.h"
#include "./pin_mapping.h"
#include "./atmosphere_peripherals.h"

#include "ble.h"
#include "adc.h"
#include "interval.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "pwm.h"
#include "uart.h"
#include "datetime.h"
#include "block.h"
#include "filesystem.h"
#include "thread_volansys.h"
#include "backlight.h"
#include "nfc.h"

#include "ui_pagecontrol.h"
#include "ui_template_statictext.h"
#include "ui_template_systemstatus.h"
#include "ui_template_singleicontext.h"
#include "ui_template_iconlines.h"
#include "ui_template_selecticon.h"
#include "rgb_led.h"
#include "user_buttons.h"
#include "buzzer.h"

// Icons
#include "icon_applications_airq.h"
#include "icon_applications.h"
#include "icon_applications_fall.h"
#include "icon_applications_humidity.h"
#include "icon_applications_led.h"
#include "icon_applications_light.h"
#include "icon_applications_motion.h"
#include "icon_applications_pressure.h"
#include "icon_applications_tap.h"
#include "icon_applications_thermostat.h"
#include "icon_info_androqrcode.h"
#include "icon_info.h"
#include "icon_info_iosqrcode.h"
#include "icon_sensors_accmag.h"
#include "icon_sensors_airq.h"
#include "icon_sensors.h"
#include "icon_sensors_gyro.h"
#include "icon_sensors_light.h"
#include "icon_sensors_pressure.h"
#include "icon_sensors_temphum.h"
#include "icon_settings_backlight.h"
#include "icon_settings_bluetooth.h"
#include "icon_settings.h"
#include "icon_settings_buzzer.h"
#include "icon_settings_nfc.h"
#include "icon_settings_reset.h"
#include "icon_settings_sensortag.h"
#include "icon_settings_thread.h"
#include "icon_nxplogoblack.h"
#include "icon_batterycharging.h"

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"

#define ATMO_HAS_BLE (1)
#define ATMO_HAS_WIFI (0)
// #define ATMO_ASYNC_TICK (1)

void ATMO_PLATFORM_Init();

void ATMO_PLATFORM_PostInit();

void ATMO_PLATFORM_DelayMilliseconds(uint32_t milliseconds);

void *ATMO_Malloc(uint32_t numBytes);

void *ATMO_Calloc(size_t num, size_t size);

void ATMO_Free(void *data);

void ATMO_Lock();

void ATMO_Unlock();

void ATMO_PLATFORM_DebugPrint(char const *format, ...);

void ATMO_PLATFORM_SendTickEvent();

bool ATMO_PLATFORM_IsInitComplete();



#endif
