#include <stddef.h>
#include <array_list.h>
#include <tap.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t* array = array_list_create(sizeof(ARRAY_TYPE));
	int i, x;
	for (i = 0; i < ARRAY_LEN; i++)
		array_list_push_back(array, &i);
	while (array_list_pop_back(array, &x) == TRUE) {
		cmp_ok(x, "==", --i, "array[i] == ARRAY_LEN - 1 - i");
	}
	array_list_free(array);
	return 0;
}
