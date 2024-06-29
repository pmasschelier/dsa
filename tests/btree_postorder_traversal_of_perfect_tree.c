#include <assert.h>
#include <btree_ref/btree_ref.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];
BT_TYPE* out[TAB_LEN];
BT_TYPE expected[TAB_LEN] = {7, 8, 3, 9, 10, 4, 1, 11, 12, 5, 13, 14, 6, 2, 0};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_ref_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	btree_postorder_traversal(btree, (void**)out);

	for (int i = 0; i < TAB_LEN; i++)
		assert(*out[i] == expected[i]);
	btree_free(btree);
	return 0;
}
