#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>
#include<stdbool.h>

#define MAX_BUFFER_SIZE 20
#define ADC_DATA_SIZE 1
#define ADC_MAX_VAL 0xFFF

typedef struct {
	int16_t data[MAX_BUFFER_SIZE];
	int16_t* input;
	int16_t* output;
	uint8_t size;
	bool overwrite;
} ringBuffer_int16;

typedef struct {
	uint32_t data[MAX_BUFFER_SIZE];
	uint32_t* input;
	uint32_t* output;
	uint8_t size;
	bool overwrite;
} ringBuffer_uint32;

void ringBuffer_int16_Init(ringBuffer_int16* handle, const uint8_t size);

bool ringBuffer_int16_Write(ringBuffer_int16* handle, const int16_t val);

bool ringBuffer_int16_Read(ringBuffer_int16* handle, int16_t* val);

void ringBuffer_int16_Resize(ringBuffer_int16* handle, const uint8_t val);

void ringBuffer_uint32_Init(ringBuffer_uint32* handle, const uint8_t size);

bool ringBuffer_uint32_Write(ringBuffer_uint32* handle, const uint32_t val);

bool ringBuffer_uint32_Read(ringBuffer_uint32* handle, uint32_t* val);

void ringBuffer_uint32_Resize(ringBuffer_uint32* handle, const uint8_t val);

#endif // #ifndef UTILITIES_H_

