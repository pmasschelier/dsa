#include "list_ref/linked_list.h"
#include "compare.h"
#include "test_macros.h"

#include <assert.h>
#include <stddef.h>
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

void linked_list_free(linked_list_t* list) {
	if (!list)
		return;
	linked_list_clean(list);
	free(list);
}

void linked_list_init(linked_list_t* list, size_t size_bytes) {
    list->size_bytes = size_bytes;
    list->begin = NULL;
    list->end = NULL;
}

void linked_list_deinit(linked_list_t* list) {
    linked_list_clean(list);
}

BOOL linked_list_empty(const linked_list_t* list) {
	return list->begin == NULL;
}

#ifdef STRUCT_RECURSIVE_IMPL
BOOL linked_list_from_tab_rec(linked_list_t* list, linked_list_node_t* ptr, void* tab, unsigned length) {
    if(length == 0)
        return TRUE;
    ptr = linked_list_insert(list, ptr, tab);
    if(ptr == NULL)
        return FALSE;
    return linked_list_from_tab_rec(list, ptr, (uint8_t*)tab + list->size_bytes, length - 1);
}
linked_list_t* linked_list_from_tab(void* tab, size_t size, unsigned length) {
	linked_list_t* list = linked_list_create(size);
	when_null_ret(list, NULL);

	if (0 == size)
		return list;

    if(!linked_list_from_tab_rec(list, NULL, tab, length)) {
        linked_list_free(list);
        return NULL;
    }
    return list;
}
#else
linked_list_t* linked_list_from_tab(void* tab, size_t size, unsigned length) {
	linked_list_t* ret;
	linked_list_t* list = linked_list_create(size);
	when_null_ret(list, NULL);

	if (0 == size)
		return list;

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
#endif

#ifdef STRUCT_RECURSIVE_IMPL
void linked_list_to_tab_rec(linked_list_node_t* node, void* tab, size_t size_bytes) {
    if(node == NULL)
        return;
    memcpy((uint8_t*)tab, node->data, size_bytes);
    linked_list_to_tab_rec(node->next, (uint8_t*)tab + size_bytes, size_bytes);
}
void* linked_list_to_tab(linked_list_t* list, void* tab) {
    linked_list_to_tab_rec(list->begin, tab, list->size_bytes);
	return tab;
}
#else
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
#endif

#ifdef STRUCT_RECURSIVE_IMPL
static unsigned linked_list_length_rec(linked_list_node_t* node,
									   unsigned int acc) {
	if (node == NULL)
		return acc;
	return linked_list_length_rec(node->next, acc + 1);
}

unsigned linked_list_length(linked_list_t* list) {
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
									const void* p) {
	assert(list);

	// On crée un nouveau noeud qui devient le nouveau suivant
	linked_list_node_t* node = malloc(sizeof(linked_list_node_t) + list->size_bytes);
	when_null_ret(node, NULL);

	// On assigne la valeur voulue au pointeur du nouveau noeud
    memcpy(node->data, p, list->size_bytes);
	linked_list_insert_node(list, prev, node);
	return node;  // On retourne le nouveau noeud
}

linked_list_node_t* linked_list_push_front(linked_list_t* list, const void* p) {
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

linked_list_node_t* linked_list_push_back(linked_list_t* list, const void* p) {
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
linked_list_node_t* linked_list_find_equals_rec(linked_list_node_t* node,
											 void* value,
											 equals_fn_t equals) {
	if (node == NULL)
		return NULL;
	if (equals(node->data, value))
		return node;
	return linked_list_find_equals_rec(node->next, value, equals);
}

linked_list_node_t* linked_list_find_equals(linked_list_t* list,
										 void* value,
										 equals_fn_t equals) {
	return linked_list_find_equals_rec(list->begin, value, equals);
}
#else
linked_list_node_t* linked_list_find_equals(linked_list_t* list,
										 void* value,
										 equals_fn_t equals) {
    linked_list_foreach(list, node) {
		if (equals(node->data, value))
			return node;
	}
	return NULL;
}
#endif

#ifdef STRUCT_RECURSIVE_IMPL
static void free_node(linked_list_t* list, linked_list_node_t* node) {
	if (!node)
		return;
	linked_list_node_t* next = node->next;
	free(node);						  // Libère le noeud courant
	free_node(list, next);			  // Libère le noeud suivant
}

void linked_list_clean(linked_list_t* list) {
	if (list != NULL) {
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

void linked_list_swap(linked_list_t* list, linked_list_node_t** a, linked_list_node_t** b) {
    linked_list_node_t* a_ptr = *a;
    linked_list_node_t* b_ptr = *b;
    if(a_ptr == b_ptr)
        return;
    linked_list_node_t a_val = *a_ptr;
    linked_list_node_t b_val = *b_ptr;
    *b_ptr = a_val;
    *a_ptr = b_val;
    if(a_val.next == b_ptr) {
        a_ptr->prev = b_ptr;
        b_ptr->next = a_ptr;
    }
    else if(b_val.next == a_ptr) {
        b_ptr->prev = a_ptr;
        a_ptr->next = b_ptr;
    }

    if(b_ptr->prev == NULL)
        list->begin = b_ptr;
    else
        b_ptr->prev->next = b_ptr;
    if(a_ptr->prev == NULL)
        list->begin = a_ptr;
    else
        a_ptr->prev->next = a_ptr;
    if(b_ptr->next == NULL)
        list->end = b_ptr;
    else
        b_ptr->next->prev = b_ptr;
    if(a_ptr->next == NULL)
        list->end = a_ptr;
    else
        a_ptr->next->prev = a_ptr;
    *a = b_ptr;
    *b = a_ptr;
}
