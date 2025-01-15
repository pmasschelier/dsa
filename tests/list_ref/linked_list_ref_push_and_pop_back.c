#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define LIST_TYPE int
#define TAB_LEN 6
const int tab[TAB_LEN] = {0, 1, 2, 0, 1, 2};

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	for (int i = 0; i < 10; i++)
		linked_list_push_back(list, ptr(TYPE_INT, i));
	for (int i = 0; i < 7; i++)
		linked_list_pop_back(list, NULL);
	for (int i = 0; i < 5; i++)
		linked_list_push_back(list, ptr(TYPE_INT, i));
	for (int i = 0; i < 2; i++)
		linked_list_pop_back(list, NULL);
	int i = 0;
	assert(linked_list_length(list) == TAB_LEN);
	node_list_ref_t* node = list->begin;
	do {
		assert(*(int*)(node->p) == tab[i++]);
	} while ((node = node->next));

	free_linked_list(list);
	return 0;
}
