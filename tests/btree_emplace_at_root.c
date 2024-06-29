#include <assert.h>
#include <btree_ref.h>
#include "ptr.h"

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree_emplace_at(btree, ROOT_PATH, ptr(TYPE_INT, 1));
	assert(*(BT_TYPE*)btree->root->p == 1);
	btree_free(btree);
	return 0;
}
