#include <assert.h>
#include <graph/graph_mat.h>

#define EDGE_COUNT 8
#define NODE_COUNT 7

const unsigned int edges[EDGE_COUNT][2] = {
	{6, 1},
	{6, 3},
	{1, 0},
	{1, 2},
	{2, 3},
	{3, 4},
	{3, 5},
	{0, 4},
};

unsigned int num[NODE_COUNT], denum[NODE_COUNT];

int main(void) {
	graph_mat_t* g = create_graph_mat(NODE_COUNT, FALSE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, 0, FALSE);
	assert(0 == graph_mat_topological_ordering(g, num, denum));
	for (unsigned i = 0; i < NODE_COUNT; i++) {
		assert(i == denum[num[i]]);
		for (unsigned j = 0; j < NODE_COUNT; j++) {
			if (graph_mat_get_edge(g, i, j) == TRUE) {
				assert(num[i] < num[j]);
			}
		}
	}
	free_graph_mat(g);
	return 0;
}
