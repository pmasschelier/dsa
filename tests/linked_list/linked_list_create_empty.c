#include <list_ref/linked_list.h>
#include <tap.h>

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(int));
	ok(list != NULL, "linked_list_create return non-null");
	cmp_ok(linked_list_length(list), "==", 0, "linked_list_create => size");

	linked_list_free(list);
	return 0;
}
