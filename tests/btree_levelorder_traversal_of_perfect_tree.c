#include <assert.h>
#include <binarytree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];
BT_TYPE* out[TAB_LEN];

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_ref_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	btree_levelorder_traversal(btree, (void**)out);

	for (int i = 0; i < TAB_LEN; i++)
		assert(*out[i] == i);
	btree_free(btree);
	return 0;
}
