#include <assert.h>
#include <binarytree.h>
#include <stdio.h>

#define TAB_LEN 21
Path paths[TAB_LEN] = {
	{0, 0x0},
	  {1, 0x0},
	{1, 0x1},
	  {2, 0x0},
	{2, 0x2},
	  {2, 0x1},
	{2, 0x3},
	{3, 0x0},
	  {3, 0x4},
	{3, 0x2},
	  {3, 0x6},
	{3, 0x1},
	  {3, 0x5},
	{3, 0x3},
	{3, 0x7},
	  {4, 0x0},
	{4, 0x8},
	  {4, 0x4},
	{4, 0xc},
	  {4, 0x2},
	{4, 0xa},
};

int main(void) {
	Path p;
	for (int i = 0; i < TAB_LEN; i++) {
		p = btree_node_to_path(i);

		assert(p.length == paths[i].length);
		assert(p.path == paths[i].path);
	}

	return 0;
}
