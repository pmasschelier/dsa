#include "htable_ref/htable_list_ref.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "errors.h"
#include "list_ref/linked_list_ref.h"
#include "test_macros.h"

htable_list_ref_t* create_htable_list(unsigned buckets, equals_fn_t equals) {
	htable_list_ref_t* htable =
		malloc(sizeof(htable_list_ref_t) + buckets * sizeof(list_ref_t));
	htable->bucket_count = buckets;
	htable->equals = equals;
	for (unsigned i = 0; i < buckets; i++) {
		htable->buckets[i].begin = NULL;
		htable->buckets[i].end = NULL;
		htable->buckets[i].size = 0;
		htable->buckets[i].free_element = free;
	}
	when_null_ret(htable, NULL);
	return htable;
}

void free_htable_list(htable_list_ref_t* htable) {
	for (unsigned i = 0; i < htable->bucket_count; i++)
		linked_list_clean(&htable->buckets[i]);
	free(htable);
}

int htable_list_insert(htable_list_ref_t* htable,
					   unsigned long long hash,
					   void* value,
					   void** found) {
	when_null_ret(htable, -ERROR_INVALID_PARAM1);
	unsigned index = hash % htable->bucket_count;
	list_ref_t* list = &htable->buckets[index];
	node_list_ref_t* node =
		linked_list_find_equals(list, value, htable->equals);
	if (node != NULL) {
		if (found != NULL)
			*found = node->p;
		return -ERROR_KEY_ALREADY_EXISTS;
	}
	linked_list_push_back(list, value);
	return 0;
}

int htable_list_insert_clone(htable_list_ref_t* htable,
							 unsigned long long hash,
							 void* value,
							 size_t size_bytes,
							 void** found) {
	when_null_ret(htable, -ERROR_INVALID_PARAM1);
	unsigned index = hash % htable->bucket_count;
	list_ref_t* list = &htable->buckets[index];
	node_list_ref_t* node =
		linked_list_find_equals(list, value, htable->equals);
	if (node != NULL) {
		if (found != NULL)
			*found = node->p;
		return -ERROR_KEY_ALREADY_EXISTS;
	}
	void* copy = malloc(size_bytes);
	memcpy(copy, value, size_bytes);
	linked_list_push_back(list, copy);
	return 0;
}

void* htable_list_get(htable_list_ref_t* htable,
					  unsigned long long hash,
					  void* value) {
	when_null_ret(htable, NULL);
	unsigned index = hash % htable->bucket_count;
	list_ref_t* list = &htable->buckets[index];
	node_list_ref_t* node =
		linked_list_find_equals(list, value, htable->equals);
	if (node != NULL)
		return node->p;
	return NULL;
}

void htable_list_clean(htable_list_ref_t* htable) {
	for (unsigned i = 0; i < htable->bucket_count; i++) {
		linked_list_clean(&htable->buckets[i]);
	}
}
