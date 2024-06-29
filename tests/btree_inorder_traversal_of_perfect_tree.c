#include <assert.h>
#include <btree_ref.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];
BT_TYPE* out[TAB_LEN];
BT_TYPE expected[TAB_LEN] = {7, 3, 8, 1, 9, 4, 10, 0, 11, 5, 12, 2, 13, 6, 14};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_ref_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	btree_inorder_traversal(btree, (void**)out);

	for (int i = 0; i < TAB_LEN; i++)
		assert(*out[i] == expected[i]);
	btree_free(btree);
	return 0;
}
