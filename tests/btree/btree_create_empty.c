#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int

int main(void) {
	btree_t* btree = btree_create(sizeof(BT_TYPE));
	cmp_ok(btree_height(btree), "==", 0);
	cmp_ok(btree_length(btree), "==", 0);
	btree_free(btree);
	return 0;
}
