#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "errors.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	for (int i = 0; i < TAB_LEN; i++) {
		int ret = avl_tree_insert(tree, ptr(TYPE_INT, tab[i]), NULL);
		assert(ret == -ERROR_NO_ERROR);
	}
	assert(tree->root != NULL);
	assert(tree->root->subtree >= PERFECT_TREE_HEIGHT);
	assert(tree->root->subtree <= 2 * PERFECT_TREE_HEIGHT);
	free_avl_tree_ref(tree);
	return 0;
}
