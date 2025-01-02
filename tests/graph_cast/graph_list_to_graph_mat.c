#include <assert.h>
#include <graph/graph_cast.h>
#include <graph/graph_list.h>
#include <weight_type.h>
#include "graph/graph_mat.h"

#define EDGE_COUNT 5
#define NODE_COUNT 4

const unsigned int edges[EDGE_COUNT][2] = {
	{0, 1},
	{0, 2},
	{0, 3},
	{1, 2},
	{3, 1},
};

const graph_weight_t weights[EDGE_COUNT] = {1, 0, 99, 1, -300};

int main(void) {
	graph_list_t* g = create_graph_list(NODE_COUNT, TRUE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_list_set_edge(g, edges[i][0], edges[i][1], TRUE, weights[i],
							FALSE);
	graph_mat_t* graph_mat = NULL;

	for (unsigned test = 0; test < 2; test++) {
		graph_mat = graph_list_to_graph_mat(g, graph_mat);
		assert(g->nb_vert == graph_mat->nb_vert);

		for (unsigned i = 0; i < g->nb_vert; i++) {
			for (unsigned j = 0; j < g->nb_vert; j++) {
				graph_list_edge_t* edge = graph_list_get_edge(g, i, j);
				if (edge == NULL)
					assert(graph_mat_get_edge(graph_mat, i, j) == FALSE);
				else {
					assert(graph_mat_get_edge(graph_mat, i, j) == TRUE);
					assert(graph_mat_get_weight(graph_mat, i, j) == edge->w);
				}
			}
		}
	}
	free_graph_list(g);
	free_graph_mat(graph_mat);
	return 0;
}
