#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "list_ref/list_ref.h"
#include "weight_type.h"

/**
 * @file graph/graph_list.h
 * @brief Graphs defined with adjacency lists
 * @ingroup graph
 *
 * Defines functions to create, free and manipulate adjacency lists
 */

/**
 * @typedef graph_list_edge_t
 * @brief Typedef for the graph_list_edge structure
 *
 */
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

/**
 * @typedef graph_list_path_node_t
 * @brief Typedef for the graph_list_path_node structure
 *
 */
typedef struct graph_list_path_node graph_list_path_node_t;

/**
 * @typedef graph_list_t
 * @brief Typedef for the graph_list structure
 *
 */
typedef struct graph_list graph_list_t;

/**
 * @struct graph_list
 * @brief Weighted graph defined with adjacency lists
 *
 * For adjacency lists the complexity of the algorithms are defined using:
 * - \f$n\f$: the number of vertices
 * - \f$d+\f$: the maximum number of neighbours of a vertex
 */
struct graph_list {
	/**
	 * @brief Number of vertices in the graph
	 */
	unsigned nb_vert;
	/**
	 * @brief Array of lists of graph_list_edge
	 *
	 * neighbours[i] contains the list of edges coming out of the i-th node of
	 * the graph
	 */
	list_ref_t* neighbours;
	/** Does the graph is weighted ? */
	/**
	 * @brief Is the graph weighted
	 *
	 * If the graph is not weighted, all the edges' weights will be forced to 1.
	 */
	BOOL is_weighted;
};

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
 *
 * _Complexity:_ \f$O(n\times d^+)\f$
 */
void free_graph_list(graph_list_t* g);

/**
 * @brief Creates or remove an edge between two vertices
 *
 * _Complexity:_ \f$O(d^+)\f$
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
 * _Complexity:_ \f$O(d^+)\f$
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
 * _Complexity:_ \f$O(n\times d^+)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_list_preorder_dfs(graph_list_t* g,
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
 * _Complexity:_ \f$O(n\times d^+)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_list_postorder_dfs(graph_list_t* g,
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
 * _Complexity:_ \f$O(n\times d^+)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex
 * @param[out] values vertices index in the order they were encountered
 * (ordered by index for a same level)
 * @param[out] father list of predecessors
 * @return number of nodes reached
 */
int graph_list_bfs(graph_list_t* g, unsigned r, int* values, int* father);

/**
 * @brief Dijkstra algorithm implementation with adjacency lists
 *
 * __The weights of the edges have to be positive.__
 *
 * values and father parameters should be allocated arrays of size g->nb_vert.
 * father is facultative and can be left NULL.
 * If father != NULL, it will be set so that father[i] would be the father of i
 * if i was encountered during the traversal.
 *
 * _Complexity:_ \f$O(n \times \ln{n}\times d^+)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance minimum distance from r to distance[i] (GRAPH_WEIGHT_INF
 * if there is no path to i)
 * @param[out] father list of predecessors
 * @return number of nodes reached or a negative error code
 */
int graph_list_dijkstra(graph_list_t* g,
						unsigned r,
						graph_weight_t* distance,
						int* father);

/**
 * @brief Computes the indegree of a vertex
 *
 * The indegree of a vertex is the number of edges going towards this vertex in
 * the graph.
 *
 * _Complexity:_ \f$O(n \times d^+)\f$
 *
 * @param g pointer to the graph
 * @param vertex index of the vertex
 */
unsigned int graph_list_indegree(graph_list_t* g, unsigned vertex);

/**
 * @brief Computes the outdegree of a vertex
 *
 * Complexity: \f$O(d^+)\f$
 *
 * The outdegree of a vertex is the number of edges going outward this vertex.
 * @param g pointer to the graph
 * @param vertex index of the vertex
 */
unsigned int graph_list_outdegree(graph_list_t* g, unsigned vertex);

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
 * _Complexity:_ \f$O(n^2 \times d^+)\f$
 *
 * @param g pointer to the graph
 * @param num num[i] is the topological number of the i vertex
 * @param denum denum[i] is the index of the vertex of topological number i
 * @return -ERROR_GRAPH_SHOULDBE_DAG if g is not a DAG and  otherwise
 */
int graph_list_topological_ordering(graph_list_t* g,
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
 * _Complexity:_ \f$O(n^2 \times d^+)\f$
 *
 * @param[in] g pointer to the graph
 * @param r Starting vertex (root)
 * @param[out] distance minimum distance from r to distance[i] (GRAPH_WEIGHT_INF
 * if there is no path to i)
 * @param[out] father list of predecessors
 * @return number of nodes reached or a negative error code
 */
int graph_list_bellman(graph_list_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father);

#endif
