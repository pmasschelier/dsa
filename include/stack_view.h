#ifndef STACK_VIEW_H
#define STACK_VIEW_H

#ifdef STACK_IMPL_LIST_REF
#include "list_ref/list_ref.h"
#include "test_macros.h"

typedef list_ref_t stack_view_t;

static inline stack_view_t* create_stack_view(size_t size) {
	stack_view_t* stack = create_list(size);
	when_null_ret(stack, NULL);
	stack->free_element = NULL;
	return stack;
}

#define create_stack_copy(size) create_list(size)
#define empty_stack(stack) empty_list(stack)
#define free_stack(stack) free_list(stack)
#define stack_view_push(stack, ptr) push_back_list(stack, ptr)
#define stack_copy_push(stack, ptr) append_back_list(stack, ptr)
#define stack_copy_pop(stack, ptr)               \
	do {                                         \
		memcpy(ptr, stack->end->p, stack->size); \
		pop_back_list(stack, NULL);              \
	} while (0);
#define stack_view_pop(stack, ptr) pop_back_list(stack, ptr)
#define stack_view_peak(stack, type) \
	((stack)->end == NULL ? NULL : (type*)(stack)->end->p)
#endif	// STACK_IMPL_LIST_REF

#ifdef STACK_IMPL_DYNARRAY
#include "dynarray.h"

typedef dynarray_t stack_view_t;

#define create_stack_view(size) create_dynarray(sizeof(void*))
#define create_stack_copy(size) create_dynarray(size)
#define empty_stack(stack) dynarray_empty(stack)
#define free_stack(stack) free_dynarray(stack)
#define stack_view_push(stack, ptr) dynarray_push_back(stack, &ptr);
#define stack_copy_push(stack, ptr) dynarray_push_back(stack, ptr);
#define stack_view_pop(stack, ptr) dynarray_pop_back(stack, ptr);
#define stack_copy_pop(stack, ptr) dynarray_pop_back(stack, ptr);
#define stack_view_peak(stack, type)                                      \
	((stack)->size == 0 ? NULL                                            \
						: *(type**)((stack)->data + ((stack)->size - 1) * \
														(stack)->size_bytes))
#define stack_copy_peak(stack, type) \
	((stack)->size == 0              \
		 ? NULL                      \
		 : (type*)(stack)->data + ((stack)->size - 1) * stack->size_bytes)
#endif	// STACK_IMPL_DYNARRAY

#endif	// !STACK_VIEW_H
