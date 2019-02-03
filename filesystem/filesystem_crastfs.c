#include "filesystem_crastfs.h"
#include "../app_src/atmosphere_platform.h"
#include "../block/block.h"

/**
 * CrastFS
 *
 * OVERVIEW
 * ========
 * The CRASTFS has some flash available. With an EEPROM emulator library, we can use it very effectively, albeit
 * with not very much storage space.
 *
 * Given a maximum file size, ATMO_CRASTFS_FILESIZE, the EEPROM is divided into slots. If a slot has the magic number at
 * the beginning, then it's a valid active slot.
 *
 * DESIGN GOALS
 * ============
 * Simplicity. By making each slot the same size, we can avoid segmentation and the need for more overhead.
 */

static ATMO_BLOCK_DeviceInfo_t deviceInfo;
static ATMO_DriverInstanceHandle_t blockHandle = 0;
static unsigned int ATMO_CRASTFS_MaxNumFiles = 0;
#define ATMO_CRASTFS_MAGICWORD (0xABBC4369)

#define ATMO_CRASTFS_FILESIZE (250)
#define ATMO_CRASTFS_FILENAME_SIZE (32)
#define ATMO_CRASTFS_MAX_DATA_SIZE (ATMO_CRASTFS_FILESIZE - sizeof(uint16_t) - ATMO_CRASTFS_FILENAME_SIZE - sizeof(uint32_t))

// Save the first 8 bytes for metadata information
#define ATMO_CRASTFS_METADATA_SIZE (8)
#define ATMO_CRASTFS_DATA_START (ATMO_CRASTFS_METADATA_SIZE)
#define ATMO_CRASTFS_CURRENT_VERSION 1

typedef struct {
	uint16_t version;
	uint8_t metadata[6];
} ATMO_CRASTS_MetaData_t;

typedef struct
{
	uint32_t magicWord;
	char fileName[ATMO_CRASTFS_FILENAME_SIZE];
	uint16_t fileSize;
	uint8_t fileData[ATMO_CRASTFS_MAX_DATA_SIZE];
} ATMO_CRASTFS_File_t;

ATMO_FILESYSTEM_DriverInstance_t CRASTFSFilesystemDriverInstance = {
		ATMO_CRASTFS_FILESYSTEM_Init,
		ATMO_CRASTFS_FILESYSTEM_SetConfiguration,
		ATMO_CRASTFS_FILESYSTEM_Wipe,
		ATMO_CRASTFS_FILESYSTEM_Mount,
		ATMO_CRASTFS_FILESYSTEM_Unmount,
		ATMO_CRASTFS_FILESYSTEM_Remove,
		ATMO_CRASTFS_FILESYSTEM_Rename,
		ATMO_CRASTFS_FILESYSTEM_Stat,
		ATMO_CRASTFS_FILESYSTEM_FileOpen,
		ATMO_CRASTFS_FILESYSTEM_FileClose,
		ATMO_CRASTFS_FILESYSTEM_FileSync,
		ATMO_CRASTFS_FILESYSTEM_FileRead,
		ATMO_CRASTFS_FILESYSTEM_FileWrite,
		ATMO_CRASTFS_FILESYSTEM_FileSeek,
		ATMO_CRASTFS_FILESYSTEM_FileTell,
		ATMO_CRASTFS_FILESYSTEM_FileSize,
		ATMO_CRASTFS_FILESYSTEM_FileRewind,
		ATMO_CRASTFS_FILESYSTEM_FileTruncate,
		ATMO_CRASTFS_FILESYSTEM_DirMk
};

ATMO_Status_t ATMO_CRASTFS_FILESYSTEM_AddDriverInstance(ATMO_DriverInstanceHandle_t *instanceNumber)
{
	ATMO_DriverInstanceData_t *driver = (ATMO_DriverInstanceData_t *)ATMO_Malloc(sizeof(ATMO_DriverInstanceData_t));
	driver->name = "Filesystem";
	driver->initialized = false;
	driver->instanceNumber = *instanceNumber;
	driver->argument = ATMO_Malloc(sizeof(uint32_t));
	*((uint32_t *)driver->argument) = 0;

	return ATMO_FILESYSTEM_AddDriverInstance(&CRASTFSFilesystemDriverInstance, driver, instanceNumber);
}
ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Init(ATMO_DriverInstanceData_t *instance, ATMO_DriverInstanceHandle_t blockDriverHandle)
{
	blockHandle = blockDriverHandle;
	ATMO_BLOCK_GetDeviceInfo(blockHandle, &deviceInfo);

	ATMO_CRASTFS_MaxNumFiles = (deviceInfo.blockCount * deviceInfo.blockSize) / ATMO_CRASTFS_FILESIZE;

	ATMO_PLATFORM_DebugPrint("Reading Metadata\r\n");

	ATMO_CRASTS_MetaData_t metadata;
	if( ATMO_BLOCK_Read(blockHandle, 0, 0, &metadata, sizeof(metadata)) != ATMO_BLOCK_Status_Success )
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	ATMO_PLATFORM_DebugPrint("Detected CRASTFS V%d\r\n", metadata.version);

	if(metadata.version != ATMO_CRASTFS_CURRENT_VERSION)
	{
		ATMO_PLATFORM_DebugPrint("Filesystem version incorrect (current version %d), wiping...\r\n", ATMO_CRASTFS_CURRENT_VERSION);
		ATMO_CRASTFS_FILESYSTEM_Wipe(instance);
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_SetConfiguration(ATMO_DriverInstanceData_t *instance, const ATMO_FILESYSTEM_Config_t *config)
{
	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Wipe(ATMO_DriverInstanceData_t *instance)
{
	ATMO_PLATFORM_DebugPrint("Wipe started\r\n");
	for(unsigned int i = 0; i < deviceInfo.blockCount; i++)
	{
		ATMO_BLOCK_Erase(blockHandle, i);
	}
	ATMO_PLATFORM_DebugPrint("Wipe ended\r\n");

	// Write metadata
	ATMO_CRASTS_MetaData_t metadata;
	memset(&metadata, 0, sizeof(metadata));
	metadata.version = ATMO_CRASTFS_CURRENT_VERSION;

	if(ATMO_BLOCK_Program(blockHandle, 0, 0, &metadata, sizeof(metadata)) != ATMO_BLOCK_Status_Success)
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Mount(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Unmount(ATMO_DriverInstanceData_t *instance)
{
	return ATMO_FILESYSTEM_Status_Success;
}

static int ATMO_CRASTFS_FILESYSTEM_FindFreeOffset()
{
	ATMO_CRASTFS_File_t file;
	unsigned int i = 0;

	for(i = 0; i < ATMO_CRASTFS_MaxNumFiles; i++)
	{
		uint32_t magicWord = 0;
		unsigned int totalOffset = (i * ATMO_CRASTFS_FILESIZE) + ATMO_CRASTFS_DATA_START;;
		unsigned int block = totalOffset / deviceInfo.blockSize;
		totalOffset = totalOffset - (block * deviceInfo.blockSize);

		if( ATMO_BLOCK_Read(blockHandle, block, totalOffset, &magicWord, sizeof(magicWord)) != ATMO_BLOCK_Status_Success )
		{
			return -1;
		}

		// We've hit the end
		if(magicWord != ATMO_CRASTFS_MAGICWORD)
		{
			return (i * ATMO_CRASTFS_FILESIZE) + ATMO_CRASTFS_DATA_START;
		}
	}

	return -1;
}

static int ATMO_CRASTFS_FILESYSTEM_FindFileOffset(const char *path)
{
	ATMO_CRASTFS_File_t file;
	unsigned int i = 0;

	for(i = 0; i < ATMO_CRASTFS_MaxNumFiles; i++)
	{
		uint32_t magicWord = 0;
		unsigned int totalOffset = (i * ATMO_CRASTFS_FILESIZE) + ATMO_CRASTFS_DATA_START;
		unsigned int block = totalOffset / deviceInfo.blockSize;
		totalOffset = totalOffset - (block * deviceInfo.blockSize);

		if( ATMO_BLOCK_Read(blockHandle, block, totalOffset, &magicWord, sizeof(magicWord)) != ATMO_BLOCK_Status_Success )
		{
			return -1;
		}

		// We've hit the end or empty file
		if(magicWord != ATMO_CRASTFS_MAGICWORD)
		{
			if(i == (ATMO_CRASTFS_MaxNumFiles - 1))
			{
				return -1;
			}

			continue;
		}

		totalOffset = (i * ATMO_CRASTFS_FILESIZE) + 4 + ATMO_CRASTFS_DATA_START;
		block = totalOffset / deviceInfo.blockSize;
		totalOffset = totalOffset - (block * deviceInfo.blockSize);

		// Read the file name
		if( ATMO_BLOCK_Read(blockHandle, block, totalOffset, file.fileName, ATMO_CRASTFS_FILENAME_SIZE) != ATMO_BLOCK_Status_Success )
		{
			return -1;
		}

		if(strcmp(file.fileName, path) == 0)
		{
			break;
		}
	}

	if(i == ATMO_CRASTFS_MaxNumFiles)
	{
		return -1;
	}

	return (i * ATMO_CRASTFS_FILESIZE) + ATMO_CRASTFS_DATA_START;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Remove(ATMO_DriverInstanceData_t *instance, const char *path)
{
	int offset = ATMO_CRASTFS_FILESYSTEM_FindFileOffset(path);

	if(offset >= 0)
	{
		// Erase magic word
		uint32_t badWord = 0;
		unsigned int block = offset / deviceInfo.blockSize;
		offset = offset - (block * deviceInfo.blockSize);
		if( ATMO_BLOCK_Program(blockHandle, block, offset, &badWord, 4) != ATMO_BLOCK_Status_Success )
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Rename(ATMO_DriverInstanceData_t *instance, const char *oldPath, const char *newPath)
{
	int offset = ATMO_CRASTFS_FILESYSTEM_FindFileOffset(oldPath);

	if(offset >= 0)
	{
		// Update name
		if(strlen(newPath) >= ATMO_CRASTFS_FILENAME_SIZE)
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}

		unsigned int block = (offset + 4) / deviceInfo.blockSize;
		offset = (offset + 4) - (block * deviceInfo.blockSize);

		if( ATMO_BLOCK_Program(blockHandle, block, offset, newPath, strlen(newPath) + 1) != ATMO_BLOCK_Status_Success )
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}
	}
	else
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_Stat(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_Info_t *info, const char *path)
{
	ATMO_CRASTFS_File_t file;
	int totalOffset = ATMO_CRASTFS_FILESYSTEM_FindFileOffset(path);

	if(totalOffset >= 0)
	{
		// Erase magic word
		uint32_t badWord = 0;
		unsigned int block = (totalOffset + 4) / deviceInfo.blockSize;
		unsigned int offset = (totalOffset + 4) - (block * deviceInfo.blockSize);
		if( ATMO_BLOCK_Read(blockHandle, block, offset, info->name, ATMO_CRASTFS_FILENAME_SIZE) != ATMO_BLOCK_Status_Success )
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}

		block = (totalOffset + 68) / deviceInfo.blockSize;
		offset = (totalOffset + 68) - (block * deviceInfo.blockSize);
		if( ATMO_BLOCK_Read(blockHandle, block, offset, file.fileSize, sizeof(file.fileSize)) != ATMO_BLOCK_Status_Success )
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}

		info->size = file.fileSize;
		info->type = ATMO_FILESYSTEM_Type_File;
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileOpen(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, const char *path, int flags)
{
	ATMO_PLATFORM_DebugPrint("Trying to open %s\r\n", path);
	// Find file offset
	int offset = ATMO_CRASTFS_FILESYSTEM_FindFileOffset(path);

	ATMO_PLATFORM_DebugPrint("Trying to open %s offset %d\r\n", path, offset);

	if(offset >= 0)
	{
		file->data = ATMO_Malloc(sizeof(int));
		*((int *)file->data) = offset;
	}
	else
	{
		// Should we create it?
		if(flags & ATMO_CREAT)
		{
			offset = ATMO_CRASTFS_FILESYSTEM_FindFreeOffset();

			// No more space
			if(offset < 0)
			{
				return ATMO_FILESYSTEM_Status_Fail;
			}

			ATMO_CRASTFS_File_t sensorFile;
			sensorFile.magicWord = ATMO_CRASTFS_MAGICWORD;

			if(strlen(path) >= ATMO_CRASTFS_FILENAME_SIZE)
			{
				return ATMO_FILESYSTEM_Status_Fail;
			}

			strcpy(sensorFile.fileName, path);
			sensorFile.fileSize = 0;

			unsigned int block = offset / deviceInfo.blockSize;
			offset = offset - (block * deviceInfo.blockSize);
			if( ATMO_BLOCK_Program(blockHandle, block, offset, &sensorFile, sizeof(sensorFile)) != ATMO_BLOCK_Status_Success )
			{
				return ATMO_FILESYSTEM_Status_Fail;
			}

			file->data = ATMO_Malloc(sizeof(int));
			*((int *)file->data) = offset;
		}
		else
		{
			return ATMO_FILESYSTEM_Status_Fail;
		}
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileClose(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file)
{
	ATMO_Free(file->data);
	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileSync(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file)
{
	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileRead(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, void *buffer, uint32_t size)
{
	ATMO_CRASTFS_File_t sensorFile;
	unsigned int block = *((int *)file->data) / deviceInfo.blockSize;
	unsigned int offset = *((int *)file->data) - (block * deviceInfo.blockSize);
	if( ATMO_BLOCK_Read(blockHandle, block, offset, &sensorFile, sizeof(sensorFile)) != ATMO_BLOCK_Status_Success )
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	memcpy(buffer, sensorFile.fileData, sensorFile.fileSize > size ? size : sensorFile.fileSize);
	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileWrite(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, void *buffer, uint32_t size)
{
	ATMO_CRASTFS_File_t sensorFile;
	unsigned int block = *((int *)file->data) / deviceInfo.blockSize;
	unsigned int offset = *((int *)file->data) - (block * deviceInfo.blockSize);
	if( ATMO_BLOCK_Read(blockHandle, block, offset, &sensorFile, sizeof(sensorFile)) != ATMO_BLOCK_Status_Success )
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	if(size > ATMO_CRASTFS_MAX_DATA_SIZE)
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	memcpy(sensorFile.fileData, (uint8_t *)buffer, size);
	sensorFile.fileSize = size;

	if( ATMO_BLOCK_Program(blockHandle, block, offset, &sensorFile, sizeof(sensorFile)) != ATMO_BLOCK_Status_Success )
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileSeek(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, uint32_t offset)
{
	return ATMO_FILESYSTEM_Status_NotSupported;
}
ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileTell(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, uint32_t *currPos)
{
	return ATMO_FILESYSTEM_Status_NotSupported;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileSize(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, uint32_t *size)
{
	ATMO_CRASTFS_File_t sensorFile;
	unsigned int block = *((int *)file->data) / deviceInfo.blockSize;
	unsigned int offset = *((int *)file->data) - (block * deviceInfo.blockSize);
	if( ATMO_BLOCK_Read(blockHandle, block, offset, &sensorFile, sizeof(sensorFile)) != ATMO_BLOCK_Status_Success )
	{
		return ATMO_FILESYSTEM_Status_Fail;
	}

	*size = sensorFile.fileSize;

	return ATMO_FILESYSTEM_Status_Success;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileRewind(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file)
{
	return ATMO_FILESYSTEM_Status_NotSupported;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_FileTruncate(ATMO_DriverInstanceData_t *instance, ATMO_FILESYSTEM_File_t *file, uint32_t size)
{
	return ATMO_FILESYSTEM_Status_NotSupported;
}

ATMO_FILESYSTEM_Status_t ATMO_CRASTFS_FILESYSTEM_DirMk(ATMO_DriverInstanceData_t *instance, const char *path)
{
	return ATMO_FILESYSTEM_Status_NotSupported;
}
