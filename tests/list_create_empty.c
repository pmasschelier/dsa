
#include <assert.h>
#include <list.h>

int main(void) {
	list_ref_t* list = create_list(sizeof(int));
	assert(list);
	assert(0 == length_list(list));

	free_list(list);
}
