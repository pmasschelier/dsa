#include <assert.h>
#include <list.h>

#define TAB_LEN 10
#define TAB_TYPE int
TAB_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = create_list(sizeof(TAB_TYPE));
	node_ref_t* node;
	for (int i = 0; i < TAB_LEN; i++) {
		push_front_list(list, ptr(TYPE_INT, i));

		node = list->end;
		for (int j = 0; j < i + 1; j++, node = node->prev)
			assert(j == *(int*)(node->p));

		node = list->begin;
		for (int j = i; j >= 0; j--, node = node->next)
			assert(j == *(int*)(node->p));
	}

	free_list(list);
}
