// hash_tabel.h

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * @file hash_table.h
 * @author Oliver Zandieh, Abolfazi Qanbari
 * @date 2024-10-10
 * @brief Generic hash table that maps keys to values.
 *
 * This header defines the interface for a generic hash table implementation.
 * The hash table can store any data types for keys and values using the elem_t union.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#pragma once

#include <stdbool.h>
#include "linked_list.h"

#define Success(v)      (option_t){.success = true, .value = v}
#define Failure()       (option_t){.success = false , .value = (elem_t){0}}
#define Successful(o)   (o.success == true)
#define Unsuccessful(o) (o.success == false)

#define No_Buckets 4096

/*
 * =========================================
 * SECTION: Custom Data Types And Aliases
 * =========================================
 */

typedef struct hash_table ioopm_hash_table_t;
typedef struct option option_t;
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);  //Changed to void to work with append_suffix
typedef size_t (*ioopm_hash_function)(elem_t key);
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);

struct option
{
  bool success;
  elem_t value;
};


/*
 * =========================================
 * SECTION: Function Declarations
 * =========================================
 */

/// @brief Create a new hash table.
/// @param hash_func Function used to hash keys.
/// @param key_eq_func Function used to compare keys for equality.
/// @param value_eq_func Function used to compare values for equality.
/// @return A new empty hash table, or NULL if memory allocation fails.
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func);

/// @brief Delete a hash table and free its memory.
/// @param ht A hash table to be deleted.
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief Add or update a key-value entry in the hash table.
/// @param ht Hash table operated upon.
/// @param key Key to insert or update.
/// @param value Value to associate with the key.
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief Lookup the value associated with a key in the hash table.
/// @param ht Hash table operated upon.
/// @param key Key to lookup.
/// @return An option_t containing the value if found, or indicating failure otherwise.
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief Remove any mapping from key to a value in the hash table.
/// @param ht Hash table operated upon.
/// @param key Key to remove.
/// @return An option_t containing the removed value if key existed, or indicating failure otherwise.
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief Returns the number of key-value entries in the hash table.
/// @param ht Hash table operated upon.
/// @return The number of key-value entries in the hash table.
int ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief Checks if the hash table is empty.
/// @param ht Hash table operated upon.
/// @return true if the hash table is empty, false otherwise.
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief Clear all the entries in a hash table.
/// @param ht Hash table operated upon.
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief Return the keys for all entries in the hash table.
/// @param ht Hash table operated upon.
/// @return A linked list containing all keys in the hash table.
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief Return the values for all entries in the hash table.
/// @param ht Hash table operated upon.
/// @return A linked list containing all values in the hash table.
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief Check if a hash table has an entry with a given key.
/// @param ht Hash table operated upon.
/// @param key The key sought.
/// @return true if the key exists in the hash table, false otherwise.
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief Check if a hash table has an entry with a given value.
/// @param ht Hash table operated upon.
/// @param value The value sought.
/// @return true if the value exists in the hash table, false otherwise.
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief Check if a predicate is satisfied by all entries in the hash table.
/// @param ht Hash table operated upon.
/// @param pred The predicate function to apply to each entry.
/// @param arg Extra argument passed to the predicate function.
/// @return true if the predicate returns true for all entries, false otherwise.
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief Check if a predicate is satisfied by any entry in the hash table.
/// @param ht Hash table operated upon.
/// @param pred The predicate function to apply to each entry.
/// @param arg Extra argument passed to the predicate function.
/// @return true if the predicate returns true for any entry, false otherwise.
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief Apply a function to all entries in the hash table.
/// @param ht Hash table operated upon.
/// @param apply_fun The function to apply to each entry.
/// @param arg Extra argument passed to the apply function.
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg);



#endif // HASH_TABLE_H
