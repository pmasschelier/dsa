#ifndef GRAPHCAST_H
#define GRAPHCAST_H

/**
 * @file graph/graph_cast.h
 * @brief Conversion functions for different graph implementations
 * @ingroup graph
 *
 * Defines function to perform conversion between graph_mat and graph_list
 * types.
 */

typedef struct graph_mat graph_mat_t;
struct graph_mat;

typedef struct graph_list graph_list_t;
struct graph_list;

/**
 * @brief Converts a graph_mat_t into a graph_list_t
 *
 * Copy a graph defined as a matrix into a graph defined as adjacency lists.
 * The created graph_list_t will have to be released using free_graph_list()
 *
 * @param graph_mat pointer to the graph to copy
 * @param graph_list pointer to an existing graph to override, can be left NULL
 * (a new graph will be allocated)
 * @return *graph_list
 */
graph_list_t* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									  graph_list_t* graph_list);

/**
 * @brief Converts a graph_list_t into a graph_mat_t
 *
 * Copy a graph defined as adjacency lists into a graph defined as a matrix.
 * The created graph_mat_t will have to be released using free_graph_mat()
 *
 * @param graph_list pointer to the graph to copy
 * @param graph_mat pointer to an existing graph to override, can be left NULL
 * (a new graph will be allocated)
 * @return *graph_mat
 */
graph_mat_t* graph_list_to_graph_mat(graph_list_t* graph_list,
									 graph_mat_t* graph_mat);

#endif	// !GRAPHCAST_H
