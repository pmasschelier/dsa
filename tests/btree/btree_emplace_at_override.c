#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int

BT_TYPE values[3] = {1, 2, 3};

int main(void) {
	btree_t* btree = btree_create(sizeof(BT_TYPE));
	btree_emplace_at(btree, ROOT_PATH, &values[0]);
	btree_emplace_at(btree, LHS_PATH, &values[1]);
	cmp_ok(*get_node_ref(btree->root->ls, BT_TYPE), "==", 2);
	btree_emplace_at(btree, LHS_PATH, &values[2]);
	cmp_ok(*get_node_ref(btree->root->ls, BT_TYPE), "==", 3);
	btree_free(btree);
	return 0;
}
