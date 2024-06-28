#include <assert.h>
#include <list_ref/list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN];

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		push_back_list(list, ptr(TYPE_INT, i));

	tab_from_list(list, tab);
	for (int i = 0; i < TAB_LEN; i++)
		assert(i == tab[i]);

	free_list(list);
}
