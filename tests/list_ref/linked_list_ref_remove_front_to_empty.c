#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, &i);

	assert(linked_list_length(list) == TAB_LEN);
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_remove(list, list->begin, NULL);

	assert(linked_list_empty(list));

	linked_list_free(list);
	return 0;
}
