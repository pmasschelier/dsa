#include <indexed_list.h>
#include <tap.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
LIST_TYPE expected1[TAB_LEN] = {0, 1, 4, 2, 3, 5, 6, 7, 8, 9};
LIST_TYPE expected2[TAB_LEN] = {8, 0, 1, 4, 2, 3, 5, 6, 7, 9};
LIST_TYPE expected3[TAB_LEN] = {8, 0, 1, 4, 2, 3, 5, 6, 9, 7};

int main(void) {
	indexed_list_t* list = indexed_list_from_tab(tab, sizeof(LIST_TYPE), TAB_LEN);
    int i;
    
    indexed_list_move_after(list, 4, 1);
    i = 0;
    indexed_list_foreach(*list, value, LIST_TYPE) {
        cmp_ok(*value, "==", expected1[i]);
        i++;
    }

    indexed_list_move_after(list, 8, -1);
    i = 0;
    indexed_list_foreach(*list, value, LIST_TYPE) {
        cmp_ok(*value, "==", expected2[i]);
        i++;
    }

    indexed_list_move_after(list, 9, 6);
    i = 0;
    indexed_list_foreach(*list, value, LIST_TYPE) {
        cmp_ok(*value, "==", expected3[i]);
        i++;
    }
	indexed_list_free(list);
	return 0;
}
