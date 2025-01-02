#include <assert.h>
#include <stdlib.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "compare.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)
#define INSERT_INDEX 5

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	int ret;
	for (int i = 0; i < TAB_LEN; i++) {
		ret = bsearch_tree_insert(tree, ptr(TYPE_INT, tab[i]), NULL);
		assert(ret == -ERROR_NO_ERROR);
	}
	node_bsearch_tree_ref_t* found;

	ret = bsearch_tree_remove(tree, &tab[INSERT_INDEX]);
	assert(ret == TRUE);
	found = bsearch_tree_find(tree, &tab[INSERT_INDEX]);
	assert(found == NULL);

	free_bsearch_tree_ref(tree);
	return 0;
}
