#ifndef DYNARRAY_H
#define DYNARRAY_H

#include "structures.h"
#define DYNARRAY_MIN_CAPACITY 8

#define get_dynarray_ref(array, index, type) \
	(type*)((array)->data + (index) * (array)->size_bytes)

typedef struct dynarray dynarray_t;

struct dynarray {
	char* data;
	unsigned size;
	unsigned capacity;
	unsigned size_bytes;
};

dynarray_t* create_dynarray(unsigned size_bytes);

void free_dynarray(dynarray_t* array);

BOOL dynarray_empty(dynarray_t* array);

void* dynarray_push_front(dynarray_t* array, void* value);

void* dynarray_push_back(dynarray_t* array, void* value);

BOOL dynarray_pop_front(dynarray_t* array, void* value);

BOOL dynarray_pop_back(dynarray_t* array, void* value);

#endif	// !DYNARRAY_H
