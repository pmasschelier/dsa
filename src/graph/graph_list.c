#include "graph/graph_list.h"
#include <stdlib.h>
#include "config.h"
#include "errors.h"
#include "fixed_xifo_view.h"
#include "list_ref/linked_list_ref.h"
#include "test_macros.h"
#include "weight_type.h"

// TODO: Implement graph_list as unordered lists (dynamic array where remove =
// swap with last element)

graph_list_t* create_graph_list(unsigned size, BOOL is_weighted) {
	graph_list_t* ret;
	when_true_ret(size == 0, NULL);

	graph_list_t* g = malloc(sizeof(graph_list_t));
	when_null_ret(g, NULL);

	g->nb_vert = size;
	g->neighbours = malloc(size * sizeof(linked_list_t));
	when_null_jmp(g->neighbours, NULL, error);
	g->is_weighted = is_weighted;

	for (unsigned i = 0; i < size; i++)
        linked_list_init(&g->neighbours[i], sizeof(graph_list_edge_t));

	return g;
error:
	free(g);
	return ret;
}

int graph_list_add_edge_noverif(graph_list_t* g,
								unsigned int a,
								unsigned int b,
								long long weight) {
	linked_list_t* neighbours = &g->neighbours[a];
	graph_list_edge_t e = {.w = weight, .to = b};
	linked_list_push_back(neighbours, &e);

	return 0;
}

graph_list_t* create_graph_list_from_predecessors(unsigned size, int* father) {
	graph_list_t* ret = create_graph_list(size, FALSE);
	when_null_ret(ret, NULL);

	for (unsigned i = 0; i < size; i++) {
		if (father[i] != -1)
			graph_list_add_edge_noverif(ret, father[i], i, 1);
	}
	return ret;
}

#ifdef STRUCT_RECURSIVE_IMPL
static linked_list_node_t* find_edge_rec(linked_list_node_t* node, unsigned int b) {
	if (node == NULL)
		return NULL;
	graph_list_edge_t* e = get_node_ref(node, graph_list_edge_t);
	if (e->to == b)
		return node;
	return find_edge_rec(node->next, b);
}

static linked_list_node_t* find_edge(graph_list_t* g,
								  unsigned int a,
								  unsigned int b) {
	linked_list_t* neighbours = &g->neighbours[a];
	linked_list_node_t* node = neighbours->begin;

	return find_edge_rec(node, b);
}
#else
static linked_list_node_t* find_edge(graph_list_t* g,
								  unsigned int a,
								  unsigned int b) {
	linked_list_t* neighbours = &g->neighbours[a];
	linked_list_node_t* node = neighbours->begin;

	graph_list_edge_t* e = NULL;
	while (node) {
		e = get_node_ref(node, graph_list_edge_t);
		if (e->to == b)
			return node;
		node = node->next;
	}
	return NULL;
}
#endif

graph_list_edge_t* graph_list_get_edge(graph_list_t* g,
									   unsigned int a,
									   unsigned int b) {
	linked_list_node_t* node = find_edge(g, a, b);
	return node == NULL ? NULL : get_node_ref(node, graph_list_edge_t);
}

void graph_list_set_edge(graph_list_t* g,
						 unsigned int a,
						 unsigned int b,
						 BOOL val,
						 graph_weight_t weight,
						 BOOL reverse) {
	if (g->is_weighted == FALSE)
		weight = 1;
	linked_list_node_t* node = find_edge(g, a, b);
	if (node && !val)
		linked_list_remove(&g->neighbours[a], node, NULL);
	if (node && val)
		get_node_ref(node, graph_list_edge_t)->w = weight;
	if (!node && val)
		graph_list_add_edge_noverif(g, a, b, weight);
	if (reverse)
		graph_list_set_edge(g, b, a, val, weight, FALSE);
}

void free_graph_list(graph_list_t* g) {
	if (g) {
		for (unsigned i = 0; i < g->nb_vert; i++)
			linked_list_clean(&g->neighbours[i]);
		free(g->neighbours);
		free(g);
	}
}

int mark_and_examine_traversal_list(graph_list_t* g,
									unsigned r,
									int* tab,
									int* father,
									LIST_STRUCT queue_or_stack) {
	when_null_ret(tab, -ERROR_INVALID_PARAM3);
	if (father)
		father[r] = -1;

	char* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	mark[r] = 1;  // Marquer r

	// We create a queue an put the root inside
	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	fixed_xifo_copy_push_back(waiting_list, &r);

	unsigned index = 0;

	while (!empty_fixed_xifo(waiting_list)) {
		int vertex;
		fixed_xifo_copy_pop_front(waiting_list, (void*)&vertex);
		if (tab != NULL)
			tab[index] = vertex;
		index++;

		linked_list_node_t* node = g->neighbours[vertex].begin;
		graph_list_edge_t* e = NULL;
		while (node) {
			e = get_node_ref(node, graph_list_edge_t);
			if (!mark[e->to]) {
				mark[e->to] = TRUE;
				if (father)
					father[e->to] = vertex;
				if (queue_or_stack == STACK)
					fixed_xifo_copy_push_front(waiting_list, &e->to);
				else
					fixed_xifo_copy_push_back(waiting_list, &e->to);
			}
			node = node->next;
		}
	}
	free(mark);
	free_fixed_xifo(waiting_list);
	return index;
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
static void graph_list_preorder_dfs_impl(graph_list_t* g,
										 unsigned current,
										 int* tab,
										 int* father,
										 char* mark,
										 unsigned* index) {
	mark[current] = TRUE;
	tab[*index] = current;
	*index += 1;
	linked_list_node_t* node = g->neighbours[current].begin;
	graph_list_edge_t* e = NULL;
	while (node) {
		e = get_node_ref(node, graph_list_edge_t);
		if (!mark[e->to]) {
			if (father)
				father[e->to] = current;
			graph_list_preorder_dfs_impl(g, e->to, tab, father, mark, index);
		}
		node = node->next;
	}
}

static void graph_list_postorder_dfs_impl(graph_list_t* g,
										  unsigned current,
										  int* tab,
										  int* father,
										  char* mark,
										  unsigned* index) {
	mark[current] = TRUE;
	linked_list_node_t* node = g->neighbours[current].begin;
	graph_list_edge_t* e = NULL;
	while (node) {
		e = get_node_ref(node, graph_list_edge_t);
		if (!mark[e->to]) {
			if (father)
				father[e->to] = current;
			graph_list_postorder_dfs_impl(g, e->to, tab, father, mark, index);
		}
		node = node->next;
	}
	tab[*index] = current;
	*index += 1;
}

int graph_list_preorder_dfs(graph_list_t* g,
							unsigned r,
							int* tab,
							int* father) {
	DFS_INIT_MARK(char)
	graph_list_preorder_dfs_impl(g, r, tab, father, mark, &index);
	DFS_DEINIT_MARK
	return index;
}

int graph_list_postorder_dfs(graph_list_t* g,
							 unsigned r,
							 int* tab,
							 int* father) {
	DFS_INIT_MARK(char)
	graph_list_postorder_dfs_impl(g, r, tab, father, mark, &index);
	DFS_DEINIT_MARK
	return index;
}
#else
int graph_list_preorder_dfs(graph_list_t* g,
							unsigned r,
							int* tab,
							int* father) {
	DFS_INIT_MARK(char)
	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	when_null_ret(waiting_list, -ERROR_ALLOCATION_FAILED);

	int pivot = r;
	fixed_xifo_copy_push_back(waiting_list, &pivot);
	do {
		fixed_xifo_copy_pop_back(waiting_list, &pivot);
		mark[pivot] = TRUE;
		tab[index++] = pivot;
		linked_list_foreach_rev(&g->neighbours[pivot], e, graph_list_edge_t) {
			if (mark[e->to] == FALSE) {
				fixed_xifo_copy_push_back(waiting_list, &e->to);
				if (father != NULL)
					father[e->to] = pivot;
			}
		}

	} while (empty_fixed_xifo(waiting_list) == FALSE);
	free_fixed_xifo(waiting_list);
	DFS_DEINIT_MARK
	return index;
}

typedef enum dfs_status { UNVISITED, VISITED, SUBTREEDONE } dfs_status_t;

int graph_list_postorder_dfs(graph_list_t* g,
							 unsigned r,
							 int* tab,
							 int* father) {
	DFS_INIT_MARK(dfs_status_t)
	fixed_xifo_view_t* waiting_list =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	when_null_ret(waiting_list, -ERROR_ALLOCATION_FAILED);

	int pivot = r;
	fixed_xifo_copy_push_back(waiting_list, &pivot);
	mark[pivot] = VISITED;
	do {
		/* fixed_xifo_copy_pop_back(waiting_list, &pivot); */
		pivot = fixed_xifo_copy_back(waiting_list, int);
		if (mark[pivot] == SUBTREEDONE) {
			if (tab != NULL)
				tab[index] = pivot;
			index++;
			fixed_xifo_copy_pop_back(waiting_list, NULL);
			continue;
		}
		linked_list_foreach_rev(&g->neighbours[pivot], e, graph_list_edge_t) {
			if (mark[e->to] == UNVISITED) {
				fixed_xifo_copy_push_back(waiting_list, &e->to);
				mark[e->to] = VISITED;
				if (father != NULL)
					father[e->to] = pivot;
			}
		}
		mark[pivot] = SUBTREEDONE;

	} while (empty_fixed_xifo(waiting_list) == FALSE);
	free_fixed_xifo(waiting_list);
	DFS_DEINIT_MARK
	return index;
}
#endif	// STRUCT_RECURSIVE_IMPL

int graph_list_bfs(graph_list_t* g, unsigned r, int* values, int* father) {
	return mark_and_examine_traversal_list(g, r, values, father, QUEUE);
}

// static unsigned father_heap(unsigned i) {
// 	return i == 0 ? 0 : (i + 1) / 2 - 1;
// }
//
// static unsigned ls_heap(unsigned i) {
// 	return (i + 1) * 2 - 1;
// }
//
// static unsigned rs_heap(unsigned i) {
// 	return (i + 1) * 2;
// }
//
// static void pullup_heap(unsigned* index, long long* val, unsigned end) {
// 	unsigned i = end;
// 	unsigned key = index[end];
//
// 	while (i >= 1 && val[key] < val[father_heap(i)]) {
// 		index[i] = index[father_heap(i)];
// 		i = father_heap(i);
// 	}
// 	index[i] = key;
// }
//
// static void build_heap(unsigned* index, long long* val, unsigned size) {
// 	for (unsigned i = 0; i < size; i++)
// 		pullup_heap(index, val, i);
// }
//
// static void pulldown_heap(unsigned* index,
// 						  long long* val,
// 						  unsigned size,
// 						  unsigned n) {
// 	BOOL found = FALSE;
// 	unsigned key = index[n];
// 	int i_min;
//
// 	while (!found && ls_heap(n) < size) {
// 		// We compute the son with the lowest value
// 		if (ls_heap(n) == size - 1)
// 			i_min = size - 1;
// 		else if (val[ls_heap(n)] <= val[rs_heap(n)])
// 			i_min = ls_heap(n);
// 		else
// 			i_min = rs_heap(n);
//
// 		// If the vertex vertex has a lower value that is lowest son
// 		if (val[key] < val[i_min]) {
// 			// We replace the vertex by its node
// 			index[n] = index[i_min];
// 			n = i_min;
// 		} else {
// 			// Otherwise the vertex is lower than its children and should not go
// 			// further down
// 			found = TRUE;
// 		}
// 	}
// 	// We overwrite the last moved son with the original vertex
// 	index[n] = key;
// }
//
// static void swap(unsigned* index, int i, int j) {
// 	unsigned a = index[i];
// 	index[i] = index[j];
// 	index[j] = a;
// }

#define SSSHORTESTPATH_INIT                                \
	when_null_ret(distance, -ERROR_INVALID_PARAM3);        \
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2); \
	for (unsigned i = 0; i < g->nb_vert; i++)              \
		distance[i] = GRAPH_WEIGHT_INF;                    \
	if (father != NULL) {                                  \
		for (unsigned i = 0; i < g->nb_vert; i++)          \
			father[i] = -1;                                \
	}                                                      \
	distance[r] = 0;

#ifndef DIJKSTRA_HEAP_IMPL
int graph_list_dijkstra(graph_list_t* g,
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
		// For each successor of pivot
		linked_list_foreach(&g->neighbours[pivot], e, graph_list_edge_t) {
			if (mark[e->to] == TRUE)
				continue;

			graph_weight_t d =
				weight_add_truncate_overflow(distance[pivot], e->w);
			if (d < distance[e->to]) {
				distance[e->to] = d;
				if (father)
					father[e->to] = pivot;
			}
		}

		// Finds the reached vertex not already marked, with the lowest
		// value
		graph_weight_t min = GRAPH_WEIGHT_INF;
		int jmin = -1;
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // For each vertex j
			if (mark[j] == FALSE && distance[j] < min) {
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

int graph_list_dijkstra(graph_list_t* g,
						unsigned r,
						graph_weight_t* distance,
						int* father) {
	SSSHORTESTPATH_INIT

	heap_view_t* heap =
		create_heap_no_check(g->nb_vert, sizeof(graph_weight_t), distance,
							 compare_min_graph_weight_t);

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

		// For each successor of pivot
		linked_list_foreach(&g->neighbours[pivot], e, graph_list_edge_t) {
			if (mark[e->to] == TRUE)
				continue;
			graph_weight_t d =
				weight_add_truncate_overflow(distance[pivot], e->w);
			if (d < distance[e->to]) {
				heap_update_up(heap, e->to, &d);
				if (father != NULL)
					father[e->to] = pivot;
			}
		}
	}
	free_heap(heap);
	free(mark);

	return number;
}
#endif	// DIJKSTRA_HEAP_IMPL

unsigned int graph_list_indegree(graph_list_t* g, unsigned vertex) {
	unsigned degree = 0;
	for (unsigned i = 0; i < g->nb_vert; i++) {
		linked_list_t* neighbours = &g->neighbours[i];
		linked_list_foreach(neighbours, edge, graph_list_edge_t) degree +=
			edge->to == vertex;
	}
	return degree;
}

unsigned int graph_list_outdegree(graph_list_t* g, unsigned vertex) {
	return linked_list_length(&g->neighbours[vertex]);
}

int graph_list_topological_ordering(graph_list_t* g,
									unsigned* num,
									unsigned* denum) {
	int ret = -ERROR_GRAPH_SHOULDBE_DAG;
	when_true_ret(g->nb_vert == 0, -ERROR_GRAPH_HAS_NO_NODE);
	when_null_ret(num, -ERROR_INVALID_PARAM2);

	int number = g->nb_vert;

	fixed_xifo_view_t* stack =
		create_fixed_xifo_copy(sizeof(unsigned), g->nb_vert);

	unsigned degre[g->nb_vert];
	for (unsigned i = 0; i < g->nb_vert; i++) {
		degre[i] = graph_list_outdegree(g, i);
		if (degre[i] == 0)
			fixed_xifo_copy_push_front(stack, &i);
	}
	if (empty_fixed_xifo(stack) == TRUE)
		goto exit;
	do {
		unsigned s;
		fixed_xifo_copy_pop_front(stack, &s);
		num[s] = --number;
		if (denum)
			denum[number] = s;
		for (unsigned t = 0; t < g->nb_vert; t++) {
			if (graph_list_get_edge(g, t, s) != NULL && --degre[t] == 0)
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

int graph_list_bellman(graph_list_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father) {
	SSSHORTESTPATH_INIT

	unsigned* num = malloc(2 * g->nb_vert * sizeof(unsigned int));
	when_null_ret(num, -ERROR_ALLOCATION_FAILED);
	unsigned* denum = num + g->nb_vert;
	int ret = graph_list_topological_ordering(g, num, denum);
	if (ret == -ERROR_GRAPH_SHOULDBE_DAG)
		goto exit;
	when_false_jmp(ret == ERROR_NO_ERROR, ret, exit);

	for (unsigned i = num[r]; i < g->nb_vert - 1; i++) {
		const unsigned x = denum[i];
		linked_list_foreach(&g->neighbours[x], e, graph_list_edge_t) {
			const graph_weight_t d =
				weight_add_truncate_overflow(distance[x], e->w);
			if (d < distance[e->to]) {
				distance[e->to] = d;
				if (father != NULL)
					father[e->to] = x;
			}
		}
	}

exit:
	free(num);
	return ret;
}

int graph_list_ford(graph_list_t* g,
					unsigned r,
					graph_weight_t* distance,
					int* father) {
	SSSHORTESTPATH_INIT

	BOOL changed;
	unsigned k = 0;

	do {
		changed = FALSE;
		k++;
		// On step k we know distance[] for paths of k-1 edges
		// So we compute distance[] for paths of k edges by iterating over each
		// edge of the graph
		for (unsigned i = 0; i < g->nb_vert; i++) {
			// Finds the predecessor of i which will allow us to minimize the
			// distance
			linked_list_foreach(&g->neighbours[i], e, graph_list_edge_t) {
				const graph_weight_t d =
					weight_add_truncate_overflow(distance[i], e->w);
				if (d < distance[e->to]) {
					changed = TRUE;
					distance[e->to] = d;
					if (father != NULL)
						father[e->to] = i;
				}
			}
		}
	} while (k != g->nb_vert && changed == TRUE);
	// If on step n-1 new shorter path as been discovered it means that the
	// graph contains an absorbing circuit (because elementary paths have at
	// most n-1 edges).
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

int graph_list_ford_dantzig(graph_list_t* g,
							unsigned r,
							graph_weight_t* distance,
							int* father,
							int* cycle) {
	when_null_ret(father, -ERROR_INVALID_PARAM4);
	// We start from a covering tree of root r and a distance function such that
	// distance[j] = distance[i] + weight(i, j)
	// These can be computed from the dijkstra algorithm
	// TODO: Replace this with a simple DFS and check if it improves
	// performances.
	int ret = graph_list_dijkstra(g, r, distance, father);
	when_false_ret(ret >= 0, ret);
	fixed_xifo_view_t* update_queue =
		create_fixed_xifo_copy(sizeof(int), g->nb_vert);
	when_null_ret(update_queue, -ERROR_ALLOCATION_FAILED);

	while (TRUE) {
		BOOL found = FALSE;
		int x, y;
		graph_weight_t d;
		// Find an edge which would reduce the distance of node if added to the
		// covering tree.
		for (unsigned i = 0; i < g->nb_vert && found == FALSE; i++) {
			linked_list_foreach(&g->neighbours[i], e, graph_list_edge_t) {
				d = weight_add_truncate_overflow(distance[i], e->w);
				if (d < distance[e->to]) {
					x = i;
					y = e->to;
					found = TRUE;
					break;
				}
			}
		}
		// If no edge was found the computed distance is optimal.
		// Proof:
		// Let's assume that no edge can improve our computed distances but that
		// there is a path C from r to a node x such that
		//   weight(C) < distance[x].
		// Then it exists an arc (u, v) such that:
		// - weight(C[r:u]) >= distance[u]
		// - weight(C[r:v]) < distance[v]
		// So: distance[v] > distance[u] + weight(u, v)
		// (u, v) should enter in the tree, hence the contradicton.
		if (found == FALSE)
			break;
		// Otherwise we update the tree and the distance
		distance[y] = d;
		father[y] = x;
		// Then we test if adding this edge created a cycle, in which case we
		// found an absorbing cycle.
		// Proof:
		// - For each edge (a, b) in the tree:
		//   distance[b] - distance[a] = weight(a, b)
		// - For (x, y): distance[y] - distance[x] > weight(x, y)
		// Hence, summing these relations for all edges of the circuit we have:
		//  0 > weight(circuit)
		if (test_if_edge_create_cycle(father, x, y) == TRUE) {
			*cycle = x;
			ret = -ERROR_GRAPH_HAS_ABSORBING_CIRCUIT;
			goto exit;
		}

		// Update the distance of all descendant of y in the covering tree with
		// a BFS.
		fixed_xifo_copy_push_back(update_queue, &y);
		do {
			fixed_xifo_copy_pop_front(update_queue, &x);
			linked_list_foreach(&g->neighbours[x], e, graph_list_edge_t) {
				if (father[e->to] == x) {
					d = weight_add_truncate_overflow(distance[x], e->w);
					distance[e->to] = d;
					fixed_xifo_copy_push_back(update_queue, &e->to);
				}
			}
		} while (empty_fixed_xifo(update_queue) == FALSE);
	}

exit:
	free_fixed_xifo(update_queue);
	return ret;
}
