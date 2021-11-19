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

static int allow_tab_father_mark(unsigned length, int** tab, int** father, char** mark) {
	if(tab) {
		*tab = malloc(sizeof(int[length]));
		if(!*tab)
			return -1;
	}
	if(father) {
		*father = malloc(sizeof(int[length]));
		if(!*father && tab) {
			free(*tab);
			return -1;
		}
	}
	*mark = calloc(length, sizeof(char));
	if(!*mark) {
		if(tab) free(*tab);
		if(father) free(*father);
		return -1;
	}
	return 0;
}

int mark_and_examine_traversal_list(GRAPH_LIST* g, unsigned r, int** tab, int** father, LIST_STRUCT queue_or_stack)
{
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1; // Marquer r
	(*father)[r] = -1;
	
	LIST* waiting_list = create_list(sizeof(int)); // Création d'une file
	push_back_list(waiting_list, ptr(TYPE_INT, r)); // Ajouter r à la liste d'attente
	
	unsigned index = 0;
	
	while(!empty_list(waiting_list))
	{
		int* vertex = pop_front_list(waiting_list);
		if(tab)
			(*tab)[index] = *vertex;
		index++;
		
		LIST_NODE* node = g->neighbours[*vertex].begin;
		EDGE_LIST *e = NULL;
		while(node) {
			e = node->p;
			if(!mark[e->p]) {
				mark[e->p] = TRUE;
				if(father)
					(*father)[e->p] = *vertex;
				if(queue_or_stack == STACK)
					push_front_list(waiting_list, ptr(TYPE_INT, e->p));
				else
					push_back_list(waiting_list, ptr(TYPE_INT, e->p));
			}
			node = node->next;
		}
		free(vertex);
	}
	return index;
}

int DFS_list(GRAPH_LIST* g, unsigned r, int** tab, int** father) 
{
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1; // Marquer r
	(*father)[r] = -1;
	(*tab)[0] = r;
	
	unsigned current = r;
	unsigned index = 1;
	
	while(TRUE) {
		BOOL any_edge = FALSE;

		LIST_NODE* node = g->neighbours[current].begin;
		EDGE_LIST *e = NULL;
		while(node) {
			e = node->p;
			if(!mark[e->p]) {
				mark[e->p] = 1;
				(*tab)[index++] = e->p;
				if(father)
					(*father)[e->p] = current;
				current = e->p;
				any_edge = TRUE;
				break;
			}
			node = node->next;
		}
		if(!any_edge) {
			if(current != r)
				current = (*father)[current];
			else
				break;
		}
	}
	return index;
}

static void DFS_list_recursive_rec(GRAPH_LIST* g, unsigned current, int** tab, int** father, char* mark, unsigned* index)
{
	mark[current] = 1;
	(*tab)[*index] = current;
	*index += 1;
	LIST_NODE* node = g->neighbours[current].begin;
	EDGE_LIST *e = NULL;
	while(node) {
		if(!mark[e->p]) {
			(*father)[e->p] = current;
			DFS_list_recursive_rec(g, e->p, tab, father, mark, index);
		}
	}
}

int DFS_list_recursive(GRAPH_LIST* g, unsigned r, int** tab, int** father) {
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	(*father)[r] = -1;
	unsigned index = 0;
	DFS_list_recursive_rec(g, r, tab, father, mark, &index);
	return index;
}