#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		tab[i] = i;
	btree_t* btree =
		btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
	btree_node_t* node = btree->root;
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", 0);
	cmp_ok(*get_node_ref(node->ls, BT_TYPE), "==", 1);
	cmp_ok(*get_node_ref(node->rs, BT_TYPE), "==", 2);
	cmp_ok(*get_node_ref(node->ls->ls, BT_TYPE), "==", 3);
	cmp_ok(*get_node_ref(node->ls->rs, BT_TYPE), "==", 4);
	cmp_ok(*get_node_ref(node->rs->ls, BT_TYPE), "==", 5);
	cmp_ok(*get_node_ref(node->rs->rs, BT_TYPE), "==", 6);
	cmp_ok(*get_node_ref(node->ls->ls->ls, BT_TYPE), "==", 7);
	cmp_ok(*get_node_ref(node->ls->ls->rs, BT_TYPE), "==", 8);
	cmp_ok(*get_node_ref(node->ls->rs->ls, BT_TYPE), "==", 9);
	cmp_ok(*get_node_ref(node->ls->rs->rs, BT_TYPE), "==", 10);
	cmp_ok(*get_node_ref(node->rs->ls->ls, BT_TYPE), "==", 11);
	cmp_ok(*get_node_ref(node->rs->ls->rs, BT_TYPE), "==", 12);
	cmp_ok(*get_node_ref(node->rs->rs->ls, BT_TYPE), "==", 13);
	cmp_ok(*get_node_ref(node->rs->rs->rs, BT_TYPE), "==", 14);
	btree_free(btree);
	return 0;
}
