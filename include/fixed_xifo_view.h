#ifndef FIXED_XIFO_VIEW_H
#define FIXED_XIFO_VIEW_H

#ifdef STACK_IMPL_LIST_REF
#include <string.h>
#include "list_ref/list_ref.h"
#include "test_macros.h"

typedef list_ref_t fixed_xifo_view_t;

static inline fixed_xifo_view_t* create_fixed_xifo_view(size_t size,
														unsigned capacity) {
	(void)capacity;
	fixed_xifo_view_t* xifo = create_list(size);
	when_null_ret(xifo, NULL);
	xifo->free_element = NULL;
	return xifo;
}

#define create_fixed_xifo_copy(size, capacity) create_list(size)

#define empty_fixed_xifo(stack) empty_list(stack)
#define free_fixed_xifo(stack) free_list(stack)

#define fixed_xifo_view_back(stack, type) get_node_ref((stack)->end, type)
#define fixed_xifo_copy_back(stack, type) *get_node_ref((stack)->end, type)
#define fixed_xifo_view_front(stack, type) get_node_ref((stack)->begin, type)
#define fixed_xifo_copy_front(stack, type) *get_node_ref((stack)->begin, type)

#define fixed_xifo_view_push_back(stack, ptr) push_back_list(stack, ptr)
#define fixed_xifo_copy_push_back(stack, ptr) append_back_list(stack, ptr)
#define fixed_xifo_view_pop_back(stack, ptr) pop_back_list(stack, ptr)
#define fixed_xifo_copy_pop_back(stack, ptr)         \
	do {                                             \
		if (ptr != NULL)                             \
			memcpy(ptr, stack->end->p, stack->size); \
		pop_back_list(stack, NULL);                  \
	} while (0);

#define fixed_xifo_view_push_front(stack, ptr) push_front_list(stack, ptr)
#define fixed_xifo_copy_push_front(stack, ptr) append_front_list(stack, ptr)
#define fixed_xifo_view_pop_front(stack, ptr) pop_front_list(stack, ptr)
#define fixed_xifo_copy_pop_front(stack, ptr)          \
	do {                                               \
		if (ptr != NULL)                               \
			memcpy(ptr, stack->begin->p, stack->size); \
		pop_front_list(stack, NULL);                   \
	} while (0);
#endif	// STACK_IMPL_LIST_REF

#ifdef STACK_IMPL_DYNARRAY
#include "circular_buffer.h"

typedef circular_buffer_t fixed_xifo_view_t;

#define create_fixed_xifo_view(size, capacity) \
	create_circular_buffer(sizeof(void*), capacity)
#define create_fixed_xifo_copy(size, capacity) \
	create_circular_buffer(size, capacity)

#define empty_fixed_xifo(stack) (circular_buffer_size(stack) == 0)
#define free_fixed_xifo(stack) free_circular_buffer(stack)

#define fixed_xifo_view_back(stack, type) *get_buffer_last(stack, type*)
#define fixed_xifo_copy_back(stack, type) *get_buffer_last(stack, type)
#define fixed_xifo_view_front(stack, type) *get_buffer_first(stack, type*)
#define fixed_xifo_copy_front(stack, type) *get_buffer_first(stack, type)

#define fixed_xifo_view_push_back(stack, ptr) \
	circular_buffer_push_back(stack, &ptr)
#define fixed_xifo_copy_push_back(stack, ptr) \
	circular_buffer_push_back(stack, ptr)
#define fixed_xifo_view_pop_back(stack, ptr) \
	circular_buffer_pop_back(stack, ptr)
#define fixed_xifo_copy_pop_back(stack, ptr) \
	circular_buffer_pop_back(stack, ptr)

#define fixed_xifo_view_push_front(stack, ptr) \
	circular_buffer_push_front(stack, &ptr)
#define fixed_xifo_copy_push_front(stack, ptr) \
	circular_buffer_push_front(stack, ptr)
#define fixed_xifo_view_pop_front(stack, ptr) \
	circular_buffer_pop_front(stack, ptr)
#define fixed_xifo_copy_pop_front(stack, ptr) \
	circular_buffer_pop_front(stack, ptr)
#endif	// STACK_IMPL_DYNARRAY

#endif	// !FIXED_XIFO_VIEW_H
