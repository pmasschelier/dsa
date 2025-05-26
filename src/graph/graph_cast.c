#include <stdlib.h>
#include <string.h>
#include "graph/graph_list.h"
#include "graph/graph_mat.h"
#include "list_ref/linked_list_ref.h"
#include "test_macros.h"
#include "weight_type.h"

int graph_list_add_edge_noverif(graph_list_t* g,
								unsigned int a,
								unsigned int b,
								long long weight);

graph_list_t* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									  graph_list_t* graph_list) {
	const size_t size = graph_mat->nb_vert;
	const BOOL weighted = graph_mat->weights != NULL;
	if (graph_list == NULL)
		graph_list = create_graph_list(size, weighted);
	else {
		for (unsigned i = 0; i < graph_list->nb_vert; i++)
			linked_list_clean(&graph_list->neighbours[i]);
		graph_list->neighbours =
			realloc(graph_list->neighbours, size * sizeof(linked_list_t));
		when_null_ret(graph_list->neighbours, NULL);
		for (unsigned i = 0; i < size; i++) {
			graph_list->neighbours[i].begin = NULL;
			graph_list->neighbours[i].end = NULL;
			graph_list->neighbours[i].size_bytes = sizeof(graph_list_edge_t);
		}
		graph_list->nb_vert = size;
		graph_list->is_weighted = weighted;
	}

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++) {
			if (graph_mat_get_edge(graph_mat, i, j)) {
				graph_list_add_edge_noverif(
					graph_list, i, j, graph_mat_get_weight(graph_mat, i, j));
			}
		}
	}

	return graph_list;
}

graph_mat_t* graph_list_to_graph_mat(graph_list_t* graph_list,
									 graph_mat_t* graph_mat) {
	const size_t size = graph_list->nb_vert;
	const BOOL weighted = graph_list->is_weighted;
	if (graph_mat == NULL)
		graph_mat = create_graph_mat(size, graph_list->is_weighted);
	else {
		graph_mat->edges =
			realloc(graph_mat->edges, size * size * sizeof(BOOL));
		when_null_ret(graph_mat->edges, NULL);
		memset(graph_mat->edges, FALSE, size * size * sizeof(BOOL));
		if (weighted) {
			graph_mat->weights = realloc(graph_mat->weights,
										 size * size * sizeof(graph_weight_t));
			when_null_ret(graph_mat->weights, NULL);
		}
	}

	for (unsigned i = 0; i < size; i++) {
		foreach_node_node((&graph_list->neighbours[i]), node) {
			graph_list_edge_t* edge = get_node_ref(node, graph_list_edge_t);
			graph_mat_set_edge(graph_mat, i, edge->to, TRUE, edge->w, FALSE);
		}
	}

	return graph_mat;
}
