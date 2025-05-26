#include <assert.h>
#include <btree_ref/btree_ref.h>

#define BT_TYPE int

BT_TYPE values[3] = {1, 2, 3};

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree_emplace_at(btree, ROOT_PATH, &values[0]);
	btree_emplace_at(btree, LHS_PATH, &values[1]);
	assert(*(BT_TYPE*)btree->root->ls->data == 2);
	btree_emplace_at(btree, LHS_PATH, &values[2]);
	assert(*(BT_TYPE*)btree->root->ls->data == 3);
	btree_free(btree);
	return 0;
}
