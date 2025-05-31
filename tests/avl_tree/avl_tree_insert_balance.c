#include <assert.h>
#include "btree/avl_tree.h"
#include "btree/bsearch_tree.h"
#include "compare.h"
#include "errors.h"

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
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);

	for (unsigned k = 0; k < 4; k++) {
		for (int i = 0; i < TAB_LEN; i++) {
			int ret = avl_tree_insert(tree, &tab[k][i], NULL);
			assert(ret == -ERROR_NO_ERROR);
		}
		assert(*(int*)tree->root->ls->data == 1);
		assert(*(int*)tree->root->data == 2);
		assert(*(int*)tree->root->rs->data == 3);
		clean_bsearch_tree(tree);
	}
	free_bsearch_tree(tree);
	return 0;
}
