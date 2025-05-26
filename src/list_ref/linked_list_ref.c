#include "list_ref/linked_list_ref.h"
#include "compare.h"
#include "test_macros.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linked_list_t* linked_list_create(size_t size_bytes) {
	linked_list_t* ret = malloc(sizeof(linked_list_t));
	when_null_ret(ret, NULL);
    ret->begin = NULL;
    ret->end = NULL;
    ret->size_bytes = size_bytes;
	return ret;
}

BOOL linked_list_empty(const linked_list_t* list) {
	return list->begin == NULL;
}

linked_list_t* linked_list_from_tab(void* tab, size_t size, unsigned length) {
	linked_list_t* ret;
	linked_list_t* list = linked_list_create(size);
	when_null_ret(list, NULL);

	if (0 == size)
		return list;

	/* NOTE: On ne copie pas le tableau d'un seul bloc parce que sinon il
	 * faudrait gérer la libération de la mémoire et c'est complique :/
	 * Peut-être à modifier */

	linked_list_node_t* ptr = NULL;
	for (unsigned i = 0; i < length; i++) {
		ptr = linked_list_insert(list, ptr, (uint8_t*)tab + i * size);
        when_null_jmp(ptr, NULL, exit);
	}

	return list;
exit:
	linked_list_free(list);
	return ret;
}

void* linked_list_to_tab(linked_list_t* list, void* tab) {
	unsigned i = 0;
	linked_list_node_t* node = list->begin;
	while (node != NULL) {
		memcpy((uint8_t*)tab + i * list->size_bytes, node->data, list->size_bytes);
		node = node->next;
		i++;
	}
	return tab;
}

#ifdef STRUCT_RECURSIVE_IMPL
static unsigned linked_list_length_rec(node_list_ref_t* node,
									   unsigned int acc) {
	if (node == NULL)
		return acc;
	return linked_list_length_rec(node->next, acc + 1);
}

unsigned linked_list_length(list_ref_t* list) {
	return linked_list_length_rec(list->begin, 0);
}
#else
unsigned linked_list_length(linked_list_t* list) {
	if (linked_list_empty(list))
		return 0;
	unsigned l = 0;
	linked_list_node_t* node = list->begin;
	do {
		l++;
	} while ((node = node->next));
	return l;
}
#endif

void linked_list_insert_node(linked_list_t* list,
							 linked_list_node_t* prev,
							 linked_list_node_t* node) {
	assert(list);
	assert(node);

	// On conserve l'adresse de l'élément suivant
	linked_list_node_t* next = prev ? prev->next : list->begin;

	// On désigne le précédant du nouveau noeud : le noeud courant
	node->prev = prev;
	// On désigne le suivant du nouveau noeud : le noeud qu'on a conservé
	node->next = next;

	if (prev)
		prev->next = node;
	else
		list->begin = node;
	if (next)
		next->prev = node;	// On désigne le précédant du noeud conservé :
	else
		list->end = node;  // Le nouveau noeud est le dernier
}

linked_list_node_t* linked_list_insert(linked_list_t* list,
									linked_list_node_t* prev,
									void* p) {
	assert(list);

	// On crée un nouveau noeud qui devient le nouveau suivant
	linked_list_node_t* node = malloc(sizeof(linked_list_node_t) + list->size_bytes);
	when_null_ret(node, NULL);

	// On assigne la valeur voulue au pointeur du nouveau noeud
    memcpy(node->data, p, list->size_bytes);
	linked_list_insert_node(list, prev, node);
	return node;  // On retourne le nouveau noeud
}

linked_list_node_t* linked_list_push_front(linked_list_t* list, void* p) {
	assert(list);

	linked_list_node_t* node = malloc(sizeof(linked_list_node_t) + list->size_bytes);
	when_null_ret(node, NULL);

    memcpy(node->data, p, list->size_bytes);
	node->prev = NULL;		   // Pas de noeud précédant
	node->next = list->begin;  // Le suivant est l'ancien premier noeud
	if (list->end == NULL)
		list->end = node;
	else
		list->begin->prev = node;
	list->begin = node;	 // Le premier noeud devient le nouveau noeud
	return node;		 // On retourne le nouveau noeud
}

/* linked_list_node_t* linked_list_append_front(linked_list_t* list, void* p) { */
/* 	void* elem = malloc(list->size_bytes); */
/* 	memcpy(elem, p, list->size_bytes); */
/* 	return linked_list_push_front(list, elem); */
/* } */

linked_list_node_t* linked_list_push_back(linked_list_t* list, void* p) {
	assert(list);

	linked_list_node_t* node = malloc(sizeof(linked_list_node_t) + list->size_bytes);
	when_null_ret(node, NULL);

    memcpy(node->data, p, list->size_bytes);
	node->prev = list->end;	 // Le noeud précédant est le dernier noeud
	node->next = NULL;		 // Pas de suivant

	if (!list->begin)  // Si la liste était vide, le nouveau noeud devient le
					   // premier
		list->begin = node;
	else
		list->end->next =
			node;	   // Le nouveau noeud devient le suivant du dernier noeud
	list->end = node;  // Le dernier noeud devient le nouveau
	return node;	   // On retourne le nouveau noeud
}

/* linked_list_node_t* linked_list_append_back(linked_list_t* list, void* p) { */
/* 	void* elem = malloc(list->size_bytes); */
/* 	memcpy(elem, p, list->size_bytes); */
/* 	return linked_list_push_back(list, elem); */
/* } */

BOOL linked_list_pop_front(linked_list_t* list, void* x) {
	if (linked_list_empty(list))
		return FALSE;
	// We store the address of the second node
	linked_list_node_t* second = list->begin->next;
	// And set its predecessor to NULL
	if (second != NULL)
		second->prev = NULL;
	else
		list->end = NULL;

	// We store the data of the first node
    if(x != NULL)
        memcpy(x, list->begin->data, list->size_bytes);

	// Frees the first node
    free(list->begin);
	// The second node become the first one
	list->begin = second;
    return TRUE;
}

BOOL linked_list_pop_back(linked_list_t* list, void* x) {
	if (linked_list_empty(list))
        return FALSE;
	linked_list_node_t* second = list->end->prev;
	if (second)
		second->next = NULL;
	else
		list->begin = NULL;

	// We store the data of the first node
    if(x != NULL)
        memcpy(x, list->end->data, list->size_bytes);

    free(list->end);
	list->end = second;	 // Le dernier noeud est maintenant l'avant-dernier
    return TRUE;
}

void linked_list_extract(linked_list_t* list, linked_list_node_t* node) {
	assert(node);
	if (node->prev)
		node->prev->next = node->next;
	else
		list->begin = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		list->end = node->prev;
}

void linked_list_remove(linked_list_t* list, linked_list_node_t* node, void* x) {
	linked_list_extract(list, node);
	if (x != NULL)
        memcpy(x, node->data, list->size_bytes);
	free(node);
}

#ifdef STRUCT_RECURSIVE_IMPL
node_list_ref_t* linked_list_find_equals_rec(node_list_ref_t* node,
											 void* value,
											 equals_fn_t equals) {
	if (node == NULL)
		return NULL;
	if (equals(node->p, value))
		return node;
	return linked_list_find_equals_rec(node->next, value, equals);
}

node_list_ref_t* linked_list_find_equals(list_ref_t* list,
										 void* value,
										 equals_fn_t equals) {
	return linked_list_find_equals_rec(list->begin, value, equals);
}
#else
linked_list_node_t* linked_list_find_equals(linked_list_t* list,
										 void* value,
										 equals_fn_t equals) {
    foreach_node(list, node) {
		if (equals(node->data, value))
			return node;
	}
	return NULL;
}
#endif

#ifdef STRUCT_RECURSIVE_IMPL
static void free_node(list_ref_t* list, node_list_ref_t* node) {
	if (!node)
		return;
	node_list_ref_t* next = node->next;
	if (list->free_element)
		list->free_element(node->p);  // Libère l'élément pointé
	free(node);						  // Libère le noeud courant
	free_node(list, next);			  // Libère le noeud suivant
}

void linked_list_clean(list_ref_t* list) {
	if (list && list->begin) {
		free_node(list, list->begin);
		list->begin = NULL;
		list->end = NULL;
	}
}
#else
void linked_list_clean(linked_list_t* list) {
	when_null_ret(list, );
	linked_list_node_t* node = list->begin;
	linked_list_node_t* next;
	while (node != NULL) {
		next = node->next;
		free(node);
		node = next;
	}
	list->begin = NULL;
	list->end = NULL;
}
#endif

void linked_list_swap(linked_list_t* list, linked_list_node_t* a, linked_list_node_t* b) {
    linked_list_node_t node = *b;
    *b = *a;
    *a = node;
    if(b->prev == NULL)
        list->begin = b;
    if(a->prev == NULL)
        list->begin = a;
    if(b->next == NULL)
        list->end = b;
    if(a->next == NULL)
        list->end = a;
}

void linked_list_free(linked_list_t* list) {
	if (!list)
		return;
	linked_list_clean(list);
	free(list);
}
