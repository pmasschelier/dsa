#include <assert.h>
#include <btree_ref/btree_ref.h>

#define BT_TYPE int
#define TAB_LEN 15

BT_TYPE tab[TAB_LEN];

int main(void) {
    for (int i = 0; i < TAB_LEN; i++)
        tab[i] = i;
    btree_ref_t *btree = btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
    node_btree_ref_t *root = btree->root;
    assert(*get_node_ref(root, BT_TYPE) == 0);
    assert(*get_node_ref(root->ls, BT_TYPE) == 1);
    assert(*get_node_ref(root->rs, BT_TYPE) == 2);
    assert(*get_node_ref(root->ls->ls, BT_TYPE) == 3);
    assert(*get_node_ref(root->ls->rs, BT_TYPE) == 4);
    assert(*get_node_ref(root->rs->ls, BT_TYPE) == 5);
    assert(*get_node_ref(root->rs->rs, BT_TYPE) == 6);
    assert(*get_node_ref(root->ls->ls->ls, BT_TYPE) == 7);
    assert(*get_node_ref(root->ls->ls->rs, BT_TYPE) == 8);
    assert(*get_node_ref(root->ls->rs->ls, BT_TYPE) == 9);
    assert(*get_node_ref(root->ls->rs->rs, BT_TYPE) == 10);
    assert(*get_node_ref(root->rs->ls->ls, BT_TYPE) == 11);
    assert(*get_node_ref(root->rs->ls->rs, BT_TYPE) == 12);
    assert(*get_node_ref(root->rs->rs->ls, BT_TYPE) == 13);
    assert(*get_node_ref(root->rs->rs->rs, BT_TYPE) == 14);
    
    node_btree_ref_t** a = &root->ls;
    node_btree_ref_t** b = &root->rs->rs;
    btree_swap_node(&a, &b);
    assert(*get_node_ref(*a, BT_TYPE) == 6);
    assert(*get_node_ref(*b, BT_TYPE) == 1);

    assert(*get_node_ref(root->ls, BT_TYPE) == 6);
    assert(*get_node_ref(root->ls->parent, BT_TYPE) == 0);
    assert(*get_node_ref(root->ls->ls, BT_TYPE) == 3);
    assert(*get_node_ref(root->ls->ls->parent, BT_TYPE) == 6);
    assert(*get_node_ref(root->ls->rs, BT_TYPE) == 4);
    assert(*get_node_ref(root->ls->rs->parent, BT_TYPE) == 6);

    assert(*get_node_ref(root->rs->rs, BT_TYPE) == 1);
    assert(*get_node_ref(root->rs->rs->parent, BT_TYPE) == 2);
    assert(*get_node_ref(root->rs->rs->ls, BT_TYPE) == 13);
    assert(*get_node_ref(root->rs->rs->ls->parent, BT_TYPE) == 1);
    assert(*get_node_ref(root->rs->rs->rs, BT_TYPE) == 14);
    assert(*get_node_ref(root->rs->rs->rs->parent, BT_TYPE) == 1);
    btree_free(btree);

    btree = btree_perfect_tree_from_tab(tab, sizeof(BT_TYPE), TAB_LEN);
    root = btree->root;
    node_btree_ref_t** c = &root->ls;
    node_btree_ref_t** d = &root->ls->rs;
    btree_swap_node(&c, &d);
    assert(*get_node_ref(*c, BT_TYPE) == 4);
    assert(*get_node_ref(*d, BT_TYPE) == 1);

    assert(*get_node_ref(root->ls, BT_TYPE) == 4);
    assert(*get_node_ref(root->ls->parent, BT_TYPE) == 0);
    assert(*get_node_ref(root->ls->ls, BT_TYPE) == 3);
    assert(*get_node_ref(root->ls->ls->parent, BT_TYPE) == 4);
    assert(*get_node_ref(root->ls->rs, BT_TYPE) == 1);
    assert(*get_node_ref(root->ls->rs->ls, BT_TYPE) == 9);
    assert(*get_node_ref(root->ls->rs->rs, BT_TYPE) == 10);
    assert(*get_node_ref(root->ls->rs->parent, BT_TYPE) == 4);

    btree_free(btree);
    return 0;
}
