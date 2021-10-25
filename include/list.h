#ifndef LIST_H
#define LIST_H

#include "structures.h"

/* Listes doublement chainée */

typedef struct LIST_NODE LIST_NODE;
struct LIST_NODE {
	void* p;
	LIST_NODE *prev, *next;
};

typedef struct LIST LIST;
struct LIST {
	size_t size;
	void (*free_element)();
	LIST_NODE *begin, *end;
};


enum type {
	TYPE_INT,
	TYPE_DOUBLE
};

/* \brief Crée un pointeur qui copie la premiere donnée envoyé
 * \param TYPE_INT pour une donnée sur 4 octets et TYPE_DOUBLE pour une donnée sur 8 octets
 * \param Le premier paramètre de la liste sera la donnée à copier
 * \return Pointeur vers l'adresse allouée !!! Le pointeur retourné doit être libéré avec free
 */
void* ptr(enum type t, ...);

/* \brief Crée une liste vide
 * \param Taille des éléments que contiendra la liste (à titre indicatif)
 * \return pointeur vers la liste.
 * !!! Toute liste créée doit être libérée avec free_list(list).
 */
LIST* create_list(size_t size);

/* \brief Renvoie TRUE ssi la liste est vide
 * Concretement : return list->begin == NULL;
 */
BOOL empty_list(LIST* list);

/* \brief Crée une liste à partir d'un tableau
 * appel : LIST* liste = list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, sizeof(int), 10);
 * !!! Le contenu du tableau est copié !!!
 * \param pointeur vers le premier élément du tableau
 * \param Taille (en octets) d'un élément du tableau
 * \param taille du tableau 
 * \return pointeur vers la liste.
 * !!! Toute liste créée doit être libérée avec free_list(list).
 * \complexity 0(size)
 */
LIST* list_from_tab(void* tab, size_t size, unsigned length);

/* \brief Rempli un tableau à partir du contenu de la liste
 * \param liste chainée à copier
 * \param pointeur vers le premier élément du tableau (qui doit être alloué
 * avec une taille suffisament grande, qui peut être déterminée avec la fonction
 * length)
 * \return tab
 * \complexity O(n)
 */
void* tab_from_list(LIST* list, void* tab);

/* \brief Calcule la longueur d'une liste chainée.
 * \param pointeur vers le premier élément de la liste chainée.
 * \return Longueur de la liste.
 */
unsigned length(LIST* list);

/* \brief Insert un élément juste après le noeud list, si list et le premier
 * élément de la liste chainée, x sera la valeur du deuxième élément après l'appel
 * de la fonction.
 * \param list pointeur vers la liste
 * \param node pointeur vers l'élément après lequel on veut insérer.
 * \param élément à insérer !!!
 * \return pointeur sur l'élément inséré (ret = list->next)
 * \complexity 0(1)
 */
LIST_NODE* insert_list(LIST* list, LIST_NODE* node, void* p);

/* \brief Les 4 fonction suivantes ajoutent(push)/retirent(pop) l'élément
 * au début(front)/à la fin(back) de la liste
 * \param pointeur sur la liste chainée.
 * \param élément à ajouter, l'élément pointé n'est pas copié, aucun allocation
 * ni libération de mémoire ne sont faites par ces fonctions sur x
 * \return Pointeur sur le noeud ajouté à la liste, NULL en cas d'échec
 * \complexity O(1)
 */
LIST_NODE* push_front_list(LIST* list, void* x);
void* pop_front_list(LIST* list);
LIST_NODE* push_back_list(LIST* list, void* x);
void* pop_back_list(LIST* list);

/* \brief Libère la liste
 * Libère récursivement tous les éléments de la liste chainée en utilisant
 * pour libérer les élements la fonction list->free_element (par défaut free)
 */
void free_list(LIST* list);

#endif
