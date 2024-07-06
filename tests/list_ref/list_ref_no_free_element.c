#include <assert.h>
#include <list_ref/list_ref.h>

#define LIST_TYPE int
LIST_TYPE x = 10;

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	list->free_element = NULL;

	push_back_list(list, &x);
	free_list(list);
}
