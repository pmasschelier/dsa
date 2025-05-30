#include <tap.h>
#include <indexed_list.h>

#define HALF_TAB_LEN 5
#define TAB_LEN (HALF_TAB_LEN << 1)
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	indexed_list_t* list = indexed_list_create(sizeof(LIST_TYPE));
	for (int i = 0; i < TAB_LEN; i++)
		indexed_list_insert(list, list->end, &i);
	cmp_ok(indexed_list_length(list), "==", TAB_LEN);
    
    LIST_TYPE val;
    indexed_list_foreach(*list, index) {
        index = indexed_list_remove(list, index, &val);
    }
	cmp_ok(indexed_list_length(list), "==", HALF_TAB_LEN);

    int i = 1;
    indexed_list_foreach(*list, value, LIST_TYPE) {
        cmp_ok(*value, "==", i);
        i += 2;
    }

	indexed_list_free(list);
	return 0;
}
