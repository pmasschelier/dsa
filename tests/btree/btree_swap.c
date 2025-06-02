#include <tap.h>
#include <btree/btree.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];

int main(void) {
    for (int i = 0; i < TAB_LEN; i++)
        tab[i] = i;
    btree_t *btree = btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
    btree_node_t *root = btree->root;
    cmp_ok(*get_node_ref(root, BT_TYPE), "==", 0);
    cmp_ok(*get_node_ref(root->ls, BT_TYPE), "==", 1);
    cmp_ok(*get_node_ref(root->rs, BT_TYPE), "==", 2);
    cmp_ok(*get_node_ref(root->ls->ls, BT_TYPE), "==", 3);
    cmp_ok(*get_node_ref(root->ls->rs, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(root->rs->ls, BT_TYPE), "==", 5);
    cmp_ok(*get_node_ref(root->rs->rs, BT_TYPE), "==", 6);
    cmp_ok(*get_node_ref(root->ls->ls->ls, BT_TYPE), "==", 7);
    cmp_ok(*get_node_ref(root->ls->ls->rs, BT_TYPE), "==", 8);
    cmp_ok(*get_node_ref(root->ls->rs->ls, BT_TYPE), "==", 9);
    cmp_ok(*get_node_ref(root->ls->rs->rs, BT_TYPE), "==", 10);
    cmp_ok(*get_node_ref(root->rs->ls->ls, BT_TYPE), "==", 11);
    cmp_ok(*get_node_ref(root->rs->ls->rs, BT_TYPE), "==", 12);
    cmp_ok(*get_node_ref(root->rs->rs->ls, BT_TYPE), "==", 13);
    cmp_ok(*get_node_ref(root->rs->rs->rs, BT_TYPE), "==", 14);
    
    btree_node_t** a = &root->ls;
    btree_node_t** b = &root->rs->rs;
    btree_swap_node(&a, &b);
    cmp_ok(*get_node_ref(*a, BT_TYPE), "==", 6);
    cmp_ok(*get_node_ref(*b, BT_TYPE), "==", 1);

    cmp_ok(*get_node_ref(root->ls, BT_TYPE), "==", 6);
    cmp_ok(*get_node_ref(root->ls->parent, BT_TYPE), "==", 0);
    cmp_ok(*get_node_ref(root->ls->ls, BT_TYPE), "==", 3);
    cmp_ok(*get_node_ref(root->ls->ls->parent, BT_TYPE), "==", 6);
    cmp_ok(*get_node_ref(root->ls->rs, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(root->ls->rs->parent, BT_TYPE), "==", 6);

    cmp_ok(*get_node_ref(root->rs->rs, BT_TYPE), "==", 1);
    cmp_ok(*get_node_ref(root->rs->rs->parent, BT_TYPE), "==", 2);
    cmp_ok(*get_node_ref(root->rs->rs->ls, BT_TYPE), "==", 13);
    cmp_ok(*get_node_ref(root->rs->rs->ls->parent, BT_TYPE), "==", 1);
    cmp_ok(*get_node_ref(root->rs->rs->rs, BT_TYPE), "==", 14);
    cmp_ok(*get_node_ref(root->rs->rs->rs->parent, BT_TYPE), "==", 1);
    btree_free(btree);

    btree = btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
    root = btree->root;
    btree_node_t** c = &root->ls;
    btree_node_t** d = &root->ls->rs;
    btree_swap_node(&c, &d);
    cmp_ok(*get_node_ref(*c, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(*d, BT_TYPE), "==", 1);

    cmp_ok(*get_node_ref(root->ls, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(root->ls->parent, BT_TYPE), "==", 0);
    cmp_ok(*get_node_ref(root->ls->ls, BT_TYPE), "==", 3);
    cmp_ok(*get_node_ref(root->ls->ls->parent, BT_TYPE), "==", 4);
    cmp_ok(*get_node_ref(root->ls->rs, BT_TYPE), "==", 1);
    cmp_ok(*get_node_ref(root->ls->rs->ls, BT_TYPE), "==", 9);
    cmp_ok(*get_node_ref(root->ls->rs->rs, BT_TYPE), "==", 10);
    cmp_ok(*get_node_ref(root->ls->rs->parent, BT_TYPE), "==", 4);

    btree_free(btree);
    return 0;
}
