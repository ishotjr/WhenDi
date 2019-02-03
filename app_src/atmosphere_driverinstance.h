
#ifndef __ATMO_DRIVERINSTANCE__H
#include "../app_src/atmosphere_typedefs.h"

#define ATMO_INSTANCE(DRIVER, NAME) ATMO_DRIVERINSTANCE_ ## DRIVER ## _ ## NAME

static unsigned int ATMO_DRIVERINSTANCE_ADC_ADC1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_BLE_BLE1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_BLOCK_BLOCK1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_DATETIME_DATETIME1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_FILESYSTEM_FILESYSTEM1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_GPIO_GPIO1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_I2C_I2C1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_I2C_I2C2 = 1;
static unsigned int ATMO_DRIVERINSTANCE_INTERVAL_INTERVAL1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_NFC_NFC1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_PWM_PWM1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_SPI_SPI1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_UART_UART1 = 0;
static unsigned int ATMO_DRIVERINSTANCE_UART_UART2 = 1;
static unsigned int ATMO_DRIVERINSTANCE_UART_UART3 = 2;
static unsigned int ATMO_DRIVERINSTANCE_UART_UART4 = 3;

#endif
