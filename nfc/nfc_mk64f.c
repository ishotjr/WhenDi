
/**
 ******************************************************************************
 * @file    interval_mk64f.c
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - RapidIoT Interval API Implementation
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


#include "nfc_mk64f.h"
#include "HAL_I2C_driver.h"
#include "HAL_ISR_driver.h"
#include "nfc_device.h"
#include "ntag_bridge.h"
#include "jsmn.h"
#include "ndef_message.h"
#include "board.h"
#include "ble.h"
#include "atmosphere_platform.h"

static NTAG_HANDLE_T ntag_handle;
static ATMO_DriverInstanceHandle_t _bleInstance;

const ATMO_NFC_DriverInstance_t mk64fNfcDriverInstance =
{
	ATMO_MK64F_NFC_Init,
	ATMO_MK64F_NFC_DeInit,
	ATMO_MK64F_NFC_SetConfiguration,
	ATMO_MK64F_NFC_SetMessage,
	ATMO_MK64F_NFC_SetMessagePtr,
	ATMO_MK64F_NFC_GetNumStoredRecords,
	ATMO_MK64F_NFC_GetRecord,
	ATMO_MK64F_NFC_SetMessageReceivedCallback,
	ATMO_MK64F_NFC_SetMessageReceivedAbilityHandle
};

//#define ATMO_NFC_DEBUG

/**
 * Since we are using a single NFC message, and records within that message can change,
 * it's difficult to just "modify" a record in place.
 * 
 * For this reason, whenever one record changes, we rewrite the whole message to EEPROM.
 * 
 * Instead of writing to the EEPROM as the message is serialized, it's about twice as fast to serialize it into
 * this internal buffer first, then do a single I2C write to the NTAG EEPROM
 * 
 * It still takes in the realm of 100ms to write 4 or 5 records to the EEPROM, so it is recommended to not change them very often...
 */
#define ATMO_NFC_INTERNALBUF_SIZE (1024)
static uint8_t nfcInternalBuffer[ATMO_NFC_INTERNALBUF_SIZE] = {0};

#define ATMO_NFC_NUM_DEFAULT_RECORDS (2)
static ATMO_NFC_Record_t defaultRecords[ATMO_NFC_NUM_DEFAULT_RECORDS];

// Erase entire tag
static void __ATMO_MK64F_NFC_FactoryResetTag()
{
	uint8_t page = 8;

	//reset default eeprom memory values (smart poster)
	NFC_WriteBytes( ntag_handle, NTAG_MEM_ADRR_I2C_ADDRESS, Default_BeginingOfMemory, Default_BeginingOfMemory_length );

	//reset pages from 8 to 56
	while( page < 56 )
	{
		NTAG_WriteBlock( ntag_handle, page, Null_Block, NTAG_I2C_BLOCK_SIZE );
		page ++;
	}

	//reset pages 56,57,58
	NTAG_WriteBlock( ntag_handle, 56, Default_Page_56, NTAG_I2C_BLOCK_SIZE );
	NTAG_WriteBlock( ntag_handle, 57, Default_Page_57, NTAG_I2C_BLOCK_SIZE );
	NTAG_WriteBlock( ntag_handle, 58, Default_Page_58, NTAG_I2C_BLOCK_SIZE );

}

static ATMO_NFC_Status_t __ATMO_MK64F_NFC_SyncToEeprom(uint16_t numBytes)
{
	if ( NTAG_OK != NFC_WriteBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY, nfcInternalBuffer, numBytes ) )
	{
		return ATMO_NFC_Status_Fail;
	}

	return ATMO_NFC_Status_Success;
}

/**
 * Write single byte to user memory
 */
static ATMO_NFC_Status_t __ATMO_MK64F_NFC_WriteUserMemoryByte( uint8_t b, uint16_t offset )
{
	nfcInternalBuffer[offset] = b;
	return ATMO_NFC_Status_Success;
}

/**
 * Write buffer to user memory
 */
static ATMO_NFC_Status_t __ATMO_MK64F_NFC_WriteUserMemory( uint8_t* data, uint16_t dataLen,
        uint16_t offset )
{
	memcpy( &nfcInternalBuffer[offset], data, dataLen );
	return ATMO_NFC_Status_Success;
}

/**
 * Calculate length in bytes of a record
 */
static unsigned int __ATMO_MK64F_NFC_CalcRecordLen( ATMO_NFC_Record_t* record )
{
	bool isShortRecord = ATMO_NFC_MSG_GET_SHORT_RECORD( record->header.flags );
	unsigned int len = ( ( isShortRecord ? 4 : 7 ) + record->header.payloadLength + record->header.typeLength );

	if( record->header.idLength == 0 )
	{
		len--;
	}

	return len;
}


/**
 * Read TLV metadata from EEPROM
 */
static ATMO_NFC_Status_t __ATMO_MK64F_NFC_ReadTLVMetaData( unsigned int* totalMsgLen, unsigned int* firstRecordStart )
{
	// Could store this locally since we should be the only ones writing
	// But technically it's not write protected, so better to just read from the EEPROM
	uint8_t header[2] = {0};

	if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY, header, 2 ) != NTAG_OK )
	{
		return ATMO_NFC_Status_Fail;
	}

	if( header[0] != 0x03 )
	{
		return ATMO_NFC_Status_Invalid;
	}

	*totalMsgLen = header[1];

	if( *totalMsgLen == 0xFF )
	{
		// 3 byte length
		if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + 2, header, 2 ) != NTAG_OK )
		{
			return ATMO_NFC_Status_Fail;
		}

		*totalMsgLen = header[0] << 8 | header[1];
		*firstRecordStart = 4;
	}
	else
	{
		*firstRecordStart = 2;
	}

	return ATMO_NFC_Status_Success;
}

/**
 * Write single NDEF record to offset in EEPROM
 */
static ATMO_NFC_Status_t _ATMO_MK64F_NFC_WriteSingleRecord( ATMO_NFC_Record_t* record, unsigned int* currentOffset )
{
	bool isShortRecord = ATMO_NFC_MSG_GET_SHORT_RECORD( record->header.flags );

	// Set ID flag if present
	if( record->header.idLength > 0 )
	{
		record->header.flags |= ATMO_NFC_FLAG_ID( 1 );
	}

	unsigned int headerSize = 0;

	// Max size of header is 7 bytes
	uint8_t headerBuf[7] = {0};
	headerBuf[headerSize++] = record->header.flags;
	headerBuf[headerSize++] = record->header.typeLength;

	// Payload length is different depending on if it's a short record or not
	if( isShortRecord )
	{
		headerBuf[headerSize++] = ( uint8_t )record->header.payloadLength;
	}
	else
	{
		headerBuf[headerSize++] = ( record->header.payloadLength >> 24 ) & 0xFF;
		headerBuf[headerSize++] = ( record->header.payloadLength >> 16 ) & 0xFF;
		headerBuf[headerSize++] = ( record->header.payloadLength >> 8 ) & 0xFF;
		headerBuf[headerSize++] = ( record->header.payloadLength ) & 0xFF;
	}

	// If ID Length present, add it
	if( record->header.idLength > 0 )
	{
		headerBuf[headerSize++] = record->header.idLength;
	}

	// Write header to user memory
	__ATMO_MK64F_NFC_WriteUserMemory( headerBuf, headerSize, *currentOffset );

	( *currentOffset ) += headerSize;

	// Write type, ID
	__ATMO_MK64F_NFC_WriteUserMemory( record->header.payloadType, record->header.typeLength, *currentOffset );
	( *currentOffset ) += record->header.typeLength;

	if( record->header.idLength > 0 )
	{
		__ATMO_MK64F_NFC_WriteUserMemory( record->header.payloadId, record->header.idLength, *currentOffset );
		( *currentOffset ) += record->header.idLength;
	}

	// Write payload
	__ATMO_MK64F_NFC_WriteUserMemory( record->payload, record->header.payloadLength, *currentOffset );
	( *currentOffset ) += record->header.payloadLength;
	return ATMO_NFC_Status_Success;
}

static unsigned int __ATMO_MK64F_NFC_CalcMessageLenPtr( unsigned int numRecords, ATMO_NFC_Record_t** records )
{
	unsigned int len = 0;

	for( int i = 0; i < numRecords; i++ )
	{
		len += __ATMO_MK64F_NFC_CalcRecordLen( records[i] );
	}

	return len;
}


static unsigned int __ATMO_MK64F_NFC_CalcMessageLen( unsigned int numRecords, ATMO_NFC_Record_t* records )
{
	unsigned int len = 0;

	for( int i = 0; i < numRecords; i++ )
	{
		len += __ATMO_MK64F_NFC_CalcRecordLen( &records[i] );
	}

	return len;
}

static bool __ATMO_MK64F_NFC_IsValidRecordHeader( uint8_t* header )
{
	// TODO some actual logic
	return true;
}

ATMO_Status_t ATMO_MK64F_NFC_AddDriverInstance( ATMO_DriverInstanceHandle_t* instanceNumber,
        ATMO_DriverInstanceHandle_t bleInstance )
{
	ATMO_DriverInstanceData_t driverInstance;

	driverInstance.name = "MK64F NFC";
	driverInstance.initialized = false;
	driverInstance.instanceNumber = 0;
	_bleInstance = bleInstance;

	return ATMO_NFC_AddDriverInstance( &mk64fNfcDriverInstance, &driverInstance, instanceNumber );
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_Init( ATMO_DriverInstanceData_t* instance )
{
	HAL_I2C_InitDevice( HAL_I2C_INIT_DEFAULT, I2C2_CLK_SRC, NTAG_I2C_SLAVE_BASEADDR );
	ntag_handle = NTAG_InitDevice( ( NTAG_ID_T )0, NTAG_I2C_SLAVE_BASEADDR );

	Connect_NTAG_A1006();	// Switch Power supply and I2C

	__ATMO_MK64F_NFC_FactoryResetTag();

	// Fill in default records (MAC and App URL)
	ATMO_BLE_MacAddress_t macAddress;
	ATMO_BLE_GetMacAddress( _bleInstance, &macAddress );
	char macStr[128];
	sprintf( macStr, "{\"id\":\"%02X%02X%02X0000%02X%02X%02X\",\"type\":\"2\",\"activetag\":\"1\"}",
	         macAddress.data[0],
	         macAddress.data[1],
	         macAddress.data[2],
	         macAddress.data[3],
	         macAddress.data[4],
	         macAddress.data[5] );

	ATMO_NFC_MakeRecordText( &defaultRecords[0], macStr );
	ATMO_NFC_MakeRecordExternal( &defaultRecords[1], "android.com:pkg", "com.nxp.iot.rapidiot",
	                             strlen( "com.nxp.iot.rapidiot" ) );

	Disconnect_NTAG_A1006();	// Switch Power supply and I2C
	return ATMO_NFC_Status_Success;
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_DeInit( ATMO_DriverInstanceData_t* instance )
{
	return ATMO_NFC_Status_Success;
}
ATMO_NFC_Status_t ATMO_MK64F_NFC_SetConfiguration( ATMO_DriverInstanceData_t* instance, const ATMO_NFC_Config_t* config )
{
	return ATMO_NFC_Status_Success;
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessagePtr( ATMO_DriverInstanceData_t* instance, unsigned int numRecords,
        ATMO_NFC_Record_t** records )
{
	Connect_NTAG_A1006();	// Switch Power supply and I2C

	unsigned int currentOffset = 0;

	// Zero out MB and ME flags for all records
	for(int i = 0; i < ATMO_NFC_NUM_DEFAULT_RECORDS; i++)
	{
		defaultRecords[i].header.flags &= (~(ATMO_NFC_MSG_BEGIN( 1 )));
		defaultRecords[i].header.flags &= (~(ATMO_NFC_MSG_END( 1 )));
	}

	for(int i = 0; i < numRecords; i++)
	{
		records[i]->header.flags &= (~(ATMO_NFC_MSG_BEGIN( 1 )));
		records[i]->header.flags &= (~(ATMO_NFC_MSG_END( 1 )));
	}

	// Always start with the default records
	defaultRecords[0].header.flags |= ATMO_NFC_MSG_BEGIN( 1 );

	if(numRecords > 0)
	{
		records[numRecords - 1]->header.flags |= ATMO_NFC_MSG_END( 1 );
	}
	else
	{
		defaultRecords[ATMO_NFC_NUM_DEFAULT_RECORDS - 1].header.flags |= ATMO_NFC_MSG_END( 1 );
	}

	// Calculate message length of default messages combined with the user messages
	unsigned int totalMsgLen = __ATMO_MK64F_NFC_CalcMessageLen( ATMO_NFC_NUM_DEFAULT_RECORDS,
	                           defaultRecords ) + __ATMO_MK64F_NFC_CalcMessageLenPtr( numRecords, records );

	if(totalMsgLen > ATMO_NFC_INTERNALBUF_SIZE)
	{
		return ATMO_NFC_Status_Fail;
	}

	// TLV NDEV 0x03
	__ATMO_MK64F_NFC_WriteUserMemoryByte( 0x03, currentOffset++ ); // NDEF Message

	// Use 3 byte version if length is bigger than 0xFE
	if( totalMsgLen >= 0xFF )
	{
		uint8_t lenData[3] = {0xFF, ( totalMsgLen >> 8 ) & 0xFF, totalMsgLen & 0xFF};
		__ATMO_MK64F_NFC_WriteUserMemory( lenData, 3, currentOffset ); // Using 3 byte length field
		currentOffset += 3;
	}
	else
	{
		__ATMO_MK64F_NFC_WriteUserMemoryByte( ( uint8_t )totalMsgLen,
		                                      currentOffset++ ); // Total message length
	}

	// Write default records
	for( unsigned int i = 0; i < ATMO_NFC_NUM_DEFAULT_RECORDS; i++ )
	{
		_ATMO_MK64F_NFC_WriteSingleRecord( &defaultRecords[i], &currentOffset );
	}

	// Write user records
	for( unsigned int i = 0; i < numRecords; i++ )
	{
		_ATMO_MK64F_NFC_WriteSingleRecord( records[i], &currentOffset );
	}

	// End TLV
	__ATMO_MK64F_NFC_WriteUserMemoryByte( 0xFE,
	                                      currentOffset++ ); // Total message length

#ifdef ATMO_NFC_DEBUG
	ATMO_PLATFORM_DebugPrint( "\r\nNFC DEBUG\r\n" );

	for( int i = 0; i < currentOffset; i++ )
	{
		ATMO_PLATFORM_DebugPrint( "%02X ", nfcInternalBuffer[i] );
	}

	ATMO_PLATFORM_DebugPrint( "\r\n\r\n" );
#endif

	__ATMO_MK64F_NFC_SyncToEeprom(currentOffset);

	Disconnect_NTAG_A1006();	// Switch Power supply and I2C
	return ATMO_NFC_Status_Success;	
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessage( ATMO_DriverInstanceData_t* instance, unsigned int numRecords,
        ATMO_NFC_Record_t* records )
{
	ATMO_NFC_Record_t *pRecords[numRecords];

	for(unsigned int i = 0; i < numRecords; i++)
	{
		pRecords[i] = &records[i];
	}

	return ATMO_MK64F_NFC_SetMessagePtr(instance, numRecords, pRecords);
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_GetNumStoredRecords( ATMO_DriverInstanceData_t* instance, unsigned int* numRecords )
{
	Connect_NTAG_A1006();	// Switch Power supply and I2C
	unsigned int totalMsgLen = 0;
	unsigned int firstRecordAddr = 0;

	// Grab the total message length and the address of the first record start
	if( __ATMO_MK64F_NFC_ReadTLVMetaData( &totalMsgLen, &firstRecordAddr ) != ATMO_NFC_Status_Success )
	{
		Disconnect_NTAG_A1006();	// Switch Power supply and I2C
		return ATMO_NFC_Status_Fail;
	}

	*numRecords = 0;

	unsigned int currentOffset = 0;

	// While there are still bytes to read
	while( currentOffset < totalMsgLen )
	{
		// Grab the raw header bytes
		uint8_t recordHeaderRaw[7] = {0};

		if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + currentOffset + firstRecordAddr, recordHeaderRaw,
		                   7 ) != NTAG_OK )
		{
			Disconnect_NTAG_A1006();	// Switch Power supply and I2C
			return ATMO_NFC_Status_Fail;
		}

		if( !__ATMO_MK64F_NFC_IsValidRecordHeader( recordHeaderRaw ) )
		{
			Disconnect_NTAG_A1006();	// Switch Power supply and I2C
			return ATMO_NFC_Status_Fail;
		}

		ATMO_NFC_Record_Header_t header;
		unsigned int headerSize = 0;
		header.flags = recordHeaderRaw[headerSize++];
		header.typeLength = recordHeaderRaw[headerSize++];

		// Payload Length
		if( ATMO_NFC_MSG_GET_SHORT_RECORD( recordHeaderRaw[0] ) )
		{
			header.payloadLength = recordHeaderRaw[headerSize++];
		}
		else
		{
			header.payloadLength = recordHeaderRaw[headerSize] << 24 | recordHeaderRaw[headerSize + 1] << 16 | recordHeaderRaw[headerSize +
			                       2] << 8 | recordHeaderRaw[headerSize + 3];
			headerSize += 4;
		}

		// ID if present
		if( ATMO_NFC_FLAG_GET_ID( recordHeaderRaw[0] ) )
		{
			header.idLength = recordHeaderRaw[headerSize++];
		}
		else
		{
			header.idLength = 0;
		}

		// Skip to the beginning of the next record
		currentOffset += header.payloadLength + header.idLength + header.typeLength + headerSize;

		( *numRecords )++;
	}

	Disconnect_NTAG_A1006();	// Switch Power supply and I2C

	return ATMO_NFC_Status_Success;
}

/**
 * TODO nick this is messy
 */
ATMO_NFC_Status_t ATMO_MK64F_NFC_GetRecord( ATMO_DriverInstanceData_t* instance, unsigned int index, ATMO_NFC_Record_t* record )
{
	Connect_NTAG_A1006();	// Switch Power supply and I2C
	unsigned int totalMsgLen = 0;
	unsigned int firstRecordAddr = 0;

	if( __ATMO_MK64F_NFC_ReadTLVMetaData( &totalMsgLen, &firstRecordAddr ) != ATMO_NFC_Status_Success )
	{
		return ATMO_NFC_Status_Fail;
	}

	unsigned int currentOffset = 0;
	unsigned int numRecords = 0;

	bool found = false;

	// While there are bytes left
	while( currentOffset < totalMsgLen )
	{
		// Read raw header data
		uint8_t recordHeaderRaw[7] = {0};

		if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + currentOffset + firstRecordAddr, recordHeaderRaw,
		                   7 ) != NTAG_OK )
		{
			Disconnect_NTAG_A1006();	// Switch Power supply and I2C
			return ATMO_NFC_Status_Fail;
		}

		if( !__ATMO_MK64F_NFC_IsValidRecordHeader( recordHeaderRaw ) )
		{
			Disconnect_NTAG_A1006();	// Switch Power supply and I2C
			return ATMO_NFC_Status_Fail;
		}

		ATMO_NFC_Record_Header_t header;
		memset( &header, 0, sizeof( header ) );
		unsigned int headerSize = 0;
		header.flags = recordHeaderRaw[headerSize++];
		header.typeLength = recordHeaderRaw[headerSize++];

		// Payload Length
		if( ATMO_NFC_MSG_GET_SHORT_RECORD( recordHeaderRaw[0] ) )
		{
			header.payloadLength = recordHeaderRaw[headerSize++];
		}
		else
		{
			header.payloadLength = recordHeaderRaw[headerSize] << 24 | recordHeaderRaw[headerSize + 1] << 16 | recordHeaderRaw[headerSize +
			                       2] << 8 | recordHeaderRaw[headerSize + 3];
			headerSize += 4;
		}

		// ID if present
		if( ATMO_NFC_FLAG_GET_ID( recordHeaderRaw[0] ) )
		{
			header.idLength = recordHeaderRaw[headerSize++];
		}
		else
		{
			header.idLength = 0;
		}

		// This is the record the user requested
		if( numRecords == index )
		{
			found = true;

			memset( record, 0, sizeof( ATMO_NFC_Record_t ) );

			currentOffset += headerSize;

			// Read the rest of the record
			// Read payload type
			if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + currentOffset + firstRecordAddr, header.payloadType,
			                   header.typeLength ) != NTAG_OK )
			{
				Disconnect_NTAG_A1006();	// Switch Power supply and I2C
				return ATMO_NFC_Status_Fail;
			}

			currentOffset += header.typeLength;

			// Read ID if present
			if( ATMO_NFC_FLAG_GET_ID( recordHeaderRaw[0] ) )
			{
				if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + currentOffset + firstRecordAddr, header.payloadId,
				                   header.idLength ) != NTAG_OK )
				{
					Disconnect_NTAG_A1006();	// Switch Power supply and I2C
					return ATMO_NFC_Status_Fail;
				}

				currentOffset += header.idLength;
			}

			memcpy( &record->header, &header, sizeof( header ) );

			// Read payload
			if( NFC_ReadBytes( ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY + currentOffset + firstRecordAddr, record->payload,
			                   header.payloadLength ) != NTAG_OK )
			{
				Disconnect_NTAG_A1006();	// Switch Power supply and I2C
				return ATMO_NFC_Status_Fail;
			}

			break;
		}

		// Skip to the start of the next record
		currentOffset += header.payloadLength + header.idLength + header.typeLength + headerSize;

		numRecords++;
	}

	Disconnect_NTAG_A1006();	// Switch Power supply and I2C

	return found ? ATMO_NFC_Status_Success : ATMO_NFC_Status_Fail;
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessageReceivedCallback( ATMO_DriverInstanceData_t* instance, ATMO_Callback_t cb )
{
	return ATMO_NFC_Status_Success;
}

ATMO_NFC_Status_t ATMO_MK64F_NFC_SetMessageReceivedAbilityHandle( ATMO_DriverInstanceData_t* instance,
        unsigned int abilityHandle )
{
	return ATMO_NFC_Status_Success;
}

