#include <assert.h>
#include <graph/graph_list.h>

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
	graph_list_t* g = create_graph_list(EDGE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_list_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++) {
		assert(NULL != graph_list_get_edge(g, edges[i][0], edges[i][1]));
	}
	free_graph_list(g);
	return 0;
}
