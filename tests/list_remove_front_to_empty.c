#include <assert.h>
#include <list_ref/list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		push_back_list(list, ptr(TYPE_INT, i));

	assert(length_list(list) == TAB_LEN);
	for (int i = 0; i < TAB_LEN; i++)
		remove_list(list, list->begin, NULL);

	assert(empty_list(list));

	free_list(list);
}
