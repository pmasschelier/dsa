#include <assert.h>
#include <graph/graph_mat.h>
#include <weight_type.h>

#define EDGE_COUNT 7
#define NODE_COUNT 5

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
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
	graph_mat_t* g = create_graph_mat(NODE_COUNT, TRUE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, weights[i],
						   FALSE);
	graph_mat_ford(g, 0, distance, father);

	for (int i = 0; i < NODE_COUNT; i++) {
		assert(expected[i] == distance[i]);
		assert(expected_fathers[i] == father[i]);
	}
	free_graph_mat(g);
	return 0;
}
