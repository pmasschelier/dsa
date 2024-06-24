#include "list_ref/algorithms.h"
#include <string.h>
#include "list_ref/list_ref.h"

list_node_ref_t* find_in_list(list_ref_t* list, void* x) {
	list_node_ref_t* node = list->begin;
	while (node) {
		if (memcmp(node->p, x, list->size) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}

static void transform_list_node(list_node_ref_t* node, void (*f)(void*)) {
	f(node->p);
	if (node->next)
		transform_list_node(node->next, f);
}

void transform_list(list_ref_t* list, void (*f)(void*)) {
	transform_list_node(list->begin, f);
}

static void filter_list_extract(list_ref_t* list,
								list_ref_t* others,
								BOOL (*filter)(void*),
								list_node_ref_t* node) {
	list_node_ref_t* next = node->next;
	if (!filter(node->p)) {
		extract_list(list, node);
		insert_list_node(others, list->end, node);
	}

	if (next)
		filter_list_extract(list, others, filter, next);
}

static void filter_list_remove(list_ref_t* list,
							   BOOL (*filter)(void*),
							   list_node_ref_t* node) {
	list_node_ref_t* next = node->next;
	if (!filter(node->p))
		remove_list(list, node, NULL);

	if (next)
		filter_list_remove(list, filter, next);
}

void filter_list(list_ref_t* list, list_ref_t** others, BOOL (*filter)(void*)) {
	if (others) {
		*others = create_list(list->size);
		filter_list_extract(list, *others, filter, list->begin);
	} else {
		filter_list_remove(list, filter, list->begin);
	}
}
