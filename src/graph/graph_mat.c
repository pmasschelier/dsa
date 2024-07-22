#include "graph/graph_mat.h"
#include <stdlib.h>
#include "errors.h"
#include "list_ref/list_ref.h"
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
	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	father[r] = -1;
	unsigned index = 0;
	graph_mat_preorder_dfs_rec(g, r, values, father, mark, &index);
	free(mark);
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
	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	father[r] = -1;
	unsigned index = 0;
	graph_mat_postorder_dfs_rec(g, r, values, father, mark, &index);
	free(mark);
	return index;
}
#else
#include "circular_buffer.h"

int graph_mat_preorder_dfs(graph_mat_t* g,
						   unsigned r,
						   int* values,
						   int* father) {
	when_null_ret(g, -ERROR_INVALID_PARAM1);
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2);
	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	mark[r] = TRUE;

	if (father != NULL)
		father[r] = -1;

	circular_buffer_t* stack = create_circular_buffer(sizeof(int), g->nb_vert);
	circular_buffer_push_back(stack, &r);

	unsigned index = 0;
	unsigned current;
	while (circular_buffer_size(stack) != 0) {
		circular_buffer_pop_back(stack, &current);
		if (values != NULL)
			values[index] = current;
		index++;
		for (unsigned i = 0; i < g->nb_vert; i++) {
			int neighbour = g->nb_vert - i - 1;
			if (graph_mat_get_edge(g, current, neighbour) && !mark[neighbour]) {
				mark[neighbour] = TRUE;
				circular_buffer_push_back(stack, &neighbour);
				if (father != NULL)
					father[neighbour] = current;
			}
		}
	}

	free_circular_buffer(stack);
	free(mark);
	return index;
}

typedef enum dfs_status { UNVISITED, VISITED, SUBTREEDONE } dfs_status_t;

int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father) {
	when_null_ret(g, -ERROR_INVALID_PARAM1);
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2);

	dfs_status_t* mark = calloc(g->nb_vert, sizeof(dfs_status_t));
	when_null_ret(mark, -ERROR_ALLOCATION_FAILED);
	mark[r] = VISITED;

	if (father != NULL)
		father[r] = -1;

	circular_buffer_t* stack = create_circular_buffer(sizeof(int), g->nb_vert);
	when_null_ret(stack, -ERROR_ALLOCATION_FAILED);
	circular_buffer_push_back(stack, &r);

	unsigned index = 0;
	unsigned current;

	while (circular_buffer_size(stack) != 0) {
		current = *get_buffer_last(stack, int);
		if (mark[current] == SUBTREEDONE) {
			if (values != NULL)
				values[index] = current;
			index++;
			circular_buffer_pop_back(stack, NULL);
			continue;
		}
		/* circular_buffer_pop_back(stack, &current); */
		BOOL has_neighbours = FALSE;
		for (unsigned i = 0; i < g->nb_vert; i++) {
			int neighbour = g->nb_vert - i - 1;
			if (graph_mat_get_edge(g, current, neighbour) &&
				mark[neighbour] == UNVISITED) {
				mark[neighbour] = VISITED;
				circular_buffer_push_back(stack, &neighbour);
				if (father != NULL)
					father[neighbour] = current;
				has_neighbours = TRUE;
			}
		}
		if (has_neighbours == FALSE)
			mark[current] = SUBTREEDONE;
	}

	free_circular_buffer(stack);
	free(mark);
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

	list_ref_t* waiting_list = create_list(sizeof(int));  // Création d'une file
	// Add the root element to the waiting list
	push_back_list(waiting_list, ptr(TYPE_INT, r));

	unsigned index = 0;

	while (!empty_list(waiting_list)) {
		int* vertex;
		pop_front_list(waiting_list, (void**)&vertex);
		if (tab)
			tab[index] = *vertex;
		index++;

		for (unsigned i = 0; i < g->nb_vert; i++) {
			if (mark[i] == TRUE || graph_mat_get_edge(g, *vertex, i) == FALSE)
				continue;
			mark[i] = TRUE;
			if (father)
				father[i] = *vertex;
			if (queue_or_stack == STACK)
				push_front_list(waiting_list, ptr(TYPE_INT, i));
			else
				push_back_list(waiting_list, ptr(TYPE_INT, i));
		}
		free(vertex);
	}
	free_list(waiting_list);
	free(mark);
	return index;
}

int graph_mat_bfs(graph_mat_t* g, unsigned r, int* values, int* father) {
	return mark_and_examine_traversal_mat(g, r, values, father, QUEUE);
}

int graph_mat_dijkstra(graph_mat_t* g,
					   unsigned r,
					   graph_weight_t* distance,
					   int* father) {
	when_null_ret(distance, -ERROR_INVALID_PARAM3);
	when_false_ret(r < g->nb_vert, -ERROR_INVALID_PARAM2);
	for (unsigned i = 0; i < g->nb_vert; i++)
		distance[i] = GRAPH_WEIGHT_INF;
	distance[r] = 0;

	if (father)
		father[r] = -1;

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
			if (!mark[j] && distance[j] >= 0 && distance[j] < min) {
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
	int ret = -1;
	when_true_ret(g->nb_vert == 0, -ERROR_GRAPH_HAS_NO_NODE);
	when_null_ret(num, -ERROR_INVALID_PARAM2);

	int number = g->nb_vert;

	list_ref_t* stack = create_list(sizeof(unsigned));

	unsigned degre[g->nb_vert];
	for (unsigned i = 0; i < g->nb_vert; i++) {
		degre[i] = graph_mat_outdegree(g, i);
		if (degre[i] == 0)
			push_front_list(stack, ptr(TYPE_INT, i));
	}
	if (empty_list(stack))
		goto exit;
	while (!empty_list(stack)) {
		unsigned* s = NULL;
		pop_front_list(stack, (void**)&s);
		num[*s] = --number;
		if (denum)
			denum[number] = *s;
		for (unsigned t = 0; t < g->nb_vert; t++) {
			if (graph_mat_get_edge(g, t, *s) && --degre[t] == 0)
				push_front_list(stack, ptr(TYPE_INT, t));
		}
		stack->free_element(s);
	}
	if (number != 0)
		goto exit;
	ret = 0;
exit:
	free_list(stack);
	return ret;
}

int graph_mat_bellman(graph_mat_t* g,
					  unsigned r,
					  graph_weight_t* distance,
					  int* father) {
	if (!distance)
		return -ERROR_INVALID_PARAM3;
	when_false_ret(r < g->nb_vert, -ERROR_GRAPH_HAS_NO_NODE);
	for (unsigned i = 0; i < g->nb_vert; i++)
		distance[i] = GRAPH_WEIGHT_INF;
	distance[r] = 0;

	unsigned* num = malloc(2 * g->nb_vert * sizeof(unsigned int));
	when_null_ret(num, -ERROR_ALLOCATION_FAILED);
	unsigned* denum = num + g->nb_vert;
	int ret = graph_mat_topological_ordering(g, num, denum);
	when_false_jmp(ret == ERROR_NO_ERROR, ret, exit);

	if (father) {
		for (unsigned i = 0; i <= num[r]; i++)
			father[denum[i]] = -1;
	}
	for (unsigned i = num[r] + 1; i < g->nb_vert; i++) {
		graph_weight_t min = GRAPH_WEIGHT_INF;
		const unsigned x = denum[i];
		int y_min = -1;
		for (unsigned j = num[r]; j < i; j++) {
			int y = denum[j];
			if (graph_mat_get_edge(g, y, x)) {
				const graph_weight_t w = graph_mat_get_weight(g, y, x);
				const graph_weight_t d =
					weight_add_truncate_overflow(distance[j], w);
				if (d < min) {
					min = d;
					y_min = j;
				}
			}
		}
		if (y_min >= 0) {
			distance[x] = min;
			father[x] = y_min;
		}
	}

exit:
	free(num);
	return ret;
}
