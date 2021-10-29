#include <stdio.h>
#include <assert.h>
#include "list.h"
#include "binarytree.h"
#include "graph_mat.h"

static void print_path(Path path);
static void print_edges(GRAPH_MAT* g);

int main() {
	LIST* liste = list_from_tab((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, sizeof(int), 10);
	
	free(pop_front_list(liste));
	push_front_list(liste, ptr(TYPE_INT, 30));
	free(pop_back_list(liste));
	push_back_list(liste, ptr(TYPE_INT, 100));
	
	LIST_NODE* parcours = liste->begin;
	while (parcours != NULL) {
		int* x = parcours->p;
		printf("%d\n", *x);
		parcours = parcours->next;
	}
	
	free_list(liste);
	
	Path p = path(10);
	print_path(p);
	
	BinaryTree* tree = perfect_BT_from_tab((T[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
	const unsigned n = size_BT(tree);
	T tab[n];
	int ret = level_order_traversal(tree, tab);
	if(ret) {
		fprintf(stderr, "Erreur lors du parcours en largeur de l'arbre binaire\n");
	}
	else {
		puts("\n\nParcours en largeur de l'arbre binaire parfait : ");
		for(unsigned i=0; i<n; i++) {
			printf("%d, ", tab[i]);
		}
		printf("\n");
	}
	
	free_BT(tree);
	
	LIST* forest = create_list(sizeof(BinaryTree));
	forest->free_element = free_BT;
	
	for(unsigned i=0; i<3; i++)
		push_back_list(forest, perfect_BT_from_tab((T[]){3*i, 3*i+1, 3*i+2}, 3));
	T contenu[3];
	
	LIST_NODE* node = forest->begin;
	while(node) {
		BinaryTree* p = node->p;
		preorder_traversal(p, contenu);
		for(int i=0; i<3; i++) printf("%d, ", contenu[i]);
		printf("\n");
		node = node->next;
	}
	
	free_list(forest); // Libère tout le contenu de la liste
	
	
	GRAPH_MAT* g = init_graph_mat(4);
	set_edge_mat(g, 0, 1, TRUE, 2, TRUE);
	set_edge_mat(g, 1, 2, TRUE, 3, TRUE);
	set_edge_mat(g, 1, 3, TRUE, 10, TRUE);
	set_edge_mat(g, 2, 3, TRUE, 4, TRUE);
	
	int *father;
	long long *distance;
	Dijkstra_mat(g, 0, &distance, &father);
	
	// Devrait afficher dans l'ordre 0, 2, 5, 9
	for(unsigned i=0; i<4; i++)
		printf("Distance de 0 à %d : %lld\n", i, distance[i]); 
	free(father);
	free(distance);
	free_graph_mat(g);
	
	g = init_graph_mat(8);
	set_edge_mat(g, 0, 1, TRUE, 3, FALSE);
	set_edge_mat(g, 0, 6, TRUE, 5, FALSE);
	set_edge_mat(g, 1, 5, TRUE, 1, FALSE);
	set_edge_mat(g, 5, 7, TRUE, 0, FALSE);
	set_edge_mat(g, 5, 4, TRUE, 5, FALSE);
	set_edge_mat(g, 7, 4, TRUE, 2, FALSE);
	set_edge_mat(g, 3, 4, TRUE, 9, FALSE);
	set_edge_mat(g, 6, 7, TRUE, 2, FALSE);
	set_edge_mat(g, 7, 2, TRUE, 8, FALSE);
	set_edge_mat(g, 2, 6, TRUE, 0, FALSE);
	print_edges(g);
	int* vertices;
	int nb = mark_and_examine_traversal_mat(g, 0, &vertices, &father, QUEUE);
	printf("Parcours BFS du graphe\n");
	for(int i=0; i<nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a', (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);
	
	nb = DFS_mat(g, 0, &vertices, &father);
	printf("Parcours DFS du graphe\n");
	for(int i=0; i<nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a', (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);
	
	set_edge_mat(g, 2, 6, FALSE, 0, FALSE); // On transforme g en DAG
	unsigned *num, *denum;
	ret = topological_numbering_mat(g, &num, &denum);
	if(ret == 0) {
		puts("La numérotation topologique du graphe a bien été effectuée :");
		for(unsigned i=0; i<g->nb_vert; i++) {
			printf("%d, étiquette : %d\n", i, num[i]);
			assert(i == denum[num[i]]);
		}
		
		ret = Bellman_mat(g, 0, &distance, &father);
		if(ret == 0) {
			for(unsigned i=0; i<g->nb_vert; i++) {
				if(distance[i] < INFINITY)
					printf("Distance de 0 à %d : %lld\n", i, distance[i]);
			}
		}
		else
			fprintf(stderr, "Erreur dans l'algorithme de Bellman\n");
	}
	else if(ret > 0)
		fprintf(stderr, "Le graphe n'est pas un DAG");
	else
		fprintf(stderr, "Code d'erreur : %d\n", ret);
	
	free(father);
	free(distance);
	free_graph_mat(g);
	
	
	return 0;
}

static void print_path(Path path) {
	if(path.length == 0) {
		printf("0");
	}
	else {
		while(path.length) {
			printf("%d", path.path & 1);
			path.path >>= 1;
			path.length--;
		}
	}
}

static void print_edges(GRAPH_MAT* g) {
	for(unsigned i=0; i<g->nb_vert; i++) {
		for(unsigned j=0; j<g->nb_vert; j++) {
			if(g->mat[i][j].b)
				printf("%c -> %c\n" , (char)i+'a', (char)j+'a');
		}
	}
}
