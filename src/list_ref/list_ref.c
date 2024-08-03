#include "list_ref/list_ref.h"
#include "test_macros.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list_ref_t* create_list(size_t size) {
	list_ref_t* ret = malloc(sizeof(list_ref_t));
	when_null_ret(ret, NULL);
	ret->begin = NULL;
	ret->end = NULL;
	ret->free_element = free;
	ret->size = size;
	return ret;
}

BOOL empty_list(list_ref_t* list) {
	return list->begin == NULL;
}

list_ref_t* list_from_tab(void* tab, size_t size, unsigned length) {
	list_ref_t* ret;
	list_ref_t* list = create_list(size);
	when_null_ret(list, NULL);

	if (0 == size)
		return list;

	/* NOTE: On ne copie pas le tableau d'un seul bloc parce que sinon il
	 * faudrait gérer la libération de la mémoire et c'est complique :/
	 * Peut-être à modifier */

	node_list_ref_t* ptr = NULL;
	void* p = NULL;
	for (unsigned i = 0; i < length; i++) {
		p = malloc(size);
		when_null_jmp(p, NULL, exit);
		memcpy(p, (char*)tab + i * size, size);
		ptr = insert_list(list, ptr, p);
	}

	return list;
exit:
	free_list(list);
	return ret;
}

void* tab_from_list(list_ref_t* list, void* tab) {
	unsigned i = 0;
	node_list_ref_t* node = list->begin;
	while (node) {
		memcpy((char*)tab + i * list->size, node->p, list->size);
		node = node->next;
		i++;
	}
	return tab;
}

#ifdef STRUCT_RECURSIVE_IMPL
static unsigned length_list_rec(node_list_ref_t* node, unsigned int acc) {
	if (node == NULL)
		return acc;
	return length_list_rec(node->next, acc + 1);
}

unsigned length_list(list_ref_t* list) {
	return length_list_rec(list->begin, 0);
}
#else
unsigned length_list(list_ref_t* list) {
	if (empty_list(list))
		return 0;
	unsigned l = 0;
	node_list_ref_t* node = list->begin;
	do {
		l++;
	} while ((node = node->next));
	return l;
}
#endif

void insert_list_node(list_ref_t* list,
					  node_list_ref_t* prev,
					  node_list_ref_t* node) {
	assert(list);
	assert(node);

	// On conserve l'adresse de l'élément suivant
	node_list_ref_t* next = prev ? prev->next : list->begin;

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

node_list_ref_t* insert_list(list_ref_t* list, node_list_ref_t* prev, void* p) {
	assert(list);

	// On crée un nouveau noeud qui devient le nouveau suivant
	node_list_ref_t* node = malloc(sizeof(node_list_ref_t));
	when_null_ret(node, NULL);

	// On assigne la valeur voulue au pointeur du nouveau noeud
	node->p = p;
	insert_list_node(list, prev, node);
	return node;  // On retourne le nouveau noeud
}

node_list_ref_t* push_front_list(list_ref_t* list, void* p) {
	assert(list);

	node_list_ref_t* node =
		malloc(sizeof(node_list_ref_t));  // On crée un nouveau noeud
	when_null_ret(node, NULL);

	node->p = p;  // On assigne la valeur voulue au pointeur du nouveau noeud
	node->prev = NULL;		   // Pas de noeud précédant
	node->next = list->begin;  // Le suivant est l'ancien premier noeud
	if (!list->end)
		list->end = node;
	else
		list->begin->prev = node;
	list->begin = node;	 // Le premier noeud devient le nouveau noeud
	return node;		 // On retourne le nouveau noeud
}

node_list_ref_t* append_front_list(list_ref_t* list, void* p) {
	void* elem = malloc(list->size);
	memcpy(elem, p, list->size);
	return push_front_list(list, elem);
}

node_list_ref_t* push_back_list(list_ref_t* list, void* p) {
	assert(list);

	node_list_ref_t* node =
		malloc(sizeof(node_list_ref_t));  // On crée un nouveau noeud
	when_null_ret(node, NULL);

	node->prev = list->end;	 // Le noeud précédant est le dernier noeud
	node->next = NULL;		 // Pas de suivant
	node->p = p;  // On assigne la valeur voulue au pointeur du nouveau noeud

	if (!list->begin)  // Si la liste était vide, le nouveau noeud devient le
					   // premier
		list->begin = node;
	else
		list->end->next =
			node;	   // Le nouveau noeud devient le suivant du dernier noeud
	list->end = node;  // Le dernier noeud devient le nouveau
	return node;	   // On retourne le nouveau noeud
}

node_list_ref_t* append_back_list(list_ref_t* list, void* p) {
	void* elem = malloc(list->size);
	memcpy(elem, p, list->size);
	return push_back_list(list, elem);
}

void pop_front_list(list_ref_t* list, void** x) {
	if (empty_list(list)) {
		if (x)
			*x = NULL;
		return;
	}
	// We store the address of the second node
	node_list_ref_t* second = list->begin->next;
	// And set its predecessor to NULL
	if (second != NULL)
		second->prev = NULL;
	else
		list->end = NULL;
	// We store the address of the data of the first node
	void* ret = list->begin->p;
	// Frees the first node
	free(list->begin);
	// The second node become the first one
	list->begin = second;
	// If the pointer is not NULL, set the referenced pointer to the poped out
	// data or free the data if applicable
	if (x)
		*x = ret;
	else if (list->free_element)
		list->free_element(ret);
}

void pop_back_list(list_ref_t* list, void** x) {
	if (empty_list(list)) {
		if (x)
			*x = NULL;
		return;
	}
	node_list_ref_t* second = list->end->prev;
	if (second)
		second->next = NULL;
	else
		list->begin = NULL;
	void* ret = list->end->p;  // On libère le noeud
	free(list->end);
	list->end = second;	 // Le dernier noeud est maintenant l'avant-dernier
	if (x)
		*x = ret;
	else if (list->free_element)
		list->free_element(ret);
}

void extract_list(list_ref_t* list, node_list_ref_t* node) {
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

void remove_list(list_ref_t* list, node_list_ref_t* node, void** x) {
	extract_list(list, node);
	if (x)
		*x = node->p;
	else if (list->free_element)
		list->free_element(node->p);
	free(node);
}

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

void clean_list(list_ref_t* list) {
	if (list && list->begin) {
		free_node(list, list->begin);
		list->begin = NULL;
		list->end = NULL;
	}
}
#else
void clean_list(list_ref_t* list) {
	when_null_ret(list, );
	node_list_ref_t* node = list->begin;
	node_list_ref_t* next;
	while (node != NULL) {
		next = node->next;
		if (list->free_element)
			list->free_element(node->p);
		free(node);
		node = next;
	}
	list->begin = NULL;
	list->end = NULL;
}
#endif

void free_list(list_ref_t* list) {
	if (!list)
		return;
	clean_list(list);
	free(list);
}
