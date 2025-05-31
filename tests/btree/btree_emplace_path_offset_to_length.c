#include <assert.h>
#include <btree/btree.h>

#define BT_TYPE int
#define VAL_OFFSET 4
#define TAB_LEN 4

BT_TYPE numbers[TAB_LEN] = {4, 5, 6, 7};
void* values[TAB_LEN];

btree_path_t pathA = {10, 0x35B};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = &numbers[i];

	btree_t* btree = create_btree(sizeof(BT_TYPE));

	btree_emplace_path(btree, pathA, values, TAB_LEN, VAL_OFFSET);

	node_btree_t* node = btree->root;
	for (int i = 0; i < VAL_OFFSET; i++) {
		/* assert(node->data == NULL); */
		node = *btree_next_node(node, &pathA);
	}
	for (int i = 0; i < TAB_LEN; i++) {
		assert(*(BT_TYPE*)node->data == numbers[i]);
		node = *btree_next_node(node, &pathA);
	}
	while (node) {
		/* assert(node->data == NULL); */
		node = *btree_next_node(node, &pathA);
	}
	btree_free(btree);
	return 0;
}
