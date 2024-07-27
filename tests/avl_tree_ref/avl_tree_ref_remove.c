#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)
#define INSERT_INDEX 5

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	int ret;
	for (int i = 0; i < TAB_LEN; i++) {
		ret = avl_tree_insert(tree, ptr(TYPE_INT, tab[i]), NULL);
		assert(ret == -ERROR_NO_ERROR);
	}
	node_avl_tree_ref_t* found;

	ret = avl_tree_remove(tree, &tab[INSERT_INDEX]);
	assert(ret == TRUE);
	found = avl_tree_find(tree, &tab[INSERT_INDEX]);
	assert(found == NULL);

	free_avl_tree_ref(tree);
	return 0;
}
