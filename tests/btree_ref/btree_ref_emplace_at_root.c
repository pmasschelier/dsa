#include <assert.h>
#include <btree_ref/btree_ref.h>

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
    BT_TYPE value = 1;
	btree_emplace_at(btree, ROOT_PATH, &value);
	assert(*(BT_TYPE*)btree->root->data == 1);
	btree_free(btree);
	return 0;
}
