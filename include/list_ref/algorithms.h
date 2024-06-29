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
node_list_ref_t* find_in_list(list_ref_t* list, void* x);

typedef void (*transform_list_fn_t)(void*);

/**
 * @brief Call the transform function on every element owned by the list
 *
 * @param list pointer to the list
 * @param f function to apply to the elements
 */
void transform_list(list_ref_t* list, transform_list_fn_t f);

typedef BOOL (*list_filter_fn_t)(void*);

// void filter_list(list_ref_t* list,
// 				 list_ref_t** others,
// 				 list_filter_fn_t* filter);

#endif	// !LISTREFALGLORITHMS_H
