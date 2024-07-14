#include <assert.h>
#include "heap_view.h"

#define HEAP_SIZE 10

static int compare(void* a, void* b) {
	return 0;
}

int data[HEAP_SIZE];

int main(void) {
	heap_view_t* heap = create_heap(HEAP_SIZE, sizeof(int), data, compare);
	assert(heap);
	assert(heap->data);
	assert(heap->size == 0);
	assert(heap->capacity == 10);
	free_heap(heap);
	return 0;
}
