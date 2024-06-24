#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stddef.h>
#include "ptr.h"

typedef struct node_btree_ref node_btree_ref_t;
struct node_btree_ref {
	void* p;
	node_btree_ref_t *ls, *rs;	// left son & right son
};

typedef struct btree_ref btree_ref_t;
struct btree_ref {
	size_t size;
	free_element_fn_t free_element;
	node_btree_ref_t* root;
};

typedef struct btree_path btree_path_t;
struct btree_path {
	char length;
	long unsigned int path;
};

const btree_path_t ROOT_PATH = {0, 0};

btree_ref_t* create_btree(size_t size);

/* \brief Renvoie le chemin correspondant à la numérotation d'un arbre binaire
 * parfait (numérotation canonique d'un tas)
 * \param numérotation du noeud dont on cherche le chemin
 * \return
 * path.length contient la longueur du chemin
 * path.path est un mot binaire sur 64 bits contenant le chemin dans l'arbre
 * (O = gauche, 1 = droite) avec le bit de plus faible poids représentant le
 * premier chemin
 */
btree_path_t btree_node_to_path(long unsigned int pos);

/* \brief Renvoie la hauteur de l'arbre
 * \param Pointeur vers la racine
 * \return hauteur de l'arbre, 0 si tree == NULL, 1 si il n'y a que la racine.
 */
unsigned btree_height(btree_ref_t* tree);

/* \brief Renvoie le nombre de noeuds de l'abre
 * \param Pointeur vers la racine
 */
unsigned btree_length(btree_ref_t* tree);

node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p);
/* \brief Parcours respectiverment en ordre prefixe, suffixe et infixe l'arbre
 * binaire et écrit les éléments dans le tableau
 * \param Arbre à parcourir
 * \param Tableau à remplir, il doit pointer vers une zone allouée de taille
 * suffisante, par exemple en ayant mesuré l'arbre au préalable avec
 * size_BT(tree) \return Nombre de noeuds de l'arbre.
 */
unsigned preorder_traversal(btree_ref_t* tree, void* tab[]);
unsigned postorder_traversal(btree_ref_t* tree, void* tab[]);
unsigned sym_traversal(btree_ref_t* tree, void* tab[]);

/* \brief Parcours en largeur de l'arbre binaire
 * \param Arbre binaire à parcourir
 * \param Pointeur vers le tableau à remplir dans l'ordre du parcours
 * Ce dernier doit avoir été alloué avec une taille suffisante
 * (cf. size(tree))
 * \return 0 si tout c'est bien passé et -1 en cas d'erreur
 */
int level_order_traversal(node_btree_ref_t* tree, void* tab[]);

/* Arbre binaires parfaits */

/* \brief Construit un arbre binaire parfait à partir d'un tableau
 * La bijection tableau <-> arbre se fait en suivant la numérotation
 * canonique des feuilles d'un arbre binaire parfait
 * \param Pointeur vers le tableau à lire
 * \param Taille du tableau à lire
 * \return Pointeur vers l'arbre binaire créé, qui devra être libéré avec
 * free_BT(tree);
 */
btree_ref_t* perfect_BT_from_tab(void* tab, size_t size, unsigned length);

void btree_free(btree_ref_t* tree);

#endif
