#include "list.h"

LIST* list_from_tab(T* tab, unsigned size, LIST** last) {
	LIST* ret = malloc(sizeof(LIST));
	LIST* ptr = ret;
	
	if(size > 0) {
		ret->x = tab[0];
		for(unsigned i=1; i<size; i++)
			ptr = insert_list(ptr, tab[i]);
	}
	if(last)
		*last = ptr;
	return ret;
}

T* tab_from_list(LIST* list, T* tab) {
	unsigned i = 0;
	while(list) {
		tab[i] = list->x;
		list = list->next;
		i++;
	}
	return tab;
}

unsigned length(LIST* list) {
	unsigned l = 0;
	while(list) {
		l++;
		list = list->next;
	}
	return l;
}

LIST* insert_list(LIST* list, T x)
{
	LIST* next = list->next;
	list->next = malloc(sizeof(LIST));
	list->next-> x = x;
	list->next->next = next;
	return list->next;
}

LIST* push_front_list(LIST** list, T x)
{
	LIST* ret = malloc(sizeof(LIST));
	ret->x = x;
	ret->next = *list;
	*list = ret;
	return ret;
}

LIST* pop_front_list(LIST** list)
{
	LIST* ret = (*list)->next;
	free(*list);
	*list = ret;
	return ret;
}

LIST* push_back_list(LIST** list, T x) {
	if(!(*list)) {
		(*list) = malloc(sizeof(LIST));
		(*list)->x = x;
		(*list)->next = NULL;
	}
	else if(!(*list)->next) {
		(*list)->next = malloc(sizeof(LIST));
		(*list)->next->next = NULL;
		(*list)->next->x = x;
	}
	else {
		push_back_list(&((*list)->next), x);
	}
	return *list;
}

LIST* pop_back_list(LIST** list) {
	if(*list) {
		if(!(*list)->next) {
			free(*list);
			*list = NULL;
		}
		if(!(*list)->next->next) {
			free((*list)->next);
			(*list)->next = NULL;
		}
		else {
			pop_back_list(&(*list)->next);
		}
	}
	return *list;
}

LIST* remove_in_list(LIST** list, T x) {
	if(*list) {
		remove_in_list(&(*list)->next, x);
		
		if((*list)->x == x)
			pop_front_list(list);
	}
	return *list;
}

void free_list(LIST* list) {
	if(list) {
		if(list->next)
			free_list(list->next);
		free(list);
	}
}
