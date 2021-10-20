#include "queue.h"

QUEUE_LIST* queue_list_from_tab(T* tab, unsigned size)
{
	LIST* end;
	LIST* begin = list_from_tab(tab, size, &end);
	QUEUE_LIST* q = malloc(sizeof(QUEUE_LIST));
	if(!q)
		return NULL;
	q->begin = begin;
	q->end = end;
	return q;
}

int push_queue_list(QUEUE_LIST* queue, T x)
{
	LIST* new = malloc(sizeof(LIST));
	if(!new)
		return -1;
	new->x = x;
	new->next = NULL;
	queue->end->next = new;
	queue->end = new;
	return 0;
}

T pop_queue_list(QUEUE_LIST* queue)
{
	LIST* begin = queue->begin->next;
	T x = queue->begin->x;
	free(queue->begin);
	queue->begin = begin;
	return x;
}

BOOL empty_queue(QUEUE_LIST* queue)
{
	return queue->begin == NULL;
}

void free_queue(QUEUE_LIST* queue)
{
	free_list(queue->begin);
	free(queue);
}
