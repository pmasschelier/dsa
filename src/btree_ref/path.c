#include "btree_ref/path.h"

const btree_path_t ROOT_PATH = {0, 0};

const btree_path_t LHS_PATH = {1, 0};

const btree_path_t RHS_PATH = {1, 1};

btree_path_t* path_lhs(btree_path_t* path_from_root) {
	path_append_branch(path_from_root, LHS_PATH);
	return path_from_root;
}

btree_path_t* path_rhs(btree_path_t* path_from_root) {
	path_append_branch(path_from_root, RHS_PATH);
	return path_from_root;
}

void path_append_branch(btree_path_t* path_from_root,
						btree_path_t next_branch) {
	path_from_root->path |= (next_branch.path << path_from_root->length);
	path_from_root->length += next_branch.length;
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

void* path_walk_forward(btree_path_t* p,
						void* choice_left,
						void* choice_right) {
	void* ret = (p->path & 1) ? choice_right : choice_left;
	p->length -= 1;
	p->path >>= 1;
	return ret;
}

void* path_walk_backward(btree_path_t* p,
						 void* choice_left,
						 void* choice_right) {
	p->length -= 1;
	void* ret = ((p->path >> p->length) & 1) ? choice_right : choice_left;
	return ret;
}
