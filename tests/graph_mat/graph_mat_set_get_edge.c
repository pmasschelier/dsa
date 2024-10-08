#include <assert.h>
#include <graph/graph_mat.h>

#define EDGE_COUNT 6
#define NODE_COUNT 5

unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
	{1, 2},
	{2, 0},
	{1, 3},
	{3, 3}
};

int main(void) {
	graph_mat_t* g = create_graph_mat(EDGE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		assert(graph_mat_get_edge(g, edges[i][0], edges[i][1]) == TRUE);
	free_graph_mat(g);
	return 0;
}
