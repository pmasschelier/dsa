#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int

int main(void) {
	btree_t* btree = btree_create(sizeof(BT_TYPE));
    BT_TYPE value = 1;
	btree_emplace_at(btree, ROOT_PATH, &value);
	cmp_ok(*get_node_ref(btree->root, BT_TYPE), "==", 1);
	btree_free(btree);
	return 0;
}
