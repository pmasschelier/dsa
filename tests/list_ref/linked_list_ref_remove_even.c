#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define HALF_TAB_LEN 5
#define TAB_LEN (HALF_TAB_LEN << 1)
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, &i);
	assert(linked_list_length(list) == TAB_LEN);
	linked_list_node_t* node = list->begin;

	linked_list_node_t* next;
	LIST_TYPE val;
	for (int i = 0; i < HALF_TAB_LEN; i++) {
		next = node->next;
		linked_list_remove(list, node, &val);
		assert(val == i * 2);
		node = next->next;
	}
	assert(linked_list_length(list) == HALF_TAB_LEN);

	node = list->begin;
	for (int i = 1; i < 10; node = node->next, i += 2)
		assert(i == *get_node_ref(node, LIST_TYPE));

	linked_list_free(list);
	return 0;
}
