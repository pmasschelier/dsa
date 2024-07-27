#include <assert.h>
#include <stddef.h>
#include "dynarray.h"

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	dynarray_t* array = create_dynarray(sizeof(ARRAY_TYPE));
	int i, x;
	for (i = 0; i < ARRAY_LEN; i++)
		dynarray_push_back(array, &i);
	while (dynarray_pop_back(array, &x) == TRUE) {
		assert(x == --i);
	}
	free_dynarray(array);
	return 0;
}
