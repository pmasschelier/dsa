#include <assert.h>
#include "btree/bsearch_tree.h"
#include "compare.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	for (int i = 0; i < TAB_LEN; i++) {
		int ret = bsearch_tree_insert(tree, &tab[i], NULL);
		assert(ret == -ERROR_NO_ERROR);
	}
	assert(tree->root != NULL);
	free_bsearch_tree(tree);
	return 0;
}
