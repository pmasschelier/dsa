#include <assert.h>
#include <list_ref/linked_list_ref.h>
#include <stdlib.h>

#define HALF_TAB_LEN 5
#define TAB_LEN (HALF_TAB_LEN << 1)
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, ptr(TYPE_INT, i));
	assert(linked_list_length(list) == TAB_LEN);
	node_list_ref_t* node = list->begin;

	node_list_ref_t* next;
	LIST_TYPE* val;
	for (int i = 0; i < HALF_TAB_LEN; i++) {
		next = node->next;
		linked_list_remove(list, node, (void**)&val);
		assert(*val == i * 2);
		free(val);
		node = next->next;
	}
	assert(linked_list_length(list) == HALF_TAB_LEN);

	node = list->begin;
	for (int i = 1; i < 10; node = node->next, i += 2)
		assert(i == *(LIST_TYPE*)(node->p));

	free_linked_list(list);
	return 0;
}
