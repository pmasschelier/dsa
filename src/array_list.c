#include "array_list.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "test_macros.h"

array_list_t* create_array_list(unsigned size_bytes) {
	when_true_ret(size_bytes == 0, NULL);
	array_list_t* ret = malloc(sizeof(array_list_t));
	ret->size_bytes = size_bytes;
	ret->size = 0;
	ret->capacity = 0;
	ret->data = NULL;
	return ret;
}

void free_array_list(array_list_t* array) {
	free(array->data);
	free(array);
}

BOOL array_list_empty(array_list_t* array) {
	return array->size == 0;
}

static void ensure_sufficient_capacity(array_list_t* array) {
	if (array->size >= array->capacity) {
		if (array->capacity < ARRAY_LIST_MIN_CAPACITY)
			array->capacity = ARRAY_LIST_MIN_CAPACITY * array->size_bytes;
		else
			array->capacity *= 2;
		array->data = realloc(array->data, array->capacity * array->size_bytes);
	}
}

static void shift_right(array_list_t* array) {
	memmove(array->data + array->size_bytes, array->data,
			array->size * array->size_bytes);
}

void* array_list_push_front(array_list_t* array, void* value) {
	ensure_sufficient_capacity(array);
	array->size++;
	shift_right(array);
	void* ptr = array->data;
	memcpy(ptr, value, array->size_bytes);
	return ptr;
}

void* array_list_push_back(array_list_t* array, void* value) {
	ensure_sufficient_capacity(array);
	void* ptr = array->data + array->size_bytes * array->size;
	memcpy(ptr, value, array->size_bytes);
	array->size++;
	return ptr;
}

static void shift_left(array_list_t* array) {
	memmove(array->data, array->data + array->size_bytes,
			array->size * array->size_bytes);
}

BOOL array_list_pop_front(array_list_t* array, void* value) {
	if (array_list_empty(array))
		return FALSE;

	if (value != NULL) {
		memcpy(value, array->data, array->size_bytes);
	}
	shift_left(array);
	array->size--;
	return TRUE;
}

BOOL array_list_pop_back(array_list_t* array, void* value) {
	if (array_list_empty(array))
		return FALSE;

	if (value != NULL) {
		char* ptr = array->data + array->size_bytes * (array->size - 1);
		memcpy(value, ptr, array->size_bytes);
	}
	array->size--;
	return TRUE;
}

int array_list_swap(array_list_t* array, unsigned a, unsigned b) {
	when_null_ret(array, -ERROR_INVALID_PARAM1);
	when_false_ret(a < array->size, -ERROR_INVALID_PARAM2);
	when_false_ret(b < array->size, -ERROR_INVALID_PARAM3);
	if (a == b)
		return -ERROR_NO_ERROR;
	unsigned char* a_ref = get_array_list_ref(array, a, unsigned char);
	unsigned char* b_ref = get_array_list_ref(array, b, unsigned char);
	for (unsigned i = 0; i < array->size_bytes; i++) {
		unsigned char byte = a_ref[i];
		a_ref[i] = b_ref[i];
		b_ref[i] = byte;
	}
	return -ERROR_NO_ERROR;
}
