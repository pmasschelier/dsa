#include "graph.h"

GRAPH_MAT* init_graph_mat(unsigned size)
{
	if(size == 0)
		return NULL;
	GRAPH_MAT* g = malloc(sizeof(GRAPH_MAT));	
	if(!g)
		return NULL;
	g->nb_vert = size;
	
	EDGE_MAT* e = calloc(size * size, sizeof(EDGE_MAT));
	if(!e)
		return NULL;
	g->mat = malloc(size * sizeof(EDGE_MAT));
	for(unsigned i = 0; i < size; i++)
		g->mat[i] = e + i*size;
	
	return g;
}

void free_graph_mat(GRAPH_MAT* g)
{
	free(*g->mat);
	free(g->mat);
	free(g);
}

void set_edge_mat(GRAPH_MAT* g,  unsigned int a, unsigned int b, BOOL val, BOOL reverse)
{
	g->mat[a][b].b = val;
	if(reverse)
		g->mat[b][a].b = val;
}

void set_edge_mat_weight(GRAPH_MAT* g,  unsigned int a, unsigned int b, int weight, BOOL reverse)
{
	g->mat[a][b].w = weight;
	if(reverse)
		g->mat[b][a].w = weight;
}

static int allow_tab_father_mark(unsigned length, int** tab, int** father, char** mark) {
	if(tab) {
		*tab = malloc(sizeof(int[length]));
		if(!*tab)
			return -1;
	}
	if(father) {
		*father = malloc(sizeof(int[length]));
		if(!*father && tab) {
			free(*tab);
			return -1;
		}
	}
	*mark = calloc(length, sizeof(char));
	if(!*mark) {
		if(tab) free(*tab);
		if(father) free(*father);
		return -1;
	}
	return 0;
}

int mark_and_examine_traversal_mat(GRAPH_MAT* g, unsigned r, int** tab, int** father, char queue_or_stack)
{
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1; // Marquer r
	(*father)[r] = -1;
	
	LIST* waiting_list = create_list(sizeof(int)); // Création d'une file
	push_back_list(waiting_list, ptr(TYPE_INT, r)); // Ajouter r à la liste d'attente
	
	unsigned index = 0;
	
	while(!empty_list(waiting_list))
	{
		int* vertex = pop_front_list(waiting_list);
		if(tab)
			(*tab)[index] = *vertex;
		index++;
		
		for(unsigned i=0; i<g->nb_vert; i++)
		{
			if(g->mat[*vertex][i].b && !mark[i]) {
				mark[i] = 1;
				if(father)
					(*father)[i] = *vertex;
				if(queue_or_stack == STACK)
					push_front_list(waiting_list, ptr(TYPE_INT, i));
				else
					push_back_list(waiting_list, ptr(TYPE_INT, i));
			}
		}
		free(vertex);
	}
	return index;
}

int DFS_mat(GRAPH_MAT* g, unsigned r, int** tab, int** father) 
{
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	mark[r] = 1; // Marquer r
	(*father)[r] = -1;
	(*tab)[0] = r;
	
	unsigned current = r;
	unsigned index = 1;
	
	while(TRUE) {
		BOOL any_edge = FALSE;
		for(unsigned i=0; i<g->nb_vert; i++) {
			if(g->mat[current][i].b && !mark[i]) {
				mark[i] = 1;
				(*tab)[index++] = i;
				if(father)
					(*father)[i] = current;
				current = i;
				any_edge = TRUE;
				break;
			}
		}
		if(!any_edge) {
			if(current != r)
				current = (*father)[current];
			else
				break;
		}
	}
	return index;
}
static void DFS_mat_recursive_rec(GRAPH_MAT* g, unsigned current, int** tab, int** father, char* mark, unsigned* index)
{
	mark[current] = 1;
	(*tab)[*index] = current;
	*index += 1;
	for(unsigned i=0; i<g->nb_vert; i++) {
		if(g->mat[current][i].b && !mark[i]) {
			(*father)[i] = current;
			DFS_mat_recursive_rec(g, i, tab, father, mark, index);
		}
	}
}

int DFS_mat_recursive(GRAPH_MAT* g, unsigned r, int** tab, int** father) {
	char* mark;
	if(allow_tab_father_mark(g->nb_vert, tab, father, &mark) != 0)
		return -1;
	(*father)[r] = -1;
	unsigned index = 0;
	DFS_mat_recursive_rec(g, r, tab, father, mark, &index);
	return index;
}

void Dijkstra_mat(GRAPH_MAT* g, unsigned r, int** father, int** distance)
{
	int* A = malloc(sizeof(int[g->nb_vert]));
	for(unsigned i=0; i < g->nb_vert; i++) 
		A[i] = 0;
	A[r] = 1;
	int* Pi = malloc(sizeof(int[g->nb_vert]));
	for(unsigned i=0; i < g->nb_vert; i++)
		Pi[i] = -1;
	Pi[r] = 0;
	int* f = malloc(sizeof(int[g->nb_vert]));
	f[r] = -1;
	
	unsigned pivot = r;
	
	for(unsigned i=0; i < g->nb_vert-1; i++) {
		for(unsigned j=0; j < g->nb_vert; j++) { // Pour tout sommet j
			if(!A[j] && g->mat[pivot][j].b) { // non encore dans A et successeur de pivot
				int d = Pi[pivot] + g->mat[pivot][j].w;
				if(Pi[j] == -1 || d < Pi[j]) {
					Pi[j] = d;
					f[j] = pivot;
				}
			}
		}
		int min = -1;
		int jmin = -1;
		for(unsigned j=0; j < g->nb_vert; j++) { // Pour tout sommet j
			if(!A[j] && Pi[j] >=0 && (min == -1 || Pi[j] < min)) { // non encore dans A
				min = Pi[j];
				jmin = j;
			}
		}
		if(jmin >= 0) {
			pivot = jmin;
			A[pivot] = 1;
		}
	}
	*father = f;
	*distance = Pi;
}

/*

static unsigned father_heap(unsigned i) {
	return i==0 ? 0 : (i+1)/2 - 1;
}

static unsigned ls_heap(unsigned i) {
	return (i+1)*2-1;
}

static unsigned rs_heap(unsigned i) {
	return (i+1)*2;
}

static void pull_up(int* tas, unsigned fin) {
	unsigned i = fin;
	int cle = tas[fin];
	
	while(i>=1 && cle > tas[parent_tas(i)]) {
		tas[i] = tas[parent_tas(i)];
		i = parent_tas(i);
	}
	tas[i] = cle;
}


void construire_tas_tableau(int* tas, unsigned taille) {
	for(unsigned i=0; i<taille; i++)
		montee(tas, i);
}

void pull_down(int* tas, unsigned taille, unsigned index) {
	int trouvee = 0;
	int cle = tas[index];
	int i_max;
	while(!trouvee && fg_tas(index) < taille) {
		// printf("indice : %d, fg : %d, fd : %d\n", index, fg_tas(index), fd_tas(index));
		if(fg_tas(index) == taille-1)
			i_max = taille-1;
		else if(tas[fg_tas(index)] >= tas[fd_tas(index)])
			i_max = fg_tas(index);
		else
			i_max = fd_tas(index);
		
		if(cle < tas[i_max]) {
			tas[index] = tas[i_max];
			index = i_max;
		}
		else
			trouvee = 1;
	}
	tas[index] = cle;
}

void tri_tas(int* tas, unsigned taille) {
	for(int p=taille-1; p>0; p--) {
		echanger(tas, 0, p);
		descente(tas, p, 0);
	}
}

*/
