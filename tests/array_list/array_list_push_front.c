#include <stddef.h>
#include <array_list.h>
#include <tap.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

int main(void) {
	array_list_t array;
    array_list_init(&array, sizeof(ARRAY_TYPE));

	for (int i = 0; i < ARRAY_LEN; i++)
		array_list_push_front(&array, &i);
	cmp_ok(array_list_min_capacity * sizeof(ARRAY_TYPE), "==", array.capacity, "array_list has minimum capacity");
	cmp_ok(ARRAY_LEN, "==", array.size, "array_list has correct size");
	for (int i = 0; i < ARRAY_LEN; i++)
		cmp_ok(*get_array_list_ref(array, i, int), "==", ARRAY_LEN - 1 - i, "array[i] = ARRAY_LEN - 1 - i");
	array_list_deinit(&array);
	return 0;
}
