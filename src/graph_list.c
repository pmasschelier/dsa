#include "graph_list.h"
#include "graph_mat.h"
#include "list_ref/list_ref.h"

GRAPH_LIST* init_graph_list(unsigned size) {
	if (size == 0)
		return NULL;
	GRAPH_LIST* g = malloc(sizeof(GRAPH_LIST));
	if (!g)
		return NULL;
	g->nb_vert = size;
	g->neighbours = malloc(size * sizeof(list_ref_t));
	if (!g->neighbours)
		return NULL;
	for (unsigned i = 0; i < size; i++) {
		g->neighbours[i].begin = NULL;
		g->neighbours[i].end = NULL;
		g->neighbours[i].size = sizeof(EDGE_LIST);
		g->neighbours[i].free_element = free;
	}

	return g;
}

static int add_edge_list_noverif(GRAPH_LIST* g,
								 unsigned int a,
								 unsigned int b,
								 long long weight) {
	list_ref_t* neighbours = &g->neighbours[a];
	EDGE_LIST* e = malloc(sizeof(EDGE_LIST));
	if (!e)
		return -1;
	*e = (EDGE_LIST){weight, b};
	push_back_list(neighbours, e);

	return 0;
}

void set_edge_list(GRAPH_LIST* g,
				   unsigned int a,
				   unsigned int b,
				   BOOL val,
				   long long weight,
				   BOOL reverse) {
	list_ref_t* neighbours = &g->neighbours[a];
	node_list_ref_t* node = neighbours->begin;

	EDGE_LIST* e = NULL;
	while (node) {
		e = node->p;
		if (e->p == b)
			break;
		node = node->next;
	}
	if (node && !val)
		remove_list(neighbours, node, NULL);
	if (node && !val)
		e->w = weight;
	if (!node && val) {
		add_edge_list_noverif(g, a, b, weight);
	}
	if (reverse)
		set_edge_list(g, b, a, val, weight, FALSE);
}

GRAPH_LIST* graph_mat_to_graph_list(graph_mat_t* graph_mat,
									GRAPH_LIST** graph_list) {
	if (!graph_list) {
		graph_list = malloc(sizeof(GRAPH_LIST*));
		*graph_list = NULL;
	}

	free_graph_list(*graph_list);
	const size_t size = graph_mat->nb_vert;
	*graph_list = init_graph_list(size);

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

void free_graph_list(GRAPH_LIST* g) {
	if (g) {
		for (unsigned i = 0; i < g->nb_vert; i++)
			clean_list(&g->neighbours[i]);
		free(g->neighbours);
		free(g);
	}
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

int mark_and_examine_traversal_list(GRAPH_LIST* g,
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

		node_list_ref_t* node = g->neighbours[*vertex].begin;
		EDGE_LIST* e = NULL;
		while (node) {
			e = node->p;
			if (!mark[e->p]) {
				mark[e->p] = TRUE;
				if (father)
					(*father)[e->p] = *vertex;
				if (queue_or_stack == STACK)
					push_front_list(waiting_list, ptr(TYPE_INT, e->p));
				else
					push_back_list(waiting_list, ptr(TYPE_INT, e->p));
			}
			node = node->next;
		}
		free(vertex);
	}
	return index;
}

int DFS_list(GRAPH_LIST* g, unsigned r, int** tab, int** father) {
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

		node_list_ref_t* node = g->neighbours[current].begin;
		EDGE_LIST* e = NULL;
		while (node) {
			e = node->p;
			if (!mark[e->p]) {
				mark[e->p] = 1;
				(*tab)[index++] = e->p;
				if (father)
					(*father)[e->p] = current;
				current = e->p;
				any_edge = TRUE;
				break;
			}
			node = node->next;
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

static void DFS_list_recursive_rec(GRAPH_LIST* g,
								   unsigned current,
								   int** tab,
								   int** father,
								   char* mark,
								   unsigned* index) {
	mark[current] = 1;
	(*tab)[*index] = current;
	*index += 1;
	node_list_ref_t* node = g->neighbours[current].begin;
	EDGE_LIST* e = NULL;
	while (node) {
		if (!mark[e->p]) {
			(*father)[e->p] = current;
			DFS_list_recursive_rec(g, e->p, tab, father, mark, index);
		}
	}
}

int DFS_list_recursive(GRAPH_LIST* g, unsigned r, int** tab, int** father) {
	char* mark;
	if (allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	(*father)[r] = -1;
	unsigned index = 0;
	DFS_list_recursive_rec(g, r, tab, father, mark, &index);
	return index;
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
		if (ls_heap(n) == size - 1)
			i_min = size - 1;
		else if (val[ls_heap(n)] <= val[rs_heap(n)])
			i_min = ls_heap(n);
		else
			i_min = rs_heap(n);

		if (val[key] < val[i_min]) {
			index[n] = index[i_min];
			n = i_min;
		} else
			found = TRUE;
	}
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

static int allow_father_index_distance_mark(unsigned length,
											int** father,
											unsigned** index,
											long long** distance,
											BOOL** mark) {
	if (father) {
		*father = malloc(sizeof(int[length]));
		if (!*father)
			return -1;
	}
	if (index) {
		*index = malloc(sizeof(unsigned[length]));
		if (!*index) {
			free(*father);
			return -1;
		}
		for (unsigned i = 0; i < length; i++)
			(*index)[i] = i;
	}
	if (distance) {
		*distance = malloc(sizeof(long long[length]));
		if (!*distance) {
			free(*index);
			free(*father);
			return -1;
		}
		for (unsigned i = 0; i < length; i++)
			(*distance)[i] = STRUCT_DIST_INF;
	}
	if (mark) {
		*mark = calloc(length, sizeof(BOOL));
		if (!*mark) {
			free(*father);
			free(*index);
			free(*distance);
			return -1;
		}
	}
	return 0;
}

int Dijkstra_list(GRAPH_LIST* g,
				  unsigned r,
				  long long** distance,
				  int** father) {
	if (!distance)
		return -1;
	BOOL* mark;
	unsigned* index;
	if (allow_father_index_distance_mark(g->nb_vert, father, &index, distance,
										 &mark) != 0)
		return -1;
	mark[r] = TRUE;
	(*distance)[r] = 0;
	if (father)
		(*father)[r] = -1;

	unsigned size = g->nb_vert;
	swap(index, 0,
		 r);  // On place r en tête de (index, distance), ce qui en fait un tas

	unsigned pivot = r;
	unsigned number = 0;  // nombre de sommets atteints par l'algorithme

	while (size >
		   0) {	 // Tant qu'il existe un sommet non marqué (donc dans le tas)
		swap(index, 0, size - 1);
		pulldown_heap(index, *distance, --size,
					  0);		   // Choisir un sommet de distance minimale
		mark[index[size]] = TRUE;  // Marquer ce sommet

		node_list_ref_t* node = g->neighbours[index[size - 1]].begin;
		EDGE_LIST* e = NULL;
		while (node != NULL) {	// Pour tout successeur de pivot
			e = node->p;

			int d = (*distance)[pivot] > STRUCT_DIST_INF - e->w
						? STRUCT_DIST_INF
						: (*distance)[pivot] + e->w;
			if (d < (*distance)[e->p]) {
				(*distance)[e->p] = d;
				if (father)
					(*father)[e->p] = pivot;
				number++;
			}

			node = node->next;
		}
	}

	return number;
}
