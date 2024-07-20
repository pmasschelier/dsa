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

static void write_heap(heap_view_t* heap,
					   /* void* elem, */
					   unsigned int idx,
					   unsigned int to) {
	heap->idx_to_pos[idx] = to;
	heap->pos_to_idx[to] = idx;
	/* heap->data[to] = elem; */
}

static void move_heap(heap_view_t* heap, unsigned int from, unsigned int to) {
	unsigned idx = heap->pos_to_idx[from];
	heap->idx_to_pos[idx] = to;
	heap->pos_to_idx[to] = idx;
	/* heap->data[to] = heap->data[from]; */
}

static void swap_heap(heap_view_t* heap, unsigned int a, unsigned int b) {
	unsigned int idxA = heap->pos_to_idx[a];
	unsigned int idxB = heap->pos_to_idx[b];
	heap->idx_to_pos[idxA] = b;
	heap->idx_to_pos[idxB] = a;
	heap->pos_to_idx[a] = idxB;
	heap->pos_to_idx[b] = idxA;
}

static void* heap_get_ptr(heap_view_t* heap, unsigned int pos) {
	return (char*)heap->data + heap->size_bytes * heap->pos_to_idx[pos];
}

static void pullup_heap(heap_view_t* heap, unsigned n) {
	unsigned int idx = heap->pos_to_idx[n];
	void* key = heap_get_ptr(heap, n);

	while (n != 0 &&
		   heap->compare(key, heap_get_ptr(heap, father_heap(n))) == 1) {
		move_heap(heap, father_heap(n), n);
		/* heap->data[n] = heap->data[father_heap(n)]; */
		n = father_heap(n);
	}
	write_heap(heap, idx, n);
	/* heap->data[n] = key; */
}

static unsigned pulldown_heap(heap_view_t* heap, unsigned n) {
	BOOL found = FALSE;
	unsigned int idx = heap->pos_to_idx[n];
	void* key = heap_get_ptr(heap, n);
	int i_max;

	while (!found && ls_heap(n) < heap->size) {
		// We compute the son with the lowest value
		void* max_son_ptr = NULL;
		if (ls_heap(n) == heap->size - 1) {
			i_max = heap->size - 1;
			max_son_ptr = heap_get_ptr(heap, ls_heap(n));
		} else {
			void* ls_ptr = heap_get_ptr(heap, ls_heap(n));
			void* rs_ptr = heap_get_ptr(heap, rs_heap(n));
			if (heap->compare(ls_ptr, rs_ptr) == 1) {
				i_max = ls_heap(n);
				max_son_ptr = ls_ptr;
			} else {
				i_max = rs_heap(n);
				max_son_ptr = rs_ptr;
			}
		}
		// If the vertex has a lower value that is biggest son
		if (heap->compare(key, max_son_ptr) == -1) {
			// We replace the vertex by its node
			move_heap(heap, i_max, n);
			n = i_max;
		} else {
			// Otherwise the vertex is lower than its children and should not go
			// further down
			found = TRUE;
		}
	}
	// We overwrite the last moved son with the original vertex
	write_heap(heap, idx, n);
	return heap->pos_to_idx[heap->size];
}

/*
static void heap_sort(unsigned* index, long long*  val, unsigned size) {
		for(int p=size-1; p>0; p--) {
				swap(index, 0, p);
				pulldown_heap(index, val, p, 0);
		}
} */

static heap_view_t* create_heap_no_data(unsigned capacity,
										size_t size_bytes,
										compare_fn_t compare) {
	heap_view_t* ret = malloc(sizeof(heap_view_t));
	TEST_PTR_FAIL_FUNC(ret, NULL, );

	ret->idx_to_pos = malloc(capacity * sizeof(unsigned));
	TEST_PTR_FAIL_FUNC(ret->idx_to_pos, NULL, free(ret));
	for (unsigned i = 0; i < capacity; i++)
		ret->idx_to_pos[i] = i;

	ret->pos_to_idx = malloc(capacity * sizeof(unsigned));
	TEST_PTR_FAIL_FUNC(ret->pos_to_idx, NULL, free(ret));
	for (unsigned i = 0; i < capacity; i++)
		ret->pos_to_idx[i] = i;

	ret->capacity = capacity;
	ret->size = 0;
	ret->size_bytes = size_bytes;
	ret->compare = compare;
	return ret;
}

heap_view_t* create_heap(unsigned capacity,
						 size_t size_bytes,
						 void* data,
						 compare_fn_t compare) {
	TEST_PTR_FAIL_FUNC(data, NULL, );
	heap_view_t* ret = create_heap_no_data(capacity, size_bytes, compare);
	TEST_PTR_FAIL_FUNC(ret, NULL, );

	ret->data = data;
	return ret;
}

/* heap_view_t* build_heap(void** tab, unsigned size, compare_fn_t compare) { */
/* 	heap_view_t* heap = create_heap(size, compare); */
/* 	TEST_PTR_FAIL_FUNC(heap, NULL, ); */
/* 	for (unsigned i = 0; i < size; i++) */
/* 		insert_heap(heap, tab[i]); */
/* 	return heap; */
/* } */

heap_view_t* create_heap_no_check(unsigned size,
								  size_t size_bytes,
								  void* data,
								  compare_fn_t compare) {
	TEST_PTR_FAIL_FUNC(data, NULL, );
	heap_view_t* ret = create_heap_no_data(size, size_bytes, compare);
	TEST_PTR_FAIL_FUNC(ret, NULL, );
	/* memcpy(ret->data, data, size * size_bytes); */
	ret->data = data;
	ret->size = size;

	return ret;
}

int heap_get_root(heap_view_t* heap) {
	if (heap->size == 0)
		return -1;
	if (heap->size == 1) {
		heap->size = 0;
		return heap->pos_to_idx[0];
	}

	swap_heap(heap, 0, heap->size - 1);
	heap->size--;
	return pulldown_heap(heap, 0);
}

static void copy_elem(heap_view_t* heap, unsigned int idx, void* elem) {
	memcpy((char*)heap->data + idx * heap->size_bytes, (char*)elem,
		   heap->size_bytes);
}

int insert_heap(heap_view_t* heap, void* elem) {
	TEST_FAIL_FUNC(heap->size < heap->capacity, -ERROR_CAPACITY_EXCEEDED, );

	copy_elem(heap, heap->size, elem);
	pullup_heap(heap, heap->size);

	return ++heap->size;
}

void heap_update_up(heap_view_t* heap, unsigned int idx, void* elem) {
	copy_elem(heap, idx, elem);
	pullup_heap(heap, heap->idx_to_pos[idx]);
}

void free_heap(heap_view_t* heap) {
	if (heap == NULL)
		return;
	/* free(heap->data); */
	free(heap->idx_to_pos);
	free(heap->pos_to_idx);
	free(heap);
}
