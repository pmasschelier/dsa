#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef struct QUEUE_LIST QUEUE_LIST;
struct QUEUE_LIST {
	LIST* begin;
	LIST* end;
};

/* \brief Crée une file à partir d'un tableau
 * \param Pointeur vers le tableau
 * \param Taille du tableau
 * \return Pointeur vers la lise créée (NULL si échec de l'allocation)
 */
QUEUE_LIST* queue_list_from_tab(T* tab, unsigned size);

/* \brief Enfile un élément
 * \param Pointeur vers la file
 * \param Élément à enfiler
 * \return 0 si tout s'est bien passé et -1 en cas d'échec
 */
int push_queue_list(QUEUE_LIST* queue, T x);

/* \brief Défile un élément
 * \param Pointeur vers la file
 * \return Valeur de l'élément
 */
T pop_queue_list(QUEUE_LIST* queue);

/* \brief Retourne TRUE si la file est vide
 * Concrétement : return queue->begin == NULL;
 */
BOOL empty_queue(QUEUE_LIST* queue);

/* \brief Libère récursivement tous les éléments de la liste
 */
void free_queue(QUEUE_LIST* queue);

#endif
