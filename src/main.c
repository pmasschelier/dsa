#include <stdio.h>
#include "list.h"
#include "binarytree.h"

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

int main() {
	LIST* liste = list_from_tab((T[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10, NULL);
	
	pop_front_list(&liste);
	push_front_list(&liste, -100);
	pop_back_list(&liste);
	push_back_list(&liste, 100);
	
	LIST* parcours = liste;
	while (parcours != NULL) {
		printf("%d\n", parcours->x);
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
		fprintf(stderr, "Erreur lors du parcours en largeur de l'arbre binaire");
	}
	else {
		puts("\n\nParcours infixe de l'arbre binaire parfait : ");
		for(unsigned i=0; i<n; i++) {
			printf("%d, ", tab[i]);
		}
	}
	
	free_BT(tree);
	
	
	BinaryTree* t1 = perfect_BT_from_tab((T[]){0, 1, 2}, 3);
	BinaryTree* t2 = perfect_BT_from_tab((T[]){3, 4, 5}, 3);
	BinaryTree* t3 = perfect_BT_from_tab((T[]){6, 7, 8}, 3);
	FOREST* f = create_forest(t1);
	push_forest(f, t2);
	push_forest(f, t3);
	T contenu[3];
	
	puts("\n\n");
	preorder_traversal(pop_forest(f), contenu);
	for(int i=0; i<3; i++) printf("%d, ", contenu[i]);
	puts("\n");
	preorder_traversal(pop_forest(f), contenu);
	for(int i=0; i<3; i++) printf("%d, ", contenu[i]);
	puts("\n");
	preorder_traversal(pop_forest(f), contenu);
	for(int i=0; i<3; i++) printf("%d, ", contenu[i]);
	puts("\n");
	
	
	
	free_BT(t1);
	free_BT(t2);
	free_BT(t3);
	
	free_forest(f);
	
	// &RAPH_MAT* g = init_graph_mat();
	
	
	return 0;
}
