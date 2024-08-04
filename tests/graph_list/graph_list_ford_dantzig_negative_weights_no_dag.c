#include <assert.h>
#include <graph/graph_list.h>
#include <weight_type.h>

#define EDGE_COUNT 8
#define NODE_COUNT 6

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
	{0, 3},
	{1, 2},
	{3, 5},
	{5, 1},
	{3, 4},
	{4, 0},
};

const graph_weight_t weights[EDGE_COUNT] = {1, 0, 99, 1, -300, 0, 2, 10};

const graph_weight_t expected[NODE_COUNT] = {0, -201, -200, 99, 101, -201};
const int expected_fathers[NODE_COUNT] = {-1, 5, 1, 0, 3, 3};

graph_weight_t distance[NODE_COUNT];
int father[NODE_COUNT];

int main(void) {
	graph_list_t* g = create_graph_list(NODE_COUNT, TRUE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_list_set_edge(g, edges[i][0], edges[i][1], TRUE, weights[i],
							FALSE);
	int ret = graph_list_ford_dantzig(g, 0, distance, father, NULL);
	assert(NODE_COUNT == ret);

	for (int i = 0; i < NODE_COUNT; i++) {
		assert(expected[i] == distance[i]);
		assert(expected_fathers[i] == father[i]);
	}
	free_graph_list(g);
	return 0;
}
