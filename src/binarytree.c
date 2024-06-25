#include "binarytree.h"
#include <stdlib.h>
#include <string.h>
#include "list_ref/list_ref.h"
#include "ptr.h"

btree_ref_t* create_btree(size_t size) {
	btree_ref_t* tree = malloc(sizeof(btree_ref_t));
	tree->size = size;
	tree->free_element = free;
	tree->root = NULL;
	return tree;
}

btree_path_t btree_node_to_path(long unsigned int pos) {
	btree_path_t path = {0, 0};
	while (pos) {
		path.path <<= 1;
		if ((pos & 1) == 0)
			path.path |= 1;
		path.length++;
		pos = (pos - 1) / 2;
	}
	return path;
}

static unsigned btree_height_rec(node_btree_ref_t* node) {
	if (node)
		return 1 + MAX(btree_height_rec(node->ls), btree_height_rec(node->rs));
	return 0;
}

unsigned btree_height(btree_ref_t* tree) {
	return btree_height_rec(tree->root);
}

unsigned btree_length(btree_ref_t* tree) {
	return preorder_traversal(tree, NULL);
}

static node_btree_ref_t** left_right(node_btree_ref_t* tree, btree_path_t p) {
	return (p.path & 1) ? &tree->rs : &tree->ls;
}

static node_btree_ref_t* btree_emplace_at_rec(node_btree_ref_t** node_ptr,
											  btree_path_t path,
											  void* p) {
	if (path.length == 0) {
		if (*node_ptr)
			(*node_ptr)->p = p;
		else {
			*node_ptr = malloc(sizeof(node_btree_ref_t));
			(*node_ptr)->p = p;
			(*node_ptr)->ls = NULL;
			(*node_ptr)->rs = NULL;
		}
		return *node_ptr;
	}

	if (*node_ptr == NULL)
		return NULL;
	node_btree_ref_t** son = left_right(*node_ptr, path);
	return btree_emplace_at_rec(
		son, (btree_path_t){path.length - 1, path.path >> 1}, p);
}

node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p) {
	return btree_emplace_at_rec(&tree->root, path, p);
}

static void preorder_traversal_rec(node_btree_ref_t* tree,
								   void* tab[],
								   unsigned* i) {
	if (tree) {
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
		preorder_traversal_rec(tree->ls, tab, i);
		preorder_traversal_rec(tree->rs, tab, i);
	}
}

unsigned preorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	preorder_traversal_rec(tree->root, tab, &i);
	return i;
}

static void postorder_traversal_rec(node_btree_ref_t* tree,
									void* tab[],
									unsigned* i) {
	if (tree) {
		preorder_traversal_rec(tree->ls, tab, i);
		preorder_traversal_rec(tree->rs, tab, i);
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
	}
}

unsigned postorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	postorder_traversal_rec(tree->root, tab, &i);
	return i;
}

static void sym_traversal_rec(node_btree_ref_t* tree,
							  void* tab[],
							  unsigned* i) {
	if (tree) {
		preorder_traversal_rec(tree->ls, tab, i);
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
		preorder_traversal_rec(tree->rs, tab, i);
	}
}

unsigned sym_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	sym_traversal_rec(tree->root, tab, &i);
	return i;
}

void btree_free_rec(node_btree_ref_t* node, free_element_fn_t free_elements) {
	if (node) {
		btree_free_rec(node->ls, free_elements);
		btree_free_rec(node->rs, free_elements);
		free_elements(node->p);
		free(node);
	}
}

void btree_free(btree_ref_t* tree) {
	btree_free_rec(tree->root, tree->free_element);
	free(tree);
}

int level_order_traversal(node_btree_ref_t* tree, void* tab[]) {
	if (tree) {
		unsigned i = 0;
		list_ref_t* foret = create_list(sizeof(node_btree_ref_t));
		foret->free_element =
			(void (*)(void*))btree_free;  // Inutile mais présent par sécurité
		if (!foret)
			return -1;
		if (!push_back_list(foret, tree))
			return -1;

		while (!empty_list(foret)) {
			node_btree_ref_t* t;
			pop_front_list(foret, (void**)&t);
			if (t) {
				if (!push_back_list(foret, t->ls))
					return -1;
				if (!push_back_list(foret, t->rs))
					return -1;
				tab[i] = t->p;
				i++;
			}
		}
		free_list(foret);
		return 0;
	} else
		return -1;
}

btree_ref_t* perfect_BT_from_tab(void* tab, size_t size, unsigned length) {
	if (size == 0)
		return NULL;
	btree_ref_t* tree = malloc(sizeof(btree_ref_t));
	tree->size = size;
	tree->free_element = free;
	tree->root = NULL;
	for (unsigned i = 0; i < length; i++) {
		void* p = malloc(size);
		memcpy(p, (char*)tab + i * size, size);
		btree_emplace_at(tree, btree_node_to_path(i), p);
	}
	return tree;
}
