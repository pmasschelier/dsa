#include <assert.h>
#include "heap_view.h"

#define HEAP_SIZE 10

DEFINE_COMPARE_SCALAR(int)

int data[HEAP_SIZE] = {9, 7, 8, 6, 5, 2, 0, 3, 4, 1};
int expected[HEAP_SIZE] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

int main(void) {
	heap_view_t* heap =
		create_heap_no_check(10, sizeof(int), data, compare_int);
	int root;
	unsigned i = 0;
	while ((root = heap_get_root(heap)) != -1) {
		assert(data[root] == expected[i]);
		i++;
	}
	assert(i == HEAP_SIZE);
	free_heap(heap);
	return 0;
}
