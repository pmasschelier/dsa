#include <tap.h>
#include <btree/btree.h>

#define TAB_LEN 0

int main(void) {
	btree_t* btree =
		btree_perfect_tree_from_tab(NULL, sizeof(int), TAB_LEN);
	ok(btree != NULL);
	cmp_ok(btree_length(btree), "==", 0);
	btree_free(btree);
	return 0;
}
