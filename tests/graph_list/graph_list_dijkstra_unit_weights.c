#include <assert.h>
#include <graph/graph_list.h>
#include <weight_type.h>

#define EDGE_COUNT 6
#define NODE_COUNT 4

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
	{1, 2},
	{2, 0},
	{2, 3},
	{3, 3}
};

const graph_weight_t expected[NODE_COUNT] = {2, 0, 1, 2};
const int expected_fathers[NODE_COUNT] = {2, -1, 1, 2};

graph_weight_t distance[NODE_COUNT];
int father[NODE_COUNT];

int main(void) {
	graph_list_t* g = create_graph_list(NODE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_list_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	assert(NODE_COUNT == graph_list_dijkstra(g, 1, distance, father));
	for (unsigned i = 0; i < NODE_COUNT; i++) {
		assert(expected[i] == distance[i]);
		assert(expected_fathers[i] == father[i]);
	}
	free_graph_list(g);
	return 0;
}
