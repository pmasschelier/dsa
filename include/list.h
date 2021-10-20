#ifndef LIST_H
#define LIST_H

#include "structures.h"

/* Listes simplement chainée */

typedef char T;

typedef struct LIST LIST;
struct LIST {
	T x;
	LIST* next;
};


/* \brief Crée une liste à partir d'un tableau
 * appel : LIST* liste = list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
 * \param pointeur vers le premier élément du tableau
 * \param taille du tableau
 * \param Si le pointeur n'est pas nul il devient un pointeur vers un pointeur vers le
 * dernier élément de la liste.
 * \return pointeur vers le premier élément de la liste.
 * !!! Toute liste créée doit être libérée avec free_liste(list).
 * \complexity 0(size)
 */
LIST* list_from_tab(T* tab, unsigned size, LIST** last);

/* \brief Rempli un tableau à partir du contenu de la liste
 * \param liste chainée à copier
 * \param pointeur vers le premier élément du tableau (qui doit être alloué
 * avec une taille suffisament grande, qui peut être déterminée avec la fonction
 * length)
 * \return tab
 * \complexity O(n)
 */
T* tab_from_list(LIST* list, T* tab);

/* \brief Calcule la longueur d'une liste chainée.
 * \param pointeur vers le premier élément de la liste chainée.
 * \return Longueur de la liste.
 */
unsigned length(LIST* list);

/* \brief Insert un élément juste après le noeud list, si list et le premier
 * élément de la liste chainée, x sera la valeur du deuxième élément après l'appel
 * de la fonction.
 * \param pointeur vers l'élément après lequel on veut insérer.
 * \param élément à insérer
 * \return pointeur sur l'élément inséré (ret = list->next)
 * \complexity 0(1)
 */
LIST* insert_list(LIST* list, T x);

/* \brief Les 4 fonction suivantes ajoutent(push)/retirent(pop) l'élément
 * au début(front)/à la fin(back) de la liste
 * \param pointeur de pointeur sur le premier élément de la liste chainée.
 * 	*list est modifiée par push_front_list et pop_front_list
 * \param élément à ajouter
 * \return *list
 * \complexity O(1), O(1), O(n), O(n)
 */
LIST* push_front_list(LIST** list, T x);
LIST* pop_front_list(LIST** list);
LIST* push_back_list(LIST** list, T x);
LIST* pop_back_list(LIST** list);

/* \brief Supprime tous les éléments égaux à x dans la liste
 * \param pointeur de pointeur sur le premier élément de la liste chainée.
 * \param élément de x
 * \return *list
 * \complexity 0(n)
 */
LIST* remove_in_list(LIST** list, T x);

/* \brief Libère la liste
 * Libère récursivement tous les éléments de la liste chainée
 */
void free_list(LIST* list);

#endif
