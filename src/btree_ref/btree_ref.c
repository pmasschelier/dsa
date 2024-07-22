#include "btree_ref/btree_ref.h"
#include <stdlib.h>
#include <string.h>
#include "btree_ref/path.h"
#include "errors.h"
#include "list_ref/list_ref.h"
#include "test_macros.h"

btree_ref_t* create_btree(size_t size) {
	btree_ref_t* ret = malloc(sizeof(btree_ref_t));
	when_null_ret(ret, NULL);
	ret->size = size;
	ret->free_element = free;
	ret->root = NULL;
	return ret;
}

#ifdef STRUCT_RECURSIVE_IMPL
static unsigned btree_height_rec(node_btree_ref_t* node, unsigned height) {
	if (node)
		return 1 + MAX(btree_height_rec(node->ls, height),
					   btree_height_rec(node->rs, height));
	return height;
}

unsigned btree_height(btree_ref_t* tree) {
	return btree_height_rec(tree->root, 0);
}
#else
struct node_height {
	node_btree_ref_t* node;
	unsigned height;
};

unsigned btree_height(btree_ref_t* tree) {
	if (tree->root == NULL)
		return 0;
	list_ref_t* queue = create_list(sizeof(struct node_height));
	when_null_ret(queue, -ERROR_ALLOCATION_FAILED);

	struct node_height root = {tree->root, 1};
	append_back_list(queue, &root);

	struct node_height* node;
	unsigned int max_height = 0;
	while (empty_list(queue) == FALSE) {
		pop_front_list(queue, (void**)&node);
		if (node->height > max_height)
			max_height = node->height;
		if (node->node->ls) {
			struct node_height ls = {node->node->ls, node->height + 1};
			append_back_list(queue, &ls);
		}
		if (node->node->rs) {
			struct node_height rs = {node->node->rs, node->height + 1};
			append_back_list(queue, &rs);
		}

		free(node);
	}
	free_list(queue);
	return max_height;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

unsigned btree_length(btree_ref_t* tree) {
	return btree_preorder_traversal(tree, NULL);
}

node_btree_ref_t** btree_next_node(node_btree_ref_t* node, btree_path_t* p) {
	node_btree_ref_t** ret = (p->path & 1) ? &node->rs : &node->ls;
	p->length -= 1;
	p->path >>= 1;
	return ret;
}

#ifdef STRUCT_RECURSIVE_IMPL
static node_btree_ref_t* btree_emplace_at_rec(node_btree_ref_t** node_ptr,
											  btree_path_t path,
											  void* p,
											  free_element_fn_t free_element) {
	if (path.length == 0) {
		if (*node_ptr) {
			if ((*node_ptr)->p && free_element)
				free_element((*node_ptr)->p);
			(*node_ptr)->p = p;
		} else {
			*node_ptr = malloc(sizeof(node_btree_ref_t));
			when_null_ret(*node_ptr, NULL);
			(*node_ptr)->p = p;
			(*node_ptr)->ls = NULL;
			(*node_ptr)->rs = NULL;
		}
		return *node_ptr;
	}

	if (*node_ptr == NULL)
		return NULL;
	node_btree_ref_t** son = btree_next_node(*node_ptr, &path);
	return btree_emplace_at_rec(son, path, p, free_element);
}

node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p) {
	return btree_emplace_at_rec(&tree->root, path, p, tree->free_element);
}
#else
node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p) {
	node_btree_ref_t** node_ptr = &tree->root;
	while (path.length != 0) {
		if (*node_ptr == NULL)
			return NULL;
		node_ptr = btree_next_node(*node_ptr, &path);
	}

	if (*node_ptr == NULL) {
		*node_ptr = malloc(sizeof(node_btree_ref_t));
		when_null_ret(*node_ptr, NULL);
		(*node_ptr)->p = p;
		(*node_ptr)->ls = NULL;
		(*node_ptr)->rs = NULL;
	} else {
		if ((*node_ptr)->p && tree->free_element)
			tree->free_element((*node_ptr)->p);
		(*node_ptr)->p = p;
	}

	return *node_ptr;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

#ifdef STRUCT_RECURSIVE_IMPL
static int btree_emplace_path_rec(node_btree_ref_t** node_ptr,
								  btree_path_t path,
								  void* values[],
								  int index,
								  size_t length,
								  unsigned written_count) {
	if (*node_ptr == NULL) {
		*node_ptr = malloc(sizeof(node_btree_ref_t));
		when_null_ret(*node_ptr, -ERROR_ALLOCATION_FAILED);
		(*node_ptr)->ls = NULL;
		(*node_ptr)->rs = NULL;
		(*node_ptr)->p = NULL;
	}
	if (index >= 0 && index < (int)length && values[index]) {
		(*node_ptr)->p = values[index];
		written_count++;
	}

	if (path.length == 0)
		return written_count;

	node_btree_ref_t** son = btree_next_node(*node_ptr, &path);
	return btree_emplace_path_rec(son, path, values, index + 1, length,
								  written_count);
}

int btree_emplace_path(btree_ref_t* tree,
					   btree_path_t path,
					   void* values[],
					   size_t length,
					   size_t offset) {
	return btree_emplace_path_rec(&tree->root, path, values, -offset, length,
								  0);
}
#else
int btree_emplace_path(btree_ref_t* tree,
					   btree_path_t path,
					   void* values[],
					   size_t length,
					   size_t offset) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	node_btree_ref_t** node_ptr = &tree->root;
	int index = -offset;
	int written_count = 0;
	while (TRUE) {
		if (*node_ptr == NULL) {
			*node_ptr = malloc(sizeof(node_btree_ref_t));
			when_null_ret(*node_ptr, -ERROR_ALLOCATION_FAILED);
			(*node_ptr)->ls = NULL;
			(*node_ptr)->rs = NULL;
			(*node_ptr)->p = NULL;
		} else {
		}

		if (index >= 0 && index < (int)length && values[index]) {
			if ((*node_ptr)->p && tree->free_element)
				tree->free_element((*node_ptr)->p);

			(*node_ptr)->p = values[index];
			written_count++;
		}
		if (path.length == 0)
			break;
		node_ptr = btree_next_node(*node_ptr, &path);
		index++;
	}
	return written_count;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

#ifdef STRUCT_RECURSIVE_IMPL
static void btree_preorder_traversal_rec(node_btree_ref_t* tree,
										 void* tab[],
										 unsigned* i) {
	if (tree) {
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
		btree_preorder_traversal_rec(tree->ls, tab, i);
		btree_preorder_traversal_rec(tree->rs, tab, i);
	}
}

int btree_preorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_preorder_traversal_rec(tree->root, tab, &i);
	return i;
}

static void btree_postorder_traversal_rec(node_btree_ref_t* tree,
										  void* tab[],
										  unsigned* i) {
	if (tree) {
		btree_postorder_traversal_rec(tree->ls, tab, i);
		btree_postorder_traversal_rec(tree->rs, tab, i);
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
	}
}

int btree_postorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_postorder_traversal_rec(tree->root, tab, &i);
	return i;
}

static void btree_inorder_traversal_rec(node_btree_ref_t* tree,
										void* tab[],
										unsigned* i) {
	if (tree) {
		btree_inorder_traversal_rec(tree->ls, tab, i);
		if (tab)
			tab[*i] = tree->p;
		*i += 1;
		btree_inorder_traversal_rec(tree->rs, tab, i);
	}
}

int btree_inorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_inorder_traversal_rec(tree->root, tab, &i);
	return i;
}
#else
int btree_preorder_traversal(btree_ref_t* tree, void* tab[]) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;
	unsigned i = 0;

	list_ref_t* forest = create_list(sizeof(node_btree_ref_t));
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);
	forest->free_element = NULL;

	node_btree_ref_t* node = tree->root;

	push_back_list(forest, node);
	while (empty_list(forest) == FALSE) {
		pop_back_list(forest, (void**)&node);
		tab[i++] = node->p;
		if (node->rs != NULL)
			push_back_list(forest, node->rs);
		if (node->ls != NULL)
			push_back_list(forest, node->ls);
	}
	free_list(forest);
	return i;
}

typedef enum dfs_status { DOWNWARD, UPWARD_LEFT, UPWARD_RIGHT } dfs_status_t;

#define btree_dfs(tree, node, preorder, inorder, postorder)     \
	when_null_ret(tree, -ERROR_INVALID_PARAM1);                 \
	if (tree->root == NULL)                                     \
		return 0;                                               \
                                                                \
	list_ref_t* forest = create_list(sizeof(node_btree_ref_t)); \
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);            \
	forest->free_element = NULL;                                \
                                                                \
	node_btree_ref_t* node;                                     \
	dfs_status_t status = DOWNWARD;                             \
	push_back_list(forest, tree->root);                         \
	while ((node = forest->end->p) != NULL) {                   \
		switch (status) {                                       \
		case DOWNWARD:                                          \
			preorder;                                           \
			if (node->ls != NULL) {                             \
				push_back_list(forest, node->ls);               \
				break;                                          \
			}                                                   \
		case UPWARD_LEFT:                                       \
			inorder;                                            \
			if (node->rs != NULL) {                             \
				push_back_list(forest, node->rs);               \
				status = DOWNWARD;                              \
				break;                                          \
			}                                                   \
		case UPWARD_RIGHT:                                      \
			postorder;                                          \
			pop_back_list(forest, NULL);                        \
			if (forest->end) {                                  \
				node_btree_ref_t* next = forest->end->p;        \
				if (next->ls == node)                           \
					status = UPWARD_LEFT;                       \
				if (next->rs == node)                           \
					status = UPWARD_RIGHT;                      \
				break;                                          \
			}                                                   \
			goto exit;                                          \
		}                                                       \
	}                                                           \
	exit:                                                       \
	free_list(forest);

int btree_postorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_dfs(tree, node, , , tab[i++] = node->p);
	return i;
}

int btree_inorder_traversal(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_dfs(tree, node, , tab[i++] = node->p, );
	return i;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

#ifdef STRUCT_RECURSIVE_IMPL
static void btree_free_rec(node_btree_ref_t* node,
						   free_element_fn_t free_elements) {
	if (node) {
		btree_free_rec(node->ls, free_elements);
		btree_free_rec(node->rs, free_elements);
		free_elements(node->p);
		free(node);
	}
}

static void btree_free_rec_no_free(node_btree_ref_t* node) {
	if (node) {
		btree_free_rec_no_free(node->ls);
		btree_free_rec_no_free(node->rs);
		free(node);
	}
}

void btree_free(btree_ref_t* tree) {
	if (tree->free_element)
		btree_free_rec(tree->root, tree->free_element);
	else
		btree_free_rec_no_free(tree->root);
	free(tree);
}
#else
void btree_free(btree_ref_t* tree) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		goto exit;
	list_ref_t* stack = create_list(sizeof(node_btree_ref_t));
	when_null_ret(stack, -ERROR_ALLOCATION_FAILED);
	stack->free_element = NULL;

	node_btree_ref_t* node;
	push_back_list(stack, tree->root);
	while (empty_list(stack) == FALSE) {
		pop_back_list(stack, (void**)&node);
		if (node->rs)
			push_back_list(stack, node->rs);
		if (node->ls)
			push_back_list(stack, node->ls);
		if (tree->free_element && node->p)
			tree->free_element(node->p);
		free(node);
	}
	free_list(stack);
exit:
	free(tree);
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

int btree_levelorder_traversal(btree_ref_t* tree, void* tab[]) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;
	unsigned i = 0;
	list_ref_t* foret = create_list(sizeof(node_btree_ref_t));
	when_null_ret(foret, -ERROR_ALLOCATION_FAILED);
	foret->free_element =
		(void (*)(void*))btree_free;  // Inutile mais présent par sécurité
	node_list_ref_t* ret = push_back_list(foret, tree->root);
	when_null_ret(ret, -ERROR_ALLOCATION_FAILED);

	while (!empty_list(foret)) {
		node_btree_ref_t* t;
		pop_front_list(foret, (void**)&t);
		if (t) {
			if (t->ls != NULL)
				push_back_list(foret, t->ls);
			if (t->rs != NULL)
				push_back_list(foret, t->rs);
			tab[i] = t->p;
			i++;
		}
	}
	free_list(foret);
	return 0;
}

btree_ref_t* btree_perfect_tree_from_tab(void* tab,
										 size_t size,
										 unsigned length) {
	btree_ref_t* ret;
	when_true_ret(size == 0 || (length != 0 && tab == NULL), NULL);
	btree_ref_t* tree = malloc(sizeof(btree_ref_t));
	when_null_ret(tree, NULL);
	tree->size = size;
	tree->free_element = free;
	tree->root = NULL;

	for (unsigned i = 0; i < length; i++) {
		void* p = malloc(size);
		when_null_jmp(p, NULL, error);
		memcpy(p, (char*)tab + i * size, size);
		btree_emplace_at(tree, btree_node_to_path(i), p);
	}
	return tree;
error:
	btree_free(tree);
	return ret;
}
