#include <assert.h>
#include <list_ref/list_ref.h>

#define TAB_LEN 10
#define TAB_TYPE int

int main(void) {
	list_ref_t* list = create_list(sizeof(TAB_TYPE));
	for (int i = 1; i < TAB_LEN; i += 2)
		push_back_list(list, ptr(TYPE_INT, i));
	node_ref_t* node = NULL;
	for (int i = 0; i < TAB_LEN; i += 2) {
		node = insert_list(list, node, ptr(TYPE_INT, i));
		node = node->next;
	}

	node = list->begin;
	for (int i = 0; i < TAB_LEN; i++, node = node->next) {
		assert(i == *(TAB_TYPE*)node->p);
	}

	free_list(list);
}
