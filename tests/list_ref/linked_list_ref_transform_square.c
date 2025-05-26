
#include <assert.h>
#include <list_ref/algorithms.h>

#define LIST_TYPE int
#define TAB_LEN 10
int tab[TAB_LEN] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};

void square(LIST_TYPE* n) {
	*n *= *n;
}

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	int i;
	for (i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, &i);
	transform_list(list, (transform_list_fn_t)square);
	linked_list_node_t* node = list->begin;
	for (i = 0; i < TAB_LEN; node = node->next, i++)
		assert(*get_node_ref(node, LIST_TYPE) == tab[i]);

	linked_list_free(list);
	return 0;
}
