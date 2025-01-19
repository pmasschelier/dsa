#ifndef htable_list_ref_H
#define htable_list_ref_H

#include <stddef.h>
#include <stdint.h>
#include "compare.h"
#include "list_ref/linked_list_ref.h"

/**
 * @defgroup htable Hashtables
 */

/**
 * @file htable_ref/htable_list_ref.h
 * @brief Hashtables with linked lists as buckets
 * @ingroup htable
 *
 * Defines functions to create, free and manipulate hashtables with linked lists
 * as buckets
 */

/**
 * @defgroup htable_list_ref Hashtables with linked lists as buckets
 * @ingroup htable
 * @{
 */

/**
 * @typedef htable_list_ref_t
 * @brief Typedef to the htablt_list_ref struct
 *
 */
typedef struct htable_list_ref htable_list_ref_t;

/**
 * @struct htable_list_ref
 * @brief Weighted graph defined with adjacency matrix
 */
struct htable_list_ref {
	/**
	 * @brief Number of vertices in the graph
	 */
	unsigned bucket_count;
	equals_fn_t equals;
	/**
	 * @brief n*n matrix containing the edges' weights
	 *
	 * If the graph is not weighted this field will be NULL. Otherwise weights[i
	 * * htable_list_ref#nb_vert + j] will contains the weight of all the edges
	 * (i, j),
	 */
	list_ref_t buckets[];
};

/**
 * @brief Creates a htable_list_ref_t with n buckets
 *
 * __Every graph created with this function should be freed using
 * free_htable_list_ref__
 * @param[in] size Number of vertices in the graph (should be strictly positive)
 * @param[in] is_weighted Should the graph be weighted
 * @return a pointer to the newly created graph or NULL if the function failed
 * @see free_htable_list_ref()
 */
htable_list_ref_t* create_htable_list(unsigned buckets, equals_fn_t equals);

/**
 * @brief Frees the graph
 *
 * _Complexity: O(1)_
 *
 * @param[in] g pointer to the graph
 */
void free_htable_list(htable_list_ref_t* g);

int htable_list_insert(htable_list_ref_t* htable,
					   unsigned long long hash,
					   void* value,
					   void** found);

int htable_list_insert_clone(htable_list_ref_t* htable,
							 unsigned long long hash,
							 void* value,
							 size_t size_bytes,
							 void** found);

void* htable_list_get(htable_list_ref_t* htable,
					  unsigned long long hash,
					  void* value);

void htable_list_clean(htable_list_ref_t* htable);

#endif
