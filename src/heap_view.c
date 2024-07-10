#include "heap_view.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "ptr.h"
#include "structures.h"
#include "test_macros.h"

static unsigned father_heap(unsigned i) {
	return i == 0 ? 0 : (i + 1) / 2 - 1;
}

static unsigned ls_heap(unsigned i) {
	return (i + 1) * 2 - 1;
}

static unsigned rs_heap(unsigned i) {
	return (i + 1) * 2;
}

static void pullup_heap(heap_view_t* heap, unsigned n) {
	void* key = heap->data[n];

	while (n != 0 && heap->compare(key, heap->data[father_heap(n)]) == 1) {
		heap->data[n] = heap->data[father_heap(n)];
		n = father_heap(n);
	}
	heap->data[n] = key;
}

static void* pulldown_heap(heap_view_t* heap, unsigned n) {
	BOOL found = FALSE;
	void* key = heap->data[n];
	int i_max;

	while (!found && ls_heap(n) < heap->size) {
		// We compute the son with the lowest value
		if (ls_heap(n) == heap->size - 1)
			i_max = heap->size - 1;
		else if (heap->compare(heap->data[ls_heap(n)],
							   heap->data[rs_heap(n)]) == 1)
			i_max = ls_heap(n);
		else
			i_max = rs_heap(n);

		// If the vertex has a lower value that is biggest son
		if (heap->compare(key, heap->data[i_max]) == -1) {
			// We replace the vertex by its node
			heap->data[n] = heap->data[i_max];
			n = i_max;
		} else {
			// Otherwise the vertex is lower than its children and should not go
			// further down
			found = TRUE;
		}
	}
	// We overwrite the last moved son with the original vertex
	heap->data[n] = key;
	return heap->data[heap->size];
}

/*
static void heap_sort(unsigned* index, long long*  val, unsigned size) {
		for(int p=size-1; p>0; p--) {
				swap(index, 0, p);
				pulldown_heap(index, val, p, 0);
		}
} */

heap_view_t* create_heap(unsigned capacity, compare_fn_t compare) {
	heap_view_t* ret = malloc(sizeof(heap_view_t));
	TEST_PTR_FAIL_FUNC(ret, NULL, );
	ret->data = calloc(capacity, sizeof(void*));
	TEST_PTR_FAIL_FUNC(ret->data, NULL, free(ret));
	ret->capacity = capacity;
	ret->size = 0;
	ret->compare = compare;
	return ret;
}

heap_view_t* build_heap(void** tab, unsigned size, compare_fn_t compare) {
	heap_view_t* heap = create_heap(size, compare);
	TEST_PTR_FAIL_FUNC(heap, NULL, );
	for (unsigned i = 0; i < size; i++)
		insert_heap(heap, tab[i]);
	return heap;
}

heap_view_t* heap_from_array(unsigned size,
							 size_t size_bytes,
							 void* data,
							 compare_fn_t compare) {
	TEST_PTR_FAIL_FUNC(data, NULL, );
	heap_view_t* ret = malloc(sizeof(heap_view_t));
	TEST_PTR_FAIL_FUNC(ret, NULL, );
	ret->data = malloc(size * sizeof(void*));
	TEST_PTR_FAIL_FUNC(ret->data, NULL, free(ret));
	for (unsigned i = 0; i < size; i++)
		ret->data[i] = (char*)data + i * size_bytes;
	ret->capacity = size;
	ret->size = size;
	ret->compare = compare;
	return ret;
}

void* heap_get_root(heap_view_t* heap) {
	if (heap->size == 0)
		return NULL;
	if (heap->size == 1)
		return heap->data[0];
	swap_ref(heap->data, 0, heap->size - 1);
	return pulldown_heap(heap, 0);
}

int insert_heap(heap_view_t* heap, void* elem) {
	TEST_FAIL_FUNC(heap->size < heap->capacity, -ERROR_CAPACITY_EXCEEDED, );
	heap->data[heap->size] = elem;
	pullup_heap(heap, heap->size++);
	return 0;
}

void free_heap(heap_view_t* heap) {
	if (heap == NULL)
		return;
	// if (heap->free_element) {
	// 	for (unsigned i = 0; i < heap->size; i++) {
	// 		if (heap->data[i] != NULL)
	// 			free(heap->data[i]);
	// 	}
	// }
	free(heap->data);
	free(heap);
}
