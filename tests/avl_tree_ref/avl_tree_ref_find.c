#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "errors.h"
#include "list_ref/list_ref.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)
#define INSERT_INDEX 5

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	for (int i = 0; i < TAB_LEN; i++) {
		int ret = avl_tree_insert(tree, ptr(TYPE_INT, tab[i]), NULL);
		assert(ret == -ERROR_NO_ERROR);
	}
	node_avl_tree_ref_t* found;
	found = avl_tree_find(tree, &tab[INSERT_INDEX]);
	assert(found != NULL);
	assert(*get_node_ref(found, int) == tab[INSERT_INDEX]);
	assert(*get_node_ref(found->ls, int) < *get_node_ref(found, int));
	assert(*get_node_ref(found, int) < *get_node_ref(found->rs, int));
	free_avl_tree_ref(tree);
	return 0;
}
