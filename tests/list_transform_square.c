
#include <assert.h>
#include <list_ref/algorithms.h>

#define TAB_TYPE int
#define TAB_LEN 10
int tab[TAB_LEN] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};

void square(TAB_TYPE* n) {
	*n *= *n;
}

int main(void) {
	list_ref_t* list = create_list(sizeof(TAB_TYPE));
	int i;
	for (i = 0; i < TAB_LEN; i++)
		push_back_list(list, ptr(TYPE_INT, i));
	transform_list(list, (transform_list_fn_t)square);
	node_ref_t* node = list->begin;
	for (i = 0; i < TAB_LEN; node = node->next, i++)
		assert(*(TAB_TYPE*)node->p == tab[i]);

	free_list(list);
	return 0;
}
