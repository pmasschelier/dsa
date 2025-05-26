#include <assert.h>
#include <btree_ref/btree_ref.h>
#include <stdlib.h>

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	BT_TYPE lhs = 1;
	BT_TYPE rhs = 1;
	assert(NULL == btree_emplace_at(btree, LHS_PATH, &lhs));
	assert(NULL == btree_emplace_at(btree, RHS_PATH, &rhs));
	btree_free(btree);
	return 0;
}
