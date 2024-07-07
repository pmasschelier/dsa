#include "graph_list.h"
#include <stdlib.h>
#include "errors.h"
#include "graph_mat.h"
#include "list_ref/list_ref.h"
#include "test_macros.h"
#include "weight_type.h"

graph_list_t* create_graph_list(unsigned size, BOOL is_weighted) {
	if (size == 0)
		return NULL;
	graph_list_t* g = malloc(sizeof(graph_list_t));
	TEST_PTR_FAIL_FUNC(g, NULL, );
	g->nb_vert = size;
	g->neighbours = malloc(size * sizeof(list_ref_t));
	TEST_PTR_FAIL_FUNC(g->neighbours, NULL, free(g));
	g->is_weighted = is_weighted;

	for (unsigned i = 0; i < size; i++) {
		g->neighbours[i].begin = NULL;
		g->neighbours[i].end = NULL;
		g->neighbours[i].size = sizeof(graph_list_edge_t);
		g->neighbours[i].free_element = free;
	}

	return g;
}

static int add_edge_list_noverif(graph_list_t* g,
								 unsigned int a,
								 unsigned int b,
								 long long weight) {
	list_ref_t* neighbours = &g->neighbours[a];
	graph_list_edge_t* e = malloc(sizeof(graph_list_edge_t));
	TEST_PTR_FAIL_FUNC(e, -ERROR_ALLOCATION_FAILED, );
	*e = (graph_list_edge_t){weight, b};
	push_back_list(neighbours, e);

	return 0;
}

static node_list_ref_t* find_edge(graph_list_t* g,
								  unsigned int a,
								  unsigned int b) {
	list_ref_t* neighbours = &g->neighbours[a];
	node_list_ref_t* node = neighbours->begin;

	graph_list_edge_t* e = NULL;
	while (node) {
		e = node->p;
		if (e->to == b)
			return node;
		node = node->next;
	}
	return NULL;
}

BOOL graph_list_get_edge(graph_list_t* g, unsigned int a, unsigned int b) {
	return find_edge(g, a, b) != NULL;
}

void graph_list_set_edge(graph_list_t* g,
						 unsigned int a,
						 unsigned int b,
						 BOOL val,
						 graph_weight_t weight,
						 BOOL reverse) {
	if (g->is_weighted == FALSE)
		weight = 1;
	node_list_ref_t* node = find_edge(g, a, b);
	if (node && !val)
		remove_list(&g->neighbours[a], node, NULL);
	if (node && val)
		((graph_list_edge_t*)node->p)->w = weight;
	if (!node && val)
		add_edge_list_noverif(g, a, b, weight);
	if (reverse)
		graph_list_set_edge(g, b, a, val, weight, FALSE);
}

graph_list_t* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									  graph_list_t** graph_list) {
	if (!graph_list) {
		graph_list = malloc(sizeof(graph_list_t*));
		*graph_list = NULL;
	}

	free_graph_list(*graph_list);
	const size_t size = graph_mat->nb_vert;
	*graph_list = create_graph_list(size, graph_mat->weights != NULL);

	for (unsigned i = 0; i < size; i++) {
		for (unsigned j = 0; j < size; j++) {
			if (graph_mat_get_edge(graph_mat, i, j)) {
				add_edge_list_noverif(*graph_list, i, j,
									  graph_mat_get_weight(graph_mat, i, j));
			}
		}
	}

	return *graph_list;
}

void free_graph_list(graph_list_t* g) {
	if (g) {
		for (unsigned i = 0; i < g->nb_vert; i++)
			clean_list(&g->neighbours[i]);
		free(g->neighbours);
		free(g);
	}
}

int mark_and_examine_traversal_list(graph_list_t* g,
									unsigned r,
									int* tab,
									int* father,
									LIST_STRUCT queue_or_stack) {
	TEST_PTR_FAIL_FUNC(tab, -ERROR_INVALID_PARAM3, );
	if (father)
		father[r] = -1;

	char* mark = calloc(g->nb_vert, sizeof(BOOL));
	TEST_PTR_FAIL_FUNC(mark, -ERROR_ALLOCATION_FAILED, );
	mark[r] = 1;  // Marquer r

	// We create a queue an put the root inside
	list_ref_t* waiting_list = create_list(sizeof(int));
	push_back_list(waiting_list, ptr(TYPE_INT, r));

	unsigned index = 0;

	while (!empty_list(waiting_list)) {
		int* vertex;
		pop_front_list(waiting_list, (void**)&vertex);
		tab[index] = *vertex;
		index++;

		node_list_ref_t* node = g->neighbours[*vertex].begin;
		graph_list_edge_t* e = NULL;
		while (node) {
			e = node->p;
			if (!mark[e->to]) {
				mark[e->to] = TRUE;
				if (father)
					father[e->to] = *vertex;
				if (queue_or_stack == STACK)
					push_front_list(waiting_list, ptr(TYPE_INT, e->to));
				else
					push_back_list(waiting_list, ptr(TYPE_INT, e->to));
			}
			node = node->next;
		}
		free(vertex);
	}
	return index;
}

// int DFS_list(graph_list_t* g, unsigned r, int** tab, int** father) {
// 	char* mark;
// 	if (allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
// 		return -1;
// 	mark[r] = 1;  // Marquer r
// 	(*father)[r] = -1;
// 	(*tab)[0] = r;
//
// 	unsigned current = r;
// 	unsigned index = 1;
//
// 	while (TRUE) {
// 		BOOL any_edge = FALSE;
//
// 		node_list_ref_t* node = g->neighbours[current].begin;
// 		graph_list_edge_t* e = NULL;
// 		while (node) {
// 			e = node->p;
// 			if (!mark[e->to]) {
// 				mark[e->to] = 1;
// 				(*tab)[index++] = e->to;
// 				if (father)
// 					(*father)[e->to] = current;
// 				current = e->to;
// 				any_edge = TRUE;
// 				break;
// 			}
// 			node = node->next;
// 		}
// 		if (!any_edge) {
// 			if (current != r)
// 				current = (*father)[current];
// 			else
// 				break;
// 		}
// 	}
// 	return index;
// }

static void graph_list_preorder_dfs_rec(graph_list_t* g,
										unsigned current,
										int* tab,
										int* father,
										char* mark,
										unsigned* index) {
	mark[current] = TRUE;
	tab[*index] = current;
	*index += 1;
	node_list_ref_t* node = g->neighbours[current].begin;
	graph_list_edge_t* e = NULL;
	while (node) {
		e = node->p;
		if (!mark[e->to]) {
			if (father)
				father[e->to] = current;
			graph_list_preorder_dfs_rec(g, e->to, tab, father, mark, index);
		}
		node = node->next;
	}
}

int graph_list_preorder_dfs(graph_list_t* g,
							unsigned r,
							int* tab,
							int* father) {
	TEST_PTR_FAIL_FUNC(tab, -ERROR_INVALID_PARAM3, );
	char* mark = calloc(g->nb_vert, sizeof(BOOL));
	if (father)
		father[r] = -1;
	unsigned index = 0;
	graph_list_preorder_dfs_rec(g, r, tab, father, mark, &index);
	free(mark);
	return index;
}

int graph_list_bfs(graph_list_t* g, unsigned r, int* values, int* father) {
	return mark_and_examine_traversal_list(g, r, values, father, QUEUE);
}

static unsigned father_heap(unsigned i) {
	return i == 0 ? 0 : (i + 1) / 2 - 1;
}

static unsigned ls_heap(unsigned i) {
	return (i + 1) * 2 - 1;
}

static unsigned rs_heap(unsigned i) {
	return (i + 1) * 2;
}

void pullup_heap(unsigned* index, long long* val, unsigned end) {
	unsigned i = end;
	unsigned key = index[end];

	while (i >= 1 && val[key] < val[father_heap(i)]) {
		index[i] = index[father_heap(i)];
		i = father_heap(i);
	}
	index[i] = key;
}

void build_heap(unsigned* index, long long* val, unsigned size) {
	for (unsigned i = 0; i < size; i++)
		pullup_heap(index, val, i);
}

void pulldown_heap(unsigned* index, long long* val, unsigned size, unsigned n) {
	BOOL found = FALSE;
	unsigned key = index[n];
	int i_min;

	while (!found && ls_heap(n) < size) {
		// We compute the son with the lowest value
		if (ls_heap(n) == size - 1)
			i_min = size - 1;
		else if (val[ls_heap(n)] <= val[rs_heap(n)])
			i_min = ls_heap(n);
		else
			i_min = rs_heap(n);

		// If the vertex vertex has a lower value that is lowest son
		if (val[key] < val[i_min]) {
			// We replace the vertex by its node
			index[n] = index[i_min];
			n = i_min;
		} else {
			// Otherwise the vertex is lower than its children and should not go
			// further down
			found = TRUE;
		}
	}
	// We overwrite the last moved son with the original vertex
	index[n] = key;
}

static void swap(unsigned* index, int i, int j) {
	unsigned a = index[i];
	index[i] = index[j];
	index[j] = a;
}

/*
static void heap_sort(unsigned* index, long long*  val, unsigned size) {
		for(int p=size-1; p>0; p--) {
				swap(index, 0, p);
				pulldown_heap(index, val, p, 0);
		}
} */

int graph_list_dijkstra(graph_list_t* g,
						unsigned r,
						graph_weight_t* distance,
						int* father) {
	TEST_PTR_FAIL_FUNC(distance, -ERROR_INVALID_PARAM3, );
	TEST_FAIL_FUNC(r < g->nb_vert, -ERROR_INVALID_PARAM2, );
	for (unsigned i = 0; i < g->nb_vert; i++)
		distance[i] = GRAPH_WEIGHT_INF;
	distance[r] = 0;

	if (father)
		father[r] = -1;

	unsigned* index = malloc(g->nb_vert * sizeof(unsigned int));
	TEST_PTR_FAIL_FUNC(index, -ERROR_ALLOCATION_FAILED, );
	for (unsigned i = 0; i < g->nb_vert; i++)
		index[i] = i;

	unsigned size = g->nb_vert;
	// We put r at the root of (index, distance) which makes it a heap
	swap(index, 0, r);

	// Number of vertices reached by the algorithm
	unsigned number = 0;

	// While there is a vertex left in the heap
	while (size > 0) {
		// Take the root of the heap (which is the non-marked vertex with the
		// lowest distance to the root)
		swap(index, 0, size - 1);
		pulldown_heap(index, distance, --size, 0);
		unsigned pivot = index[size];

		node_list_ref_t* node = g->neighbours[pivot].begin;
		graph_list_edge_t* e = NULL;
		// For each successor of pivot
		while (node != NULL) {
			e = node->p;

			graph_weight_t d =
				weight_add_truncate_overflow(distance[pivot], e->w);
			if (d < distance[e->to]) {
				distance[e->to] = d;
				if (father)
					father[e->to] = pivot;
			}

			node = node->next;
		}
	}

	free(index);

	return number;
}
