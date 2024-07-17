#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "list_ref/list_ref.h"
#include "weight_type.h"

/**
 * @file graph/graph_list.h
 * @brief Graphs defined with adjacency lists
 *
 * Defines functions to create, free and manipulate adjacency lists
 */

/* Graphe représenté par des listes d'adjacence */
typedef struct graph_list_edge graph_list_edge_t;
/**
 * @struct graph_list_edge
 * @brief A graph edge
 *
 * A graph edge has a weight and goes **from** a node A **to** a node B
 */
struct graph_list_edge {
	graph_weight_t w; /**< Weight of the edge  */
	unsigned to;	  /**< Destination vertex */
};

typedef struct graph_list_path_node graph_list_path_node_t;

struct graph_list_path_node {
	unsigned index;
	int father;
	graph_weight_t dist;
};

typedef struct graph_list graph_list_t;
/**
 * @struct graph_list
 * @brief Weighted graph defined with adjacency lists
 */
struct graph_list {
	unsigned nb_vert;
	list_ref_t* neighbours;
	BOOL is_weighted;
};

/* \brief Crée un graphe sous forme de listes d'adjacence
 * \param Nombre de sommets du graphe (doit être strictement positif)
 * \return Renvoie un pointeur vers le graphe alloué (toutes les cases mémoire
 * ont été mises à zéro avec calloc) le graphe doit être libéré avec
 * free_graph_list(g); La fonction renvoie nulle si l'allocation à échoué.
 */

/**
 * @brief Creates a graph_list_t with no edge
 *
 * __Every graph created with this function should be freed using
 * free_graph_list__
 * @param[in] size Number of vertices in the graph (should be strictly positive)
 * @param[in] is_weighted Should the graph be weighted
 * @return a pointer to the newly created graph or NULL if the function failed
 * @see free_graph_list()
 */
graph_list_t* create_graph_list(unsigned size, BOOL is_weighted);

/**
 * @brief Frees the graph
 * @param[in] g pointer to the graph
 */
void free_graph_list(graph_list_t* g);

/**
 * @brief Creates or remove an edge between two vertices
 *
 * @param[inout] g pointer to the graph
 * @param a origin vertex
 * @param b target vertex
 * @param val TRUE: the edge is created, FALSE: the edges is removed
 * @param weight if val == TRUE, this will be the weight of the new edge
 * @param reverse create or remove the symetrical edge (b, a)
 */
void graph_list_set_edge(graph_list_t* g,
						 unsigned int a,
						 unsigned int b,
						 BOOL val,
						 graph_weight_t weight,
						 BOOL reverse);

/**
 * @brief Tests if the graph has an (a, b) edge
 *
 * Complexity: O(E)
 *
 * @param[in] g pointer to the graph
 * @param a origin vertex
 * @param b target vertex
 * @return a pointer to the (a, b) edge if it exists, NULL otherwise
 */
graph_list_edge_t* graph_list_get_edge(graph_list_t* g,
									   unsigned int a,
									   unsigned int b);

/**
 * @brief Preorder Depth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * Complexity: O(n)
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_list_preorder_dfs(graph_list_t* g, unsigned r, int* tab, int* father);

/**
 * @brief Postorder Depth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * Complexity: O(n)
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_list_postorder_dfs(graph_list_t* g,
							 unsigned r,
							 int* tab,
							 int* father);

/**
 * @brief Breadth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * Complexity: O(n)
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values[out] vertices index in the order they were encountered
 * (ordered by index for a same level)
 * @param[out] father[out] list of predecessors
 * @return number of nodes reached
 */
int graph_list_bfs(graph_list_t* g, unsigned r, int* values, int* father);

/* \brief Implémentation de l'algorithme de Dijkstra avec un graphe sous forme
 * de listes d'adjacence \param g Pointeur vers le graphe, !!! Les arêtes du
 * graphe ne doivent avoir que des poids posisitfs. \param r Racine du graphe
 * \param distance Après la fonction distance[i] = d(r, i) et INFINI si le
 * sommet n'est pas atteint \param father Si father != NULL, après la fonction
 * father[i] est le père de i Ces deux derniers pointeurs vont être modifiés
 * pour pointer vers des tableau alloués de la taille graphe->nb_vert !!! Ils
 * devront être libérés par l'utilisateur !!! \return Nombre de sommets atteints
 * par l'algorithme et -1 en cas d'échec \complexity O(n²)
 */
int graph_list_dijkstra(graph_list_t* g,
						unsigned r,
						graph_list_path_node_t* result);

/* \brief Numérotation topologique du graphe
 * Associe à chaque sommet i d'un graphe orienté acyclique (DAG) un numéro
 * num[i] tel que si j est un ascendant de i (*num)[j] < (*num)[i]
 * \param g Pointeur vers le graphe, !!! Ce doit être un DAG !!!
 * \param num Après la fonction (*num)[i] est le numéro topologique de i
 * \param denum si denum != NULL : après la fonction (*denum)[num[i]]=i
 * Ces trois derniers pointeurs vont être modifiés pour pointer vers des tableau
 * alloués de la taille graphe->nb_vert !!! Ils devront être libérés par
 * l'utilisateur !!! \return 0 si la numérotation a bien eu lieu -1 si father ==
 * NULL ou g->nb_vert == 0 -2 s'il y a eu une erreur d'allocation >0 si g
 * n'était pas un DAG \complexity 0(n²)
 */
int topological_numbering_list(graph_list_t* g,
							   unsigned** num,
							   unsigned** denum);

/* \brief Implémentation de l'algorithme de Bellman avec un graphe sous forme de
 * listes d'adjacence \param g Pointeur vers le graphe, !!! Ce doit être un DAG
 * !!! \param r Racine du graphe \param distance Après la fonction distance[i] =
 * d(r, i) et INFINI si le sommet n'est pas atteint \param father Si father !=
 * NULL, après la fonction father[i] est le père de i Ces deux derniers
 * pointeurs vont être modifiés pour pointer vers des tableau alloués de la
 * taille graphe->nb_vert !!! Ils devront être libérés par l'utilisateur !!!
 * \return Nombre de sommets atteints par l'algorithme et -1 en cas d'échec
 * \complexity O(n²)
 */
int Bellman_list(graph_list_t* g,
				 unsigned r,
				 long long** distance,
				 int** father);

#endif
