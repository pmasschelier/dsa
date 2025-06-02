#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];
BT_TYPE* out[TAB_LEN];
BT_TYPE expected[TAB_LEN] = {7, 3, 8, 1, 9, 4, 10, 0, 11, 5, 12, 2, 13, 6, 14};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	btree_dfs_array(btree, NULL, (void**)out, NULL);

	for (int i = 0; i < TAB_LEN; i++)
		cmp_ok(*out[i], "==", expected[i]);
	btree_free(btree);
	return 0;
}
