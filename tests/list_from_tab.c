#include <assert.h>
#include <list_ref.h>

#define TAB_LEN 10
#define TAB_TYPE int
TAB_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = list_from_tab(tab, sizeof(TAB_TYPE), TAB_LEN);
	node_ref_t* node = list->begin;
	int i = 0;
	do {
		assert(i++ == *(int*)(node->p));
	} while (NULL != (node = node->next));

	free_list(list);
}
