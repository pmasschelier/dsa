#include <assert.h>
#include <stdio.h>
#include "btree_ref/btree_ref.h"
#include "graph/graph_list.h"
#include "graph/graph_mat.h"
#include "list_ref/list_ref.h"

static void print_path(btree_path_t path);
static void print_edges_mat(graph_mat_t* g);
static void print_edges_list(GRAPH_LIST* g);

int main(void) {
	list_ref_t* liste =
		list_from_tab((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, sizeof(int), 10);
	pop_front_list(liste, NULL);
	push_front_list(liste, ptr(TYPE_INT, 30));
	pop_back_list(liste, NULL);
	push_back_list(liste, ptr(TYPE_INT, 100));

	node_list_ref_t* parcours = liste->begin;
	while (parcours != NULL) {
		int* x = parcours->p;
		printf("%d\n", *x);
		parcours = parcours->next;
	}

	free_list(liste);

	btree_path_t p = btree_node_to_path(10);
	print_path(p);

	btree_ref_t* tree = btree_perfect_tree_from_tab(
		(T[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, sizeof(T), 10);
	const unsigned n = btree_length(tree);
	T tab[n];
	int ret = btree_levelorder_traversal(tree, (void*)tab);
	if (ret) {
		fprintf(stderr,
				"Erreur lors du parcours en largeur de l'arbre binaire\n");
	} else {
		puts("\n\nParcours en largeur de l'arbre binaire parfait : ");
		for (unsigned i = 0; i < n; i++) {
			printf("%d, ", tab[i]);
		}
		printf("\n");
	}

	btree_free(tree);

	list_ref_t* forest = create_list(sizeof(node_btree_ref_t));
	forest->free_element = (free_element_fn_t)btree_free;

	for (int i = 0; i < 3; i++)
		push_back_list(forest, btree_perfect_tree_from_tab(
								   (T[]){3 * i, 3 * i + 1, 3 * i + 2}, 3));
	T contenu[3];

	node_list_ref_t* node = forest->begin;
	while (node) {
		node_btree_ref_t* p = node->p;
		btree_preorder_traversal(p, contenu);
		for (int i = 0; i < 3; i++)
			printf("%d, ", contenu[i]);
		printf("\n");
		node = node->next;
	}

	free_list(forest);	// Libère tout le contenu de la liste

	graph_mat_t* g = create_graph_mat(4);
	set_edge_mat(g, 0, 1, TRUE, 2, TRUE);
	set_edge_mat(g, 1, 2, TRUE, 3, TRUE);
	set_edge_mat(g, 1, 3, TRUE, 10, TRUE);
	set_edge_mat(g, 2, 3, TRUE, 4, TRUE);

	int* father;
	long long* distance;
	Dijkstra_mat(g, 0, &distance, &father);

	// Devrait afficher dans l'ordre 0, 2, 5, 9
	for (unsigned i = 0; i < 4; i++)
		printf("Distance de 0 à %d : %lld\n", i, distance[i]);
	free(father);
	free(distance);
	free_graph_mat(g);

	g = create_graph_mat(8);
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
	print_edges_mat(g);

	int* vertices;
	int nb = mark_and_examine_traversal_mat(g, 0, &vertices, &father, QUEUE);
	printf("Parcours BFS du graphe\n");
	for (int i = 0; i < nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a',
			   (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);

	nb = DFS_mat(g, 0, &vertices, &father);
	printf("Parcours DFS du graphe\n");
	for (int i = 0; i < nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a',
			   (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);

	puts(
		"\n# On transforme le graphe représenté par une matrice d'adjacence en "
		"un graphe représenté par des listes d'adjacence :");

	GRAPH_LIST* g_list = NULL;
	graph_mat_to_graph_list(g, &g_list);
	print_edges_list(g_list);

	nb = mark_and_examine_traversal_list(g_list, 0, &vertices, &father, QUEUE);
	printf("Parcours BFS du graphe\n");
	for (int i = 0; i < nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a',
			   (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);

	nb = DFS_list(g_list, 0, &vertices, &father);
	printf("Parcours DFS du graphe\n");
	for (int i = 0; i < nb; i++)
		printf("%c, père : %c\n", (char)vertices[i] + 'a',
			   (char)father[vertices[i]] + 'a');
	free(father);
	free(vertices);

	set_edge_mat(g, 2, 6, FALSE, 0, FALSE);	 // On transforme g en DAG
	unsigned *num, *denum;
	ret = topological_numbering_mat(g, &num, &denum);
	if (ret == 0) {
		puts("La numérotation topologique du graphe a bien été effectuée :");
		for (unsigned i = 0; i < g->nb_vert; i++) {
			printf("%d, étiquette : %d\n", i, num[i]);
			assert(i == denum[num[i]]);
		}

		ret = Bellman_mat(g, 0, &distance, &father);
		if (ret == 0) {
			for (unsigned i = 0; i < g->nb_vert; i++) {
				if (distance[i] < INFINITY)
					printf("Distance de 0 à %d : %lld\n", i, distance[i]);
			}
		} else
			fprintf(stderr, "Erreur dans l'algorithme de Bellman\n");
	} else if (ret > 0)
		fprintf(stderr, "Le graphe n'est pas un DAG");
	else
		fprintf(stderr, "Code d'erreur : %d\n", ret);

	free(father);
	free(distance);
	free_graph_mat(g);

	return 0;
}

static void print_path(btree_path_t path) {
	if (path.length == 0) {
		printf("0");
	} else {
		while (path.length) {
			printf("%d", path.path & 1);
			path.path >>= 1;
			path.length--;
		}
	}
}

static void print_edges_mat(graph_mat_t* g) {
	for (unsigned i = 0; i < g->nb_vert; i++) {
		for (unsigned j = 0; j < g->nb_vert; j++) {
			if (g->mat[i][j].b)
				printf("%c -> %c\n", (char)i + 'a', (char)j + 'a');
		}
	}
}

static void print_edges_list(GRAPH_LIST* g) {
	for (unsigned i = 0; i < g->nb_vert; i++) {
		node_list_ref_t* node = g->neighbours[i].begin;
		while (node) {
			EDGE_LIST* e = node->p;
			printf("%c -> %c\n", (char)i + 'a', (char)(e->p) + 'a');
			node = node->next;
		}
	}
}
