// hash_table_tests.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

/// @brief Macro to find the index of a value in an array.
/// @param array The array to search.
/// @param size The size of the array.
/// @param value The value to find.
/// @param index The variable to store the index if found (-1 if not found).
#define FIND_INDEX(array, size, value, index)     \
  do{                                             \
    index = -1;                                   \
    for(size_t i = 0; i < (size); ++i){           \
      if((array)[i] == (value)){                  \
        index = (int)i;                           \
        break;                                    \
      }                                           \
    }                                             \
  } while(0)

/// @brief Number of keys used in tests.
#define NUM_KEYS 10


/*
 * =========================================
 * SECTION: Private Function Definitions
 * =========================================
 */

/// @brief Hash function for integer keys.
/// @param key The key to hash.
/// @return The hash value.
static size_t int_hash_function(elem_t key) {
    return (size_t)key.intValue;
}

/// @brief Equality function for integer keys.
/// @param a First integer key.
/// @param b Second integer key.
/// @return true if keys are equal, false otherwise.
static bool int_eq_function(elem_t a, elem_t b) {
    return a.intValue == b.intValue;
}

/*
/// @brief Hash function for string keys.
/// @param key The key to hash.
/// @return The hash value.
static size_t string_hash_function(elem_t key) {
    char *str = key.ptrValue;
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
*/

/// @brief Equality function for string keys.
/// @param a First string key.
/// @param b Second string key.
/// @return true if keys are equal, false otherwise.
static bool string_eq_function(elem_t a, elem_t b) {
    return strcmp(a.ptrValue, b.ptrValue) == 0;
}

/// @brief Predicate function to compare values.
/// @param key The key (unused).
/// @param value The value to compare.
/// @param extra The target value to compare against.
/// @return true if values are equal, false otherwise.
static bool value_equals(elem_t key, elem_t value, void *extra) {
  (void)key; // Suppress unused variable warning
  char *target_value = (char *)extra;
  return strcmp(value.ptrValue, target_value) == 0;
}

/// @brief Apply function to append a suffix to string values.
/// @param key The key (unused).
/// @param value Pointer to the value to modify.
/// @param extra The suffix to append.
static void append_suffix(elem_t key, elem_t *value, void *extra) {
  (void)key;
  char *suffix = (char *)extra;

  char *original_value = (char *)value->ptrValue;
  size_t new_length = strlen(original_value) + strlen(suffix) + 1;
  char *new_value = calloc(new_length, sizeof(char));
  if (new_value == NULL) {
      return;
  }

  strcpy(new_value, original_value);
  strcat(new_value, suffix);

  value->ptrValue = new_value;
}

/// @brief Apply function to destroy string values.
/// @param key The key (unused).
/// @param value Pointer to the value to destroy.
/// @param extra Unused extra parameter.
static void destroy_value(elem_t key, elem_t *value, void *extra){
  (void)key;
  (void)extra;
  free(value->ptrValue);
  value->ptrValue = NULL;
}


/*
 * =========================================
 * SECTION: Initialize and clean the suite
 * =========================================
 */

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}


/*
 * =========================================
 * SECTION: Tests
 * =========================================
 */

void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

  CU_ASSERT_PTR_NOT_NULL(ht);
   
  ioopm_hash_table_destroy(ht);
}

void test_insert_lookup(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);
    
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));

    option_t result;

    result = ioopm_hash_table_lookup(ht, int_elem(1));
    CU_ASSERT(Successful(result));
    CU_ASSERT_STRING_EQUAL(result.value.ptrValue, "One");

    result = ioopm_hash_table_lookup(ht, int_elem(2));
    CU_ASSERT(Successful(result));
    CU_ASSERT_STRING_EQUAL(result.value.ptrValue, "Two");

    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two updated"));
    result = ioopm_hash_table_lookup(ht, int_elem(2));
    CU_ASSERT(Successful(result));
    CU_ASSERT_STRING_EQUAL(result.value.ptrValue, "Two updated");

    result = ioopm_hash_table_lookup(ht, int_elem(99));
    CU_ASSERT(Unsuccessful(result));
    
    ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);
   
   for (size_t i = 0; i < 18; ++i) /// 18 is number of buckets + 1
     {
       CU_ASSERT(Unsuccessful(ioopm_hash_table_lookup(ht, int_elem((int)i))));
     }
   
   ioopm_hash_table_destroy(ht);
}

void test_remove_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);
    
    for(size_t i = 0; i < No_Buckets; ++i){
        CU_ASSERT(Unsuccessful(ioopm_hash_table_remove(ht, int_elem((int)i))));
    }

    ioopm_hash_table_destroy(ht);
}

void test_null_value_remove() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    for (size_t i = 0; i < No_Buckets; ++i) {
        ioopm_hash_table_insert(ht, int_elem((int)i), ptr_elem(NULL));
    }

    for (size_t i = 0; i < No_Buckets; ++i) {
        CU_ASSERT(Successful(ioopm_hash_table_remove(ht, int_elem((int)i))));
    }

    ioopm_hash_table_destroy(ht);
}

void test_same_entry_remove() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(18), ptr_elem("Eighteen"));
    ioopm_hash_table_insert(ht, int_elem(35), ptr_elem("Thirtyfive"));
    ioopm_hash_table_insert(ht, int_elem(52), ptr_elem("Fiftytwo"));
    ioopm_hash_table_insert(ht, int_elem(69), ptr_elem("Sixtynine"));

    option_t removed;

    removed = ioopm_hash_table_remove(ht, int_elem(1));
    CU_ASSERT(Successful(removed));
    CU_ASSERT_STRING_EQUAL(removed.value.ptrValue, "One");

    removed = ioopm_hash_table_remove(ht, int_elem(35));
    CU_ASSERT(Successful(removed));
    CU_ASSERT_STRING_EQUAL(removed.value.ptrValue, "Thirtyfive");

    removed = ioopm_hash_table_remove(ht, int_elem(69));
    CU_ASSERT(Successful(removed));
    CU_ASSERT_STRING_EQUAL(removed.value.ptrValue, "Sixtynine");

    CU_ASSERT(Unsuccessful(ioopm_hash_table_remove(ht, int_elem(70))));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_size() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Three"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Four"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 4);

    ioopm_hash_table_remove(ht, int_elem(1));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(18), ptr_elem("Eighteen"));
    ioopm_hash_table_insert(ht, int_elem(35), ptr_elem("Thirtyfive"));
    ioopm_hash_table_insert(ht, int_elem(52), ptr_elem("Fiftytwo"));
    ioopm_hash_table_insert(ht, int_elem(69), ptr_elem("Sixtynine"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 8);

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_clear(ht);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_clear() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_clear(ht);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(18), ptr_elem("Eighteen"));
    ioopm_hash_table_insert(ht, int_elem(35), ptr_elem("Thirtyfive"));
    ioopm_hash_table_insert(ht, int_elem(52), ptr_elem("Fiftytwo"));
    ioopm_hash_table_insert(ht, int_elem(69), ptr_elem("Sixtynine"));
    ioopm_hash_table_clear(ht);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_keys() {
    int keys[NUM_KEYS] = {3, 10, 42, 0, 99, 2, 7, 4, 5, 6};
    char *values[NUM_KEYS] = {"three", "ten", "fortytwo", "zero", "ninetynine", "two", "seven", "four", "five", "six"};
    bool found[NUM_KEYS] = {false};

    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    for (size_t i = 0; i < NUM_KEYS; ++i) {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }

    ioopm_list_t *key_list = ioopm_hash_table_keys(ht);
    size_t list_size;
    ioopm_linked_list_size(key_list, &list_size);

    for (size_t i = 0; i < list_size; ++i) {
        elem_t key_elem;
        ioopm_linked_list_get(key_list, i, &key_elem);
        int key = key_elem.intValue;

        int index_in_keys_array = -1;
        FIND_INDEX(keys, NUM_KEYS, key, index_in_keys_array);

        if (index_in_keys_array != -1) {
            found[index_in_keys_array] = true;
        } else {
            CU_FAIL("Found a key that was never inserted!");
        }
    }

    for (size_t i = 0; i < NUM_KEYS; ++i) {
        CU_ASSERT(found[i]);
    }

    ioopm_linked_list_destroy(key_list);
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_values() {
    int keys[NUM_KEYS] = {3, 10, 42, 0, 99, 2, 7, 4, 5, 6};
    char *values[NUM_KEYS] = {"three", "ten", "fortytwo", "zero", "ninetynine", "two", "seven", "four", "five", "six"};

    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    for (size_t i = 0; i < NUM_KEYS; ++i) {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }

    ioopm_list_t *key_list = ioopm_hash_table_keys(ht);
    ioopm_list_t *value_list = ioopm_hash_table_values(ht);

    size_t list_size;
    ioopm_linked_list_size(key_list, &list_size);

    for (size_t i = 0; i < list_size; ++i) {
        elem_t key_elem, value_elem;
        ioopm_linked_list_get(key_list, i, &key_elem);
        ioopm_linked_list_get(value_list, i, &value_elem);

        int key = key_elem.intValue;
        char *value = value_elem.ptrValue;

        int index_in_keys_array = -1;
        FIND_INDEX(keys, NUM_KEYS, key, index_in_keys_array);
        if (index_in_keys_array != -1) {
            CU_ASSERT_STRING_EQUAL(value, values[index_in_keys_array]);
        } else {
            CU_FAIL("Found a key that was never inserted!");
        }
    }

    ioopm_linked_list_destroy(key_list);
    ioopm_linked_list_destroy(value_list);
    ioopm_hash_table_destroy(ht);
}

void test_has_key_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(1)));

    ioopm_hash_table_destroy(ht);
}

void test_has_key_same_entry() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(18), ptr_elem("Eighteen"));
    ioopm_hash_table_insert(ht, int_elem(35), ptr_elem("Thirtyfive"));
    ioopm_hash_table_insert(ht, int_elem(52), ptr_elem("Fiftytwo"));
    ioopm_hash_table_insert(ht, int_elem(69), ptr_elem("Sixtynine"));

    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(1)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(69)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(35)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(0)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(40)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(70)));

    for (size_t i = 0; i < 20; ++i) {
        ioopm_hash_table_insert(ht, int_elem((int)(i * No_Buckets)), ptr_elem(NULL));
    }

    for (size_t i = 0; i < 20; ++i) {
        CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem((int)(i * No_Buckets))));
    }

    ioopm_hash_table_destroy(ht);
}


void test_has_key_different_entries() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Three"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Four"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("Five"));

    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(1)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(3)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(5)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(0)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(40)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(70)));

    ioopm_hash_table_destroy(ht);
}

void test_has_value_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("One")));

    ioopm_hash_table_destroy(ht);
}

void test_has_value_same_entry() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(18), ptr_elem("Eighteen"));
    ioopm_hash_table_insert(ht, int_elem(35), ptr_elem("Thirtyfive"));
    ioopm_hash_table_insert(ht, int_elem(52), ptr_elem("Fiftytwo"));
    ioopm_hash_table_insert(ht, int_elem(69), ptr_elem("Sixtynine"));

    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("One")));
    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("Eighteen")));
    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("Sixtynine")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Zero")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Forty")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Seventy")));

    ioopm_hash_table_destroy(ht);
}

void test_has_value_different_entries() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Three"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Four"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("Five"));

    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("One")));
    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("Two")));
    CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem("Three")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Zero")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Forty")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Seventy")));

    ioopm_hash_table_destroy(ht);
}

void test_any_empty_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    bool result = ioopm_hash_table_any(ht, value_equals, NULL);
    CU_ASSERT_FALSE(result);

    ioopm_hash_table_destroy(ht);
}

void test_any_no_match() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Three"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Four"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("Five"));

    char *search_value = "NOT_EXIST";
    bool result = ioopm_hash_table_any(ht, value_equals, (void *)search_value);
    CU_ASSERT_FALSE(result);

    ioopm_hash_table_destroy(ht);
}

void test_any_some_matches() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("One"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Three"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Four"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("Five"));

    char *search_value_one = "One";
    bool result = ioopm_hash_table_any(ht, value_equals, (void *)search_value_one);
    CU_ASSERT(result);

    char *search_value_three = "Three";
    result = ioopm_hash_table_any(ht, value_equals, (void *)search_value_three);
    CU_ASSERT(result);

    char *search_value_five = "Five";
    result = ioopm_hash_table_any(ht, value_equals, (void *)search_value_five);
    CU_ASSERT(result);

    ioopm_hash_table_destroy(ht);
}


void test_all_empty_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    bool result = ioopm_hash_table_all(ht, value_equals, NULL);
    CU_ASSERT_TRUE(result);

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_all_match() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("Same"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("Same"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("Same"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("Same"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("Same"));

    char *search_value = "Same";
    bool result = ioopm_hash_table_all(ht, value_equals, (void *)search_value);
    CU_ASSERT(result);

    ioopm_hash_table_destroy(ht);
}

void test_apply_to_all_modify_values() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("one"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("two"));
    ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("three"));

    // Apply function to append "_modified" to all values
    const char *suffix = "_modified";
    ioopm_hash_table_apply_to_all(ht, append_suffix, (void *)suffix);

    char *value = ioopm_hash_table_lookup(ht, int_elem(1)).value.ptrValue;
    CU_ASSERT_STRING_EQUAL(value, "one_modified");

    value = ioopm_hash_table_lookup(ht, int_elem(2)).value.ptrValue;
    CU_ASSERT_STRING_EQUAL(value, "two_modified");

    value = ioopm_hash_table_lookup(ht, int_elem(3)).value.ptrValue;
    CU_ASSERT_STRING_EQUAL(value, "three_modified");

    ioopm_hash_table_apply_to_all(ht, destroy_value, NULL);
    ioopm_hash_table_destroy(ht);
}

void test_apply_to_all_empty_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash_function, int_eq_function, string_eq_function);

    ioopm_hash_table_apply_to_all(ht, append_suffix, NULL);

    ioopm_hash_table_destroy(ht);
}


/*
 * =========================================
 * SECTION: Main
 * =========================================
 */

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("Unit tests for hash table", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "Create and destroy ht", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Insert and lookup a KV", test_insert_lookup) == NULL) ||
    (CU_add_test(my_test_suite, "Look up through an entire empty hashtable", test_lookup_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Remove all entries through an entire empty hashtable", test_remove_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Remove elements from the same entry", test_same_entry_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Remove elements with value NULL", test_null_value_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Get size of the hash table", test_hash_table_size) == NULL) ||
    (CU_add_test(my_test_suite, "If hash table is empty or not", test_hash_table_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Clear the hash table", test_hash_table_clear) == NULL) ||
    (CU_add_test(my_test_suite, "Checking keys", test_hash_table_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Checking values", test_hash_table_values) == NULL) ||
    (CU_add_test(my_test_suite, "If an empty hash table contains the key", test_has_key_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table contains the key - same entry", test_has_key_same_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table contains the key - different entries", test_has_key_different_entries) == NULL) ||
    (CU_add_test(my_test_suite, "If an empty hash table contains the value", test_has_value_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table contains the value - same entry", test_has_value_same_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table contains the value - different entries", test_has_value_different_entries) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table empty - Any - No matches", test_any_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with some KV:s - Any - No matches", test_any_no_match) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with some KV:s - Any - Some matches", test_any_some_matches) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with some KV:s - All - No mathes", test_all_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with some KV:s - All - All match", test_hash_table_all_all_match) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with some KV:s - apply_all - Applied to all", test_apply_to_all_modify_values) == NULL) ||
    (CU_add_test(my_test_suite, "Hash table with none KV:s - apply_all - Applied to all", test_apply_to_all_empty_table) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
