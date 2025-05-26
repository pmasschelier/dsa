#include <assert.h>
#include <list_ref/linked_list.h>

#define LIST_TYPE int
#define TAB_LEN 10

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	int i;
	for (i = 0; i < TAB_LEN; i++)
		linked_list_push_back(list, &i);
	int elem;
	while (linked_list_pop_back(list, &elem)) {
        i--;
		assert(i == elem);
	}
	assert(linked_list_empty(list));

    int zero = 0;
	linked_list_push_back(list, &zero);
	assert(1 == linked_list_length(list));

	linked_list_free(list);
	return 0;
}
