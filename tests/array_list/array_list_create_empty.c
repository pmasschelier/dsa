#include <assert.h>
#include <stddef.h>
#include "array_list.h"

#define ARRAY_TYPE int

int main(void) {
	array_list_t* array = create_array_list(sizeof(ARRAY_TYPE));
	assert(array != NULL);
	assert(array->size == 0);
	assert(array->capacity == 0);
	assert(array->size_bytes == sizeof(ARRAY_TYPE));
	assert(array->data == NULL);
	free_array_list(array);
	return 0;
}
