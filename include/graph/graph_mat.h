#ifndef GRAPH_MAT_H
#define GRAPH_MAT_H

#include "structures.h"
#include "weight_type.h"

/**
 * @defgroup graph Graphs
 */

/**
 * @file graph/graph_mat.h
 * @brief Graphs defined with adjacency matrix
 * @ingroup graph
 *
 * Defines functions to create, free and manipulate adjacency matrices
 */

/**
 * @defgroup graph_mat Adjacency matrices
 * @ingroup graph
 * @{
 */

/**
 * @typedef graph_mat_t
 * @brief Typedef to the graph_mat struct
 *
 */
typedef struct graph_mat graph_mat_t;

/**
 * @struct graph_mat
 * @brief Weighted graph defined with adjacency matrix
 */
struct graph_mat {
	/**
	 * @brief Number of vertices in the graph
	 */
	unsigned nb_vert;
	/**
	 * @brief n*n matrix containing the edges' weights
	 *
	 * If the graph is not weighted this field will be NULL. Otherwise weights[i
	 * * size + j] will contains the weight of all the edges (i, j),
	 */
	graph_weight_t* weights;
	/**
	 * @brief n*n matrix, edges[i][j] == TRUE iif an edge (i, j) exists
	 */
	BOOL* edges;
};

/**
 * @brief Creates a graph_mat_t with no edge
 *
 * __Every graph created with this function should be freed using
 * free_graph_mat__
 * @param[in] size Number of vertices in the graph (should be strictly positive)
 * @param[in] is_weighted Should the graph be weighted
 * @return a pointer to the newly created graph or NULL if the function failed
 * @see free_graph_mat()
 */
graph_mat_t* create_graph_mat(unsigned size, BOOL is_weighted);

/**
 * @brief Frees the graph
 *
 * _Complexity: O(1)_
 *
 * @param[in] g pointer to the graph
 */
void free_graph_mat(graph_mat_t* g);

/**
 * @brief Creates or remove an edge between two vertices
 *
 * _Complexity: O(1)_
 *
 * @param[inout] g pointer to the graph
 * @param a origin vertex
 * @param b target vertex
 * @param val TRUE: the edge is created, FALSE: the edges is removed
 * @param weight if val == TRUE, this will be the weight of the new edge
 * @param reverse create or remove the symetrical edge (b, a)
 */
void graph_mat_set_edge(graph_mat_t* g,
						unsigned int a,
						unsigned int b,
						BOOL val,
						graph_weight_t weight,
						BOOL reverse);

/**
 * @brief Tests if the graph has an (a, b) edge
 *
 * _Complexity: O(1)_
 *
 * @param[in] g pointer to the graph
 * @param a origin vertex
 * @param b target vertex
 * @return TRUE iif the graph contains an (a, b) edge
 */
BOOL graph_mat_get_edge(graph_mat_t* g, unsigned int a, unsigned b);

/**
 * @brief Returns the weight of the (a, b) edge
 *
 * _Complexity: O(1)_
 *
 * @param[in] g pointer to the graph
 * @param a origin vertex
 * @param b target vertex
 * @return weight of the edge (if g is not a weighted graph returns 1)
 */
graph_weight_t graph_mat_get_weight(graph_mat_t* g, unsigned int a, unsigned b);

/**
 * @brief Preorder Depth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * _Complexity:_ \f$O(n^2)\f$
 *
 * Because of the graph implementation, for each vertex we have to check all
 * other vertices, which causes the quadratic complexity
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_mat_preorder_dfs(graph_mat_t* g,
						   unsigned r,
						   int* values,
						   int* father);

/**
 * @brief Postorder Depth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * _Complexity: O(n)_
 *
 * Because of the graph implementation, for each vertex we have to check all
 * other vertices, which causes the quadratic complexity
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father);

/**
 * @brief Breadth-First Search Traversal of a graph
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * _Complexity: O(n)_
 *
 * Because of the graph implementation, for each vertex we have to check all
 * other vertices, which causes the quadratic complexity
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * (ordered by index for a same level)
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_mat_bfs(graph_mat_t* g, unsigned r, int* values, int* father);

/**
 * @brief Computes the indegree of a vertex
 *
 * The indegree of a vertex is the number of edges going towards this vertex in
 * the graph.
 *
 * _Complexity: O(n)_
 *
 * @param g pointer to the graph
 * @param vertex index of the vertex
 */
unsigned int graph_mat_indegree(graph_mat_t* g, unsigned vertex);

/**
 * @brief Computes the outdegree of a vertex
 *
 * _Complexity: O(n)_
 *
 * The outdegree of a vertex is the number of edges going outward this vertex.
 * @param g pointer to the graph
 * @param vertex index of the vertex
 */
unsigned int graph_mat_outdegree(graph_mat_t* g, unsigned vertex);

/**
 * @brief Topological ordering of a graph
 *
 * Computes for each vertex of the a directly oriented acyclic graph (DAG) a
 * number num[i] such that is j is a predecessor of i, num[j] < num[i]
 *
 * g shoud be a DAG otherwise -ERROR_GRAPH_SHOULDBE_DAG will be returned (which
 * makes this function useful to test if a graph is a DAG).
 *
 * denum is facultative and can be left NULL, if it is not NULL it will contains
 * the node indices in topological order, such that i = denum[num[i]]
 *
 * _Complexity:_ \f$O(n^2)\f$
 *
 * @param g pointer to the graph
 * @param num num[i] is the topological number of the i vertex
 * @param denum denum[i] is the index of the vertex of topological number i
 * @return -ERROR_GRAPH_SHOULDBE_DAG if g is not a DAG and 0 otherwise
 */
int graph_mat_topological_ordering(graph_mat_t* g,
								   unsigned* num,
								   unsigned* denum);

/**
 * @defgroup graph_mat_ssshortesthpath Single Source Shortest Path algorithms
 *
 * Single-source shortest path algorithms compute the shortest path from a given
 * node called "root" to every other node of its graph.
 * The only situation where a shortest path can't be found in a graph is when
 * it contains an absorbing circuit, which is a cycle whose sum weight is
 * negative.
 *
 * This library offers 4
 * different implementations according to the assumption you can make on the
 * graph.
 *
 * - Dijkstra for graphs with positive weights.
 * - Bellman for directed acyclic graphs.
 * - Ford in the general case.
 * - Ford-Dantzig which also work for the general case but will return the
 * absorbing circuit in case of failure.
 *
 * Every algoriths take as input a pointer to the graph, the root node and
 * output arrays distance and father such that:
 * - distance[i] is the smallest distance from root to node i
 * - father[i] is the predecessor of node i in the shortest path from root to
 * node i
 *
 * If a node i can't be reached from the root we have:
 * - distance[i] = GRAPH_WEIGHT_INF
 * - father[i] = -1
 *
 * The arrays father and distance should be allocated by the user with a size of
 * graph_mat#nb_vert. However in some algorithms father is facultative and can
 * be left NULL.<br>
 * For instance allocating them on the heap:
 * ```c
 * graph_weight_t* distance = malloc(g->nb_vert * sizeof(graph_weight_t));
 * int* father = malloc(g->nb_vert * sizeof(int));
 * ```
 *
 * The shortest path algorithms can return these **error codes**:
 * - -ERROR_INVALID_PARAM1 when g is NULL
 * - -ERROR_INVALID_PARAM2 when r >= g->nb_vert
 * - -ERROR_INVALID_PARAM3 when distance is NULL
 *
 * In Ford-Dantzig implementation father is not facultative so we also have:
 * - -ERROR_INVALID_PARAM4 when father is NULL
 *
 * We can return a -ERROR_ALLOCATION_FAILED in case of malloc failure.
 *
 * Bellman will return -ERROR_GRAPH_SHOULDBE_DAG if the graph is not a directed
 * acyclic graph.
 *
 * Ford and Ford-Dantzig will return ERROR_GRAPH_HAS_ABSORBING_CIRCUIT if the
 * graph contains an absorbing circuit.
 *
 */

/**
 * @brief Dijkstra algorithm implementation with adjacency matrix
 * @ingroup graph_mat_ssshortesthpath
 *
 * __The weights of the edges have to be positive.__
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 *
 * _Complexity:_
 * - \f$O(V^2 ln(V))\f$ if dijkstra_heap == true
 * - \f$O(V^2)\f$ if dijkstra_heap == false
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance distance[i] is the minimum distance from root to node i
 * (GRAPH_WEIGHT_INF if there is no path to i)
 * @param[out] father predecessor[i] is the predecessor of node i in the
 * shortest path from root to node i
 * @return number of nodes reached or a negative error code
 */
int graph_mat_dijkstra(graph_mat_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father);

/**
 * @brief Bellman algorithm implementation with adjacency matrix
 * @ingroup graph_mat_ssshortesthpath
 *
 * __g must be a Directed Acyclic Graph (DAG)__
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 *
 * _Complexity:_ \f$O(V^2)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance distance[i] is the minimum distance from root to node i
 * (GRAPH_WEIGHT_INF if there is no path to i)
 * @param[out] father predecessor[i] is the predecessor of node i in the
 * shortest path from root to node i
 * @return number of nodes reached or a negative error code
 */
int graph_mat_bellman(graph_mat_t* g,
					  unsigned r,
					  graph_weight_t* distance,
					  int* father);

/**
 * @brief Ford algorithm implementation with adjacency matrix
 * @ingroup graph_mat_ssshortesthpath
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 *
 * _Complexity:_ \f$O(V^3)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance distance[i] is the minimum distance from root to node i
 * (GRAPH_WEIGHT_INF if there is no path to i)
 * @param[out] father predecessor[i] is the predecessor of node i in the
 * shortest path from root to node i
 * @return number of nodes reached or a negative error code
 */
int graph_mat_ford(graph_mat_t* g,
				   unsigned r,
				   graph_weight_t* distance,
				   int* father);

/**
 * @brief Ford-Dantzig algorithm implementation with adjacency matrix
 * @ingroup graph_mat_ssshortesthpath
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 *
 * This implementation allows to get the index of the last node of the
 * first-encountered absorbing circuit.
 *
 * This means that if you keep search for the ancestor of *cycle in father you
 * will eventually encounter the node *cycle again.
 *
 * _Complexity:_ \f$O(V^3)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance distance[i] is the minimum distance from root to node i
 * (GRAPH_WEIGHT_INF if there is no path to i)
 * @param[out] father predecessor[i] is the predecessor of node i in the
 * shortest path from root to node i
 * @param[out] cycle pointer to the index of a node of the absorbing circuit
 * @return number of nodes reached or a negative error code
 */
int graph_mat_ford_dantzig(graph_mat_t* g,
						   unsigned r,
						   graph_weight_t* distance,
						   int* father,
						   int* cycle);

// int Ford_and_Fulkerson(GRAPH_MAT* g, unsigned src, unsigned well, )

/** @} */

#endif
