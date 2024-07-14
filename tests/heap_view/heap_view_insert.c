#include <assert.h>
#include "heap_view.h"
#include "ptr.h"

#define HEAP_SIZE 4

static int compare(int* a, int* b) {
	return (*a > *b) - (*b > *a);
}

int data[HEAP_SIZE] = {9, 8, 7, 10};
int heap_array[HEAP_SIZE];

int main(void) {
	heap_view_t* heap =
		create_heap(HEAP_SIZE, sizeof(int), heap_array, (compare_fn_t)compare);
	for (int i = 0; i < 3; i++)
		insert_heap(heap, data + i);
	assert(heap->size == 3);
	assert(heap->capacity == HEAP_SIZE);
	for (int i = 0; i < 3; i++)
		assert(data[i] == heap_array[i]);
	insert_heap(heap, data + 3);
	assert(data[3] == heap_array[heap->pos_to_idx[0]]);
	free_heap(heap);
	return 0;
}
