#include <assert.h>
#include <list_ref/list_ref.h>

#define HALF_TAB_LEN 5
#define TAB_LEN (HALF_TAB_LEN << 1)
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		push_back_list(list, ptr(TYPE_INT, i));
	assert(length_list(list) == TAB_LEN);
	list_node_ref_t* node = list->begin;

	list_node_ref_t* next;
	LIST_TYPE* val;
	for (int i = 0; i < HALF_TAB_LEN; i++) {
		next = node->next;
		remove_list(list, node, (void**)&val);
		assert(*val == i * 2);
		free(val);
		node = next->next;
	}
	assert(length_list(list) == HALF_TAB_LEN);

	node = list->begin;
	for (int i = 1; i < 10; node = node->next, i += 2)
		assert(i == *(LIST_TYPE*)(node->p));

	free_list(list);
}
