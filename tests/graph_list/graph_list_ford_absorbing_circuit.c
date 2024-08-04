#include <assert.h>
#include <graph/graph_list.h>
#include <weight_type.h>
#include "errors.h"

#define EDGE_COUNT 7
#define NODE_COUNT 5

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{2, 0},
	{0, 3},
	{1, 2},
	{3, 1},
	{3, 4},
	{4, 0},
};

const graph_weight_t weights[EDGE_COUNT] = {1, 0, 99, 1, -300, 2, 10};

const graph_weight_t expected[NODE_COUNT] = {0, -201, -200, 99, 101};
const int expected_fathers[NODE_COUNT] = {-1, 3, 1, 0, 3};

graph_weight_t distance[NODE_COUNT];
int father[NODE_COUNT];

int main(void) {
	graph_list_t* g = create_graph_list(NODE_COUNT, TRUE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_list_set_edge(g, edges[i][0], edges[i][1], TRUE, weights[i],
							FALSE);
	int ret = graph_list_ford(g, 0, distance, father);
	assert(ret == -ERROR_GRAPH_HAS_ABSORBING_CIRCUIT);

	free_graph_list(g);
	return 0;
}
