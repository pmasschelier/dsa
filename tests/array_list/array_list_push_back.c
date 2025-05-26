#include <assert.h>
#include <stddef.h>
#include "array_list.h"

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t array;
    array_list_init(&array, sizeof(ARRAY_TYPE));

	for (int i = 0; i < ARRAY_LEN; i++)
		array_list_push_back(&array, &i);
	assert(array_list_min_capacity * sizeof(ARRAY_TYPE) == array.capacity);
	assert(ARRAY_LEN == array.size);
	for (int i = 0; i < ARRAY_LEN; i++)
		assert(*get_array_list_ref(array, i, int) == i);

	array_list_deinit(&array);
	return 0;
}
