#include "graph_list.h"

GRAPH_LIST* init_graph_list(unsigned size)
{
	if(size == 0)
		return NULL;
	GRAPH_LIST* g = malloc(sizeof(GRAPH_LIST));	
	if(!g)
		return NULL;
	g->nb_vert = size;
	g->neighbours = malloc(sizeof(LIST[size]));
	if(!g->neighbours)
		return NULL;
	for(unsigned i = 0; i < size; i++) {
		g->neighbours[i].begin = NULL;
		g->neighbours[i].end = NULL;
		g->neighbours[i].size = sizeof(EDGE_LIST);
		g->neighbours[i].free_element = free;
	}
	
	return g;
}

static int add_edge_list_noverif(GRAPH_LIST* g,  unsigned int a, unsigned int b, long long weight) {
	LIST* neighbours = &g->neighbours[a];
	EDGE_LIST* e = malloc(sizeof(EDGE_LIST));
	if(!e)
		return -1;
	*e = (EDGE_LIST){weight, b};
	push_back_list(neighbours, e);

	return 0;
}

void set_edge_list(GRAPH_LIST* g,  unsigned int a, unsigned int b, BOOL val, long long weight, BOOL reverse)
{
	LIST* neighbours = &g->neighbours[a];
	LIST_NODE* node = neighbours->begin;

	EDGE_LIST* e = NULL;
	while(node) {
		e = node->p;
		if(e->p == b)
			break;
		node = node->next;
	}
	if(node && !val)
		remove_list(neighbours, node);
	if(node && !val)
		e->w = weight;
	if(!node && val) {
		add_edge_list_noverif(g, a, b, weight);
	}
	if(reverse)
		set_edge_list(g, b, a, val, weight, FALSE);
}


GRAPH_LIST* graph_mat_to_graph_list(GRAPH_MAT* graph_mat, GRAPH_LIST** graph_list)
{
	if(!graph_list) {
		graph_list = malloc(sizeof(GRAPH_LIST*));
		*graph_list = NULL;
	}
	
	free_graph_list(*graph_list);
	const size_t size = graph_mat->nb_vert;
	*graph_list = init_graph_list(size);

	for(unsigned i=0; i < size; i++) {
		for(unsigned j=0; j < size; j++) {
			if(graph_mat->mat[i][j].b) {
				add_edge_list_noverif(*graph_list, i, j, graph_mat->mat[i][j].w);
			}
		}
	}

	return *graph_list;
}

void free_graph_list(GRAPH_LIST* g)
{
	if(g) {
		for(unsigned i=0; i < g->nb_vert; i++)
			clean_list(&g->neighbours[i]);
		free(g->neighbours);
		free(g);
	}
}



