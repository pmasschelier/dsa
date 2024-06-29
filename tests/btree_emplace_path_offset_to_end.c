#include <assert.h>
#include <btree_ref/btree_ref.h>
#include <ptr.h>
#include <stdlib.h>

#define BT_TYPE int
#define VAL_OFFSET 4
#define PATH_LEN 6

BT_TYPE numbers[PATH_LEN] = {4, 5, 6, 7, 8, 9};
void* values[PATH_LEN];

btree_path_t pathA = {10, 0x35B};

int main(void) {
	for (int i = 0; i < PATH_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = NULL;

	btree_emplace_path(btree, pathA, values, PATH_LEN, VAL_OFFSET);

	node_btree_ref_t* node = btree->root;
	for (int i = 0; i < 4; i++) {
		assert(node->p == NULL);
		node = *btree_next_node(node, &pathA);
	}
	for (int i = 0; i < PATH_LEN; i++) {
		assert(*(BT_TYPE*)node->p == numbers[i]);
		node = *btree_next_node(node, &pathA);
	}
	btree_free(btree);
	return 0;
}
