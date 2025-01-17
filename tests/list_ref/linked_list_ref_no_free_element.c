#include <assert.h>
#include <list_ref/linked_list_ref.h>

#define LIST_TYPE int
LIST_TYPE x = 10;

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(LIST_TYPE));
	list->free_element = NULL;

	linked_list_push_back(list, &x);
	free_linked_list(list);
	return 0;
}
