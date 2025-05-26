#include "array_list.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "test_macros.h"

size_t array_list_min_capacity = 8;

array_list_t* array_list_create(unsigned size_bytes) {
	when_true_ret(size_bytes == 0, NULL);
	array_list_t* ret = malloc(sizeof(array_list_t));
    when_null_ret(ret, NULL);
	ret->size_bytes = size_bytes;
	ret->size = 0;
	ret->capacity = 0;
	ret->data = NULL;
    return ret;
}

void array_list_free(array_list_t* array) {
    if(array == NULL)
        return;
    free(array->data);
	free(array);
}

void array_list_init(array_list_t* array, unsigned size_bytes) {
	array->size_bytes = size_bytes;
	array->size = 0;
	array->capacity = 0;
	array->data = NULL;
}

void array_list_deinit(array_list_t* array) {
    if(array == NULL)
        return;
    free(array->data);
	array->size = 0;
	array->capacity = 0;
    array->data = NULL;
}

BOOL array_list_empty(array_list_t* array) {
	return array->size == 0;
}

static void ensure_sufficient_capacity(array_list_t* array) {
	if (array->size >= array->capacity) {
		if (array->capacity < array_list_min_capacity)
			array->capacity = array_list_min_capacity * array->size_bytes;
		else
			array->capacity *= 2;
		array->data = realloc(array->data, array->capacity * array->size_bytes);
	}
}

static void shift_right(array_list_t* array) {
	memmove(array->data + array->size_bytes, array->data,
			array->size * array->size_bytes);
}

void* array_list_push_front(array_list_t* array, const void* value) {
	ensure_sufficient_capacity(array);
	array->size++;
	shift_right(array);
	void* ptr = array->data;
	memcpy(ptr, value, array->size_bytes);
	return ptr;
}

void* array_list_push_back(array_list_t* array, const void* value) {
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
	unsigned char* a_ref = get_array_list_ref(*array, a, unsigned char);
	unsigned char* b_ref = get_array_list_ref(*array, b, unsigned char);
    
	for (unsigned i = 0; i < array->size_bytes; i++) {
		unsigned char byte = a_ref[i];
		a_ref[i] = b_ref[i];
		b_ref[i] = byte;
	}
	return -ERROR_NO_ERROR;
}

int array_list_swap_and_pop_back(array_list_t* array,
								  unsigned i,
								  void* value) {
	if (array_list_empty(array) == TRUE)
		return FALSE;
	int ret = array_list_swap(array, i, array->size - 1);
	when_false_ret(-ERROR_NO_ERROR == ret, ret);
	return array_list_pop_back(array, value) ? -ERROR_NO_ERROR : -ERROR_IS_EMPTY;
}
