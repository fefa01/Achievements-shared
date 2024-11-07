// iterator.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "iterator.h"


/*
 * =========================================
 * SECTION: Custom Data Types And Aliases
 * =========================================
 */

/// @brief Struct representing an iterator over a linked list.
struct list_iterator{
    ioopm_list_t *list;
    node_t *current;
    node_t *previous;
};


/*
 * =========================================
 * SECTION: Function Definitions
 * =========================================
 */

ioopm_list_iterator_t *ioopm_iterator_create(ioopm_list_t *list){
    CHECK_NULL(list, "Can not create iterator for a NULL list", NULL);
    
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    CHECK_NULL(iter, "Memory allocation failed for iterator", NULL);

    iter->list = list;
    iter->current = list->head;
    iter->previous = NULL;

    return iter;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter){
    if(!iter) return;

    free(iter);
}

ioopm_status_t ioopm_iterator_has_next(ioopm_list_iterator_t *iter, bool *result){
    CHECK_NULL(iter, "The iterator is NULL, unable to operate on it", IOOPM_ERROR_NULL_ITERATOR);
    
    if(result){
        *result = (iter->current != NULL);
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_iterator_next(ioopm_list_iterator_t *iter, elem_t *next){
    CHECK_NULL(iter, "The iterator is NULL, unable to operate on it", IOOPM_ERROR_NULL_ITERATOR);
    CHECK_NULL(iter->current, "The iterator has no more values to access", IOOPM_ERROR_INVALID_INDEX);

    if(next){
        *next = iter->current->data;
    }

    iter->previous = iter->current;
    iter->current = iter->current->next;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_iterator_remove(ioopm_list_iterator_t *iter, elem_t *removed){
    CHECK_NULL(iter, "The iterator is NULL, unable to operate on it", IOOPM_ERROR_NULL_ITERATOR);
    CHECK_NULL(iter->current, "The iterator has no more values to access", IOOPM_ERROR_INVALID_INDEX);

    node_t *to_remove = iter->current;

    if(iter->previous){
        iter->previous->next = iter->current->next;
    }
    else{
        iter->list->head = iter->current->next;
    }

    if(iter->current == iter->list->tail){
        iter->list->tail = iter->previous;
    }

    if(removed){
        *removed = to_remove->data;
    }

    iter->current = iter->current->next;

    free(to_remove);
    iter->list->size--;

    return IOOPM_SUCCESS;
}

//TODO: there is som bug with the insert function, when insert multiple entries, other operations on iterator does not
//      work properly!
ioopm_status_t ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element){
    CHECK_NULL(iter, "The iterator is NULL, unable to operate on it.", IOOPM_ERROR_NULL_ITERATOR);

    node_t *new_entry = calloc(1, sizeof(node_t));
    CHECK_NULL(new_entry, "Memory allocation failed for the new node.", IOOPM_ERROR_MEMORY_ALLOCATION);

    new_entry->data = element;
    new_entry->next = iter->current;

    if(!(iter->previous)){
        iter->list->head = new_entry;
        if(!(iter->list->tail)){
            iter->list->tail = new_entry;
        }
    } else {
        iter->previous->next = new_entry;
    }

    if(!(iter->current)){
        iter->list->tail = new_entry;
    }

    iter->current = new_entry;
    iter->list->size++;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_iterator_reset(ioopm_list_iterator_t *iter){
    CHECK_NULL(iter, "The iterator is NULL, unable to reset it", IOOPM_ERROR_NULL_ITERATOR);
    
    iter->current = iter->list->head;
    iter->previous = NULL;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_iterator_current(ioopm_list_iterator_t *iter, elem_t *current){
    CHECK_NULL(iter, "The iterator is NULL, unable to retrieve current element", IOOPM_ERROR_NULL_ITERATOR);
    CHECK_NULL(iter->current, "Iterator is not pointing to a valid element (end of list or not started)", IOOPM_ERROR_INVALID_INDEX);
    
    if(current){
        *current = iter->current->data;
    }

    return IOOPM_SUCCESS;
}
