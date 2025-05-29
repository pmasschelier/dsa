#include <tap.h>
#include <list_ref/algorithms.h>

struct time {
	int h, m, s;
};

#define TAB_LEN 3
#define LIST_TYPE struct time
LIST_TYPE tab[TAB_LEN] = {
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8}
};

LIST_TYPE x = {10, 11, 12};

int main(void) {
	linked_list_t* list = linked_list_from_tab(tab, sizeof(LIST_TYPE), TAB_LEN);
	linked_list_node_t* node;
	node = find_in_list(list, &x);
	ok(node == NULL, "%d found in list", x);

	linked_list_free(list);
	return 0;
}
