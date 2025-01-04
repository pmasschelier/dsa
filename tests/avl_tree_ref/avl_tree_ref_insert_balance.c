#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "btree_ref/bsearch_tree_ref.h"
#include "compare.h"
#include "errors.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

#define TAB_LEN 3

KEY_TYPE tab[4][TAB_LEN] = {
	{1, 2, 3},
	{1, 3, 2},
	{3, 2, 1},
	{3, 1, 2}
};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);

	for (unsigned k = 0; k < 4; k++) {
		for (int i = 0; i < TAB_LEN; i++) {
			int ret = avl_tree_insert(tree, ptr(TYPE_INT, tab[k][i]), NULL);
			assert(ret == -ERROR_NO_ERROR);
		}
		assert(*(int*)tree->root->ls->p == 1);
		assert(*(int*)tree->root->p == 2);
		assert(*(int*)tree->root->rs->p == 3);
		clean_bsearch_tree_ref(tree);
	}
	free_bsearch_tree_ref(tree);
	return 0;
}
