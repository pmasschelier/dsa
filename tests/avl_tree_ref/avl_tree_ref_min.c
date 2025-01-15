#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "list_ref/linked_list_ref.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define PERFECT_TREE_HEIGHT 4
#define TAB_LEN (1 << PERFECT_TREE_HEIGHT)
#define INSERT_INDEX 5

KEY_TYPE tab[TAB_LEN] = {11, 0, 15, 5, 4, 13, 6, 8, 12, 2, 1, 7, 10, 9, 14, 3};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	for (int i = 0; i < TAB_LEN; i++) {
		avl_tree_insert(tree, ptr(TYPE_INT, tab[i]), NULL);
	}
	node_bsearch_tree_ref_t* found;
	found = bsearch_tree_min(tree);
	assert(*get_node_ref(found, int) == 0);
	free_bsearch_tree_ref(tree);
	return 0;
}
