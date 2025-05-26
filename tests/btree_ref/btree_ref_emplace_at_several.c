#include <assert.h>
#include <btree_ref/btree_ref.h>

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
		btree_emplace_at(btree, paths[i], &i);
	node_btree_ref_t* node = btree->root;
	assert(*(BT_TYPE*)node->data == 0);
	assert(*(BT_TYPE*)node->ls->data == 1);
	assert(*(BT_TYPE*)node->ls->rs->data == 2);
	assert(*(BT_TYPE*)node->ls->rs->ls->data == 3);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->data == 4);
	assert(*(BT_TYPE*)node->rs->data == 5);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->data == 6);
	assert(*(BT_TYPE*)node->ls->rs->rs->data == 7);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->data == 8);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->ls->data == 9);
	assert(*(BT_TYPE*)node->ls->rs->ls->rs->rs->rs->rs->data == 10);
	btree_free(btree);
	return 0;
}
