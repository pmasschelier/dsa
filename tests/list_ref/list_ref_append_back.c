#include <assert.h>
#include <list_ref/list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	node_list_ref_t* node;
	for (int i = 0; i < TAB_LEN; i++) {
		append_back_list(list, &i);

		node = list->begin;
		for (int j = 0; j < i + 1; j++, node = node->next)
			assert(j == *(int*)(node->p));

		node = list->end;
		for (int j = i; j >= 0; j--, node = node->prev)
			assert(j == *(int*)(node->p));
	}

	free_list(list);
}
