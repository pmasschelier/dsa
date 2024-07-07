#include "graph/graph_list.h"
#include "graph/graph_mat.h"

int graph_list_add_edge_noverif(graph_list_t* g,
								unsigned int a,
								unsigned int b,
								long long weight);

graph_list_t* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									  graph_list_t** graph_list) {
	if (!graph_list) {
		graph_list = malloc(sizeof(graph_list_t*));
		*graph_list = NULL;
	}

	free_graph_list(*graph_list);
	const size_t size = graph_mat->nb_vert;
	*graph_list = create_graph_list(size, graph_mat->weights != NULL);

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++) {
			if (graph_mat_get_edge(graph_mat, i, j)) {
				graph_list_add_edge_noverif(
					*graph_list, i, j, graph_mat_get_weight(graph_mat, i, j));
			}
		}
	}

	return *graph_list;
}
