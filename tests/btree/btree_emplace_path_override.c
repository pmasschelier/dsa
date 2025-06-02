#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int
#define PATH_LEN 9
#define TAB_LEN (PATH_LEN + 1)

BT_TYPE numbers[TAB_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
BT_TYPE override[TAB_LEN] = {10, 1, 12, 3, 14, 5, 16, 7, 18, 9};
void* values[TAB_LEN];

btree_path_t pathA = {PATH_LEN, 0x0AF};

int main(void) {
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = &numbers[i];

	btree_t* btree = btree_create(sizeof(BT_TYPE));

	btree_emplace_path(btree, pathA, values, TAB_LEN, 0);
	for (int i = 0; i < TAB_LEN; i++)
		values[i] = (i % 2 == 0) ? &override[i] : NULL;
	btree_emplace_path(btree, pathA, values, TAB_LEN, 0);

	node_btree_t* node = btree->root;
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[0]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[1]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[2]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[3]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[4]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[5]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[6]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[7]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[8]);
	node = *btree_next_node(node, &pathA);
	cmp_ok(*get_node_ref(node, BT_TYPE), "==", override[9]);
	btree_free(btree);
	return 0;
}
