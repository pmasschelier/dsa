#include <assert.h>
#include <btree_ref/btree_ref.h>

#define BT_TYPE int
#define VAL_OFFSET 4
#define PATH_LEN 6
#define TAB_LEN (PATH_LEN + 1)

BT_TYPE numbers[TAB_LEN] = {4, 5, 6, 7, 8, 9};
void* values[TAB_LEN];

btree_path_t pathA = {10, 0x35B};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = &numbers[i];

	btree_ref_t* btree = create_btree(sizeof(BT_TYPE));

	btree_emplace_path(btree, pathA, values, TAB_LEN, VAL_OFFSET);

	node_btree_ref_t* node = btree->root;
	for (int i = 0; i < VAL_OFFSET; i++) {
		/* assert(node->data == NULL); */
		node = *btree_next_node(node, &pathA);
	}
	for (int i = 0; i < TAB_LEN; i++) {
		assert(*(BT_TYPE*)node->data == numbers[i]);
		node = *btree_next_node(node, &pathA);
	}
	btree_free(btree);
	return 0;
}
