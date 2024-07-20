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
 * @brief Dijkstra algorithm implementation with adjacency matrix
 *
 * __The weights of the edges have to be positive.__
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * _Complexity:_ \f$O(n^2)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance minimum distance from r to distance[i] (GRAPH_WEIGHT_INF
 * if there is no path to i)
 * @param[out] father list of predecessors
 * @return number of nodes reached or a negative error code
 */
int graph_mat_dijkstra(graph_mat_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father);

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
 * @brief Bellman algorithm implementation with adjacency matrix
 *
 * __g must be a Directed Acyclic Graph (DAG)__
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the predecessor
 * of i in the shortest path from r to i if i can be reached from r and -1
 * otherwise.
 *
 * _Complexity:_ \f$O(n^2)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance minimum distance from r to distance[i] (GRAPH_WEIGHT_INF
 * if there is no path to i)
 * @param[out] father list of predecessors
 * @return number of nodes reached or a negative error code
 */
int graph_mat_bellman(graph_mat_t* g,
					  unsigned r,
					  graph_weight_t* distance,
					  int* father);

// int Ford_and_Fulkerson(GRAPH_MAT* g, unsigned src, unsigned well, )

#endif
