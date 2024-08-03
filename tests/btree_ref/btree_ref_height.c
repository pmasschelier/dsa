#include <assert.h>
#include <btree_ref/btree_ref.h>
#include <ptr.h>
#include <stdlib.h>

#define BT_TYPE int
#define PATHA_LEN 9
#define PATHB_LEN 7
#define PATHB_OFFSET 3

#define TAB_LEN (PATHA_LEN + 1)

BT_TYPE numbers[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
void* values[TAB_LEN];

btree_path_t pathA = {PATHA_LEN, 0x0AF};
btree_path_t pathB = {PATHB_LEN, 0x3F};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = NULL;

	btree_emplace_path(btree, pathA, values, TAB_LEN, 0);
	btree_emplace_path(btree, pathB, values, TAB_LEN, PATHB_OFFSET);

	int height = btree_height(btree);
	assert(height == PATHA_LEN + 1);
	btree_free(btree);
	return 0;
}
