#include <assert.h>
#include <list_ref/linked_list_ref.h>

int main(void) {
	list_ref_t* list = create_linked_list(sizeof(int));
	assert(list);
	assert(0 == linked_list_length(list));

	free_linked_list(list);
	return 0;
}
