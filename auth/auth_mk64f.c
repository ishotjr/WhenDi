// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "atmosphere_platform.h"
#include "a100x_interface.h"
#include "auth_mk64f.h"

static SemaphoreHandle_t a1006Mutex = NULL;

// #define ATMO_AUTH_DEBUG

const ATMO_I2C_Peripheral_t atmo_a1006_i2c_config =
{
	.baudRate = ATMO_I2C_BaudRate_400kHz,
	.operatingMode = ATMO_I2C_OperatingMode_Master
};

static uint8_t AuthI2CRead_Simple( uint8_t addr, uint8_t* writeBuf, uint32_t writeSize, uint8_t* readBuf, uint32_t readSize )
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterRead( 2, addr, writeBuf, writeSize, readBuf, readSize, 1000 );
	return ( status == ATMO_I2C_Status_Success ) ? 0 : 1;
}

static uint8_t AuthI2CWrite_Simple( uint8_t addr, uint8_t* writeBuf, uint32_t writeSize )
{
	ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite( 2, addr, NULL, 0, writeBuf, writeSize, 1000 );
	return ( status == ATMO_I2C_Status_Success ) ? 0 : 1;
}

uint8_t ATMO_MK64F_Auth_Init()
{
    a1006Mutex = xSemaphoreCreateMutex();
    ATMO_I2C_SetConfiguration( 2, &atmo_a1006_i2c_config ); // A1006 Bus

	// Initialize auth chip
	a1006_IoFunc_t authIo;
	authIo.I2C_Read = AuthI2CRead_Simple;
	authIo.I2C_Write = AuthI2CWrite_Simple;
	authIo.WaitMsec = ATMO_PLATFORM_DelayMilliseconds;
	authIo.i2c_Address = 0x50;
	authIo.cert_slot = 0;
	A1006_Init_Driver( &authIo );
}

uint8_t ATMO_MK64F_Auth_GetUid(uint8_t *uid)
{
#ifdef ATMO_AUTH_DEBUG
    ATMO_PLATFORM_DebugPrint("[AUTH] Getting UID\r\n");
#endif
    xSemaphoreTake( a1006Mutex, portMAX_DELAY );
    Connect_NTAG_A1006();
    uint8_t status = A1006_Get_Uid(uid);
    Disconnect_NTAG_A1006();
    xSemaphoreGive( a1006Mutex );
    return status;
}

uint8_t ATMO_MK64F_Auth_GetCert(uint8_t *cert)
{
#ifdef ATMO_AUTH_DEBUG
    ATMO_PLATFORM_DebugPrint("[AUTH] Getting Cert\r\n");
#endif
    xSemaphoreTake( a1006Mutex, portMAX_DELAY );
    Connect_NTAG_A1006();
    uint8_t status = A1006_Get_Cert(cert);
    Disconnect_NTAG_A1006();
    xSemaphoreGive( a1006Mutex );
    return status;
}

uint8_t ATMO_MK64F_Auth_Authenticate(uint8_t *challenge, uint8_t *response)
{
#ifdef ATMO_AUTH_DEBUG
    ATMO_PLATFORM_DebugPrint("[AUTH] Authenticating\r\n");
#endif
    xSemaphoreTake( a1006Mutex, portMAX_DELAY );
    Connect_NTAG_A1006();
    uint8_t status = A1006_Set_Challenge(challenge);

    if(status == 0)
    {
        ATMO_PLATFORM_DelayMilliseconds(500);
        status = A1006_Get_Response(response);
    }

    Disconnect_NTAG_A1006();
    xSemaphoreGive( a1006Mutex );
    return status;  
}

uint8_t ATMO_MK64F_Auth_SetChallenge(uint8_t *challenge)
{
#ifdef ATMO_AUTH_DEBUG
    ATMO_PLATFORM_DebugPrint("[AUTH] Setting Challenge\r\n");
#endif
    xSemaphoreTake( a1006Mutex, portMAX_DELAY );
    Connect_NTAG_A1006();
    uint8_t status = A1006_Set_Challenge(challenge);
    Disconnect_NTAG_A1006();
    xSemaphoreGive( a1006Mutex );
    return status;
}

uint8_t ATMO_MK64F_Auth_GetResponse(uint8_t *response)
{
#ifdef ATMO_AUTH_DEBUG
    ATMO_PLATFORM_DebugPrint("[AUTH] Getting Response\r\n");
#endif
    xSemaphoreTake( a1006Mutex, portMAX_DELAY );
    Connect_NTAG_A1006();
    uint8_t status = A1006_Get_Response(response);
    Disconnect_NTAG_A1006();
    xSemaphoreGive( a1006Mutex );
    return status;
}