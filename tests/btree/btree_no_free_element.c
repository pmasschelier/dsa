#include <btree/btree.h>

#define BT_TYPE int

BT_TYPE x = 10;

int main(void) {
	btree_t* btree = btree_create(sizeof(BT_TYPE));
	btree_emplace_at(btree, ROOT_PATH, &x);
	btree_free(btree);
	return 0;
}
