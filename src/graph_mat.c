#include "graph_mat.h"
#include <stdlib.h>
#include "list_ref/list_ref.h"
#include "malloc_fail_macro.h"

const long long int INFINITY = LLONG_MAX;

graph_mat_t* create_graph_mat(unsigned size, BOOL has_weights) {
	if (size == 0)
		return NULL;
	graph_mat_t* g = malloc(sizeof(graph_mat_t));
	MALLOC_FAIL_TEST_FUNC(g, NULL, );
	g->nb_vert = size;

	g->edges = calloc(size * size, sizeof(BOOL));
	if (!g->edges)
		goto error_alloc;
	if (has_weights) {
		g->weights = calloc(size * size, sizeof(long long));
		if (!g->weights)
			goto error_alloc2;
	} else
		g->weights = NULL;
	return g;
error_alloc2:
	free(g->edges);
error_alloc:
	free(g);
	return NULL;
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
						long long weight,
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

BOOL graph_mat_get_weight(graph_mat_t* g, unsigned int a, unsigned b) {
	return g->weights[a * g->nb_vert + b];
}

static int allow_tab_father_mark(unsigned length,
								 int** tab,
								 int** father,
								 BOOL** mark) {
	if (tab) {
		*tab = malloc(length * sizeof(int));
		if (!*tab)
			return -1;
	}
	if (father) {
		*father = malloc(length * sizeof(int));
		if (!*father)
			goto error_alloc;
	}
	*mark = calloc(length, sizeof(BOOL));
	if (!*mark)
		goto error_alloc2;
	return 0;
error_alloc2:
	if (father)
		free(*father);
error_alloc:
	if (tab)
		free(*tab);
	return -1;
}

int mark_and_examine_traversal_mat(graph_mat_t* g,
								   unsigned r,
								   int** tab,
								   int** father,
								   LIST_STRUCT queue_or_stack) {
	BOOL* mark;
	if (allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1;  // Marquer r
	(*father)[r] = -1;

	list_ref_t* waiting_list = create_list(sizeof(int));  // Création d'une file
	push_back_list(waiting_list,
				   ptr(TYPE_INT, r));  // Ajouter r à la liste d'attente

	unsigned index = 0;

	while (!empty_list(waiting_list)) {
		int* vertex;
		pop_front_list(waiting_list, (void**)&vertex);
		if (tab)
			(*tab)[index] = *vertex;
		index++;

		for (unsigned i = 0; i < g->nb_vert; i++) {
			if (graph_mat_get_edge(g, *vertex, i) && !mark[i]) {
				mark[i] = TRUE;
				if (father)
					(*father)[i] = *vertex;
				if (queue_or_stack == STACK)
					push_front_list(waiting_list, ptr(TYPE_INT, i));
				else
					push_back_list(waiting_list, ptr(TYPE_INT, i));
			}
		}
		free(vertex);
	}
	return index;
}

int DFS_mat(graph_mat_t* g, unsigned r, int** tab, int** father) {
	BOOL* mark;
	if (allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1;  // Marquer r
	(*father)[r] = -1;
	(*tab)[0] = r;

	unsigned current = r;
	unsigned index = 1;

	while (TRUE) {
		BOOL any_edge = FALSE;
		for (unsigned i = 0; i < g->nb_vert; i++) {
			if (graph_mat_get_edge(g, current, i) && !mark[i]) {
				mark[i] = 1;
				if (tab)
					(*tab)[index] = i;
				index++;
				if (father)
					(*father)[i] = current;
				current = i;
				any_edge = TRUE;
				break;
			}
		}
		if (!any_edge) {
			if (current != r)
				current = (*father)[current];
			else
				break;
		}
	}
	return index;
}

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
	if (!mark)
		return -1;
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
			father[i] = current;
		}
	}
	values[*index] = current;
	*index += 1;
}

int graph_mat_postorder_dfs(graph_mat_t* g,
							unsigned r,
							int* values,
							int* father) {
	BOOL* mark = calloc(g->nb_vert, sizeof(BOOL));
	if (!mark)
		return -1;
	father[r] = -1;
	unsigned index = 0;
	graph_mat_postorder_dfs_rec(g, r, values, father, mark, &index);
	free(mark);
	return index;
}

static int allow_father_distance_mark(unsigned length,
									  int** father,
									  long long** distance,
									  BOOL** mark) {
	if (father) {
		*father = malloc(sizeof(int[length]));
		if (!*father)
			return -1;
	}
	if (distance) {
		*distance = malloc(sizeof(long long int[length]));
		if (!*distance)
			goto error_alloc;
		for (unsigned i = 0; i < length; i++)
			(*distance)[i] = INFINITY;
	}
	if (mark) {
		*mark = calloc(length, sizeof(BOOL));
		if (!*mark)
			goto error_alloc2;
	}
	return 0;
error_alloc2:
	if (distance)
		free(*distance);
error_alloc:
	if (father)
		free(*father);
	return -1;
}

int Dijkstra_mat(graph_mat_t* g,
				 unsigned r,
				 long long** distance,
				 int** father) {
	if (!distance)
		return -1;
	BOOL* mark;
	if (allow_father_distance_mark(g->nb_vert, father, distance, &mark) != 0)
		return -1;
	mark[r] = TRUE;
	(*distance)[r] = 0;
	if (father)
		(*father)[r] = -1;

	unsigned pivot = r;
	unsigned number = 0;  // nombre de sommets atteints par l'algorithme

	for (unsigned i = 0; i < g->nb_vert - 1; i++) {
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // Pour tout sommet j
			if (!mark[j] &&
				graph_mat_get_edge(
					g, pivot, j)) {	 // non encore dans A et successeur de pivot
				const long long d =
					(*distance)[pivot] >
							INFINITY - graph_mat_get_weight(g, pivot, j)
						? INFINITY
						: (*distance)[pivot] +
							  graph_mat_get_weight(g, pivot, j);
				if (d < (*distance)[j]) {
					(*distance)[j] = d;
					if (father)
						(*father)[j] = pivot;
					number++;
				}
			}
		}
		long long min = INFINITY;
		int jmin = -1;
		for (unsigned j = 0; j < g->nb_vert; j++) {	 // Pour tout sommet j
			if (!mark[j] && (*distance)[j] >= 0 &&
				(*distance)[j] < min) {	 // non encore dans A
				min = (*distance)[j];
				jmin = j;
			}
		}
		if (jmin >= 0) {
			pivot = jmin;
			mark[pivot] = TRUE;
		}
	}

	return number;
}

int topological_numbering_mat(graph_mat_t* g,
							  unsigned** num,
							  unsigned** denum) {
	if (g->nb_vert <= 0)
		return -1;
	if (num) {
		*num = malloc(sizeof(unsigned[g->nb_vert]));
		if (!*num)
			return -2;
	} else
		return -1;
	if (denum) {
		*denum = malloc(sizeof(unsigned[g->nb_vert]));
		if (!*denum) {
			free(num);
			return -2;
		}
	}

	int number = g->nb_vert - 1;

	list_ref_t* pile = create_list(sizeof(unsigned));

	unsigned degre[g->nb_vert];
	for (unsigned i = 0; i < g->nb_vert; i++) {
		degre[i] = 0;
		for (unsigned j = 0; j < g->nb_vert; j++) {
			if (graph_mat_get_edge(g, i, j))
				degre[i]++;	 // On calcule le degré extérieur du sommet i
		}
		if (degre[i] == 0)
			push_front_list(pile, ptr(TYPE_INT, i));
	}
	while (!empty_list(pile)) {
		unsigned* s = NULL;
		pop_front_list(pile, (void**)&s);
		(*num)[*s] = number;
		if (denum)
			(*denum)[number] = *s;
		number--;
		for (unsigned t = 0; t < g->nb_vert; t++) {
			if (graph_mat_get_edge(g, t, *s))
				if (--degre[t] == 0)
					push_front_list(pile, ptr(TYPE_INT, t));
		}
		free(s);
	}
	return number + 1;
}

int Bellman_mat(graph_mat_t* g,
				unsigned r,
				long long** distance,
				int** father) {
	if (!distance)
		return -1;
	if (allow_father_distance_mark(g->nb_vert, father, distance, NULL) != 0)
		return -1;
	(*distance)[r] = 0;
	(*father)[r] = -1;

	unsigned *num, *denum;
	if (topological_numbering_mat(g, &num, &denum) != 0)
		return -1;
	for (unsigned i = num[r] + 1; i < g->nb_vert; i++) {
		long long min = INFINITY;
		unsigned x = denum[i];
		int y = -1;
		for (unsigned j = 0; j < g->nb_vert; j++) {
			long long d =
				(*distance)[j] > INFINITY - graph_mat_get_weight(g, j, x)
					? INFINITY
					: (*distance)[j] + graph_mat_get_weight(g, j, x);
			if (graph_mat_get_weight(g, j, x) && num[j] < i && d < min) {
				min = d;
				y = j;
			}
		}
		if (y >= 0) {
			(*distance)[x] = min;
			(*father)[x] = y;
		}
	}

	return 0;
}
