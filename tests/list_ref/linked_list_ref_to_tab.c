#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN];

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, ptr(TYPE_INT, i));

	linked_list_to_tab(list, tab);
	for (int i = 0; i < TAB_LEN; i++)
		assert(i == tab[i]);

	free_linked_list(list);
	return 0;
}
