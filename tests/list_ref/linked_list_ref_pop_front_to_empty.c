#include <assert.h>
#include <list_ref/linked_list_ref.h>
#include <stdlib.h>

#define LIST_TYPE int
#define TAB_LEN 10

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, ptr(TYPE_INT, i));
	int* elem;
	linked_list_pop_front(list, (void**)&elem);
	for (int i = 0; elem; i++) {
		assert(i == *elem);
		free(elem);
		linked_list_pop_front(list, (void**)&elem);
	}
	free(elem);
	assert(linked_list_empty(list));

	linked_list_push_back(list, ptr(TYPE_INT, 0));
	assert(1 == linked_list_length(list));

	free_linked_list(list);
	return 0;
}
