// linked_list_tests.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <CUnit/Basic.h>
#include <stdlib.h>
#include "linked_list.h"

/*
 * =========================================
 * SECTION: Private Function Definitions
 * =========================================
 */

/// @brief Predicate function to check if a value equals a target value.
/// @param value The value to check.
/// @param extra Pointer to the target value.
/// @return true if values are equal, false otherwise.
static bool value_equal(elem_t value, void *extra){
    if(!extra) return false;
    
    elem_t other_value = *(elem_t *)extra;
    return value.intValue == other_value.intValue;
}

/// @brief Apply function to multiply a value by a factor.
/// @param value Pointer to the value to modify.
/// @param extra Pointer to the multiplier.
static void multiply_value(elem_t *value, void *extra){
    if(!extra) return;

    elem_t multiply_with = *(elem_t *)extra;
    value->intValue *= multiply_with.intValue;
}

/// @brief Equality function for elements.
/// @param a First element.
/// @param b Second element.
/// @return true if elements are equal, false otherwise.
/// @note copmaring only integers
static bool elem_eq(elem_t a, elem_t b) {
    return a.intValue == b.intValue;
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

void test_create_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_PTR_NOT_NULL(list);

    ioopm_linked_list_destroy(list);
}

void test_append_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_append(NULL, int_elem(1)), IOOPM_ERROR_NULL_LIST);
}

void test_append(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(1)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(2)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(3)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(4)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(5)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(6)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(7)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_append(list, int_elem(8)), IOOPM_SUCCESS);

    ioopm_linked_list_destroy(list);
}

void test_prepend_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(NULL, int_elem(1)), IOOPM_ERROR_NULL_LIST);
}

void test_prepend(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(1)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(2)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(3)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(4)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(5)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(6)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(7)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_prepend(list, int_elem(8)), IOOPM_SUCCESS);

    ioopm_linked_list_destroy(list);
}

void test_insert_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_insert(NULL, 1, int_elem(1)), IOOPM_ERROR_NULL_LIST);
}

void test_insert_to_invalid_index(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, -1, int_elem(1)), IOOPM_ERROR_INVALID_INDEX);
    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, 1, int_elem(1)), IOOPM_ERROR_INVALID_INDEX);

    ioopm_linked_list_destroy(list);
}

void test_insert(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, 0, int_elem(1)), IOOPM_SUCCESS);
    
    ioopm_linked_list_insert(list, 0, int_elem(1));
    ioopm_linked_list_insert(list, 1, int_elem(1));
    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, 0, int_elem(1)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, 4, int_elem(1)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_linked_list_insert(list, 5, int_elem(1)), IOOPM_SUCCESS);

    ioopm_linked_list_destroy(list);
}

void test_remove_NULL_list(){
    elem_t removed_value;
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(NULL, 1, &removed_value), IOOPM_ERROR_NULL_LIST);
}

void test_remove_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    elem_t removed_value;
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(list, 0, &removed_value), IOOPM_ERROR_INVALID_INDEX);

    ioopm_linked_list_destroy(list);
}

void test_remove(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    elem_t removed_value;

    ioopm_linked_list_insert(list, 0, int_elem(5));
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(list, 0, &removed_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed_value.intValue, 5);

    ioopm_linked_list_insert(list, 0, int_elem(5));
    ioopm_linked_list_insert(list, 1, int_elem(10));
    ioopm_linked_list_insert(list, 2, int_elem(15));
    ioopm_linked_list_insert(list, 3, int_elem(20));
    ioopm_linked_list_insert(list, 4, int_elem(25));
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(list, 4, &removed_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed_value.intValue, 25);
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(list, 1, &removed_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed_value.intValue, 10);
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(list, 1, NULL), IOOPM_SUCCESS);

    ioopm_linked_list_destroy(list);
}

void test_get_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_get(NULL, 1, NULL), IOOPM_ERROR_NULL_LIST);
}

void test_get_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0, NULL), IOOPM_ERROR_INVALID_INDEX);

    ioopm_linked_list_destroy(list);
}

void test_get(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    elem_t retrieved_value;

    ioopm_linked_list_insert(list, 0, int_elem(5));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0, &retrieved_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(retrieved_value.intValue, 5);

    ioopm_linked_list_insert(list, 0, int_elem(5));
    ioopm_linked_list_insert(list, 1, int_elem(10));
    ioopm_linked_list_insert(list, 2, int_elem(15));
    ioopm_linked_list_insert(list, 3, int_elem(20));
    ioopm_linked_list_insert(list, 4, int_elem(25));
    ioopm_linked_list_insert(list, 5, int_elem(30));
    ioopm_linked_list_insert(list, 6, int_elem(35));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 6, &retrieved_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(retrieved_value.intValue, 35);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 3, &retrieved_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(retrieved_value.intValue, 20);

    ioopm_linked_list_destroy(list);
}

void test_contains_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_contains(NULL, int_elem(1), NULL), IOOPM_ERROR_NULL_LIST);
}

void test_contains_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    
    bool result = true;
    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(1), &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    ioopm_linked_list_destroy(list);
}

void test_contains(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    
    ioopm_linked_list_insert(list, 0, int_elem(5));
    ioopm_linked_list_insert(list, 1, int_elem(10));
    ioopm_linked_list_insert(list, 2, int_elem(15));
    ioopm_linked_list_insert(list, 3, int_elem(20));
    ioopm_linked_list_insert(list, 4, int_elem(25));
    ioopm_linked_list_insert(list, 5, int_elem(50));
    
    bool result = true;

    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(-35), &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(35), &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(15), &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(50), &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    CU_ASSERT_EQUAL(ioopm_linked_list_contains(list, int_elem(5), &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    ioopm_linked_list_destroy(list);
}

void test_size_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_size(NULL, NULL), IOOPM_ERROR_NULL_LIST);
}

void test_size(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    size_t size = 0;

    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(size, 0);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(size, 1);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    ioopm_linked_list_insert(list, 1, int_elem(2));
    ioopm_linked_list_insert(list, 2, int_elem(2));
    ioopm_linked_list_insert(list, 3, int_elem(2));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(size, 5);

    ioopm_linked_list_remove(list, 0, NULL);
    ioopm_linked_list_remove(list, 3, NULL);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(size, 3);

    ioopm_linked_list_destroy(list);
}

void test_is_empty_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_is_empty(NULL, NULL), IOOPM_ERROR_NULL_LIST);
}

void test_is_empty(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    bool result = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_is_empty(list, &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    ioopm_linked_list_insert(list, 0, int_elem(2));
    CU_ASSERT_EQUAL(ioopm_linked_list_is_empty(list, &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    ioopm_linked_list_destroy(list);
}

void test_clear_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_clear(NULL), IOOPM_ERROR_NULL_LIST);
}

void test_clear(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    
    CU_ASSERT_EQUAL(ioopm_linked_list_clear(list), IOOPM_SUCCESS);

    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(100));
    ioopm_linked_list_insert(list, 2, int_elem(1000));
    ioopm_linked_list_insert(list, 3, int_elem(10000));
    ioopm_linked_list_insert(list, 4, int_elem(100000));
    CU_ASSERT_EQUAL(ioopm_linked_list_clear(list), IOOPM_SUCCESS);

    ioopm_linked_list_destroy(list);
}

void test_all_NULL_list(){
    CU_ASSERT_EQUAL(ioopm_linked_list_all(NULL, NULL, NULL, NULL), IOOPM_ERROR_NULL_LIST);
}

void test_all_NULL_property(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, NULL, NULL, NULL), IOOPM_ERROR_NULL_PROPERTY);

    ioopm_linked_list_destroy(list);
}

void test_all_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    bool result = false;

    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, value_equal, NULL, NULL), IOOPM_SUCCESS);

    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, value_equal, NULL, &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    int compare_to = 3;
    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, value_equal, &compare_to, &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    ioopm_linked_list_destroy(list);
}

void test_all(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    bool result = false;

    ioopm_linked_list_insert(list, 0, int_elem(3));
    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, value_equal, NULL, &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    int compare_to = 3;
    CU_ASSERT_EQUAL(ioopm_linked_list_all(list, value_equal, &compare_to, &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    ioopm_linked_list_destroy(list);
}

void test_any(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    bool result = false;

    ioopm_linked_list_insert(list, 0, int_elem(3));
    CU_ASSERT_EQUAL(ioopm_linked_list_any(list, value_equal, NULL, &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    int compare_to = 3;
    CU_ASSERT_EQUAL(ioopm_linked_list_any(list, value_equal, &compare_to, &result), IOOPM_SUCCESS);
    CU_ASSERT(result);

    compare_to = 4;
    CU_ASSERT_EQUAL(ioopm_linked_list_any(list, value_equal, &compare_to, &result), IOOPM_SUCCESS);
    CU_ASSERT_FALSE(result);

    ioopm_linked_list_destroy(list);
}

void test_apply_all_NULL_function(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    int multiplier = 3;

    CU_ASSERT_EQUAL(ioopm_linked_list_apply_to_all(list, NULL, &multiplier), IOOPM_ERROR_NULL_FUNCTION);
    
    ioopm_linked_list_destroy(list);
}

void test_apply_all_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    int multiplier = 3;

    CU_ASSERT_EQUAL(ioopm_linked_list_apply_to_all(list, multiply_value, &multiplier), IOOPM_SUCCESS);
    
    ioopm_linked_list_destroy(list);
}

void test_apply_all(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    for(size_t i = 0; i < 10; ++i){
        ioopm_linked_list_insert(list, i, int_elem(i));
    }

    int multiplier = 3;
    CU_ASSERT_EQUAL(ioopm_linked_list_apply_to_all(list, multiply_value, &multiplier), IOOPM_SUCCESS);

    elem_t retrieved_value;
    for(size_t i = 0; i < 10; ++i){
        CU_ASSERT_EQUAL(ioopm_linked_list_get(list, i, &retrieved_value), IOOPM_SUCCESS);
        CU_ASSERT_EQUAL(retrieved_value.intValue, (i * 3));
    }
    
    ioopm_linked_list_destroy(list);
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
  CU_pSuite my_test_suite = CU_add_suite("Tests for linked lists", init_suite, clean_suite);
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
    (CU_add_test(my_test_suite, "Create an empty list", test_create_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Appending to a NULL list", test_append_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Appending", test_append) == NULL) ||
    (CU_add_test(my_test_suite, "Prepending to a NULL list", test_prepend_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Prepending", test_prepend) == NULL) ||
    (CU_add_test(my_test_suite, "Inserting to a NULL list", test_insert_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Inserting to an invalid index", test_insert_to_invalid_index) == NULL) ||
    (CU_add_test(my_test_suite, "Inserting", test_insert) == NULL) ||
    (CU_add_test(my_test_suite, "Removing NULL list", test_remove_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Removing empty list", test_remove_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Removing", test_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Get NULL list", test_get_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Get empty list", test_get_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Getting", test_get) == NULL) ||
    (CU_add_test(my_test_suite, "Contains NULL list", test_contains_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Contains empty list", test_contains_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Contains", test_contains) == NULL) ||
    (CU_add_test(my_test_suite, "Size NULL list", test_size_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Size", test_size) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty NULL list", test_is_empty_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty", test_is_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Clear NULL list", test_clear_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Clear", test_clear) == NULL) ||
    (CU_add_test(my_test_suite, "All NULL list", test_all_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "All NULL property", test_all_NULL_property) == NULL) ||
    (CU_add_test(my_test_suite, "All empty list", test_all_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "All", test_all) == NULL) ||
    (CU_add_test(my_test_suite, "Any", test_any) == NULL) ||
    (CU_add_test(my_test_suite, "Apply all NULL function", test_apply_all_NULL_function) == NULL) ||
    (CU_add_test(my_test_suite, "Apply all empty list", test_apply_all_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Apply all", test_apply_all) == NULL) ||
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
