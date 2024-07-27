#include "dynarray.h"
#include <stdlib.h>
#include <string.h>
#include "test_macros.h"

dynarray_t* create_dynarray(unsigned size_bytes) {
	when_true_ret(size_bytes == 0, NULL);
	dynarray_t* ret = malloc(sizeof(dynarray_t));
	ret->size_bytes = size_bytes;
	ret->size = 0;
	ret->capacity = 0;
	ret->data = NULL;
	return ret;
}

void free_dynarray(dynarray_t* array) {
	free(array->data);
	free(array);
}

BOOL dynarray_empty(dynarray_t* array) {
	return array->size == 0;
}

static void ensure_sufficient_capacity(dynarray_t* array) {
	if (array->size >= array->capacity) {
		if (array->capacity < DYNARRAY_MIN_CAPACITY)
			array->capacity = DYNARRAY_MIN_CAPACITY;
		else
			array->capacity *= 2;
		array->data = realloc(array->data, array->capacity * array->size_bytes);
	}
}

static void shift_right(dynarray_t* array) {
	memmove(array->data + array->size_bytes, array->data,
			array->size * array->size_bytes);
}

void* dynarray_push_front(dynarray_t* array, void* value) {
	ensure_sufficient_capacity(array);
	array->size++;
	shift_right(array);
	void* ptr = array->data;
	memcpy(ptr, value, array->size_bytes);
	return ptr;
}

void* dynarray_push_back(dynarray_t* array, void* value) {
	ensure_sufficient_capacity(array);
	void* ptr = array->data + array->size_bytes * array->size;
	memcpy(ptr, value, array->size_bytes);
	array->size++;
	return ptr;
}

static void shift_left(dynarray_t* array) {
	memmove(array->data, array->data + array->size_bytes,
			array->size * array->size_bytes);
}

BOOL dynarray_pop_front(dynarray_t* array, void* value) {
	if (dynarray_empty(array)) {
		return FALSE;
	}

	if (value != NULL) {
		memcpy(value, array->data, array->size_bytes);
	}
	shift_left(array);
	array->size--;
	return TRUE;
}

BOOL dynarray_pop_back(dynarray_t* array, void* value) {
	if (dynarray_empty(array)) {
		return FALSE;
	}

	if (value != NULL) {
		char* ptr = array->data + array->size_bytes * (array->size - 1);
		memcpy(value, ptr, array->size_bytes);
	}
	array->size--;
	return TRUE;
}
