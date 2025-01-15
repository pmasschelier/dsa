#include <assert.h>
#include <stddef.h>
#include "array_list.h"

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t* array = create_array_list(sizeof(ARRAY_TYPE));
	for (int i = 0; i < ARRAY_LEN; i++)
		array_list_push_front(array, &i);
	assert(array->capacity = 32);
	assert(array->size = ARRAY_LEN);
	for (int i = 0; i < ARRAY_LEN; i++)
		assert(*get_array_list_ref(array, i, int) == ARRAY_LEN - 1 - i);
	free_array_list(array);
	return 0;
}
