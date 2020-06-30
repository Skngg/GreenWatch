#include "utilities.h"

void ringBuffer_int16_Init(ringBuffer_int16* handle, const uint8_t size) {
	handle->size = size;
	handle->input = &(handle->data)[0];
	handle->output = &(handle->data)[0];
	handle->overwrite = false;
}

bool ringBuffer_int16_Write(ringBuffer_int16* handle, const int16_t val) {
	*(handle->input) = val;

	if (handle->input >= &(handle->data[(handle->size) - 1])) {
		handle->input = &(handle->data[0]);
	}
	else {
		++(handle->input);
	}

	//if (handle->overwrite) {
	//	if (handle->output >= &(handle->data[(handle->size) - 1])) {
	//		handle->output = &(handle->data[0]);
	//	}
	//	else {
	//		++(handle->output);
	//	}
	//	handle->overwrite = false;
	//}

	//if (handle->input >= &(handle->data[(handle->size) - 1])) {
	//	handle->input = &(handle->data[0]);
	//}
	//else {
	//	++(handle->input);
	//}

	//if (handle->input == handle->output) {
	//	handle->overwrite = true;
	//}

	return true;
}

bool ringBuffer_int16_Read(ringBuffer_int16* handle, int16_t* val) {
	
	//handle->overwrite = false;
	//
	//if (handle->output == handle->input) {
	//	return false;
	//}

	*val = *(handle->output);


	if (handle->output >= &(handle->data[(handle->size) - 1])) {
		handle->output = &(handle->data[0]);
	}
	else {
		++(handle->output);
	}

	return true;
}

void ringBuffer_int16_Resize(ringBuffer_int16* handle, const uint8_t val) {
	handle->size = val <= MAX_BUFFER_SIZE ? val : MAX_BUFFER_SIZE;
}

void ringBuffer_uint32_Init(ringBuffer_uint32* handle, const uint8_t size) {
	handle->size = size;
	handle->input = &(handle->data)[0];
	handle->output = &(handle->data)[0];
	handle->overwrite = false;
}

bool ringBuffer_uint32_Write(ringBuffer_uint32* handle, const uint32_t val) {
	*(handle->input) = val;
	
	//if (handle->overwrite) {
	//	if (handle->output >= &(handle->data[(handle->size) - 1])) {
	//		handle->output = &(handle->data[0]);
	//	}
	//	else {
	//		++(handle->output);
	//	}
	//	handle->overwrite = false;
	//}


	if (handle->input >= &(handle->data[(handle->size) - 1])) {
		handle->input = &(handle->data[0]);
	}
	else {
		++(handle->input);
	}

	//if (handle->input == handle->output) {
	//	handle->overwrite = true;
	//}

	return true;
}

bool ringBuffer_uint32_Read(ringBuffer_uint32* handle, uint32_t* val) {
	
	//if (handle->output == handle->input) {
	//	return false;
	//}
	
	*val = *(handle->output);

	//handle->overwrite = false;

	if (handle->output >= &(handle->data[(handle->size) - 1])) {
		handle->output = &(handle->data[0]);
	}
	else {
		++(handle->output);
	}
	
	return true;
}

void ringBuffer_uint32_Resize(ringBuffer_uint32* handle, const uint8_t val) {
	handle->size = val <= MAX_BUFFER_SIZE ? val : MAX_BUFFER_SIZE;
}
