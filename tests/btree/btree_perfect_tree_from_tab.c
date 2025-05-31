#include <assert.h>
#include <btree/btree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	node_btree_t* node = btree->root;
	assert(*(BT_TYPE*)node->data == 0);
	assert(*(BT_TYPE*)node->ls->data == 1);
	assert(*(BT_TYPE*)node->rs->data == 2);
	assert(*(BT_TYPE*)node->ls->ls->data == 3);
	assert(*(BT_TYPE*)node->ls->rs->data == 4);
	assert(*(BT_TYPE*)node->rs->ls->data == 5);
	assert(*(BT_TYPE*)node->rs->rs->data == 6);
	assert(*(BT_TYPE*)node->ls->ls->ls->data == 7);
	assert(*(BT_TYPE*)node->ls->ls->rs->data == 8);
	assert(*(BT_TYPE*)node->ls->rs->ls->data == 9);
	assert(*(BT_TYPE*)node->ls->rs->rs->data == 10);
	assert(*(BT_TYPE*)node->rs->ls->ls->data == 11);
	assert(*(BT_TYPE*)node->rs->ls->rs->data == 12);
	assert(*(BT_TYPE*)node->rs->rs->ls->data == 13);
	assert(*(BT_TYPE*)node->rs->rs->rs->data == 14);
	btree_free(btree);
	return 0;
}
