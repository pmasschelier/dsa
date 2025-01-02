#include <assert.h>
#include <graph/graph_cast.h>
#include <graph/graph_mat.h>
#include <stddef.h>
#include <weight_type.h>
#include "graph/graph_list.h"
#include "list_ref/list_ref.h"

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
	graph_mat_t* g = create_graph_mat(NODE_COUNT, TRUE);
	for (int i = 0; i < EDGE_COUNT; i++)
		graph_mat_set_edge(g, edges[i][0], edges[i][1], TRUE, weights[i],
						   FALSE);
	graph_list_t* graph_list = NULL;
	for (unsigned test = 0; test < 2; test++) {
		graph_list = graph_mat_to_graph_list(g, graph_list);
		assert(g->nb_vert == graph_list->nb_vert);
		for (unsigned i = 0; i < g->nb_vert; i++) {
			foreach_node_node((&graph_list->neighbours[i]), node) {
				graph_list_edge_t* edge = node->p;
				assert(graph_mat_get_edge(g, i, edge->to) == TRUE);
				assert(graph_mat_get_weight(g, i, edge->to) == edge->w);
			}
		}
	}
	free_graph_mat(g);
	free_graph_list(graph_list);
	return 0;
}
