#include <assert.h>
#include <list_ref/list_ref.h>
#include <stdlib.h>

#define TAB_TYPE int
#define TAB_LEN 10

int main(void) {
	list_ref_t* list = create_list(sizeof(TAB_TYPE));
	int i;
	for (i = 0; i < TAB_LEN; i++)
		push_back_list(list, ptr(TYPE_INT, i));
	int* elem;
	pop_back_list(list, (void**)&elem);
	while (i--, elem) {
		assert(i == *elem);
		free(elem);
		pop_back_list(list, (void**)&elem);
	}
	free(elem);
	assert(empty_list(list));

	push_back_list(list, ptr(TYPE_INT, 0));
	assert(1 == length_list(list));

	free_list(list);
	return 0;
}
