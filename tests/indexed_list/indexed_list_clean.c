#include "subprojects/libtap-0.1.0/tap.h"
#include <indexed_list.h>
#include <tap.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	indexed_list_t* list = indexed_list_from_tab(tab, sizeof(LIST_TYPE), TAB_LEN);
    cmp_mem(list->data, tab, TAB_LEN * sizeof(LIST_TYPE));
	indexed_list_clean(list);
	cmp_ok(indexed_list_length(list), "==", 0);

	indexed_list_free(list);
	return 0;
}
