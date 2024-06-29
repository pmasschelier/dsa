#include <assert.h>
#include <btree_ref.h>

#define BT_TYPE int

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	assert(btree_height(btree) == 0);
	assert(btree_length(btree) == 0);
	btree_free(btree);
	return 0;
}
