// hash_table.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"

/*
 * =========================================
 * SECTION: Custom Data Types And Aliases
 * =========================================
 */

struct elem
{
    int integer;
    bool boolean;
    float floater;
    unsigned int unint;
    void *p;
    char *str;
};

typedef struct elem elem_t; //jag la till så man kan köra
typedef struct entry entry_t;

struct entry
{
  elem_t key;
  elem_t value;
  entry_t *next;
};

struct hash_table
{
  entry_t *buckets[No_Buckets];   
  size_t size;
  ioopm_hash_function hash_func;
  ioopm_eq_function key_eq_func;
  ioopm_eq_function value_eq_func;
};

//Förklara gärna mer vad det är för strukt, vad är target_elem samt eq_args_t?
typedef struct {
  ioopm_eq_function eq_func;
  elem_t target_elem;
} eq_args_t;


/*
 * =========================================
 * SECTION: Function Definitions
 * =========================================
 */

/// @brief Creates a new entry.
/// @param key The key of the entry.
/// @param value The value associated with the key.
/// @param next Pointer to the next entry in the linked list (may be NULL).
/// @return A pointer to the newly created entry, or NULL if memory allocation fails.
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next){
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  if(!new_entry) return NULL;

  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;

  return new_entry;
}

/// @brief fis a linked list of entries starting from the given entry.
/// @param entry The starting entry of the linked list to destroy.
static void entry_destroy(entry_t *entry){
  while(entry){
    entry_t *next_entry = entry->next;
    free(entry);
    entry = next_entry;
  }
}

/// @brief Finds the entry before the entry containing the given key.
/// @param first_entry The first entry in the linked list (may be a dummy head).
/// @param key The key to search for.
/// @param key_eq_func Function to compare keys for equality.
/// @return A pointer to the entry before the one containing the key, or NULL if not found.
static entry_t *find_previous_entry_for_key(entry_t *first_entry, elem_t key, ioopm_eq_function key_eq_func){
  if(!first_entry) return NULL;

  entry_t *cursor = first_entry;
  while(cursor->next && key_eq_func(cursor->next->key, key) == false){
    cursor = cursor->next;
  }

  return cursor;
}

/// @brief Predicate function to check if a key matches a target key.
/// @param key The key to check.
/// @param value The associated value (unused).
/// @param extra A pointer to an eq_args_t struct containing the target key and equality function.
/// @return true if the key matches the target key, false otherwise.
static bool key_match(elem_t key, elem_t value, void *extra){
  eq_args_t *args = (eq_args_t *)extra;

  return args->eq_func(key, args->target_elem);
}

/// @brief Predicate function to check if a value matches a target value.
/// @param key The key (unused).
/// @param value The value to check.
/// @param extra A pointer to an eq_args_t struct containing the target value and equality function.
/// @return true if the value matches the target value, false otherwise.
static bool value_match(elem_t key, elem_t value, void *extra){
  eq_args_t *args = (eq_args_t *)extra;

  return args->eq_func(value, args->target_elem);
}



ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func){
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  if(!ht) return NULL;
  //Känns som att ha en dummy node för varje bucket göra ht onödigt stort?
  for(size_t i = 0; i < No_Buckets; ++i){
    ht->buckets[i] = entry_create(int_elem(0), ptr_elem(NULL), NULL);
  }

  ht->size = 0;
  ht->hash_func = hash_func;
  ht->key_eq_func = key_eq_func;
  ht->value_eq_func = value_eq_func;

  return ht;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  for(size_t i = 0; i < No_Buckets; ++i){
    entry_t *entry = ht->buckets[i];
    entry_destroy(entry);
  }
  
  free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
  int bucket = ((ht->hash_func(key) % No_Buckets) + No_Buckets) % No_Buckets;

  entry_t *entry = find_previous_entry_for_key(ht->buckets[bucket], key, ht->key_eq_func);
  entry_t *next = entry->next;

  if(next && ht->key_eq_func(next->key, key)){
    next->value = value;
  }
  else{
    entry->next = entry_create(key, value, next);
    ht->size += 1;
  }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key){
  entry_t *tmp = find_previous_entry_for_key(ht->buckets[ht->hash_func(key) % No_Buckets], key, ht->key_eq_func);
  entry_t *next = tmp->next;

  if(next && ht->key_eq_func(next->key, key)){
    return Success(next->value);
  }

  return Failure();
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key){
  entry_t *dummy = ht->buckets[ht->hash_func(key) % No_Buckets]; //segfault om ht->buckets är null?

  entry_t *prev = dummy;
  entry_t *current = prev->next;

//hur hanteras det om det är den första bucketen som ska bort?
  while(current){
    if(ht->key_eq_func(current->key, key)){
      prev->next = current->next;
      elem_t value = current->value;
      free(current);
      //Sätta current till NULL? förebygga för ev. dangling pointers
      ht->size -= 1;
      return Success(value);
    }

    prev = current;
    current = current->next;
  }

  return Failure();
}

int ioopm_hash_table_size(ioopm_hash_table_t *ht){
  if(!ht) return 0;

  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht){
  return !ht || ht->size == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht){
  if(!ht) return;

  for(size_t i = 0; i < No_Buckets; ++i){
    if(ht->buckets[i]){
      entry_t *entry = ht->buckets[i]->next;
      entry_destroy(entry);
      ht->buckets[i]->next = NULL;
    }
  }

  ht->size = 0;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) {
  if (!ht) return NULL;

  ioopm_list_t *keys_list = ioopm_linked_list_create(ht->key_eq_func);

  for (size_t i = 0; i < No_Buckets; ++i) {
    entry_t *entry = ht->buckets[i]->next; // Skip the dummy head if you have one
    while (entry) {
      ioopm_linked_list_append(keys_list, entry->key);
      entry = entry->next;
    }
  }

  return keys_list;
}


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
  if (!ht) return NULL;

  ioopm_list_t *values_list = ioopm_linked_list_create(ht->value_eq_func);

  for (size_t i = 0; i < No_Buckets; ++i) {
    entry_t *entry = ht->buckets[i]->next; // Skip the dummy head if you have one
    while (entry) {
      ioopm_linked_list_append(values_list, entry->value);
      entry = entry->next;
    }
  }

  return values_list;
}


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key){
  eq_args_t args = {.eq_func = ht->key_eq_func, .target_elem = key};

  return ioopm_hash_table_any(ht, key_match, &args);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value){
  eq_args_t args = {.eq_func = ht->value_eq_func, .target_elem = value};

  return ioopm_hash_table_any(ht, value_match, &args);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  if(!ht || !pred) return false;
  //Kan vara förvirrande utan att ha ngt felmeddelande som säger vad som är fel för !ht eller !pred
  // då kanske inte användaren vet om det inte fanns någon som matchade predikatet eller om det ör ett tomt ht exempelvis

  for(size_t i = 0; i < No_Buckets; ++i){
    entry_t *entry = ht->buckets[i]->next;
    while(entry){
      if(pred(entry->key, entry->value, arg)){
        return true;
      }

      entry = entry->next;
    }
  }

  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  if(!ht || !pred) return false;

  for(size_t i = 0; i < No_Buckets; ++i){
    entry_t *entry = ht->buckets[i]->next;

    while(entry){
      if(!pred(entry->key, entry->value, arg)){
        return false;
      }

      entry = entry->next;
    }
  }

  return true;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg){
  if(!ht || !apply_fun) return;

  for(size_t i = 0; i < No_Buckets; ++i){
    entry_t *entry = ht->buckets[i]->next;
    while(entry){
      apply_fun(entry->key, &(entry->value), arg);
      entry = entry->next;
    }
  }
}
