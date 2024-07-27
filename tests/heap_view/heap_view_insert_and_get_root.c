#include <assert.h>
#include "heap_view.h"

#define HEAP_SIZE 4

DEFINE_COMPARE_SCALAR(int)

int data[HEAP_SIZE] = {9, 8, 7, 10};
int heap_array[HEAP_SIZE];

int main(void) {
	heap_view_t* heap =
		create_heap(HEAP_SIZE, sizeof(int), heap_array, compare_int);
	for (int i = 0; i < HEAP_SIZE; i++)
		insert_heap(heap, data + i);
	assert(heap->size == HEAP_SIZE);
	assert(heap->capacity == HEAP_SIZE);
	for (int i = 10; i > 6; i--) {
		int root = heap_get_root(heap);
		assert(heap_array[root] == i);
	}
	free_heap(heap);
	return 0;
}
