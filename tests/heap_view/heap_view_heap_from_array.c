#include <assert.h>
#include "heap_view.h"

#define HEAP_SIZE 10

DEFINE_COMPARE_SCALAR(int)

int data[HEAP_SIZE] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

int main(void) {
	heap_view_t* heap =
		create_heap_no_check(10, sizeof(int), data, compare_int);
	assert(heap);
	assert(heap->data);
	for (int i = 0; i < HEAP_SIZE; i++)
		assert(((int*)heap->data)[i] == data[i]);
	assert(heap->size == HEAP_SIZE);
	assert(heap->capacity == HEAP_SIZE);
	free_heap(heap);
	return 0;
}
