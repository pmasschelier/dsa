#include <assert.h>
#include <graph/graph_mat.h>

#define EDGE_COUNT 6
#define NODE_COUNT 4

const unsigned int edges[EDGE_COUNT][2] = {
	{1, 0},
	{1, 2},
	{1, 3},
	{2, 1},
	{2, 3},
	{3, 3}
};

int tab[NODE_COUNT], father[NODE_COUNT];

int main(void) {
	graph_mat_t* g = create_graph_mat(NODE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	unsigned int degree = graph_mat_outdegree(g, 1);
	assert(degree == 3);
	free_graph_mat(g);
	return 0;
}
