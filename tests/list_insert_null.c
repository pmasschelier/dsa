#include <assert.h>
#include <list_ref/list_ref.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	list_ref_t* list = create_list(sizeof(LIST_TYPE));
	node_list_ref_t* node;
	for (int i = 0; i < TAB_LEN; i++) {
		node = insert_list(list, NULL, ptr(TYPE_INT, i));
		assert(i == *(LIST_TYPE*)node->p);

		node = list->begin;
		for (int j = i; j >= 0; j--, node = node->next) {
			assert(j == *(int*)(node->p));
		}

		node = list->end;
		for (int j = 0; j < i + 1; j++, node = node->prev) {
			assert(j == *(int*)(node->p));
		}
	}

	free_list(list);
}
