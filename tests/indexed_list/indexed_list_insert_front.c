#include <tap.h>
#include <indexed_list.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	indexed_list_t* list = indexed_list_create(sizeof(LIST_TYPE));
	int index;
	for (int i = 0; i < TAB_LEN; i++) {
		index = indexed_list_insert(list, -1, &i);
		cmp_ok(*get_array_ref(*list, index, LIST_TYPE), "==", i, "Insert %d", i);

        int j = 0;
        indexed_list_foreach_rev(*list, value, LIST_TYPE) {
			cmp_ok(*value, "==", j);
            j++;
        }
        indexed_list_foreach(*list, value, LIST_TYPE) {
            j--;
			cmp_ok(*value, "==", j);
		}
	}

	indexed_list_free(list);
	return 0;
}
