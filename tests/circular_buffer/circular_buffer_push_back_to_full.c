#include <assert.h>
#include <stddef.h>
#include "circular_buffer.h"
#include "errors.h"

#define BUF_LEN 10
#define BUF_TYPE int

BUF_TYPE data[BUF_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	circular_buffer_t* buffer =
		create_circular_buffer(sizeof(BUF_TYPE), BUF_LEN);
	int i = 0;
	while (circular_buffer_push_back(buffer, &data[i++]) == ERROR_NO_ERROR)
		;
	for (i = 0; i < BUF_LEN; i++)
		assert(((int*)buffer->data)[i] == i);
	free_circular_buffer(buffer);
	return 0;
}
