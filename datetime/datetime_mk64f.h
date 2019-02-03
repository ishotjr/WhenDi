 /* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ATMO_MK64F_DateTime__H
#define __ATMO_MK64F_DateTime__H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "datetime.h"


#ifdef __cplusplus
	extern "C"{
#endif

/* Exported Constants --------------------------------------------------------*/

/* Exported Macros -----------------------------------------------------------*/

/* Exported Types ------------------------------------------------------------*/

ATMO_Status_t ATMO_MK64F_DateTime_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber);

/**
 * Initialize DateTime Driver
 *
 * @param[in] instance
 *
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_Init(ATMO_DriverInstanceData_t *instance);

/**
 * This routine de-initializes the datetime driver
 *
 * @param[in] instance
 * @return status.
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_DeInit(ATMO_DriverInstanceData_t *instance);

/**
 * Set datetime configuration
 *
 * @param[in] instance
 * @param[in] config
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetConfiguration(ATMO_DriverInstanceData_t *instance, const ATMO_DateTime_Config_t *config);

/**
 * Retrieve the current date and time
 *
 * @param[in] instance
 * @param[out] datetime
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTime(ATMO_DriverInstanceData_t *instance, ATMO_DateTime_Time_t *datetime);

/**
 * Retrieve the current date and time
 *
 * @param[in] instance
 * @param[out] datetime - seconds since unix epoch
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTimeEpoch(ATMO_DriverInstanceData_t *instance, uint32_t *datetime);

/**
 * Retrieve the current date and time
 *
 * @param[in] instance
 * @param[out] datetime - ISO 8601 String
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_GetDateTimeIsoStr(ATMO_DriverInstanceData_t *instance, char *datetime, uint32_t bufferLen);

/**
 * Set the current date and time
 *
 * @param[in] instance
 * @param[in] datetime
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTime(ATMO_DriverInstanceData_t *instance, ATMO_DateTime_Time_t *datetime);

/**
 * Set the current date and time
 *
 * @param[in] instance
 * @param[in] datetime - seconds since unix epoch
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTimeEpoch(ATMO_DriverInstanceData_t *instance, uint32_t datetime);

/**
 * Set the current date and time
 *
 * @param[in] instance
 * @param[in] datetime - ISO 8601 String
 * @return status
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_SetDateTimeIsoStr(ATMO_DriverInstanceData_t *instance, const char* datetime);

/**
 * Convert ATMO Time structure to a standard struct tm
 */
struct tm ATMO_MK64F_DateTime_AtmoTimeToTm(ATMO_DateTime_Time_t *atmoTime);

/**
 * Register callback to be called when the minute changes or the time was set
 */
ATMO_DateTime_Status_t ATMO_MK64F_DateTime_RegisterMinChangeCb(ATMO_Callback_t callback);

void ATMO_MK64F_DateTime_SetEnabled(bool enabled);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_MK64F_DateTime__H */
