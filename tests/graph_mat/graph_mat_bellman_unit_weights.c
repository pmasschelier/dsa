#include <assert.h>
#include <graph_mat.h>
#include <weight_type.h>
#include "config.h"

#define EDGE_COUNT 6
#define NODE_COUNT 6

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
	{1, 2},
	{2, 3},
	{3, 4},
	{3, 5}
};

const graph_weight_t expected[NODE_COUNT] = {GRAPH_WEIGHT_INF, 0, 1, 2, 3, 3};
const int expected_fathers[NODE_COUNT] = {-1, -1, 1, 2, 3, 3};

graph_weight_t distance[NODE_COUNT];
int father[NODE_COUNT];

int main(void) {
	graph_mat_t* g = create_graph_mat(NODE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	assert(0 == graph_mat_bellman(g, 1, distance, father));

	for (int i = 0; i < NODE_COUNT; i++) {
		assert(expected[i] == distance[i]);
		assert(expected_fathers[i] == father[i]);
	}
	free_graph_mat(g);
	return 0;
}
