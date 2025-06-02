#include <tap.h>
#include <btree/btree.h>

int main(void) {
	cmp_ok(btree_node_to_path(0).length, "==", 0);
	return 0;
}
