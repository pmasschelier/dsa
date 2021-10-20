#include "binarytree.h"

Path path(long unsigned int pos) {
	Path path = {0, 0};
	while(pos) {
		if(pos%2 == 1) {
			path.path <<= 1;
		}
		else {
			path.path <<= 1;
			path.path += 1;
		}
		path.length ++;
		pos = (pos-1)/2;
	}
	return path;
}

unsigned height_BT(BinaryTree* tree) {
	if(tree)
		return 1 + MAX(height_BT(tree->ls), height_BT(tree->rs));
	return 0;
}

static BinaryTree** left_right(BinaryTree* tree, Path p) {
	return (p.path&1) ? &tree->rs : &tree->ls;
}

BinaryTree* edit_BT(BinaryTree* tree, Path p, T x) {
	if(p.length == 0) {
		if(tree)
			tree->x = x;
		return tree;
	}
	
	BinaryTree** son = left_right(tree, p);
	
	if(p.length == 1) {
		if(*son)
			(*son)->x = x;
		else {
			*son = malloc(sizeof(BinaryTree));
			(*son)->x = x;
			(*son)->ls = NULL;
			(*son)->rs = NULL;
		}
		return *son;
	}
	else {
		if(!(*son))
			return NULL;
		return edit_BT(*son, (Path) {p.length - 1, p.path >> 1}, x);
	}
}

unsigned size_BT(BinaryTree* tree) {
	return preorder_traversal(tree, NULL);
}

static void preorder_traversal_rec(BinaryTree* tree, T* tab, unsigned* i)
{
	if(tree) {
		if(tab)
			tab[*i] = tree->x;
		*i += 1;
		preorder_traversal_rec(tree->ls, tab, i);
		preorder_traversal_rec(tree->rs, tab, i);
	}
}

unsigned preorder_traversal(BinaryTree* tree, T* tab) {
	unsigned i=0;
	preorder_traversal_rec(tree, tab, &i);
	return i;
}

static void postorder_traversal_rec(BinaryTree* tree, T* tab, unsigned* i)
{
	if(tree) {
		preorder_traversal_rec(tree->ls, tab, i);
		preorder_traversal_rec(tree->rs, tab, i);
		if(tab)
			tab[*i] = tree->x;
		*i += 1;
	}
}

unsigned postorder_traversal(BinaryTree* tree, T* tab) {
	unsigned i=0;
	postorder_traversal_rec(tree, tab, &i);
	return i;
}

static void sym_traversal_rec(BinaryTree* tree, T* tab, unsigned* i)
{
	if(tree) {
		preorder_traversal_rec(tree->ls, tab, i);
		if(tab)
			tab[*i] = tree->x;
		*i += 1;
		preorder_traversal_rec(tree->rs, tab, i);
	}
}

unsigned sym_traversal(BinaryTree* tree, T* tab) {
	unsigned i=0;
	sym_traversal_rec(tree, tab, &i);
	return i;
}

void free_BT(BinaryTree* tree)
{
	if(tree) {
		free_BT(tree->ls);
		free_BT(tree->rs);
		free(tree);
	}
}

/////////////////////////////////////////
///// Implémentation de la forêt	/////
/////////////////////////////////////////

FOREST* create_forest() 
{
	FOREST* f = malloc(sizeof(FOREST));
	if(!f)
		return NULL;
	f->begin = NULL;
	f->end = NULL;
	return f;
}


int push_forest(FOREST* f, BinaryTree* tree)
{
	FOREST_NODE* new = malloc(sizeof(FOREST_NODE));
	if(!new)
		return -1;
	new->tree = tree;
	new->next = NULL;
	if(empty_forest(f)) {
		f->begin = new;
		f->end = new;
	}
	else {
		f->end->next = new;
		f->end = new;
	}
	return 0;
}

BinaryTree* pop_forest(FOREST* f)
{
	if(empty_forest(f))
		return NULL;
	FOREST_NODE* begin = f->begin->next;
	BinaryTree* tree = f->begin->tree;
	free(f->begin);
	f->begin = begin;
	return tree;
}

BOOL empty_forest(FOREST* f)
{
	return (f->begin == NULL);
}

static void free_forest_node(FOREST_NODE* fn) {
	if(fn) {
		free_forest_node(fn->next);
		free(fn);
	}
}
void free_forest(FOREST* f)
{
	if(f) {
		free_forest_node(f->begin);
		free(f); 
	}
}

//////////////////////////////////////////////////////////////////////////////

int level_order_traversal(BinaryTree* tree, T* tab) {
	if(tree) {
		unsigned i=0;
		FOREST* f = create_forest();
		if(!f)
			return -1;
		if(push_forest(f, tree) != 0)
			return -1;
		
		while(!empty_forest(f)) {
			BinaryTree* t = pop_forest(f);
			if(t) {
				if(push_forest(f, t->ls) != 0)
					return -1;
				if(push_forest(f, t->rs))
					return -1;
				tab[i] = t->x;
				i++;
			}
		}
		free_forest(f);
		return 0;
	}
	else
		return -1;
}

BinaryTree* perfect_BT_from_tab(T* tab, unsigned size) {
	if(size == 0) 
		return NULL;
	BinaryTree* tree = malloc(sizeof(BinaryTree));
	for(unsigned i=0; i<size; i++) {
		edit_BT(tree, path(i), tab[i]);
	}
	return tree;
}
