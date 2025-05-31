#include <stddef.h>
#include <array_list.h>
#include <tap.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t array;
    array_list_init(&array, sizeof(ARRAY_TYPE));

	for (int i = 0; i < ARRAY_LEN; i++)
		array_list_push_back(&array, &i);
	cmp_ok(array.capacity, "==", array_list_min_capacity * sizeof(ARRAY_TYPE), "array_list has minimum capacity");
	cmp_ok(array.size, "==", ARRAY_LEN, "array_list has correct size");
	for (int i = 0; i < ARRAY_LEN; i++)
		cmp_ok(*get_array_ref(array, i, int), "==", i, "array[i] == i");

	array_list_deinit(&array);
	return 0;
}
