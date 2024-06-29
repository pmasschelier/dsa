#include <assert.h>
#include <binarytree.h>
#include "ptr.h"

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree_emplace_at(btree, ROOT_PATH, ptr(TYPE_INT, 1));
	btree_emplace_at(btree, LHS_PATH, ptr(TYPE_INT, 2));
	assert(*(BT_TYPE*)btree->root->ls->p == 2);
	btree_emplace_at(btree, LHS_PATH, ptr(TYPE_INT, 3));
	assert(*(BT_TYPE*)btree->root->ls->p == 3);
	btree_free(btree);
	return 0;
}
