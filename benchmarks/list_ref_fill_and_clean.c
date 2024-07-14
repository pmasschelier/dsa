
#include <assert.h>
#include <list_ref/list_ref.h>
#include "ptr.h"

#define COUNT 100000000

int main(void) {
	list_ref_t* list = create_list(sizeof(int));
	for (int i = 0; i < COUNT; i++) {
		push_back_list(list, ptr(TYPE_INT, i));
	}

	free_list(list);
}
