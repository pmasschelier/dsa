#ifndef STRUCT_HEAP_H
#define STRUCT_HEAP_H

#include <stdlib.h>
#include "ptr.h"

typedef unsigned (*getid_fn_t)(void*);

typedef struct heap_view heap_view_t;

struct heap_view {
	compare_fn_t compare;
	getid_fn_t getid;
	void* data;
	unsigned* idx_to_pos;
	unsigned* pos_to_idx;
	unsigned size;
	size_t size_bytes;
	unsigned capacity;
};

heap_view_t* create_heap(unsigned capacity,
						 size_t size_bytes,
						 void* data,
						 compare_fn_t compare);

// heap_view_t* build_heap(void** tab, unsigned size, compare_fn_t compare);

heap_view_t* create_heap_no_check(unsigned size,
								  size_t size_bytes,
								  void* data,
								  compare_fn_t compare);

int heap_get_root(heap_view_t* heap);

int insert_heap(heap_view_t* heap, void* elem);

void heap_update_up(heap_view_t* heap, unsigned int idx);

void free_heap(heap_view_t* heap);

#endif
