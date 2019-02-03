#ifndef __ATMO_NFC_MK64F_H
#define __ATMO_NFC_MK64F_H

#include "nfc.h"

ATMO_Status_t ATMO_MK64F_NFC_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber,
        ATMO_DriverInstanceHandle_t bleInstance );
ATMO_NFC_Status_t ATMO_MK64F_NFC_Init( ATMO_DriverInstanceData_t* instance );
ATMO_NFC_Status_t ATMO_MK64F_NFC_DeInit( ATMO_DriverInstanceData_t* instance );
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetConfiguration( ATMO_DriverInstanceData_t* instance, const ATMO_NFC_Config_t* config );
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessage( ATMO_DriverInstanceData_t* instance, unsigned int numRecords,
        ATMO_NFC_Record_t* records );
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessagePtr( ATMO_DriverInstanceData_t* instance, unsigned int numRecords,
        ATMO_NFC_Record_t** records );
ATMO_NFC_Status_t ATMO_MK64F_NFC_GetNumStoredRecords( ATMO_DriverInstanceData_t* instance, unsigned int* numRecords );
ATMO_NFC_Status_t ATMO_MK64F_NFC_GetRecord( ATMO_DriverInstanceData_t* instance, unsigned int index,
        ATMO_NFC_Record_t* record );
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessageReceivedCallback( ATMO_DriverInstanceData_t* instance, ATMO_Callback_t cb );
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessageReceivedAbilityHandle( ATMO_DriverInstanceData_t* instance,
        unsigned int abilityHandle );

#endif
