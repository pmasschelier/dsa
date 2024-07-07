#include <assert.h>
#include <graph/graph_mat.h>

#define EDGE_COUNT 3
#define NODE_COUNT 3

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{1, 2},
	{2, 0},
};

unsigned int num[NODE_COUNT], denum[NODE_COUNT];

int main(void) {
	graph_mat_t* g = create_graph_mat(NODE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	assert(-1 == graph_mat_topological_ordering(g, num, denum));
	free_graph_mat(g);
	return 0;
}
