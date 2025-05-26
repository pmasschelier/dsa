#include "btree_ref/btree_ref.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "btree_ref/path.h"
#include "errors.h"
#include "lambda.h"
#include "list_ref/linked_list_ref.h"
#include "test_macros.h"

#ifndef STRUCT_RECURSIVE_IMPL
#include "stack_view.h"
#endif

btree_ref_t* create_btree(size_t size) {
	btree_ref_t* ret = malloc(sizeof(btree_ref_t));
	when_null_ret(ret, NULL);
	ret->size = size;
	ret->root = NULL;
	return ret;
}

node_btree_ref_t* create_btree_leaf(
	void* value,
	node_btree_ref_t* parent,
    uintptr_t priv_init,
    size_t size_bytes)
{
	node_btree_ref_t* leaf = malloc(sizeof(node_btree_ref_t) + size_bytes);
	when_null_ret(leaf, NULL);
	leaf->ls = NULL;
	leaf->rs = NULL;
	leaf->parent = parent;
    leaf->priv = priv_init;
    if(value != NULL)
        memcpy(leaf->data, value, size_bytes);
    return leaf;
}

void btree_swap_node(node_btree_ref_t*** a, node_btree_ref_t*** b) {
    node_btree_ref_t* a_ptr = **a;
    node_btree_ref_t* b_ptr = **b;
    node_btree_ref_t a_val = *a_ptr;
    node_btree_ref_t b_val = *b_ptr;

    // Swap references
    **a = b_ptr;
    **b = a_ptr;

    // Swap references
    if(a_val.ls == b_ptr) {
        a_ptr->parent = b_ptr;
        b_ptr->ls = a_ptr;
        b_ptr->rs = a_val.rs;
        *b = &b_ptr->ls;
    }
    else  if(a_val.rs == b_ptr) {
        a_ptr->parent = b_ptr;
        b_ptr->ls = a_val.ls;
        b_ptr->rs = a_ptr;
        *b = &b_ptr->rs;
    }
    else {
        a_ptr->parent = b_val.parent;
        b_ptr->ls = a_val.ls;
        b_ptr->rs = a_val.rs;
    }
    if(b_val.ls == a_ptr) {
        b_ptr->parent = a_ptr;
        a_ptr->ls = b_ptr;
        a_ptr->rs = b_val.rs;
        *a = &a_ptr->ls;
    }
    else if(b_val.rs == a_ptr) {
        b_ptr->parent = a_ptr;
        a_ptr->ls = b_val.ls;
        a_ptr->rs = b_ptr;
        *a = &a_ptr->rs;
    }
    else {
        b_ptr->parent = a_val.parent;
        a_ptr->ls = b_val.ls;
        a_ptr->rs = b_val.rs;
    }
    a_ptr->priv = b_val.priv;
    b_ptr->priv = a_val.priv;

    // Update children
    if(a_ptr->ls != NULL)
        a_ptr->ls->parent = a_ptr;
    if(a_ptr->rs != NULL)
        a_ptr->rs->parent = a_ptr;
    if(b_ptr->ls != NULL)
        b_ptr->ls->parent = b_ptr;
    if(b_ptr->rs != NULL)
        b_ptr->rs->parent = b_ptr;
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
	stack_view_t* stack = create_stack_copy(sizeof(struct node_height));
	when_null_ret(stack, -ERROR_ALLOCATION_FAILED);

	struct node_height root = {tree->root, 1};
	stack_copy_push(stack, &root);

	struct node_height node;
	unsigned int max_height = 0;
	while (empty_stack(stack) == FALSE) {
		stack_copy_pop(stack, (void*)&node);
		if (node.height > max_height)
			max_height = node.height;
		if (node.node->ls) {
			struct node_height ls = {node.node->ls, node.height + 1};
			stack_copy_push(stack, &ls);
		}
		if (node.node->rs) {
			struct node_height rs = {node.node->rs, node.height + 1};
			stack_copy_push(stack, &rs);
		}
	}
	free_stack(stack);
	return max_height;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

unsigned btree_length(btree_ref_t* tree) {
	return btree_preorder_traversal(tree, NULL);
}

node_btree_ref_t** btree_next_node(node_btree_ref_t* node, btree_path_t* p) {
	return path_walk_forward(p, &node->ls, &node->rs);
}

#ifdef STRUCT_RECURSIVE_IMPL
static node_btree_ref_t* btree_emplace_at_rec(node_btree_ref_t** node_ptr,
                                              node_btree_ref_t* parent,
											  btree_path_t path,
											  void* p, size_t size_bytes) {
	if (path.length == 0) {
		if (*node_ptr) {
            memcpy((*node_ptr)->data, p, size_bytes);
		} else {
            *node_ptr = create_btree_leaf(NULL, parent, 0, size_bytes);
			when_null_ret(*node_ptr, NULL);
		}
		return *node_ptr;
	}

	if (*node_ptr == NULL)
		return NULL;
	node_btree_ref_t** son = btree_next_node(*node_ptr, &path);
	return btree_emplace_at_rec(son, *node_ptr, path, p, size_bytes);
}

node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p) {
	return btree_emplace_at_rec(&tree->root, NULL, path, p, tree->size);
}
#else
node_btree_ref_t *btree_emplace_at(btree_ref_t *tree, btree_path_t path,
                                   void *p) {
    node_btree_ref_t **node_ptr = &tree->root;
    node_btree_ref_t *parent = NULL;
    while (path.length != 0) {
        if (*node_ptr == NULL)
            return NULL;
        parent = *node_ptr;
        node_ptr = btree_next_node(*node_ptr, &path);
    }

    if (*node_ptr == NULL) {
        *node_ptr = create_btree_leaf(p, parent, 0, tree->size);
        when_null_ret(*node_ptr, NULL);
    }
    else {
        memcpy((*node_ptr)->data, p, tree->size);
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
                                  size_t size_bytes,
								  unsigned written_count) {
	if (*node_ptr == NULL) {
		*node_ptr = malloc(sizeof(node_btree_ref_t));
		when_null_ret(*node_ptr, -ERROR_ALLOCATION_FAILED);
		(*node_ptr)->ls = NULL;
		(*node_ptr)->rs = NULL;
	}
	if (index >= 0 && index < (int)length && values[index]) {
        memcpy((*node_ptr)->data, values[index], size_bytes);
		written_count++;
	}

	if (path.length == 0)
		return written_count;

	node_btree_ref_t** son = btree_next_node(*node_ptr, &path);
	return btree_emplace_path_rec(son, path, values, index + 1, length, size_bytes, written_count);
}

int btree_emplace_path(btree_ref_t* tree,
					   btree_path_t path,
					   void* values[],
					   size_t length,
					   size_t offset) {
	return btree_emplace_path_rec(&tree->root, path, values, -offset, length, tree->size, 0);
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
            *node_ptr = create_btree_leaf(NULL, NULL, 0, tree->size);
			when_null_ret(*node_ptr, -ERROR_ALLOCATION_FAILED);
		}

		if (index >= 0 && index < (int)length && values[index]) {
            memcpy((*node_ptr)->data, values[index], tree->size);
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
void btree_preorder_traversal_rec(node_btree_ref_t* node, lambda_t* lambda, unsigned* i) {
    if(node == NULL)
        return;
    lambda->fn(lambda->priv, node->data);
    *i += 1;
    btree_preorder_traversal_rec(node->ls, lambda, i);
    btree_preorder_traversal_rec(node->rs, lambda, i);
}

int btree_preorder_traversal(btree_ref_t* tree, lambda_t* lambda) {
	unsigned i = 0;
    btree_preorder_traversal_rec(tree->root, lambda, &i);
    return i;
}

static void btree_preorder_traversal_array_rec(node_btree_ref_t* node,
										 void* tab[],
										 unsigned* i) {
	if (node == NULL)
        return;
    if (tab != NULL)
        tab[*i] = node->data;
    *i += 1;
    btree_preorder_traversal_array_rec(node->ls, tab, i);
    btree_preorder_traversal_array_rec(node->rs, tab, i);
}

int btree_preorder_traversal_array(btree_ref_t* tree, void* tab[]) {
	unsigned i = 0;
	btree_preorder_traversal_array_rec(tree->root, tab, &i);
	return i;
}

int btree_dfs_array_rec(node_btree_ref_t* node,
			  void* preorder[], unsigned *a,
			  void* inorder[], unsigned *b,
			  void* postorder[], unsigned *c) {
    if(node == NULL)
        return 0;
    if(preorder != NULL)
        preorder[*a++] = node->data;
    btree_dfs_array_rec(node->ls, preorder, a, inorder, b, postorder, c);
    if(inorder != NULL)
        inorder[*b++] = node->data;
    btree_dfs_array_rec(node->rs, preorder, a, inorder, b, postorder, c);
    if(postorder != NULL)
        postorder[*c++] = node->data;
    return -ERROR_NO_ERROR;
}

int btree_dfs_array(btree_ref_t* tree,
			  void* preorder[],
			  void* inorder[],
			  void* postorder[]) {
    unsigned a = 0, b = 0, c = 0;
    return btree_dfs_array_rec(tree->root, preorder, &a, inorder, &b, postorder, &c);
}
int btree_dfs_rec(node_btree_ref_t* node,
			  lambda_t* preorder,
			  lambda_t* inorder,
			  lambda_t* postorder) {
    if(node == NULL)
        return 0;
    if(preorder != NULL)
        preorder->fn(preorder->priv, node->data);
    btree_dfs_rec(node->ls, preorder, inorder, postorder);
    if(inorder != NULL)
        inorder->fn(preorder->priv, node->data);
    btree_dfs_rec(node->rs, preorder, inorder, postorder);
    if(postorder != NULL)
        postorder->fn(preorder->priv, node->data);
    return -ERROR_NO_ERROR;
}

int btree_dfs(btree_ref_t* tree,
			  lambda_t* preorder,
			  lambda_t* inorder,
			  lambda_t* postorder) {
    return btree_dfs_rec(tree->root, preorder, inorder, postorder);
}
#else
int btree_preorder_traversal_array(btree_ref_t* tree, void* tab[]) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;
	unsigned i = 0;

	stack_view_t* forest = create_stack_view(sizeof(node_btree_ref_t*));
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);

	node_btree_ref_t* node = tree->root;

	stack_view_push(forest, node);
	while (empty_stack(forest) == FALSE) {
		stack_view_pop(forest, (void**)&node);
		if (tab != NULL)
			tab[i] = node->data;
		i++;
		if (node->rs != NULL)
			stack_view_push(forest, node->rs);
		if (node->ls != NULL)
			stack_view_push(forest, node->ls);
	}
	free_stack(forest);
	return i;
}

int btree_preorder_traversal(btree_ref_t* tree, lambda_t* lambda) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;
	unsigned i = 0;

	stack_view_t* forest = create_stack_view(sizeof(node_btree_ref_t*));
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);

	node_btree_ref_t* node = tree->root;
	BOOL call = (lambda != NULL) && (lambda->fn != NULL);

	stack_view_push(forest, node);
	while (empty_stack(forest) == FALSE) {
		stack_view_pop(forest, (void**)&node);
		if (call)
			lambda->fn(lambda->priv, node->data);
		i++;
		if (node->rs != NULL)
			stack_view_push(forest, node->rs);
		if (node->ls != NULL)
			stack_view_push(forest, node->ls);
	}
	free_stack(forest);
	return i;
}

typedef enum dfs_status { DOWNWARD, UPWARD_LEFT, UPWARD_RIGHT } dfs_status_t;

int btree_dfs(btree_ref_t* tree,
			  lambda_t* preorder,
			  lambda_t* inorder,
			  lambda_t* postorder) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;

    int count = 0;

	stack_view_t* forest = create_stack_view(sizeof(node_btree_ref_t*));
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);

	node_btree_ref_t* node = tree->root;
	dfs_status_t status = DOWNWARD;
	stack_view_push(forest, node);

	BOOL precall = (preorder != NULL) && (preorder->fn != NULL);
	BOOL incall = (inorder != NULL) && (inorder->fn != NULL);
	BOOL postcall = (postorder != NULL) && (postorder->fn != NULL);

	do {
		switch (status) {
		case DOWNWARD:
            count++;
			if (precall)
				preorder->fn(preorder->priv, node->data);
            // If there is a left son, append to stack
            // and continue DOWNWARD
			if (node->ls != NULL) {
				stack_view_push(forest, node->ls);
				break;
			}
            // Otherwise fallthrough
		case UPWARD_LEFT:
			if (incall)
				inorder->fn(inorder->priv, node->data);
            // If there is a right son, append to stack
            // and continue DOWNWARD
			if (node->rs != NULL) {
				stack_view_push(forest, node->rs);
				status = DOWNWARD;
				break;
			}
            // Otherwise fallthrough
		case UPWARD_RIGHT:
			if (postcall)
				postorder->fn(postorder->priv, node->data);
			stack_view_pop(forest, NULL);
            // If there is no node left on the stack step out of the loop
			node_btree_ref_t* next = stack_view_peak(forest, node_btree_ref_t);
			if (next == NULL)
				goto exit;
            // Else if the current node is the left son of the next node on the stack
            // we continue UPWARD_LEFT
			if (next->ls == node)
				status = UPWARD_LEFT;
            // Otherwise we continue UPWARD_RIGHT
			else if (next->rs == node)
				status = UPWARD_RIGHT;
		}
        // The next node considered is the node on top of the stack
		node = stack_view_peak(forest, node_btree_ref_t);
	} while (node != NULL);
exit:
	free_stack(forest);
    return count;
}

int btree_dfs_array(btree_ref_t* tree,
			  void* preorder[],
			  void* inorder[],
			  void* postorder[]) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;

    int precount = 0, incount = 0, postcount = 0;

	stack_view_t* forest = create_stack_view(sizeof(node_btree_ref_t*));
	when_null_ret(forest, -ERROR_ALLOCATION_FAILED);

	node_btree_ref_t* node = tree->root;
	dfs_status_t status = DOWNWARD;
	stack_view_push(forest, node);

	do {
		switch (status) {
		case DOWNWARD:
			if (preorder != NULL)
                preorder[precount] = &node->data;
            precount++;
            // If there is a left son, append to stack
            // and continue DOWNWARD
			if (node->ls != NULL) {
				stack_view_push(forest, node->ls);
				break;
			}
            // Otherwise fallthrough
		case UPWARD_LEFT:
			if (inorder != NULL)
				inorder[incount] = &node->data;
            incount++;
            // If there is a right son, append to stack
            // and continue DOWNWARD
			if (node->rs != NULL) {
				stack_view_push(forest, node->rs);
				status = DOWNWARD;
				break;
			}
            // Otherwise fallthrough
		case UPWARD_RIGHT:
			if (postorder != NULL)
				postorder[postcount] = &node->data;
            postcount++;
			stack_view_pop(forest, NULL);
            // If there is no node left on the stack step out of the loop
			node_btree_ref_t* next = stack_view_peak(forest, node_btree_ref_t);
			if (next == NULL)
				goto exit;
            // Else if the current node is the left son of the next node on the stack
            // we continue UPWARD_LEFT
			if (next->ls == node)
				status = UPWARD_LEFT;
            // Otherwise we continue UPWARD_RIGHT
			else if (next->rs == node)
				status = UPWARD_RIGHT;
		}
        // The next node considered is the node on top of the stack
		node = stack_view_peak(forest, node_btree_ref_t);
	} while (node != NULL);
exit:
	free_stack(forest);
    return precount;
}

/* int btree_preorder_traversal(btree_ref_t* tree, void* tab[]) { */
/* 	unsigned i = 0; */
/* 	btree_dfs(tree, node, tab[i++] = node->p, , ); */
/* 	return i; */
/* } */

/* int btree_postorder_traversal(btree_ref_t* tree, void* tab[]) { */
/* 	unsigned i = 0; */
/* 	btree_dfs(tree, node, , , tab[i++] = node->p); */
/* 	return i; */
/* } */

/* int btree_inorder_traversal(btree_ref_t* tree, void* tab[]) { */
/* 	unsigned i = 0; */
/* 	btree_dfs(tree, node, , tab[i++] = node->p, ); */
/* 	return i; */
/* } */
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

#ifdef STRUCT_RECURSIVE_IMPL
static void btree_clean_rec(node_btree_ref_t* node) {
	if (node->ls)
		btree_clean_rec(node->ls);
	if (node->rs)
		btree_clean_rec(node->rs);
	free(node);
}

void btree_clean(btree_ref_t* tree) {
	if (tree->root != NULL)
        btree_clean_rec(tree->root);
	tree->root = NULL;
}
#else
void btree_clean(btree_ref_t* tree) {
	if (tree == NULL)
		return;
	if (tree->root == NULL)
		return;
	stack_view_t* stack = create_stack_view(sizeof(node_btree_ref_t*));
	when_null_ret(stack, );

	node_btree_ref_t* node;
	stack_view_push(stack, tree->root);
	while (empty_stack(stack) == FALSE) {
		stack_view_pop(stack, (void**)&node);
		if (node->rs)
			stack_view_push(stack, node->rs);
		if (node->ls)
			stack_view_push(stack, node->ls);
		free(node);
	}
	free_stack(stack);
	tree->root = NULL;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

void btree_free(btree_ref_t* tree) {
	btree_clean(tree);
	free(tree);
}

int btree_levelorder_traversal(btree_ref_t* tree, void* tab) {
	when_null_ret(tree, -ERROR_INVALID_PARAM1);
	if (tree->root == NULL)
		return 0;
	unsigned i = 0;
    
	linked_list_t forest = LINKED_LIST_INIT(node_btree_ref_t*);
	linked_list_node_t* ret = linked_list_push_back(&forest, &tree->root);
	when_null_ret(ret, -ERROR_ALLOCATION_FAILED);

    node_btree_ref_t* t;
	while (linked_list_pop_front(&forest, &t)) {
        if (t->ls != NULL)
            linked_list_push_back(&forest, &t->ls);
        if (t->rs != NULL)
            linked_list_push_back(&forest, &t->rs);
        memcpy((uint8_t*)tab + i * tree->size, t->data, tree->size);
        i++;
	}
	linked_list_deinit(&forest);
	return 0;
}

btree_ref_t* btree_perfect_tree_from_tab(void* tab,
										 size_t size,
										 unsigned length) {
	when_true_ret(size == 0 || (length != 0 && tab == NULL), NULL);
	btree_ref_t* tree = malloc(sizeof(btree_ref_t));
	when_null_ret(tree, NULL);
	tree->size = size;
	tree->root = NULL;

	for (unsigned i = 0; i < length; i++) {
		if(NULL == btree_emplace_at(tree, btree_node_to_path(i), (uint8_t*)tab + i * size))
            goto error;
	}
	return tree;
error:
	btree_free(tree);
	return NULL;
}
