#ifndef GRAPH_H
#define GRAPH_H

#include "structures.h"

/* Graphe représenté par une matrice d'adjacence */
typedef struct EDGE_MAT EDGE_MAT;
struct EDGE_MAT
{
	int w; // Poids de l'arcs
	BOOL b; // L'arc existe ?
};

typedef struct GRAPH_MAT GRAPH_MAT;
struct GRAPH_MAT 
{
	unsigned nb_vert;
	EDGE_MAT** mat;
};

GRAPH_MAT* init_graph_mat(unsigned size);
void add_edge(unsigned int a, unsigned int b);
void free_graph_mat(GRAPH_MAT* g);

void Djikstra_mat(GRAPH_MAT* g, unsigned r, int** father, int** distance);

#endif
