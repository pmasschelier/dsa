#include <assert.h>
#include <stddef.h>
#include "dynarray.h"

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	dynarray_t* array = create_dynarray(sizeof(ARRAY_TYPE));
	for (int i = 0; i < ARRAY_LEN; i++)
		dynarray_push_back(array, &i);
	assert(array->capacity = 32);
	assert(array->size = ARRAY_LEN);
	for (int i = 0; i < ARRAY_LEN; i++)
		assert(*get_dynarray_ref(array, i, int) == i);
	free_dynarray(array);
	return 0;
}
