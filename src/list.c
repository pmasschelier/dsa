#include "list.h"

void* ptr(enum type t, ...)
{
	va_list ap;
	va_start(ap, t);
	void* p = NULL;
	
	switch(t) {
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

LIST* create_list(size_t size)
{
	LIST* ret = malloc(sizeof(LIST));
	if(!ret)
		return NULL;
	ret->begin = NULL;
	ret->end = NULL;
	ret->free_element = free;
	ret->size = size;
	return ret;
}

BOOL empty_list(LIST* list)
{
	return list->begin == NULL;
}

LIST* list_from_tab(void* tab, size_t size, unsigned length)
{
	LIST* ret = malloc(sizeof(LIST));
	ret->free_element = free;
	ret->size = size;
	ret->begin = NULL;
	ret->end = NULL;
	
	if(size > 0) {
		ret->begin = malloc(sizeof(LIST_NODE));
		if(!ret->begin)
			return NULL;
		
		/* On ne copie pas le tableau d'un seul bloc parce que sinon il faudrait
		 * gérer la libération de la mémoire et c'est complique :/
		 * Peut-être à modifier */
		
		LIST_NODE* ptr = ret->begin;
		ret->begin->p = malloc(size);
		ret->begin->prev = NULL;
		memcpy(ret->begin->p, tab, size);
		for(unsigned i=1; i<length; i++) {
			void* p = malloc(size);
			memcpy(p, (char*)tab + i*size, size);
			ptr = insert_list(ret, ptr, p);
		}
	}
	return ret;
}

void* tab_from_list(LIST* list, void* tab) {
	unsigned i = 0;
	LIST_NODE* node = list->begin;
	while(node) {
		memcpy((char*)tab + i*list->size, node->p, list->size);
		node = node->next;
		i++;
	}
	return tab;
}

unsigned length_list(LIST* list) {
	unsigned l = 0;
	LIST_NODE* node = list->begin;
	while(node) {
		l++;
		node = node->next;
	}
	return l;
}

LIST_NODE* insert_list(LIST* list, LIST_NODE* node, void* p)
{
	LIST_NODE* next = node->next; // On conserve l'adresse de l'élément suivant
	node->next = malloc(sizeof(LIST_NODE)); // On crée un nouveau noeud qui devient le nouveau suivant
	node->next->p = p; // On assigne la valeur voulue au pointeur du nouveau noeud
	node->next->prev = node; // On désigne le précédant du nouveau noeud : le noeud courant
	node->next->next = next; // On désigne le suivant du nouveau noeud : le noeud qu'on a conservé
	if(!next)
		list->end = node->next; // Le nouveau noeud est le dernier
	else
		next->prev = node->next; // On désigne le précédant du noeud conservé : le nouveau noeud
	return node->next; // On retourne le nouveau noeud
}

LIST_NODE* push_front_list(LIST* list, void* p)
{
	LIST_NODE* ret = malloc(sizeof(LIST_NODE)); // On crée un nouveau noeud
	if(!ret)
		return NULL;
	ret->p = p; // On assigne la valeur voulue au pointeur du nouveau noeud
	ret->prev = NULL; // Pas de noeud précédant
	ret->next = list->begin; // Le suivant est l'ancien premier noeud
	list->begin = ret; // Le premier noeud devient le nouveau noeud
	return ret; // On retourne le nouveau noeud
}

void* pop_front_list(LIST* list)
{
	if(empty_list(list))
		return NULL;
	LIST_NODE* second = list->begin->next; // On conserve l'adresse du deuxieme noeud
	if(second)
		second->prev = NULL; // Pas de noeud précédant
	void* ret = list->begin->p; // On conserve l'adresse contenue dans le premier noeud
	free(list->begin); // On libère le premier noeud
	list->begin = second; // Le deuxième noeud devient le premier
	if(empty_list(list)) // Si maintenant la liste est vide
		list->end = NULL; // Il n'y a plus de dernier élément
	return ret; // On retourne l'adresse qui était dans le premier noeud
}

LIST_NODE* push_back_list(LIST* list, void* p) {
	if(!list)
		return NULL;
	LIST_NODE* node = malloc(sizeof(LIST_NODE)); // On crée un nouveau noeud
	if(!node)
		return NULL;
	node->prev = list->end; // Le noeud précédant est le dernier noeud
	node->next = NULL; // Pas de suivant
	node->p = p; // On assigne la valeur voulue au pointeur du nouveau noeud
	if(empty_list(list))
		list->begin = node;
	else
		list->end->next = node; // Le nouveau noeud devient le suivant du dernier noeud
	list->end = node; // Le dernier noeud devient le nouveau
	if(!list->begin) // Si la liste était vide, le nouveau noeud devient le premier
		list->begin = node;
	return node; // On retourne le nouveau noeud
}

void* pop_back_list(LIST* list) {
	if(!list || !list->end)
		return NULL;
	list->end = list->end->prev; // Le dernier noeud est maintenant l'avant-dernier 
	void* ret = list->end->next->p; // On conserve l'adresse contenue dans le premier noeud
	free(list->end->next); // On libère le noeud
	list->end->next = NULL; // Le dernier noeud n'a plus de suivant
	if(!list->end)
		list->begin = NULL;
	return ret; // On retourne l'adresse qui était dans le dernier noeud
}

LIST_NODE* find_list(LIST* list, void* x)
{
	LIST_NODE* node = list->begin;
	while(node) {
		if(memcmp(node->p, x, list->size) == 0)
			return node;
		node = node->next;
	}
	return NULL;
}

void remove_list(LIST* list, LIST_NODE* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list->free_element(node->p);
	free(node);
}

static void free_node(LIST* list, LIST_NODE* node) {
	if(node) {
		free_node(list, node->next); // Libère le noeud suivant
		list->free_element(node->p); // Libère l'élément pointé
		free(node); // Libère le noeud courant
	}
}

void clean_list(LIST* list)
{
	if(list && list->begin) {
		free_node(list, list->begin);
		list->begin = NULL;
		list->end = NULL;
	}
}

void free_list(LIST* list) {
	if(list && list->begin) {
		free_node(list, list->begin);
		free(list);
	}
}
