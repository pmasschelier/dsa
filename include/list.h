#ifndef LIST_H
#define LIST_H

#include "structures.h"
#include <stdlib.h>

/** \file file.h
 * \brief Doubly linked lists
 */

typedef struct node_ref node_ref_t;
struct node_ref {
  void *p;
  node_ref_t *prev, *next;
};

typedef void (*free_element_fn_t)(void *);

typedef struct list_ref list_ref_t;
/** \struct LIST
 * \brief Doubly linked list
 * \see node_ref_t
 */
struct list_ref {
  /**< size in bytes of a list member */
  size_t size;
  /**< Function called on a list member to free its memory */
  free_element_fn_t free_element;
  /**< Pointer to the first element of the list (NULL if list is empty) */
  node_ref_t *begin;
  /**< Pointer to the last element of the list */
  node_ref_t *end;
};

enum type { TYPE_INT, TYPE_DOUBLE };

/** \brief Crée un pointeur qui copie la premiere donnée envoyé
 * \param TYPE_INT pour une donnée sur 4 octets et TYPE_DOUBLE pour une donnée
 * sur 8 octets
 * \param ... Le premier paramètre de la liste sera la donnée à copier
 * \return Pointeur vers l'adresse allouée !!! Le pointeur retourné doit être
 * libéré avec free !!!
 */
void *ptr(enum type t, ...);

/** \brief Crée une liste vide
 * \param Taille des éléments que contiendra la liste (à titre indicatif)
 * \return pointeur vers la liste.
 * !!! Toute liste créée doit être libérée avec free_list(list).
 */
list_ref_t *create_list(size_t size);

/** \brief Renvoie TRUE ssi la liste est vide
 * Concretement : return list->begin == NULL;
 * \param list pointeur sur la liste à tester
 */
BOOL empty_list(list_ref_t *list);

/** \brief Crée une liste à partir d'un tableau
 * appel : LIST* liste = list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
 * sizeof(int), 10);
 * !!! Le contenu du tableau est copié !!!
 * \param pointeur vers le premier élément du tableau
 * \param Taille (en octets) d'un élément du tableau
 * \param taille du tableau
 * \return pointeur vers la liste.
 * !!! Toute liste créée doit être libérée avec free_list(list).
 * \complexity 0(size)
 */
list_ref_t *list_from_tab(void *tab, size_t size, unsigned length);

/** \brief Rempli un tableau à partir du contenu de la liste
 * \param liste chainée à copier
 * \param pointeur vers le premier élément du tableau (qui doit être alloué
 * avec une taille suffisament grande, qui peut être déterminée avec la fonction
 * length)
 * \return tab
 * \complexity O(n)
 */
void *tab_from_list(list_ref_t *list, void *tab);

/** \brief Calcule la longueur d'une liste chainée.
 * \param pointeur vers le premier élément de la liste chainée.
 * \return Longueur de la liste.
 */
unsigned length_list(list_ref_t *list);

/** \brief Insert un élément juste après le noeud list, si list et le premier
 * élément de la liste chainée, x sera la valeur du deuxième élément après
 * l'appel de la fonction. \param list pointeur vers la liste \param node
 * pointeur vers l'élément après lequel on veut insérer. \param élément à
 * insérer !!! \return pointeur sur l'élément inséré (ret = list->next)
 * \complexity 0(1)
 */
node_ref_t *insert_list(list_ref_t *list, node_ref_t *node, void *p);

/** \brief Les 4 fonction suivantes ajoutent(push)/retirent(pop) l'élément
 * au début(front)/à la fin(back) de la liste
 * \param pointeur sur la liste chainée.
 * \param élément à ajouter, l'élément pointé n'est pas copié, aucun allocation
 * ni libération de mémoire ne sont faites par ces fonctions sur x
 * \return Pointeur sur le noeud ajouté à la liste, NULL en cas d'échec
 * Les pointeurs renvoyés par les fonctions pop devront être libérés
 * \complexity O(1)
 */
node_ref_t *push_front_list(list_ref_t *list, void *x);
node_ref_t *push_back_list(list_ref_t *list, void *x);
void pop_front_list(list_ref_t *list, void **x);
void pop_back_list(list_ref_t *list, void **x);

/** \brief Renvoie le premier élément de la liste égale à *x
 * La comparaison est faite avec memcmp
 * \param list pointeur sur la liste chainée.
 * \param x pointeur sur l'élément à comparer
 * \return un pointeur sur l'élément s'il a été trouve, NULL sinon
 * \complexity O(n)
 */
node_ref_t *find_list(list_ref_t *list, void *x);

/** \brief Supprime le noeud de la liste
 * !!! Libère l'élément pointé !!!
 * \param list pointeur sur la liste chainée.
 * \param node noeud à supprimer
 *
 */
void remove_list(list_ref_t *list, node_ref_t *node, void **x);

/** \brief Supprime tous les noeuds de la liste
 * !!! Libère les éléments pointés !!!
 * \param list pointeur sur la liste chainée.
 */
void clean_list(list_ref_t *list);

/** \brief Libère la liste
 * Libère récursivement tous les éléments de la liste chainée en utilisant
 * pour libérer les élements la fonction list->free_element (par défaut free)
 */
void free_list(list_ref_t *list);

#endif
