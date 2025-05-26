#include <assert.h>
#include <list_ref/linked_list.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	linked_list_node_t* node;
	for (int i = 0; i < TAB_LEN; i++) {
		node = linked_list_insert(list, NULL, &i);
		assert(i == *get_node_ref(node, LIST_TYPE));

		node = list->begin;
		for (int j = i; j >= 0; j--, node = node->next) {
			assert(j == *get_node_ref(node, LIST_TYPE));
		}

		node = list->end;
		for (int j = 0; j < i + 1; j++, node = node->prev) {
			assert(j == *get_node_ref(node, LIST_TYPE));
		}
	}

	linked_list_free(list);
	return 0;
}
