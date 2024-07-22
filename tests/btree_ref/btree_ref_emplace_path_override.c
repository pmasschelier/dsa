#include <assert.h>
#include <btree_ref/btree_ref.h>
#include <ptr.h>
#include <stdio.h>
#include <stdlib.h>

#define BT_TYPE int
#define PATH_LEN 9
#define TAB_LEN (PATH_LEN + 1)

BT_TYPE numbers[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
BT_TYPE override[TAB_LEN] = {10, 1, 12, 3, 14, 5, 16, 7, 18, 9};
void* values[TAB_LEN];

btree_path_t pathA = {PATH_LEN, 0x0AF};

const int FREE_COUNT = TAB_LEN + TAB_LEN / 2;
int free_count;

static void free_counter(void* ptr) {
	(void)ptr;
	free_count++;
}

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));
	btree->free_element = free_counter;

	btree_emplace_path(btree, pathA, values, TAB_LEN, 0);
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = (i % 2 == 0) ? &override[i] : NULL;
	btree_emplace_path(btree, pathA, values, TAB_LEN, 0);

	node_btree_ref_t* node = btree->root;
	assert(*(BT_TYPE*)node->p == override[0]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[1]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[2]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[3]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[4]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[5]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[6]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[7]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[8]);
	node = *btree_next_node(node, &pathA);
	assert(*(BT_TYPE*)node->p == override[9]);
	btree_free(btree);
	assert(free_count == FREE_COUNT);
	return 0;
}
