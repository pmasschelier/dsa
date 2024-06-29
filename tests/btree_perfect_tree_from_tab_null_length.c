
#include <assert.h>
#include <binarytree.h>

#define TAB_LEN 0

int main(void) {
	btree_ref_t* btree = btree_perfect_tree_from_tab(NULL, 0, TAB_LEN);
	assert(btree == NULL);
	return 0;
}
