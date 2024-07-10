#ifndef STRUCT_HEAP_H
#define STRUCT_HEAP_H

#include <stdlib.h>
#include "ptr.h"
typedef struct heap_view heap_view_t;

struct heap_view {
	compare_fn_t compare;
	void** data;
	unsigned size;
	unsigned capacity;
};

heap_view_t* create_heap(unsigned capacity, compare_fn_t compare);

heap_view_t* build_heap(void** tab, unsigned size, compare_fn_t compare);

heap_view_t* heap_from_array(unsigned size,
							 size_t size_bytes,
							 void* data,
							 compare_fn_t compare);

void* heap_get_root(heap_view_t* heap);

int insert_heap(heap_view_t* heap, void* elem);

void free_heap(heap_view_t* heap);

#endif
