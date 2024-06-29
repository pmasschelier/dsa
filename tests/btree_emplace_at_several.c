#include <assert.h>
#include <btree_ref/btree_ref.h>
#include "ptr.h"

#define BT_TYPE int
#define PATHS_LEN 11

btree_path_t paths[PATHS_LEN] = {
	{0,	0x0},
	   {1,  0x0},
	 {2,	 0x2},
	{3,	0x2},
	   {4,  0xA},
	 {1,	 0x1},
	{5, 0x1A},
	   {3,  0x6},
	 {6, 0x3A},
	{7, 0x3A},
	   {7, 0x7A}
};

int main(void) {
	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	for (int i = 0; i < PATHS_LEN; i++)
		btree_emplace_at(btree, paths[i], ptr(TYPE_INT, i));
	node_btree_ref_t* node = btree->root;
	assert(*(BT_TYPE*)node->p == 0);
	assert(*(BT_TYPE*)node->ls->p == 1);
	assert(*(BT_TYPE*)node->ls->rs->p == 2);
	assert(*(BT_TYPE*)node->ls->rs->ls->p == 3);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->p == 4);
	assert(*(BT_TYPE*)node->rs->p == 5);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->p == 6);
	assert(*(BT_TYPE*)node->ls->rs->rs->p == 7);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->p == 8);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->ls->p == 9);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->rs->p == 10);
	btree_free(btree);
	return 0;
}
