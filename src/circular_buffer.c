#include "circular_buffer.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "test_macros.h"

circular_buffer_t* create_circular_buffer(unsigned size_bytes,
										  unsigned capacity) {
	when_true_ret(capacity == 0, NULL);
	circular_buffer_t* ret;
	circular_buffer_t* cbuffer = malloc(sizeof(circular_buffer_t));
	when_null_ret(cbuffer, NULL);
	cbuffer->data = malloc(size_bytes * capacity);
	when_null_jmp(cbuffer->data, NULL, exit);

	cbuffer->size_bytes = size_bytes;
	cbuffer->capacity = capacity;
	cbuffer->last = -1;
	cbuffer->first = 0;
	cbuffer->size = 0;

	return cbuffer;
exit:
	free(cbuffer);
	return ret;
}

void free_circular_buffer(circular_buffer_t* b) {
	free(b->data);
	free(b);
}

int circular_buffer_pop_front(circular_buffer_t* b, void* p) {
	if (b->size == 0)
		return -ERROR_IS_EMPTY;
	if (p != NULL)
		memcpy(p, b->data + b->first * b->size_bytes, b->size_bytes);
	b->first = (b->first + 1) % b->capacity;
	b->size--;
	return -ERROR_NO_ERROR;
}

int circular_buffer_pop_back(circular_buffer_t* b, void* p) {
	if (b->size == 0)
		return -ERROR_IS_EMPTY;
	if (p != NULL)
		memcpy(p, b->data + b->last * b->size_bytes, b->size_bytes);
	b->last = (b->last + b->capacity - 1) % b->capacity;
	b->size--;
	return -ERROR_NO_ERROR;
}

int circular_buffer_push_front(circular_buffer_t* b, void* d) {
	if (b->size == (int)b->capacity)
		return -ERROR_CAPACITY_EXCEEDED;
	b->first = (b->first + b->capacity - 1) % b->capacity;
	memcpy(b->data + b->first * b->size_bytes, d, b->size_bytes);
	b->size++;
	return -ERROR_NO_ERROR;
}

int circular_buffer_push_back(circular_buffer_t* b, void* d) {
	if (b->size == (int)b->capacity)
		return -ERROR_CAPACITY_EXCEEDED;
	b->last = (b->last + 1) % b->capacity;
	memcpy(b->data + b->last * b->size_bytes, d, b->size_bytes);
	b->size++;
	return -ERROR_NO_ERROR;
}

int circular_buffer_size(circular_buffer_t* b) {
	return b->size;
}
