#include "block_mk64f.h"
#include "spi_flash_driver.h"
#include "atmosphere_platform.h"

// FreeRTOS Includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

static SemaphoreHandle_t blockEnableMutex = NULL;

ATMO_BLOCK_DriverInstance_t mk64fBlockDriverInstance = {
	ATMO_MK64F_BLOCK_Init,
	ATMO_MK64F_BLOCK_Read,
	ATMO_MK64F_BLOCK_Program,
	ATMO_MK64F_BLOCK_Erase,
	ATMO_MK64F_BLOCK_Sync,
	ATMO_MK64F_BLOCK_GetDeviceInfo
};

static bool ATMO_MK64F_BLOCK_Enabled = true;

#define SPI_FLASH_START_SECTOR (200)
#define SPI_FLASH_START_OFFSET (FLASH_SECTOR_SIZE * SPI_FLASH_START_SECTOR)

#define ATMO_BLOCK_DELAY_MS (2)


ATMO_Status_t ATMO_MK64F_BLOCK_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber)
{
	static ATMO_DriverInstanceData_t driver;
	driver.name = "MK64F Block";
	driver.initialized = false;
	driver.instanceNumber = *instanceNumber;
	driver.argument = NULL;

	return ATMO_BLOCK_AddDriverInstance(&mk64fBlockDriverInstance, &driver, instanceNumber);
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_Init(ATMO_DriverInstanceData_t *instance)
{
	blockEnableMutex = xSemaphoreCreateMutex();

	if( SPI_Flash_Configure(FLASH_PAGE_SIZE, FLASH_SECTOR_SIZE, 256) != 0 )
	{
		return ATMO_BLOCK_Status_Fail;
	}

	return ATMO_BLOCK_Status_Success;
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_Read(ATMO_DriverInstanceData_t *instance, uint32_t block, uint32_t offset, void *buffer, uint32_t size)
{
	if(!ATMO_MK64F_BLOCK_Enabled)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	uint32_t byteOffset = (block * FLASH_SECTOR_SIZE) + offset + SPI_FLASH_START_OFFSET;
	if(SPI_Flash_Read(byteOffset, size, buffer) != 0)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	SPI_Bus_Share_Release_Access();

	ATMO_PLATFORM_DelayMilliseconds(ATMO_BLOCK_DELAY_MS);

	return ATMO_BLOCK_Status_Success;
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_Program(ATMO_DriverInstanceData_t *instance, uint32_t block, uint32_t offset, void *buffer, uint32_t size)
{
	if(!ATMO_MK64F_BLOCK_Enabled)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	uint32_t byteOffset = (block * FLASH_SECTOR_SIZE) + offset + SPI_FLASH_START_OFFSET;
	if(SPI_Flash_Write(byteOffset, size, buffer) != 0)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	SPI_Bus_Share_Release_Access();

	ATMO_PLATFORM_DelayMilliseconds(ATMO_BLOCK_DELAY_MS);

	return ATMO_BLOCK_Status_Success;
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_Erase(ATMO_DriverInstanceData_t *instance, uint32_t block)
{
	if(!ATMO_MK64F_BLOCK_Enabled)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	if(SPI_Flash_Erase_Block(block * FLASH_SECTOR_SIZE + SPI_FLASH_START_OFFSET, FLASH_SECTOR_SIZE) != 0)
	{
		return ATMO_BLOCK_Status_Fail;
	}

	SPI_Bus_Share_Release_Access();

	ATMO_PLATFORM_DelayMilliseconds(ATMO_BLOCK_DELAY_MS);

	return ATMO_BLOCK_Status_Success;
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_Sync(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_BLOCK_Status_Success;
}

ATMO_BLOCK_Status_t ATMO_MK64F_BLOCK_GetDeviceInfo(ATMO_DriverInstanceData_t *instance, ATMO_BLOCK_DeviceInfo_t *info)
{
	info->blockCount = 256 - SPI_FLASH_START_SECTOR;
	info->blockSize = FLASH_SECTOR_SIZE;
	info->progSize = 16;
	info->readSize = 16;
	return ATMO_BLOCK_Status_Success;

}

void ATMO_MK64F_BLOCK_SetEnabled(bool enabled)
{
	xSemaphoreTake( blockEnableMutex, portMAX_DELAY );
	ATMO_MK64F_BLOCK_Enabled = enabled;
	xSemaphoreGive( blockEnableMutex );
}
