#include <assert.h>
#include <stddef.h>
#include "dynarray.h"

#define ARRAY_TYPE int

int main(void) {
	dynarray_t* array = create_dynarray(sizeof(ARRAY_TYPE));
	assert(array != NULL);
	assert(array->size == 0);
	assert(array->capacity == 0);
	assert(array->size_bytes == sizeof(ARRAY_TYPE));
	assert(array->data == NULL);
	free_dynarray(array);
	return 0;
}
