#include <tap.h>
#include <array_list.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void) {
	array_list_t list = ARRAY_LIST_INIT(LIST_TYPE);
    array_list_append(&list, tab, TAB_LEN);
    
    array_list_foreach(list, value, LIST_TYPE) {
        cmp_ok(*value, "==", tab[index]);
    }

	array_list_deinit(&list);
	return 0;
}
