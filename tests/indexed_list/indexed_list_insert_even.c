#include <tap.h>
#include <indexed_list.h>

#define TAB_LEN 10
#define LIST_TYPE int

int main(void) {
	indexed_list_t* list = indexed_list_create(sizeof(LIST_TYPE));
	for (int i = 1; i < TAB_LEN; i += 2)
		indexed_list_insert(list, list->end, &i);
	int i = 0, created;
    indexed_list_foreach(*list, index) {
		created = indexed_list_insert(list, list->prev[index], &i);
        cmp_ok(created, "==", list->prev[index]);
        i += 2;
    }
    
    i = 0;
    indexed_list_foreach(*list, value, LIST_TYPE) {
        cmp_ok(*value, "==", i++);
    }

	indexed_list_free(list);
	return 0;
}
