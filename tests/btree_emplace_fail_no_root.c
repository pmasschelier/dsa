#include <assert.h>
#include <btree_ref/btree_ref.h>
#include <ptr.h>
#include <stdlib.h>

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	void* lhs = ptr(TYPE_INT, 1);
	void* rhs = ptr(TYPE_INT, 1);
	assert(NULL == btree_emplace_at(btree, LHS_PATH, lhs));
	assert(NULL == btree_emplace_at(btree, RHS_PATH, rhs));
	free(lhs);
	free(rhs);
	btree_free(btree);
	return 0;
}
