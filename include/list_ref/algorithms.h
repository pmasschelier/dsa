#ifndef LISTREFALGLORITHMS_H
#define LISTREFALGLORITHMS_H

#include "list_ref/list_ref.h"

/** \brief Renvoie le premier élément de la liste égale à *x
 * La comparaison est faite avec memcmp
 * \param list pointeur sur la liste chainée.
 * \param x pointeur sur l'élément à comparer
 * \return un pointeur sur l'élément s'il a été trouve, NULL sinon
 * \complexity O(n)
 */
node_ref_t* find_in_list(list_ref_t* list, void* x);

#endif	// !LISTREFALGLORITHMS_H
