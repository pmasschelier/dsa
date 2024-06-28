#include <assert.h>
#include <binarytree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_ref_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	node_btree_ref_t* node = btree->root;
	assert(*(BT_TYPE*)node->p == 0);
	assert(*(BT_TYPE*)node->ls->p == 1);
	assert(*(BT_TYPE*)node->rs->p == 2);
	assert(*(BT_TYPE*)node->ls->ls->p == 3);
	assert(*(BT_TYPE*)node->ls->rs->p == 4);
	assert(*(BT_TYPE*)node->rs->ls->p == 5);
	assert(*(BT_TYPE*)node->rs->rs->p == 6);
	assert(*(BT_TYPE*)node->ls->ls->ls->p == 7);
	assert(*(BT_TYPE*)node->ls->ls->rs->p == 8);
	assert(*(BT_TYPE*)node->ls->rs->ls->p == 9);
	assert(*(BT_TYPE*)node->ls->rs->rs->p == 10);
	assert(*(BT_TYPE*)node->rs->ls->ls->p == 11);
	assert(*(BT_TYPE*)node->rs->ls->rs->p == 12);
	assert(*(BT_TYPE*)node->rs->rs->ls->p == 13);
	assert(*(BT_TYPE*)node->rs->rs->rs->p == 14);
	btree_free(btree);
	return 0;
}
