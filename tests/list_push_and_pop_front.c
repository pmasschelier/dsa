#include <assert.h>
#include <list_ref/list_ref.h>

#define LIST_TYPE int
#define TAB_LEN 6
const int tab[TAB_LEN] = {0, 1, 2, 0, 1, 2};

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	for (int i = 0; i < 10; i++)
		push_front_list(list, ptr(TYPE_INT, i));
	for (int i = 0; i < 7; i++)
		pop_front_list(list, NULL);
	for (int i = 0; i < 5; i++)
		push_front_list(list, ptr(TYPE_INT, i));
	for (int i = 0; i < 2; i++)
		pop_front_list(list, NULL);
	int i = 0;
	assert(length_list(list) == TAB_LEN);
	list_node_ref_t* node = list->end;
	do {
		assert(*(int*)(node->p) == tab[i++]);
	} while ((node = node->prev));

	free_list(list);
	return 0;
}
