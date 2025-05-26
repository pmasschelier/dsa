#include <stddef.h>
#include <array_list.h>
#include <tap.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

const ARRAY_TYPE VALUES[ARRAY_LEN] = { -12, 0, 3, 4, 2, -1, 7, 65, -3, 8, 1, -9, 6, 5, 9, -6, -8, -4, -2, -5};

int main(void) {
	array_list_t array = ARRAY_LIST_INIT(ARRAY_TYPE);
	for (unsigned i = 0; i < ARRAY_LEN; i++)
		array_list_push_back(&array, &VALUES[i]);
    for (unsigned i = 0; i < ARRAY_LEN; i++) {
        int min = *get_array_list_ref(array, i, int);
        unsigned j_min = i;
        for (unsigned j = i + 1; j < ARRAY_LEN; j++) {
            int x = *get_array_list_ref(array, j, int);
            if(x < min) {
                min = x;
                j_min = j;
            }
        }
        array_list_swap(&array, i, j_min);
    }
	for (unsigned i = 1; i < ARRAY_LEN; i++) {
        int a = *get_array_list_ref(array, i - 1, int);
        int b = *get_array_list_ref(array, i, int);
        cmp_ok(a, "<=", b, "array[%d] <= array[%d]", i-1, i);
    }
	array_list_deinit(&array);
	return 0;
}
