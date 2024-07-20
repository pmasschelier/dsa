#include <assert.h>
#include <stddef.h>
#include "circular_buffer.h"

#define BUF_LEN 10
#define BUF_TYPE int

int main(void) {
	circular_buffer_t* buffer =
		create_circular_buffer(sizeof(BUF_TYPE), BUF_LEN);
	assert(buffer != NULL);
	assert(buffer->capacity == BUF_LEN);
	assert(buffer->first == 0);
	assert(buffer->last == -1);
	assert(buffer->size == 0);
	assert(buffer->data != NULL);
	assert(buffer->size_bytes == sizeof(BUF_TYPE));
	free_circular_buffer(buffer);
	return 0;
}
