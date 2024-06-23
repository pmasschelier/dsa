#include "graph_mat.h"
#include <stdlib.h>
#include "list_ref/list_ref.h"

const long long int INFINITY = LLONG_MAX;

GRAPH_MAT* init_graph_mat(unsigned size) {
	if (size == 0)
		return NULL;
	GRAPH_MAT* g = malloc(sizeof(GRAPH_MAT));
	if (!g)
		return NULL;
	g->nb_vert = size;

	EDGE_MAT* e = calloc(size * size, sizeof(EDGE_MAT));
	if (!e)
		return NULL;
	g->mat = malloc(size * sizeof(EDGE_MAT));
	for (unsigned i = 0; i < size; i++)
		g->mat[i] = e + i * size;

	return g;
}

void free_graph_mat(GRAPH_MAT* g) {
	free(*g->mat);
	free(g->mat);
	free(g);
}

void set_edge_mat(GRAPH_MAT* g,
				  unsigned int a,
				  unsigned int b,
				  BOOL val,
				  long long weight,
				  BOOL reverse) {
	g->mat[a][b] = (EDGE_MAT){weight, val};
	if (reverse)
		g->mat[b][a] = (EDGE_MAT){weight, val};
}

static int allow_tab_father_mark(unsigned length,
								 int** tab,
								 int** father,
								 char** mark) {
	if (tab) {
		*tab = malloc(length * sizeof(int));
		if (!*tab)
			return -1;
	}
	if (father) {
		*father = malloc(length * sizeof(int));
		if (!*father && tab) {
			free(*tab);
			return -1;
		}
	}
	*mark = calloc(length, sizeof(char));
	if (!*mark) {
		if (tab)
			free(*tab);
		if (father)
			free(*father);
		return -1;
	}
	return 0;
}

int mark_and_examine_traversal_mat(GRAPH_MAT* g,
								   unsigned r,
								   int** tab,
								   int** father,
								   LIST_STRUCT queue_or_stack) {
	char* mark;
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
			if (g->mat[*vertex][i].b && !mark[i]) {
				mark[i] = 1;
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

int DFS_mat(GRAPH_MAT* g, unsigned r, int** tab, int** father) {
	char* mark;
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
			if (g->mat[current][i].b && !mark[i]) {
				mark[i] = 1;
				(*tab)[index++] = i;
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

static void DFS_mat_recursive_rec(GRAPH_MAT* g,
								  unsigned current,
								  int** tab,
								  int** father,
								  char* mark,
								  unsigned* index) {
	mark[current] = 1;
	(*tab)[*index] = current;
	*index += 1;
	for (unsigned i = 0; i < g->nb_vert; i++) {
		if (g->mat[current][i].b && !mark[i]) {
			(*father)[i] = current;
			DFS_mat_recursive_rec(g, i, tab, father, mark, index);
		}
	}
}

int DFS_mat_recursive(GRAPH_MAT* g, unsigned r, int** tab, int** father) {
	char* mark;
	if (allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	(*father)[r] = -1;
	unsigned index = 0;
	DFS_mat_recursive_rec(g, r, tab, father, mark, &index);
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
		if (!*distance) {
			free(*father);
			return -1;
		}
		for (unsigned i = 0; i < length; i++)
			(*distance)[i] = INFINITY;
	}
	if (mark) {
		*mark = calloc(length, sizeof(BOOL));
		if (!*mark) {
			free(*father);
			free(*distance);
			return -1;
		}
	}
	return 0;
}

int Dijkstra_mat(GRAPH_MAT* g, unsigned r, long long** distance, int** father) {
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
				g->mat[pivot][j]
					.b) {  // non encore dans A et successeur de pivot
				const long long d =
					(*distance)[pivot] > INFINITY - g->mat[pivot][j].w
						? INFINITY
						: (*distance)[pivot] + g->mat[pivot][j].w;
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

int topological_numbering_mat(GRAPH_MAT* g, unsigned** num, unsigned** denum) {
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
			if (g->mat[i][j].b)
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
			if (g->mat[t][*s].b)
				if (--degre[t] == 0)
					push_front_list(pile, ptr(TYPE_INT, t));
		}
		free(s);
	}
	return number + 1;
}

int Bellman_mat(GRAPH_MAT* g, unsigned r, long long** distance, int** father) {
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
			long long d = (*distance)[j] > INFINITY - g->mat[j][x].w
							  ? INFINITY
							  : (*distance)[j] + g->mat[j][x].w;
			if (g->mat[j][x].b && num[j] < i && d < min) {
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
