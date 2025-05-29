#include <tap.h>
#include <list_ref/linked_list.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	linked_list_node_t* node;
	for (int i = 0; i < TAB_LEN; i++) {
		linked_list_push_back(list, &i);

		node = list->begin;
		for (int j = 0; j < i + 1; j++, node = node->next)
			cmp_ok(*get_node_ref(node, LIST_TYPE), "==", j);

		node = list->end;
		for (int j = i; j >= 0; j--, node = node->prev)
			cmp_ok(*get_node_ref(node, LIST_TYPE), "==", j);
	}

	linked_list_free(list);
	return 0;
}
