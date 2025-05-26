#include <assert.h>
#include <list_ref/linked_list.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	for (int i = 1; i < TAB_LEN; i += 2)
		linked_list_push_back(list, &i);
	linked_list_node_t* node = NULL;
	for (int i = 0; i < TAB_LEN; i += 2) {
		node = linked_list_insert(list, node, &i);
		node = node->next;
	}

	node = list->begin;
	for (int i = 0; i < TAB_LEN; i++, node = node->next) {
		assert(i == *get_node_ref(node, LIST_TYPE));
	}

	linked_list_free(list);
	return 0;
}
