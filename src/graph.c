#include "graph.h"

GRAPH_MAT* init_graph_mat(unsigned size)
{
	GRAPH_MAT* g = malloc(sizeof(GRAPH_MAT));	
	if(!g)
		return NULL;
	g->nb_vert = size;
	
	EDGE_MAT* e = calloc(size * size, sizeof(EDGE_MAT));
	if(!e)
		return NULL;
	g->mat = malloc(size * sizeof(EDGE_MAT));
	for(unsigned i = 0; i < size; i++)
		g->mat[i] = e + i*size;
	
	return g;
}

void free_graph_mat(GRAPH_MAT* g)
{
	for(unsigned i=0; i<g->nb_vert; i++) {
		free(g->mat[i]);
	}
	free(g->mat);
	free(g);
}

void set_edge_mat(GRAPH_MAT* g,  unsigned int a, unsigned int b, BOOL val, BOOL reverse)
{
	g->mat[a][b].b = val;
	if(reverse)
		g->mat[b][a].b = val;
}

void set_edge_mat_weight(GRAPH_MAT* g,  unsigned int a, unsigned int b, int weight, BOOL reverse)
{
	g->mat[a][b].w = weight;
	if(reverse)
		g->mat[b][a].w = weight;
}

void Dijkstra_mat(GRAPH_MAT* g, unsigned r, int** father, int** distance)
{
	int* A = malloc(sizeof(int[g->nb_vert]));
	for(unsigned i=0; i < g->nb_vert; i++) 
		A[i] = 0;
	A[r] = 1;
	int* Pi = malloc(sizeof(int[g->nb_vert]));
	for(unsigned i=0; i < g->nb_vert; i++)
		Pi[i] = -1;
	Pi[r] = 0;
	int* f = malloc(sizeof(int[g->nb_vert]));
	f[r] = -1;
	
	unsigned pivot = r;
	
	for(unsigned i=0; i < g->nb_vert-1; i++) {
		for(unsigned j=0; j < g->nb_vert; j++) { // Pour tout sommet j
			if(!A[j] && g->mat[pivot][j].b) { // non encore dans A et successeur de pivot
				int d = Pi[pivot] + g->mat[pivot][j].w;
				if(Pi[j] == -1 || d < Pi[j]) {
					Pi[j] = d;
					f[j] = pivot;
				}
			}
		}
		int min = -1;
		int jmin = -1;
		for(unsigned j=0; j < g->nb_vert; j++) { // Pour tout sommet j
			if(!A[j] && Pi[j] >=0 && (min == -1 || Pi[j] < min)) { // non encore dans A
				min = Pi[j];
				jmin = j;
			}
		}
		if(jmin >= 0) {
			pivot = jmin;
			A[pivot] = 1;
		}
	}
	*father = f;
	*distance = Pi;
}
