// linked_list.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"


/*
 * =========================================
 * SECTION: Function Definitions
 * =========================================
 */

/// @brief Create a new node with the given data.
/// @param data The data to store in the node.
/// @return Pointer to the new node on success, or NULL on failure.
static node_t *create_entry(elem_t data){
    node_t *new_entry = calloc(1, sizeof(node_t));
    if(!new_entry) return NULL;

    new_entry->data = data;
    new_entry->next = NULL;

    return new_entry;
}

/// @brief Get the node at a specific index in the list.
/// @param list The linked list.
/// @param index The index of the node to retrieve.
/// @return Pointer to the node at the given index, or NULL if not found.
static node_t *get_entry_at(ioopm_list_t *list, size_t index){
    if(!list || index >= list->size) return NULL;

    node_t *current = list->head;
    for(size_t i = 0; i < index; ++i){
        if(!current){
            return NULL;
        }

        current = current->next;
    }

    return current;
}


ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func){
    ioopm_list_t *new_list = calloc(1, sizeof(ioopm_list_t));
    if(!new_list) return NULL;

    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    new_list->eq_func = eq_func;

    return new_list;
}

void ioopm_linked_list_destroy(ioopm_list_t *list){
    if(!list) return;

    node_t *current = list->head;
    while(current != NULL){
        node_t *next_entry = current->next;
        free(current);
        current = next_entry;
    }

    free(list);
}

ioopm_status_t ioopm_linked_list_append(ioopm_list_t *list, elem_t value){
    CHECK_NULL(list, "The list is Null, unable to append", IOOPM_ERROR_NULL_LIST);

    node_t *new_entry = create_entry(value);
    CHECK_NULL(new_entry, "Failed to allocate memory for the new entry/node", IOOPM_ERROR_MEMORY_ALLOCATION);

    if(list->tail){
        list->tail->next = new_entry;
    }
    else{
        list->head = new_entry;
    }

    list->tail = new_entry;
    list->size++;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value){
    CHECK_NULL(list, "The list is NULL, failed to prepend", IOOPM_ERROR_NULL_LIST);

    node_t *new_entry = create_entry(value);
    CHECK_NULL(new_entry, "Failed to allocate memory for the new entry/node", IOOPM_ERROR_MEMORY_ALLOCATION);

    if(!list->head){
        list->head = new_entry;
        list->tail = new_entry;
    }
    else{
        new_entry->next = list->head;
        list->head = new_entry;
    }

    list->size++;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value){
    CHECK_NULL(list, "The list is NULL, failed to insert", IOOPM_ERROR_NULL_LIST);
    
    if(index > list->size){
        LOG_ERROR("Invalid index - Out of bounds");
        return IOOPM_ERROR_INVALID_INDEX;
    }

    node_t *new_entry = create_entry(value);
    CHECK_NULL(new_entry, "Failed to allocate memory for the new entry/node", IOOPM_ERROR_MEMORY_ALLOCATION);

    if(index == 0){
        new_entry->next = list->head;
        list->head = new_entry;

        if(list->size == 0){
            list->tail = new_entry;
        }
    }
    else if(index == list->size){
        list->tail->next = new_entry;
        list->tail = new_entry;
    }
    else{
        node_t *current = list->head;
        for(size_t i = 0; i < index - 1; ++i){
            current = current->next;
        }

        new_entry->next = current->next;
        current->next = new_entry;
    }

    list->size++;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index, elem_t *removed_value){
    CHECK_NULL(list, "The list is NULL, failed to remove", IOOPM_ERROR_NULL_LIST);
    
    if(index >= list->size){
        LOG_ERROR("Invalid index - Out of bounds");
        return IOOPM_ERROR_INVALID_INDEX;
    }

    node_t *to_remove;

    if(index == 0){
        to_remove = list->head;
        list->head = list->head->next;
        if(list->size == 1){
            list->tail = NULL;
        }
    }
    else{
        node_t *prev = get_entry_at(list, index - 1);
        if(!prev || !prev->next){
            LOG_ERROR("Failed to locate the entry for removal");
            return IOOPM_ERROR_INVALID_INDEX;
        }
        to_remove = prev->next;
        prev->next = to_remove->next;
        if(index == list->size - 1){
            list->tail = prev;
        }
    }

    if(removed_value){
        *removed_value = to_remove->data;
    }

    free(to_remove);
    list->size--;

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_get(ioopm_list_t *list, size_t index, elem_t *retrieved_values){
    CHECK_NULL(list, "The list is NULL, failed to retrieve", IOOPM_ERROR_NULL_LIST);

    if(index >= list->size){
        LOG_ERROR("Invalid index - Out of bounds");
        return IOOPM_ERROR_INVALID_INDEX;
    }

    node_t *current = list->head;
    for(size_t i = 0; i < index; ++i){
        current = current->next;
    }

    if(retrieved_values){
        *retrieved_values = current->data;
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, bool *result){
    CHECK_NULL(list, "The list is NULL, failed to check containment", IOOPM_ERROR_NULL_LIST);

    node_t *current = list->head;
    for(size_t i = 0; i < list->size; ++i){
        if(list->eq_func(current->data, element)){
            if(result){
                *result = true;
            }

            return IOOPM_SUCCESS;
        }

        current = current->next;
    }

    if(result){
        *result = false;
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_size(ioopm_list_t *list, size_t *size){
    CHECK_NULL(list, "The list is NULL", IOOPM_ERROR_NULL_LIST);
    
    if(size){
        *size = list->size;
    }
    
    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_is_empty(ioopm_list_t *list, bool *result){
    CHECK_NULL(list, "The list is NULL", IOOPM_ERROR_NULL_LIST);

    if(result){
        *result = list->size == 0;
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_clear(ioopm_list_t *list){
    CHECK_NULL(list, "The list is NULL, unable to clear", IOOPM_ERROR_NULL_LIST);

    node_t *current = list->head;
    while(current){
        node_t *next_entry = current->next;
        free(current);
        current = next_entry;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate_lists *prop, void *extra, bool *result){
    CHECK_NULL(list, "The list is NULL", IOOPM_ERROR_NULL_LIST);
    CHECK_NULL(prop, "The proprety is NULL, can not be applied to any element", IOOPM_ERROR_NULL_PROPERTY);

    node_t *current = list->head;
    while(current){
        if(!prop(current->data, extra)){
            if(result){
                *result = false;
            }

            return IOOPM_SUCCESS;
        }

        current = current->next;
    }

    if(result){
        *result = true;
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate_lists *prop, void *extra, bool *result){
    CHECK_NULL(list, "The list is NULL", IOOPM_ERROR_NULL_LIST);
    CHECK_NULL(prop, "The proprety is NULL, can not be applied to any element", IOOPM_ERROR_NULL_PROPERTY);

    node_t *current = list->head;
    while(current){
        if(prop(current->data, extra)){
            if(result){
                *result = true;
            }

            return IOOPM_SUCCESS;
        }

        current = current->next;
    }

    if(result){
        *result = false;
    }

    return IOOPM_SUCCESS;
}

ioopm_status_t ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function_lists *fun, void *extra){
    CHECK_NULL(list, "The list is NULL", IOOPM_ERROR_NULL_LIST);
    CHECK_NULL(fun, "The function is NULL, can not be applied to any element", IOOPM_ERROR_NULL_FUNCTION);

    node_t *current = list->head;
    while(current){
        fun(&current->data, extra);

        current = current->next;
    }

    return IOOPM_SUCCESS;
}
