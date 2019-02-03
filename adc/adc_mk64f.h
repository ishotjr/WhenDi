#ifndef __ATMO_ADC_MK64F_H
#define __ATMO_ADC_MK64F_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "adc.h"

#ifdef __cplusplus
	extern "C"{
#endif


/* Exported Function Prototypes -----------------------------------------------*/

/**
 * This routine registers the MK64F ADC driver with the main ADC module
 *
 * @param instanceNumber: Returned instance number
 * @return status
 */
ATMO_Status_t ATMO_MK64F_ADC_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber );

/**
 * This function initializes the ADC driver instance
 *
 * @param instance :The peripheral instance to be initialized
 * @return status
 */
ATMO_ADC_Status_t ATMO_MK64F_ADC_Init(ATMO_DriverInstanceData_t *instance);

/**
 * This routine de-initializes the specified ADC peripheral instance.
 *
 * @param instance :The peripheral instance to be de-initialized.
 * @return status.
 */
ATMO_ADC_Status_t ATMO_MK64F_ADC_DeInit(ATMO_DriverInstanceData_t *instance);

/**
 * This routine initiates an ADC conversion and reads the raw data. The value is formated as
 * signed 2s complement with 'precision' data bits plus sign bit (sign extended). The precision
 * is set during initialization. This is a blocking function and will not return until all
 * samples have been taken or a timeout occurs.
 *
 * @param instance :The peripheral instance from which to read conversion data.
 * @param pin :The ADC pin for which to do a conversion.
 * @param value :The pointer to a variable for storing the value read from the ADC peripheral.
 * @param numSamplesToAverage :The number of data samples to read from the ADC peripheral and calculate an average.
 * @return status.
 */
ATMO_ADC_Status_t ATMO_MK64F_ADC_ReadRaw(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, int32_t *value, uint32_t numSamplesToAverage);

/**
 * This routine initiates an ADC conversion and converts the raw data to voltage. The calculated
 * voltage is returned as an integer where 1 lsb = the specified unit(e.g. volts, millivolts,
 * microvolts). This is a blocking function and will not return until all samples have been taken
 * or a timeout occurs.
 *
 * @param instance :The peripheral instance from which to read conversion data.
 * @param pin :The ADC pin for which to do a conversion.
 * @param units :A scaling factor for formating the measured value.
 * @param voltage :The pointer to a variable for storing the value read from the ADC peripheral.
 * @param numSamplesToAverage :The number of data samples to read from the ADC peripheral and calculate an average.
 * @return status.
 */
ATMO_ADC_Status_t ATMO_MK64F_ADC_Read(ATMO_DriverInstanceData_t *instance, ATMO_GPIO_Device_Pin_t pin, ATMO_ADC_VoltageUnits_t units, int32_t *voltage, uint32_t numSamplesToAverage);

#ifdef __cplusplus
}
#endif

#endif /* __ATMO_ADC__H */
