#include "atmosphere_ringbuffer.h"

bool ATMO_RingBuffer_Init(ATMO_RingBuffer_t *buf, uint32_t capacity, uint32_t elementSize)
{
	buf->entry = ATMO_Malloc(capacity * elementSize);

	if(buf->entry == NULL)
	{
		return false;
	}

	buf->head = 0;
	buf->tail = 0;
	buf->count = 0;
	buf->capacity = capacity;
	buf->elementSize = elementSize;
	return true;
}

bool ATMO_RingBuffer_InitWithBuf(ATMO_RingBuffer_t *buf, uint8_t *bufData, uint32_t capacity, uint32_t elementSize)
{
	buf->entry = bufData;

	if(buf->entry == NULL)
	{
		return false;
	}

	buf->head = 0;
	buf->tail = 0;
	buf->count = 0;
	buf->capacity = capacity;
	buf->elementSize = elementSize;
	return true;
}


bool ATMO_RingBuffer_Full(ATMO_RingBuffer_t *buf)
{
	return buf->count >= buf->capacity;
}

bool ATMO_RingBuffer_Empty(ATMO_RingBuffer_t *buf)
{
	return buf->count == 0;
}

void *ATMO_RingBuffer_Head(ATMO_RingBuffer_t *buf)
{
	uint8_t *bufBytes = (uint8_t *)buf->entry;
	return &bufBytes[buf->head * buf->elementSize];
}

void *ATMO_RingBuffer_Index(ATMO_RingBuffer_t *buf, uint32_t index)
{
	if(index > buf->count) {
		return NULL;
	}
	
	uint8_t *bufBytes = (uint8_t *)buf->entry;
	return &bufBytes[index * buf->elementSize];
}

void *ATMO_RingBuffer_Tail(ATMO_RingBuffer_t *buf)
{
	uint8_t *bufBytes = (uint8_t *)buf->entry;
	return &bufBytes[buf->tail * buf->elementSize];
}

void *ATMO_RingBuffer_Pop(ATMO_RingBuffer_t *buf)
{
	void *entry = ATMO_RingBuffer_Head(buf);
	buf->head = (buf->head + 1) % buf->capacity;
	buf->count--;
	return entry;
}

bool ATMO_RingBuffer_Push(ATMO_RingBuffer_t *buf, void* data)
{
	void *entry = ATMO_RingBuffer_Tail(buf);
	memcpy(entry, data, buf->elementSize);
	buf->tail = (buf->tail + 1) % buf->capacity;
	buf->count++;
	return true;
}
