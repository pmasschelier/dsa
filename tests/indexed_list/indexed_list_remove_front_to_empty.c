#include <tap.h>
#include <indexed_list.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	indexed_list_t* list = indexed_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		indexed_list_insert(list, list->end, &i);

	cmp_ok(indexed_list_length(list), "==", TAB_LEN);
	for (int i = 0; i < TAB_LEN; i++)
		indexed_list_remove(list, list->begin, NULL);

	cmp_ok(indexed_list_length(list), "==", 0);

	indexed_list_free(list);
	return 0;
}
