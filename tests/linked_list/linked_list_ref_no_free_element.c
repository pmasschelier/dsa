#include <assert.h>
#include <list_ref/linked_list.h>

#define LIST_TYPE int
LIST_TYPE x = 10;

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));

	linked_list_push_back(list, &x);
	linked_list_free(list);
	return 0;
}
