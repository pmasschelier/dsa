#include <assert.h>
#include "heap_view.h"

#define HEAP_SIZE 11

DEFINE_COMPARE_SCALAR(int)

int data[HEAP_SIZE] = {0xF, 0xD, 0xC, 0xB, 0xA, 0x9, 0x7, 0x6, 0x5, 0x4, 0x3};
int expected[HEAP_SIZE] = {0xF, 0xE, 0xC, 0xB, 0xD, 0x9,
						   0x7, 0x6, 0x5, 0xA, 0x3};

int main(void) {
	heap_view_t* heap =
		create_heap_no_check(HEAP_SIZE, sizeof(int), data, compare_int);
	unsigned idx = HEAP_SIZE - 2;
	unsigned int pos = heap->idx_to_pos[idx];
	assert(pos == HEAP_SIZE - 2);

	int value = 0xE;
	heap_update_up(heap, idx, &value);

	for (int i = 0; i < HEAP_SIZE; i++)
		assert(expected[i] == data[heap->pos_to_idx[i]]);
	free_heap(heap);
	return 0;
}
