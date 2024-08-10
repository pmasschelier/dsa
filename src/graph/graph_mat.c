#include "graph/graph_mat.h"
#include <stdlib.h>
#include "errors.h"
#include "fixed_xifo_view.h"
#include "structures.h"
#include "test_macros.h"
#include "weight_type.h"

graph_mat_t* create_graph_mat(unsigned size, BOOL is_weighted) {
	graph_mat_t* ret;
	if (size == 0)
		return NULL;
	graph_mat_t* g = malloc(sizeof(graph_mat_t));
	when_null_ret(g, NULL);
	g->nb_vert = size;

	g->edges = calloc(size * size, sizeof(BOOL));
	when_null_jmp(g->edges, NULL, error);
	if (is_weighted) {
		g->weights = calloc(size * size, sizeof(graph_weight_t));
		when_null_jmp(g->weights, NULL, error2);
	} else
		g->weights = NULL;
	return g;
error2:
	free(g->edges);
error:
	free(g);
	return ret;
}

void free_graph_mat(graph_mat_t* g) {
	free(g->edges);
	if (g->weights)
		free(g->weights);
	free(g);
}

void graph_mat_set_edge(graph_mat_t* g,
						unsigned int a,
						unsigned int b,
						BOOL val,
						graph_weight_t weight,
						BOOL reverse) {
	g->edges[a * g->nb_vert + b] = val;
	if (g->weights)
		g->weights[a * g->nb_vert + b] = weight;
	if (reverse) {
		g->edges[b * g->nb_vert + a] = val;
		if (g->weights)
			g->weights[b * g->nb_vert + a] = weight;
	}
}

BOOL graph_mat_get_edge(graph_mat_t* g, unsigned int a, unsigned b) {
	return g->edges[a * g->nb_vert + b];
}

graph_weight_t graph_mat_get_weight(graph_mat_t* g,
									unsigned int a,
									unsigned b) {
	return g->weights ? g->weights[a * g->nb_vert + b] : 1;
}

#define DFS_INIT_MARK(type)                                \
	unsigned index = 0;                                    \
	when_null_ret(g, -ERROR_INVALID_PARAM1);               \
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2); \
	type* mark = calloc(g->nb_vert, sizeof(type));         \
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);         \
	if (father != NULL)                                    \
		father[r] = -1;
#define DFS_DEINIT_MARK free(mark);

#ifdef STRUCT_RECURSIVE_IMPL
static void graph_mat_preorder_dfs_rec(graph_mat_t* g,
									   unsigned current,
									   int* values,
									   int* father,
									   BOOL* mark,
									   unsigned* index) {
	mark[current] = 1;
	values[*index] = current;
	*index += 1;
	for (unsigned i = 0; i < g->nb_vert; i++) {
		if (graph_mat_get_edge(g, current, i) && !mark[i]) {
			father[i] = current;
			graph_mat_preorder_dfs_rec(g, i, values, father, mark, index);
		}
	}
}

int graph_mat_preorder_dfs(graph_mat_t* g,
						   unsigned r,
						   int* values,
						   int* father) {
	DFS_INIT_MARK(BOOL)
	graph_mat_preorder_dfs_rec(g, r, values, father, mark, &index);
	DFS_DEINIT_MARK
	return index;
}

static void graph_mat_postorder_dfs_rec(graph_mat_t* g,
										unsigned current,
										int* values,
										int* father,
										BOOL* mark,
										unsigned* index) {
	mark[current] = 1;
	for (unsigned i = 0; i < g->nb_vert; i++) {
		if (graph_mat_get_edge(g, current, i) && !mark[i]) {
			graph_mat_postorder_dfs_rec(g, i, values, father, mark, index);
			if (father != NULL)
				father[i] = current;
		}
	}
	if (values != NULL)
		values[*index] = current;
	*index += 1;
}

int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father) {
	DFS_INIT_MARK(BOOL)
	graph_mat_postorder_dfs_rec(g, r, values, father, mark, &index);
	DFS_DEINIT_MARK
	return index;
}
#else
int graph_mat_preorder_dfs(graph_mat_t* g,
						   unsigned r,
						   int* values,
						   int* father) {
	DFS_INIT_MARK(char)
	mark[r] = TRUE;

	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	fixed_xifo_copy_push_back(waiting_list, &r);

	unsigned current;
	do {
		fixed_xifo_copy_pop_back(waiting_list, &current);
		if (values != NULL)
			values[index] = current;
		index++;
		for (unsigned i = 0; i < g->nb_vert; i++) {
			int neighbour = g->nb_vert - i - 1;
			if (graph_mat_get_edge(g, current, neighbour) && !mark[neighbour]) {
				mark[neighbour] = TRUE;
				fixed_xifo_copy_push_back(waiting_list, &neighbour);
				if (father != NULL)
					father[neighbour] = current;
			}
		}
	} while (empty_fixed_xifo(waiting_list) == FALSE);

	free_fixed_xifo(waiting_list);
	DFS_DEINIT_MARK
	return index;
}

typedef enum dfs_status { UNVISITED, VISITED, SUBTREEDONE } dfs_status_t;

int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father) {
	DFS_INIT_MARK(dfs_status_t)
	mark[r] = VISITED;

	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	when_null_ret(waiting_list, -ERROR_ALLOCATION_FAILED);
	fixed_xifo_copy_push_back(waiting_list, &r);

	unsigned current;
	do {
		current = fixed_xifo_copy_back(waiting_list, int);
		if (mark[current] == SUBTREEDONE) {
			if (values != NULL)
				values[index] = current;
			index++;
			fixed_xifo_copy_pop_back(waiting_list, NULL);
			continue;
		}
		/* circular_buffer_pop_back(stack, &current); */
		for (unsigned i = 0; i < g->nb_vert; i++) {
			int neighbour = g->nb_vert - i - 1;
			if (graph_mat_get_edge(g, current, neighbour) &&
				mark[neighbour] == UNVISITED) {
				mark[neighbour] = VISITED;
				fixed_xifo_copy_push_back(waiting_list, &neighbour);
				if (father != NULL)
					father[neighbour] = current;
			}
		}
		mark[current] = SUBTREEDONE;
	} while (empty_fixed_xifo(waiting_list) == FALSE);

	free_fixed_xifo(waiting_list);
	DFS_DEINIT_MARK
	return index;
}
#endif /* ifdef STRUCT_RECURSIVE_IMPL */

static int mark_and_examine_traversal_mat(graph_mat_t* g,
										  unsigned r,
										  int* tab,
										  int* father,
										  LIST_STRUCT queue_or_stack) {
	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	mark[r] = TRUE;	 // Marquer r
	father[r] = -1;

	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	// Add the root element to the waiting list
	fixed_xifo_copy_push_back(waiting_list, &r);

	unsigned index = 0;

	while (empty_fixed_xifo(waiting_list) == FALSE) {
		int vertex;
		fixed_xifo_copy_pop_front(waiting_list, (void*)&vertex);
		if (tab != NULL)
			tab[index] = vertex;
		index++;

		for (unsigned i = 0; i < g->nb_vert; i++) {
			if (mark[i] == TRUE || graph_mat_get_edge(g, vertex, i) == FALSE)
				continue;
			mark[i] = TRUE;
			if (father)
				father[i] = vertex;
			if (queue_or_stack == STACK)
				fixed_xifo_copy_push_front(waiting_list, &i);
			else
				fixed_xifo_copy_push_back(waiting_list, &i);
		}
	}
	free_fixed_xifo(waiting_list);
	free(mark);
	return index;
}

int graph_mat_bfs(graph_mat_t* g, unsigned r, int* values, int* father) {
	return mark_and_examine_traversal_mat(g, r, values, father, QUEUE);
}

#define SSSHORTESTPATH_INIT                                \
	when_null_ret(g, -ERROR_INVALID_PARAM1);               \
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2); \
	when_null_ret(distance, -ERROR_INVALID_PARAM3);        \
	for (unsigned i = 0; i < g->nb_vert; i++)              \
		distance[i] = GRAPH_WEIGHT_INF;                    \
	if (father) {                                          \
		for (unsigned i = 0; i < g->nb_vert; i++)          \
			father[i] = -1;                                \
	}                                                      \
	distance[r] = 0;

#ifndef DIJKSTRA_HEAP_IMPL
int graph_mat_dijkstra(graph_mat_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father) {
	SSSHORTESTPATH_INIT

	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	mark[r] = TRUE;

	unsigned pivot = r;
	unsigned count = 1;	 // count of vertices reached by the algorithm
	for (unsigned i = 0; i < g->nb_vert - 1; i++) {
		// Updates the distance of all the pivots's neighbours
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // For each vertex j
			// which is a successor of pivot and haven't been marked
			if (mark[j] == TRUE || graph_mat_get_edge(g, pivot, j) == FALSE)
				continue;
			const graph_weight_t w = graph_mat_get_weight(g, pivot, j);
			const graph_weight_t d =
				weight_add_truncate_overflow(distance[pivot], w);
			if (d < distance[j]) {
				distance[j] = d;
				if (father)
					father[j] = pivot;
			}
		}

		// Finds the reached vertex not already marked, with the lowest
		// value
		graph_weight_t min = GRAPH_WEIGHT_INF;
		int jmin = -1;
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // For each vertex j
			if (mark[j] == FALSE && distance[j] >= 0 && distance[j] < min) {
				min = distance[j];
				jmin = j;
			}
		}
		// If none was found, the algorithm is terminated
		if (jmin == -1)
			break;
		pivot = jmin;
		mark[pivot] = TRUE;
		count++;
	}
	free(mark);

	return count;
}

#else
#include "compare.h"
#include "heap_view.h"

DEFINE_COMPARE_MIN_SCALAR(graph_weight_t)

int graph_mat_dijkstra(graph_mat_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father) {
	SSSHORTESTPATH_INIT

	heap_view_t* heap =
		create_heap_no_check(g->nb_vert, sizeof(graph_weight_t), distance,
							 compare_min_graph_weight_t);
	when_null_ret(heap, -ERROR_ALLOCATION_FAILED);

	// We put r at the root of (index, distance) which makes it a heap
	heap->idx_to_pos[r] = 0;
	heap->idx_to_pos[0] = r;
	heap->pos_to_idx[r] = 0;
	heap->pos_to_idx[0] = r;

	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);

	// Number of vertices reached by the algorithm
	unsigned number = 0;
	int pivot;

	// While there is a vertex left in the heap
	while ((pivot = heap_get_root(heap)) != -ERROR_IS_EMPTY) {
		// Take the root of the heap (which is the non-marked vertex with the
		// lowest distance to the root)
		if (distance[pivot] == GRAPH_WEIGHT_INF)
			break;
		mark[pivot] = TRUE;
		number++;

		// Updates the distance of all the pivots's neighbours
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // For each vertex j
			// which is a successor of pivot and haven't been marked
			if (mark[j] == TRUE || graph_mat_get_edge(g, pivot, j) == FALSE)
				continue;
			const graph_weight_t w = graph_mat_get_weight(g, pivot, j);
			const graph_weight_t d =
				weight_add_truncate_overflow(distance[pivot], w);
			if (d < distance[j]) {
				heap_update_up(heap, j, (void*)&d);
				if (father != NULL)
					father[j] = pivot;
			}
		}
	}
	free_heap(heap);
	free(mark);

	return number;
}
#endif	// DIJKSTRA_HEAP_IMPL

unsigned int graph_mat_indegree(graph_mat_t* g, unsigned vertex) {
	unsigned int degree = 0;
	for (unsigned j = 0; j < g->nb_vert; j++) {
		if (graph_mat_get_edge(g, j, vertex))
			degree++;
	}
	return degree;
}

unsigned int graph_mat_outdegree(graph_mat_t* g, unsigned vertex) {
	unsigned int degree = 0;
	for (unsigned j = 0; j < g->nb_vert; j++) {
		if (graph_mat_get_edge(g, vertex, j))
			degree++;
	}
	return degree;
}

int graph_mat_topological_ordering(graph_mat_t* g,
								   unsigned* num,
								   unsigned* denum) {
	int ret = -ERROR_GRAPH_SHOULDBE_DAG;
	when_true_ret(g->nb_vert == 0, -ERROR_GRAPH_HAS_NO_NODE);
	when_null_ret(num, -ERROR_INVALID_PARAM2);

	int number = g->nb_vert;

	fixed_xifo_view_t* stack =
		create_fixed_xifo_copy(sizeof(unsigned), g->nb_vert);
	when_null_ret(stack, -ERROR_ALLOCATION_FAILED);

	unsigned degre[g->nb_vert];
	for (unsigned i = 0; i < g->nb_vert; i++) {
		degre[i] = graph_mat_outdegree(g, i);
		if (degre[i] == 0)
			fixed_xifo_copy_push_front(stack, &i);
	}
	if (empty_fixed_xifo(stack) == TRUE)
		goto exit;
	do {
		unsigned s;
		fixed_xifo_copy_pop_front(stack, (void*)&s);
		num[s] = --number;
		if (denum)
			denum[number] = s;
		for (unsigned t = 0; t < g->nb_vert; t++) {
			if (graph_mat_get_edge(g, t, s) && --degre[t] == 0)
				fixed_xifo_copy_push_front(stack, &t);
		}
	} while (empty_fixed_xifo(stack) == FALSE);
	if (number != 0)
		goto exit;
	ret = -ERROR_NO_ERROR;
exit:
	free_fixed_xifo(stack);
	return ret;
}

int graph_mat_bellman(graph_mat_t* g,
					  unsigned r,
					  graph_weight_t* distance,
					  int* father) {
	SSSHORTESTPATH_INIT

	unsigned* num = malloc(2 * g->nb_vert * sizeof(unsigned int));
	when_null_ret(num, -ERROR_ALLOCATION_FAILED);
	unsigned* denum = num + g->nb_vert;
	int ret = graph_mat_topological_ordering(g, num, denum);
	when_false_jmp(ret == ERROR_NO_ERROR, ret, exit);

	for (unsigned i = num[r] + 1; i < g->nb_vert; i++) {
		graph_weight_t min = GRAPH_WEIGHT_INF;
		const unsigned x = denum[i];
		int y_min = -1;
		for (unsigned j = num[r]; j < i; j++) {
			int y = denum[j];
			if (graph_mat_get_edge(g, y, x) == FALSE)
				continue;
			const graph_weight_t w = graph_mat_get_weight(g, y, x);
			const graph_weight_t d =
				weight_add_truncate_overflow(distance[y], w);
			if (d < min) {
				min = d;
				y_min = y;
			}
		}
		if (y_min >= 0) {
			distance[x] = min;
			if (father != NULL)
				father[x] = y_min;
		}
	}

exit:
	free(num);
	return ret;
}

int graph_mat_ford(graph_mat_t* g,
				   unsigned r,
				   graph_weight_t* distance,
				   int* father) {
	SSSHORTESTPATH_INIT

	BOOL changed;
	unsigned k = 0;

	do {
		changed = FALSE;
		// We extend all paths (which are for now at most k-segments long)
		k++;
		for (unsigned i = 0; i < g->nb_vert; i++) {
			for (unsigned j = 0; j < g->nb_vert; j++) {
				if (graph_mat_get_edge(g, i, j) == FALSE)
					continue;

				// For each edge we try to update the target
				const graph_weight_t w = graph_mat_get_weight(g, i, j);
				const graph_weight_t d =
					weight_add_truncate_overflow(distance[i], w);
				if (d < distance[j]) {
					changed = TRUE;
					distance[j] = d;
					if (father != NULL)
						father[j] = i;
				}
			}
		}
	} while (k != g->nb_vert && changed == TRUE);
	// If a path of size n-1 was extended to a path of size n it means there is
	// an absorbent circuit in the graph (because shortest paths cannot be
	// longer than n-1).
	if (changed == TRUE)
		return -ERROR_GRAPH_HAS_ABSORBING_CIRCUIT;
	return -ERROR_NO_ERROR;
}

static BOOL test_if_edge_create_cycle(int* father, int a, int b) {
	int current = a;
	while (current != -1) {
		if (current == b)
			return TRUE;
		current = father[current];
	}
	return FALSE;
}

int graph_mat_ford_dantzig(graph_mat_t* g,
						   unsigned r,
						   graph_weight_t* distance,
						   int* father,
						   int* cycle) {
	when_null_ret(father, -ERROR_INVALID_PARAM4);
	int ret = graph_mat_dijkstra(g, r, distance, father);
	when_false_ret(ret >= 0, ret);
	fixed_xifo_view_t* update_queue =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	when_null_ret(update_queue, -ERROR_ALLOCATION_FAILED);

	while (TRUE) {
		BOOL found = FALSE;
		int x, y;
		graph_weight_t d;
		for (unsigned i = 0; i < g->nb_vert && found == FALSE; i++) {
			for (unsigned j = 0; j < g->nb_vert; j++) {
				if (graph_mat_get_edge(g, i, j) == FALSE)
					continue;

				// For each edge we test if the target can be updated
				const graph_weight_t w = graph_mat_get_weight(g, i, j);
				d = weight_add_truncate_overflow(distance[i], w);
				if (d < distance[j]) {
					x = i;
					y = j;
					found = TRUE;
					break;
				}
			}
		}
		if (found == FALSE)
			break;
		distance[y] = d;
		father[y] = x;
		if (test_if_edge_create_cycle(father, x, y) == TRUE) {
			*cycle = x;
			ret = -ERROR_GRAPH_HAS_ABSORBING_CIRCUIT;
			goto exit;
		}
		fixed_xifo_copy_push_back(update_queue, &y);

		do {
			fixed_xifo_copy_pop_front(update_queue, &x);
			for (unsigned y = 0; y < g->nb_vert; y++) {
				if (father[y] == x) {
					const graph_weight_t w = graph_mat_get_weight(g, y, x);
					d = weight_add_truncate_overflow(distance[x], w);
					distance[y] = d;
					fixed_xifo_copy_push_back(update_queue, &y);
				}
			}
		} while (empty_fixed_xifo(update_queue) == FALSE);
	}

exit:
	free_fixed_xifo(update_queue);
	return ret;
}
