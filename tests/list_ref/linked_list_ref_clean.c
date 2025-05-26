
#include <assert.h>
#include <list_ref/linked_list.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	linked_list_t* list = linked_list_from_tab(tab, sizeof(LIST_TYPE), TAB_LEN);
	linked_list_clean(list);
	assert(0 == linked_list_length(list));

	linked_list_free(list);
	return 0;
}
