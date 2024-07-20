
#include <assert.h>
#include <btree_ref/btree_ref.h>

#define TAB_LEN 0

int main(void) {
	btree_ref_t* btree =
		btree_perfect_tree_from_tab(NULL, sizeof(int), TAB_LEN);
	assert(btree != NULL);
	assert(btree_length(btree) == 0);
	btree_free(btree);
	return 0;
}
