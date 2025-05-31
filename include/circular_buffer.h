#ifndef STRUCT_CBUFFER_H
#define STRUCT_CBUFFER_H

/**
 * @file circular_buffer.h
 * @brief Circular buffers definition
 * Defines functions to create, free and manipulate circular buffers
 * @ingroup cbuffer_ref
 */

#include <stdint.h>
#define get_buffer_first(buffer, type) \
	(type*)((buffer)->data + (buffer)->first * (buffer)->size_bytes)

#define get_buffer_last(buffer, type) \
	(type*)((buffer)->data + (buffer)->last * (buffer)->size_bytes)

/**
 * @defgroup circular_buffer Circular buffers
 * @{
 */

/**
 * @typedef circular_buffer_t
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
 * index of the last element of the buffer.
 */
struct circular_buffer {
	uint8_t* data;
	/**< Pointer to the allocated memory for the circular buffer */
	unsigned size_bytes;
	/**< Size (in bytes) of an element of the buffer */
	unsigned capacity;
	/**< The maximum number of elements in the buffer */
	int size;
	/**< The number of elements in the buffer */
	int first;
	/**< The index of the first element of the buffer */
	int last;
	/**< The index of the last element of the buffer */
};

/**
 * @brief Creates an empty circular buffer
 *
 * __Every array_list created with this function should be freed using
 * circular_buffer_free()__
 *
 * ```
 * circular_buffer_t* buffer = circular_create(sizeof(int), 20);
 * if(buffer == NULL)
 *     goto error;
 *
 * // Use the array here
 *
 * circular_buffer_free(buffer);
 * ```
 *
 * _Complexity_: \f$O(1)\f$
 * @param size_bytes Size (in bytes) of an element in the buffer
 * @param capacity The maximum number of elements in the buffer
 * @return A pointer to the newly created buffer
 * @see circular_buffer_free
 */
circular_buffer_t* circular_buffer_create(unsigned size_bytes,
										  unsigned capacity);

/**
 * @brief Frees a circular buffer
 *
 * _Complexity_: \f$O(1)\f$
 * @param b Pointer to the buffer
 * @see circular_buffer_create
 */
void circular_buffer_free(circular_buffer_t* b);

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
