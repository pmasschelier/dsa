#ifndef STRUCT_CBUFFER_H
#define STRUCT_CBUFFER_H

/**
 * @file circular_buffer.h
 * @brief Circular buffers definition
 * Defines functions to create, free and manipulate circular buffers
 * @ingroup cbuffer_ref
 */

#define get_buffer_last(buffer, type) \
	(type*)((buffer)->data + buffer->last * buffer->size_bytes)

/**
 * @defgroup cbuffer_ref Circular buffers
 * @{
 */

/**
 * @typedef cbuffer_ref_t
 * @brief Typedef for the circular_buffer structure
 *
 */
typedef struct circular_buffer circular_buffer_t;

/**
 * @struct circular_buffer
 * @brief A circular buffer
 *
 * A circular buffer holds a pointer to its data, the size (in bytes) of an
 * element of the buffer, the max number of elements in the buffer, the actual
 * number of elements in the buffer, the index of the first element and the
 * index of the last element of the buffer. (NULL if it is the first node) and a
 * pointer to its successor (NULL if it is the last node)
 */
struct circular_buffer {
	char* data; /**< Pointer to the allocated memory for the circular buffer */
	unsigned size_bytes; /**< Size (in bytes) of an element of the buffer */
	unsigned capacity;	 /**< The maximum number of elements in the buffer */
	int first;			 /**< The index of the first element of the buffer */
	int last;			 /**< The index of the last element of the buffer */
	int size;			 /**< The number of elements in the buffer */
};

circular_buffer_t* create_circular_buffer(unsigned size_bytes,
										  unsigned capacity);

/**
 * @brief Frees a circular buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 */
void free_circular_buffer(circular_buffer_t* b);

/**
 * @brief Removes an element at the beginning of the buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @param p If p != NULL the removed element is copied into *p.
 * @return -ERROR_IS_EMPTY if the buffer is empty, ERROR_NO_ERROR
 * otherwise
 */
int circular_buffer_pop_front(circular_buffer_t* b, void* p);

/**
 * @brief Removes an element at the end of the buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @param p If p != NULL the removed element is copied into *p.
 * @return -ERROR_IS_EMPTY if the buffer is empty, ERROR_NO_ERROR
 * otherwise
 */
int circular_buffer_pop_back(circular_buffer_t* b, void* p);

/**
 * @brief Adds an element at the beginning of the buffer
 *
 * A shallow copy of the element is made into the buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @param p Pointer to the element to copy
 * @return -ERROR_CAPACITY_EXCEEDED if the buffer is full, ERROR_NO_ERROR
 * otherwise
 */
int circular_buffer_push_front(circular_buffer_t* b, void* d);

/**
 * @brief Adds an element at the end of the buffer
 *
 * A shallow copy of the element is made into the buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @param p Pointer to the element to copy
 * @return -ERROR_CAPACITY_EXCEEDED if the buffer is full, ERROR_NO_ERROR
 * otherwise
 */
int circular_buffer_push_back(circular_buffer_t* b, void* d);

/**
 * @brief Returns the size of the buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @return circular_buffer#size
 */
int circular_buffer_size(circular_buffer_t* b);

#endif	// !STRUCT_CBUFFER_H
