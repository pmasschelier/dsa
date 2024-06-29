#include <assert.h>
#include <btree_ref.h>
#include <ptr.h>
#include <stdlib.h>

#define BT_TYPE int
#define PATH_LEN 10

BT_TYPE numbers[PATH_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
void* values[PATH_LEN];

btree_path_t pathA = {10, 0x0AF};

int main(void) {
	for (int i = 0; i < PATH_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = NULL;

	btree_emplace_path(btree, pathA, values, PATH_LEN, 0);
	node_btree_ref_t* node = btree->root;
	assert(*(BT_TYPE*)node->p == numbers[0]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[1]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[2]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[3]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[4]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[5]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[6]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[7]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[8]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == numbers[9]);
	btree_free(btree);
	return 0;
}
