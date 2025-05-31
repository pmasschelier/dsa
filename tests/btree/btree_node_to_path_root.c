#include <assert.h>
#include <btree/btree.h>

int main(void) {
	assert(btree_node_to_path(0).length == 0);
	return 0;
}
