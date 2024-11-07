// iterator.h

#ifndef ITERATOR_H
#define ITERATOR_H


/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#pragma once

#include "linked_list.h"


/*
 * =========================================
 * SECTION: Custom Data Types And Aliases
 * =========================================
 */

typedef struct list_iterator ioopm_list_iterator_t;


/*
 * =========================================
 * SECTION: Function Declarations
 * =========================================
 */

/// @brief Create an iterator for a given list.
/// @param list The list to be iterated over.
/// @return An iterator positioned at the start of the list, or NULL on failure.
ioopm_list_iterator_t *ioopm_iterator_create(ioopm_list_t *list);

/// @brief Destroy the iterator and free its resources.
/// @param iter The iterator to destroy.
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);

/// @brief Check if there are more elements to iterate over.
/// @param iter The iterator.
/// @param result Pointer to store the result (true if more elements exist).
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_iterator_has_next(ioopm_list_iterator_t *iter, bool *result);

/// @brief Move the iterator to the next element.
/// @param iter The iterator.
/// @param next Pointer to store the next value.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_iterator_next(ioopm_list_iterator_t *iter, elem_t *next);


/// @brief Remove the current element from the underlying list.
/// @param iter The iterator.
/// @param removed Pointer to store the removed value.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
/// @note After removal, the iterator's current element will be the next element in the list.
ioopm_status_t ioopm_iterator_remove(ioopm_list_iterator_t *iter, elem_t *removed);

/// @brief Insert a new element into the underlying list, making the current element its next.
/// @param iter The iterator.
/// @param element The element to be inserted.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reset the iterator to the start of the underlying list.
/// @param iter The iterator.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Retrieve the current element from the underlying list.
/// @param iter The iterator.
/// @param current Pointer to store the current value.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_iterator_current(ioopm_list_iterator_t *iter, elem_t *current);






#endif  //ITERATOR_H
