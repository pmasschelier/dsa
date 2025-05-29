
#include <list_ref/linked_list.h>
#include <compare.h>
#include <tap.h>

#define TAB_LEN 10
#define LIST_TYPE int
LIST_TYPE tab[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

DEFINE_EQUALS_SCALAR(int)

LIST_TYPE a = 6;
LIST_TYPE b = 10;

int main(void) {
	linked_list_t* list = linked_list_from_tab(tab, sizeof(LIST_TYPE), TAB_LEN);
	ok(NULL != linked_list_find_equals(list, &a, equals_int), "%d is in list", a);
	ok(NULL == linked_list_find_equals(list, &b, equals_int), "%d is in list", b);

	linked_list_free(list);
	return 0;
}
