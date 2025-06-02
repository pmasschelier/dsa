#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int

int main(void) {
	btree_t* btree = btree_create(sizeof(BT_TYPE));
	BT_TYPE lhs = 1;
	BT_TYPE rhs = 1;
	ok(NULL == btree_emplace_at(btree, LHS_PATH, &lhs));
	ok(NULL == btree_emplace_at(btree, RHS_PATH, &rhs));
	btree_free(btree);
	return 0;
}
