#include <assert.h>
#include <stddef.h>
#include "array_list.h"

#define ARRAY_TYPE int

int main(void) {
	array_list_t* array = array_list_create(sizeof(ARRAY_TYPE));
	assert(array != NULL);
	assert(array->size == 0);
	assert(array->capacity == 0);
	assert(array->size_bytes == sizeof(ARRAY_TYPE));
	assert(array->data == NULL);
	array_list_free(array);
	return 0;
}
