#ifndef STRUCT_HEAP_H
#define STRUCT_HEAP_H

#include <stdlib.h>
#include "ptr.h"

typedef unsigned (*getid_fn_t)(void*);

/**
 * @defgroup heap_view Indexed heap
 * @{
 */

/**
 * @typedef heap_view_t
 * @brief Typedef for the heap_view structure
 *
 */
typedef struct heap_view heap_view_t;

/**
 * @struct heap_view
 * @brief An indexed heap
 *
 * An indexed heap sorts a list of indexes #pos_to_idx based on their values
 * stored in #data using the #compare function.<br>
 * There is #size elements in the heap, with #size <= #capacity, each element
 * should be of size #size_bytes.<br>
 * The position of the idx-th element in the heap
 * can be known thanks to #idx_to_pos which is useful to update an element
 * on-the-fly.
 *
 * @note The heap implementation is a "view" on the data, which means that the
 * array pointed by the #data pointer won't be freed at any moment by the heap.
 * You have to manage it manually and it should remains valid at least as long
 * as the heap is used.
 *
 * @see create_heap() create_heap_no_check()
 */
struct heap_view {
	compare_fn_t compare;
	/**< function used to compare two elements of #data */
	void* data;
	/**< value + idx * size_bytes is the value of the idx-th element */
	unsigned* idx_to_pos;
	/**< idx_to_pos[idx] is the position of the idx-th element in the heap array
	 */
	unsigned* pos_to_idx;
	/**< pos_to_idx[pos] is the index of the element at the position pos in the
	 * heap array. */
	unsigned size;
	/**< Number of elements in the heap */
	size_t size_bytes;
	/**< Size (in bytes) of an element of the array referenced by data */
	unsigned capacity;
	/**< Maximum number of element that the heap can contain */
};

/**
 * @brief Create an empty heap with a fixed capacity
 *
 * Complexity: O(1)
 * @param capacity Number max of element that will be added to the heap
 * @param size_bytes Size (in bytes) of an element of data
 * @param data Pointer to an array of element that can be compared using the
 * compare pointer function. The array should be of size *capacity*
 * @param compare Pointer to a function which takes two pointers to elements of
 * data
 * @return A pointer to the newly created heap
 */
heap_view_t* create_heap(unsigned capacity,
						 size_t size_bytes,
						 void* data,
						 compare_fn_t compare);

// heap_view_t* build_heap(void** tab, unsigned size, compare_fn_t compare);

/**
 * @brief Create a filled heap assuming correct ordering
 *
 * Creates a heap which gets its internal structure directly from the ordering
 * of the array referenced by the data pointer. This array should be of length
 * *size* and contain elements of size *size_bytes* bytes. It will be untouched
 * by this function.
 *
 * When we say that the array referenced by data should have a heap structure it
 * means that for every n, whith @f$u(n) = data[n - 1]@f$, we should have:
 * @f[ u(2n) \leq u(n) @f]
 * and @f[u(2n+1) \leq u(n)@f]
 *
 * **Reminder:** the array data is merely referenced and won't be copied nor
 * freed at any moment. Its allocation and deallocation has to be handled by the
 * developper.
 *
 * Complexity: 0(1)
 * @param size Number of element in the heap
 * @param size_bytes Size (in bytes) of the elements in the heap
 * @param data Pointer to an array with a heap structure
 * @param compare Pointer to a function which takes two pointers to elements of
 * data
 * @return A pointer to the newly created heap.
 */
heap_view_t* create_heap_no_check(unsigned size,
								  size_t size_bytes,
								  void* data,
								  compare_fn_t compare);

/**
 * @brief Return the maximum element of the heap and remove it from the heap
 *
 *
 * Complexity: O(ln(n))
 * @param heap Pointer to the heap
 * @return The index of the top element of the heap
 */
int heap_get_root(heap_view_t* heap);

/**
 * @brief Insert an element into the heap
 *
 * The memory referenced by *elem* pointer should be of heap_view#size_bytes
 * bytes.<br>
 * These bytes will be copied into heap_view#data at index
 * heap_view#size and the index will be rearranged to keep the structure of a
 * heap.
 *
 * Complexity: O(ln(n))
 * @param heap pointer to the heap
 * @param elem pointer to the element to insert in the heap
 * @return heap->size in case of success and -ERROR_CAPACITY_EXCEEDED if the
 * heap is full
 */
int insert_heap(heap_view_t* heap, void* elem);

/**
 * @brief Update the value of an element of the heap toward +inf
 *
 * Update the value of the element idx of the heap.
 * The value passed by parameter elem should be greater than the current value
 * according to the comparison function heap_view#compare
 *
 * Complexity: O(ln(n))
 * @param heap Pointer to the heap
 * @param idx Index of the element to be updated
 * @param elem Pointer to the new value of the idx-th element
 */
void heap_update_up(heap_view_t* heap, unsigned int idx, void* elem);

/**
 * @brief Free a heap
 *
 * Frees the heap and its indexes
 * \warning This function won't free the array referenced by heap_view#data. The
 * memory of this array has to be managed by the developper.
 *
 * Complexity: O(1)
 * @param heap Pointer to the heap
 */
void free_heap(heap_view_t* heap);

/** @} */  // end of heap_view

#endif
