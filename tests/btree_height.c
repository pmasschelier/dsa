#include <assert.h>
#include <binarytree.h>
#include <ptr.h>
#include <stdlib.h>

#define BT_TYPE int
#define PATHA_LEN 10
#define PATHB_LEN 7
#define PATHB_OFFSET 4

BT_TYPE numbers[PATHA_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
void* values[PATHA_LEN];

btree_path_t pathA = {10, 0x0AF};
btree_path_t pathB = {7, 0x3F};

int main(void) {
	for (int i = 0; i < PATHA_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = NULL;

	btree_emplace_path(btree, pathA, values, PATHA_LEN, 0);
	btree_emplace_path(btree, pathB, values, PATHB_LEN, PATHB_OFFSET);

	assert(btree_height(btree) == PATHA_LEN);
	btree_free(btree);
	return 0;
}
