#include <assert.h>
#include <stddef.h>
#include "array_list.h"

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t* array = create_array_list(sizeof(ARRAY_TYPE));
	int i, x;
	for (i = 0; i < ARRAY_LEN; i++)
		array_list_push_back(array, &i);
	while (array_list_pop_back(array, &x) == TRUE) {
		assert(x == --i);
	}
	free_array_list(array);
	return 0;
}
