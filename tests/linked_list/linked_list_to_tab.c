#include <tap.h>
#include <list_ref/linked_list.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN];

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, &i);

	linked_list_to_tab(list, tab);
	for (int i = 0; i < TAB_LEN; i++)
		cmp_ok(tab[i], "==", i);

	linked_list_free(list);
	return 0;
}
