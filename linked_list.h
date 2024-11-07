// linked_list.h

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#pragma once

#include <stdbool.h>
#include "common.h"

/// @brief Macro to create an integer element.
/// @param x The integer value.
/// @return An elem_t containing the integer.
#define int_elem(x) (elem_t){.intValue = x}

/// @brief Macro to create a pointer element.
/// @param x The pointer value.
/// @return An elem_t containing the pointer.
#define ptr_elem(x) (elem_t){.ptrValue = x}

/// @brief Macro to log an error message to stderr.
/// @param msg The error message.
#define LOG_ERROR(msg)  fprintf(stderr, "ERROR: %s:%d  %s \n", __FILE__, __LINE__, msg)

/// @brief Macro to check if a parameter is NULL and return an error if so.
/// @param param The parameter to check.
/// @param msg The error message.
/// @param error_code The error code to return.
#define CHECK_NULL(param, msg, error_code)          \
    do{                                             \
        if(!(param)){                               \
            LOG_ERROR(#msg);                        \
            return (error_code);                    \
        }                                           \
    } while(0)


/*
 * =========================================
 * SECTION: Custom Data Types And Aliases
 * =========================================
 */

typedef bool ioopm_predicate_lists(elem_t value, void *extra);       
typedef void ioopm_apply_function_lists(elem_t *value, void *extra);    
typedef struct list ioopm_list_t;
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);
typedef struct node node_t;
typedef enum ioopm_status ioopm_status_t;

struct list{
    node_t *head;                   /// Pointer to the first node in the list.
    node_t *tail;                   /// Pointer to the last node in the list.
    size_t size;                    /// Number of elements in the list.
    ioopm_eq_function eq_func;      /// Function to compare elements for equality.
};

struct node {
    elem_t data;        /// The data stored in the node.
    node_t *next;       /// Pointer to the next node in the list.
};

enum ioopm_status{
    IOOPM_SUCCESS,
    IOOPM_ERROR_NULL_LIST,
    IOOPM_ERROR_INVALID_INDEX,
    IOOPM_ERROR_MEMORY_ALLOCATION,
    IOOPM_ERROR_NULL_PROPERTY,
    IOOPM_ERROR_NULL_FUNCTION,
    IOOPM_ERROR_NULL_ITERATOR
};

/*
 * =========================================
 * SECTION: Function Declarations
 * =========================================
 */

/// @brief Creates a new empty linked list.
/// @param eq_func Function to compare elements for equality.
/// @return Pointer to the new linked list.
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func);

/// @brief Destroys the linked list and frees its memory.
/// @param list The linked list to destroy.
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Appends a value to the end of the linked list.
/// @param list The linked list.
/// @param value The value to append.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Prepends a value to the beginning of the linked list.
/// @param list The linked list.
/// @param value The value to prepend.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Inserts a value at a specific index in the linked list.
/// @param list The linked list.
/// @param index The index to insert at.
/// @param value The value to insert.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value);

/// @brief Removes a value at a specific index from the linked list.
/// @param list The linked list.
/// @param index The index of the value to remove.
/// @param removed_value Pointer to store the removed value.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index, elem_t *removed_value);

/// @brief Retrieves a value at a specific index from the linked list.
/// @param list The linked list.
/// @param index The index of the value to retrieve.
/// @param retrieved_values Pointer to store the retrieved value.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_get(ioopm_list_t *list, size_t index, elem_t *retrieved_values);

/// @brief Checks if the linked list contains a specific element.
/// @param list The linked list.
/// @param element The element to search for.
/// @param result Pointer to store the result (true if found, false otherwise).
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, bool *result);

/// @brief Retrieves the size of the linked list.
/// @param list The linked list.
/// @param size Pointer to store the size.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_size(ioopm_list_t *list, size_t *size);

/// @brief Checks if the linked list is empty.
/// @param list The linked list.
/// @param result Pointer to store the result (true if empty, false otherwise).
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_is_empty(ioopm_list_t *list, bool *result);

/// @brief Clears all elements from the linked list.
/// @param list The linked list.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Checks if a property holds for all elements in the linked list.
/// @param list The linked list.
/// @param prop The property function to apply.
/// @param extra Additional argument to pass to the property function.
/// @param result Pointer to store the result (true if property holds for all, false otherwise).
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate_lists *prop, void *extra, bool *result);

/// @brief Checks if a property holds for any element in the linked list.
/// @param list The linked list.
/// @param prop The property function to apply.
/// @param extra Additional argument to pass to the property function.
/// @param result Pointer to store the result (true if property holds for any, false otherwise).
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate_lists *prop, void *extra, bool *result);

/// @brief Applies a function to all elements in the linked list.
/// @param list The linked list.
/// @param fun The function to apply.
/// @param extra Additional argument to pass to the function.
/// @return IOOPM_SUCCESS on success, or an appropriate error code on failure.
ioopm_status_t ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function_lists *fun, void *extra);






#endif // LINKED_LIST_H
