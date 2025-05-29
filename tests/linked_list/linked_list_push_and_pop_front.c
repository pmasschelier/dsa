#include <tap.h>
#include <list_ref/linked_list.h>

#define LIST_TYPE int
#define TAB_LEN 6
const int tab[TAB_LEN] = {0, 1, 2, 0, 1, 2};

int main(void) {
	linked_list_t* list = linked_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < 10; i++)
		linked_list_push_front(list, &i);
	for (int i = 0; i < 7; i++)
		linked_list_pop_front(list, NULL);
	for (int i = 0; i < 5; i++)
		linked_list_push_front(list, &i);
	for (int i = 0; i < 2; i++)
		linked_list_pop_front(list, NULL);
	int i = 0;
	cmp_ok(linked_list_length(list), "==", TAB_LEN);
	linked_list_node_t* node = list->end;
	do {
		cmp_ok(*get_node_ref(node, LIST_TYPE), "==", tab[i++]);
	} while ((node = node->prev));

	linked_list_free(list);
	return 0;
}
