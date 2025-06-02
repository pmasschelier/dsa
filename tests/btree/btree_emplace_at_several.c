#include <btree/btree.h>
#include <tap.h>

#define BT_TYPE int
#define PATHS_LEN 11

btree_path_t paths[PATHS_LEN] = {
    {0, 0x0},  {1, 0x0}, {2, 0x2},  {3, 0x2},  {4, 0xA},  {1, 0x1},
    {5, 0x1A}, {3, 0x6}, {6, 0x3A}, {7, 0x3A}, {7, 0x7A},
};

int main(void) {
    btree_t *btree = btree_create(sizeof(BT_TYPE));
    for (int i = 0; i < PATHS_LEN; i++)
        btree_emplace_at(btree, paths[i], &i);
    btree_node_t *node = btree->root;
    cmp_ok(*get_node_ref(node, BT_TYPE), "==", 0);
    cmp_ok(*get_node_ref(node->ls, BT_TYPE), "==", 1);
    cmp_ok(*get_node_ref(node->ls->rs, BT_TYPE), "==", 2);
    cmp_ok(*get_node_ref(node->ls->rs->ls, BT_TYPE), "==", 3);
    cmp_ok(*get_node_ref(node->ls->rs->ls->rs, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(node->rs, BT_TYPE), "==", 5);
    cmp_ok(*get_node_ref(node->ls->rs->ls->rs->rs, BT_TYPE), "==", 6);
    cmp_ok(*get_node_ref(node->ls->rs->rs, BT_TYPE), "==", 7);
    cmp_ok(*get_node_ref(node->ls->rs->ls->rs->rs->rs, BT_TYPE), "==", 8);
    cmp_ok(*get_node_ref(node->ls->rs->ls->rs->rs->rs->ls, BT_TYPE), "==", 9);
    cmp_ok(*get_node_ref(node->ls->rs->ls->rs->rs->rs->rs, BT_TYPE), "==", 10);
    btree_free(btree);
    return 0;
}
