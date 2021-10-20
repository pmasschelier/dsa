#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "structures.h"

typedef struct BinaryTree BinaryTree;
struct BinaryTree {
	T x;
	BinaryTree *ls, *rs; // left son & right son
};


typedef	struct Path Path;
struct Path {
	char length;
	long unsigned int path;
};

/* \brief Renvoie le chemin correspondant à la numérotation d'un arbre binaire
 * parfait (numérotation canonique d'un tas)
 * \param numérotation du noeud dont on cherche le chemin
 * \return 
 * path.length contient la longueur du chemin
 * path.path est un mot binaire sur 64 bits contenant le chemin dans l'arbre
 * (O = gauche, 1 = droite) avec le bit de plus faible poids représentant le premier chemin
 */
Path path(long unsigned int pos);

/* \brief Renvoie la hauteur de l'arbre
 * \param Pointeur vers la racine
 * \return hauteur de l'arbre, 0 si tree == NULL, 1 si il n'y a que la racine.
 */
unsigned height_BT(BinaryTree* tree);

/* \brief Renvoie le nombre de noeuds de l'abre
 * \param Pointeur vers la racine
 */
unsigned size_BT(BinaryTree* tree);

/* \brief Parcours respectiverment en ordre prefixe, suffixe et infixe l'arbre
 * binaire et écrit les éléments dans le tableau
 * \param Arbre à parcourir
 * \param Tableau à remplir, il doit pointer vers une zone allouée de taille
 * suffisante, par exemple en ayant mesuré l'arbre au préalable avec size_BT(tree)
 * \return Nombre de noeuds de l'arbre.
 */
unsigned preorder_traversal(BinaryTree* tree, T* tab);
unsigned postorder_traversal(BinaryTree* tree, T* tab);
unsigned sym_traversal(BinaryTree* tree, T* tab);

void free_BT(BinaryTree* tree);

/* Une forêt est, ici, une file d'arbres */
typedef struct FOREST_NODE FOREST_NODE;
struct FOREST_NODE {
	BinaryTree* tree;
	FOREST_NODE* next;
};

typedef struct FOREST FOREST;
struct FOREST {
	FOREST_NODE* begin;
	FOREST_NODE* end;
};

/* \brief Crée une file d'arbres binaires vide (une forêt)
 * Renvoie un pointeur f sur un FOREST alloué (qui devra être libéré avec
 * free_forest) tel que f->begin = NULL et f->end = NULL
 */
FOREST* create_forest();

/* \brief Ajoute un arbre à la fin de la forêt (enfile)
 * \param Forêt sur laquelle on veut enfiler
 * \param Arbre qu'on veut enfiler
 * \return 0 si tout c'est bien passé et -1 en cas d'erreur (échec de l'allocation)
 */
int push_forest(FOREST* queue, BinaryTree* tree);

/* \brief Dépile la forêt
 * \param pointeur vers la forêt
 * \return pointeur vers l'arbre dépilé
 */
BinaryTree* pop_forest(FOREST* queue);

/* \brief Renvoie TRUE ssi la forêt est vide
 * Concrètement : return f->begin == NULL;
 */
BOOL empty_forest(FOREST* queue);

/* \brief Libère récursivement tous les noeuds de la forêt
 * !!! Cette fonction ne libère pas les arbres qui sont dans la foret
 * Après avoir libéré la forêt, chaque arbre doit être libéré individuellement.
 */
void free_forest(FOREST* queue);

/* \brief Parcours en largeur de l'arbre binaire
 * \param Arbre binaire à parcourir
 * \param Pointeur vers le tableau à remplir dans l'ordre du parcours
 * Ce dernier doit avoir été alloué avec une taille suffisante
 * (cf. size(tree))
 * \return 0 si tout c'est bien passé et -1 en cas d'erreur
 */
int level_order_traversal(BinaryTree* tree, T* tab);

/* Arbre binaires parfaits */

/* \brief Construit un arbre binaire parfait à partir d'un tableau
 * La bijection tableau <-> arbre se fait en suivant la numérotation
 * canonique des feuilles d'un arbre binaire parfait
 * \param Pointeur vers le tableau à lire
 * \param Taille du tableau à lire
 * \return Pointeur vers l'arbre binaire créé, qui devra être libéré avec
 * free_BT(tree);
 */
BinaryTree* perfect_BT_from_tab(T* tab, unsigned size);

#endif
