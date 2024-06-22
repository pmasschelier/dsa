#include "list_ref.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void* ptr(enum type t, ...) {
	va_list ap;
	va_start(ap, t);
	void* p = NULL;

	switch (t) {
	case TYPE_INT:
		p = malloc(sizeof(int));
		int i = va_arg(ap, int);
		memcpy(p, &i, sizeof(int));
		break;
	case TYPE_DOUBLE:
		p = malloc(sizeof(double));
		double d = va_arg(ap, double);
		memcpy(p, &d, sizeof(double));
		break;
	default:
		break;
	}

	va_end(ap);
	return p;
}

list_ref_t* create_list(size_t size) {
	list_ref_t* ret = malloc(sizeof(list_ref_t));
	if (!ret)
		return NULL;
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
	list_ref_t* ret = create_list(size);

	if (0 == size)
		return ret;

	// ret->begin = malloc(sizeof(node_ref_t));
	// if (!ret->begin)
	//     return NULL;

	/* NOTE: On ne copie pas le tableau d'un seul bloc parce que sinon il
	 * faudrait gérer la libération de la mémoire et c'est complique :/
	 * Peut-être à modifier */

	// node_ref_t* ptr = ret->begin;
	// ret->begin->p = malloc(size);
	// ret->begin->prev = NULL;
	// memcpy(ret->begin->p, tab, size);
	node_ref_t* ptr = NULL;
	for (unsigned i = 0; i < length; i++) {
		void* p = malloc(size);
		memcpy(p, (char*)tab + i * size, size);
		ptr = insert_list(ret, ptr, p);
	}

	return ret;
}

void* tab_from_list(list_ref_t* list, void* tab) {
	unsigned i = 0;
	node_ref_t* node = list->begin;
	while (node) {
		memcpy((char*)tab + i * list->size, node->p, list->size);
		node = node->next;
		i++;
	}
	return tab;
}

unsigned length_list(list_ref_t* list) {
	if (empty_list(list))
		return 0;
	unsigned l = 0;
	node_ref_t* node = list->begin;
	do {
		l++;
	} while ((node = node->next));
	return l;
}

node_ref_t* insert_list(list_ref_t* list, node_ref_t* prev, void* p) {
	assert(list);

	// On crée un nouveau noeud qui devient le nouveau suivant
	node_ref_t* node = malloc(sizeof(node_ref_t));
	if (!node)
		return NULL;

	// On conserve l'adresse de l'élément suivant
	node_ref_t* next = prev ? prev->next : list->begin;

	// On assigne la valeur voulue au pointeur du nouveau noeud
	node->p = p;
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
	return node;		   // On retourne le nouveau noeud
}

node_ref_t* push_front_list(list_ref_t* list, void* p) {
	assert(list);

	node_ref_t* node = malloc(sizeof(node_ref_t));	// On crée un nouveau noeud
	if (!node)
		return NULL;

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

node_ref_t* push_back_list(list_ref_t* list, void* p) {
	assert(list);

	node_ref_t* node = malloc(sizeof(node_ref_t));	// On crée un nouveau noeud
	if (!node)
		return NULL;

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

void pop_front_list(list_ref_t* list, void** x) {
	if (empty_list(list)) {
		if (x)
			*x = NULL;
		return;
	}
	node_ref_t* second =
		list->begin->next;	// On conserve l'adresse du deuxieme noeud
	if (second)
		second->prev = NULL;  // Pas de noeud précédant
	else					  // Si maintenant la liste est vide
		list->end = NULL;	  // Il n'y a plus de dernier élément
	void* ret =
		list->begin->p;	 // On conserve l'adresse contenue dans le premier noeud
	free(list->begin);	 // On libère le premier noeud
	list->begin = second;  // Le deuxième noeud devient le premier
	if (x)
		*x = ret;  // On retourne l'adresse qui était dans le premier noeud
	else
		list->free_element(ret);
}

void pop_back_list(list_ref_t* list, void** x) {
	if (empty_list(list)) {
		if (x)
			*x = NULL;
		return;
	}
	node_ref_t* second = list->end->prev;
	if (second)
		second->next = NULL;
	else
		list->begin = NULL;
	void* ret = list->end->p;  // On libère le noeud
	free(list->end);
	list->end = second;	 // Le dernier noeud est maintenant l'avant-dernier
	if (x)
		*x = ret;
	else
		list->free_element(ret);
}

node_ref_t* find_list(list_ref_t* list, void* x) {
	node_ref_t* node = list->begin;
	while (node) {
		if (memcmp(node->p, x, list->size) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}

void remove_list(list_ref_t* list, node_ref_t* node, void** x) {
	assert(node);
	if (!node)
		return;
	if (node->prev)
		node->prev->next = node->next;
	else
		list->begin = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		list->end = node->prev;
	if (x)
		*x = node->p;
	else
		list->free_element(node->p);
	free(node);
}

static void free_node(list_ref_t* list, node_ref_t* node) {
	if (node) {
		free_node(list, node->next);  // Libère le noeud suivant
		list->free_element(node->p);  // Libère l'élément pointé
		free(node);					  // Libère le noeud courant
	}
}

void clean_list(list_ref_t* list) {
	if (list && list->begin) {
		free_node(list, list->begin);
		list->begin = NULL;
		list->end = NULL;
	}
}

void free_list(list_ref_t* list) {
	if (list) {
		if (list->begin)
			free_node(list, list->begin);
		free(list);
	}
}
