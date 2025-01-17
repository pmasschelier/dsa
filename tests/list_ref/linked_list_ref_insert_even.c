#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	for (int i = 1; i < TAB_LEN; i += 2)
		linked_list_push_back(list, ptr(TYPE_INT, i));
	node_list_ref_t* node = NULL;
	for (int i = 0; i < TAB_LEN; i += 2) {
		node = linked_list_insert(list, node, ptr(TYPE_INT, i));
		node = node->next;
	}

	node = list->begin;
	for (int i = 0; i < TAB_LEN; i++, node = node->next) {
		assert(i == *(LIST_TYPE*)node->p);
	}

	free_linked_list(list);
	return 0;
}
