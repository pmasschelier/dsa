#include <indexed_list.h>
#include <tap.h>

int main(void) {
	indexed_list_t* list = indexed_list_create(sizeof(int));
	ok(list != NULL);
	cmp_ok(indexed_list_length(list), "==", 0);

	indexed_list_free(list);
	return 0;
}
