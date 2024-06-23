#include "list_ref/algorithms.h"
#include <string.h>

node_ref_t* find_in_list(list_ref_t* list, void* x) {
	node_ref_t* node = list->begin;
	while (node) {
		if (memcmp(node->p, x, list->size) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}
