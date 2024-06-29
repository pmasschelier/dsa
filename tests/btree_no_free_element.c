#include <assert.h>
#include <btree_ref.h>

#define BT_TYPE int

BT_TYPE x = 10;

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = NULL;
	btree_emplace_at(btree, ROOT_PATH, &x);
	btree_free(btree);
	return 0;
}
