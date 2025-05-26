#include "list_ref/algorithms.h"
#include <string.h>
#include "list_ref/linked_list_ref.h"

linked_list_node_t* find_in_list(linked_list_t* list, void* x) {
	linked_list_node_t* node = list->begin;
	while (node) {
		if (memcmp(node->data, x, list->size_bytes) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}

static void transform_list_node(linked_list_node_t* node, void (*f)(void*)) {
	f(node->data);
	if (node->next)
		transform_list_node(node->next, f);
}

void transform_list(linked_list_t* list, void (*f)(void*)) {
	transform_list_node(list->begin, f);
}

static void filter_list_extract(linked_list_t* list,
								linked_list_t* others,
								BOOL (*filter)(void*),
								linked_list_node_t* node) {
	linked_list_node_t* next = node->next;
	if (!filter(node->data)) {
		linked_list_extract(list, node);
		linked_list_insert_node(others, list->end, node);
	}

	if (next)
		filter_list_extract(list, others, filter, next);
}

static void filter_list_remove(linked_list_t* list,
							   BOOL (*filter)(void*),
							   linked_list_node_t* node) {
	linked_list_node_t* next = node->next;
	if (!filter(node->data))
		linked_list_remove(list, node, NULL);

	if (next)
		filter_list_remove(list, filter, next);
}

void filter_list(linked_list_t* list, linked_list_t** others, BOOL (*filter)(void*)) {
	if (others) {
		*others = linked_list_create(list->size_bytes);
		filter_list_extract(list, *others, filter, list->begin);
	} else {
		filter_list_remove(list, filter, list->begin);
	}
}
