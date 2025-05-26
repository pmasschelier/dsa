#include <assert.h>
#include <list_ref/linked_list_ref.h>

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(int));
	assert(list);
	assert(0 == linked_list_length(list));

	linked_list_free(list);
	return 0;
}
