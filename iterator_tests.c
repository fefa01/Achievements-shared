// iterator_tests.c

/*
 * =========================================
 * SECTION: Includes and Macros
 * =========================================
 */

#include <CUnit/Basic.h>
#include "iterator.h"

/*
 * =========================================
 * SECTION: Private Function Definitions
 * =========================================
 */

/// @brief Equality function for elements.
/// @param a First element.
/// @param b Second element.
/// @return true if elements are equal, false otherwise.
static bool elem_eq(elem_t a, elem_t b) {
    // Implement comparison logic based on the type stored
    // For simplicity, let's assume we're comparing integers
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

void test_iter_create_NULL_list(){
    CU_ASSERT_PTR_NULL(ioopm_iterator_create(NULL));
}

void test_iter_create_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    CU_ASSERT_PTR_NOT_NULL(iter);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_create_none_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    ioopm_linked_list_append(list , int_elem(1));

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    CU_ASSERT_PTR_NOT_NULL(iter);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_destroy(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    ioopm_linked_list_append(list , int_elem(1));

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_has_next_NULL_iter(){
    CU_ASSERT_EQUAL(ioopm_iterator_has_next(NULL, NULL), 6);
}

void test_iter_has_next_empty_list(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    
    bool result;

    CU_ASSERT_EQUAL(ioopm_iterator_has_next(iter, &result), 0);
    CU_ASSERT_FALSE(result);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_has_next(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    ioopm_linked_list_append(list , int_elem(1));
    
    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);

    bool result;

    CU_ASSERT_EQUAL(ioopm_iterator_has_next(iter, &result), 0);
    CU_ASSERT(result);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_next(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    for(size_t i = 0; i < 10; ++i){
        ioopm_linked_list_append(list, int_elem(i));
    }

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);

    elem_t next_value;
    for(size_t i = 0; i < 10; ++i){
        CU_ASSERT_EQUAL(ioopm_iterator_next(iter, &next_value), 0);
        CU_ASSERT_EQUAL(next_value.intValue, i);
    }

    CU_ASSERT_EQUAL(ioopm_iterator_next(iter, &next_value), 2);
    CU_ASSERT_EQUAL(next_value.intValue, 9);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_remove(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    for(size_t i = 0; i < 10; ++i){
        ioopm_linked_list_append(list, int_elem(i));
    }

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    elem_t removed;
    size_t list_size = 0;

    CU_ASSERT_EQUAL(ioopm_iterator_remove(iter, &removed), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed.intValue, 0);

    CU_ASSERT_EQUAL(ioopm_iterator_remove(iter, &removed), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed.intValue, 1);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 8);

    ioopm_iterator_next(iter, NULL);    /// 2
    ioopm_iterator_next(iter, NULL);    /// 3
    ioopm_iterator_next(iter, NULL);    /// 4
    CU_ASSERT_EQUAL(ioopm_iterator_remove(iter, &removed), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(removed.intValue, 5);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 7);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_insert(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);
    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    elem_t current;
    size_t list_size = 0;

    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 0);

    CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, int_elem(5)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(current.intValue, 5);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 1);

    CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, int_elem(10)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(current.intValue, 10);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 2);

    CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, int_elem(15)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(current.intValue, 15);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 3);

    CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, int_elem(20)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(current.intValue, 20);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 4);

    CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, int_elem(25)), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(current.intValue, 25);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(list_size, 5);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_reset(){
    ioopm_list_t *list = ioopm_linked_list_create(*elem_eq);

    for(size_t i = 0; i < 10; ++i){
        ioopm_linked_list_append(list, int_elem(i));
    }

    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);

    elem_t next_value;
    for(size_t i = 0; i < 10; ++i){
        CU_ASSERT_EQUAL(ioopm_iterator_next(iter, &next_value), 0);
        CU_ASSERT_EQUAL(next_value.intValue, i);
    }

    CU_ASSERT_EQUAL(ioopm_iterator_reset(iter), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter, &next_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter, &next_value), IOOPM_SUCCESS);
    CU_ASSERT_EQUAL(next_value.intValue, 1);
    

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}


// void test_iter_insert(){
//     ioopm_list_t *list = ioopm_linked_list_create();
//     ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
//     int current = 0;
//     size_t list_size = 0;

//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(list_size, 0);

//     CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, 5), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(current, 5);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(list_size, 1);

//     CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, 10), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, 15), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, 20), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(ioopm_iterator_insert(iter, 25), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(ioopm_iterator_current(iter, &current), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(current, 25);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list, &list_size), IOOPM_SUCCESS);
//     CU_ASSERT_EQUAL(list_size, 5);

//     ioopm_iterator_destroy(iter);
//     ioopm_linked_list_destroy(list);
// }

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
  CU_pSuite my_test_suite = CU_add_suite("Tests for iterator", init_suite, clean_suite);
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
    (CU_add_test(my_test_suite, "Iterator create NULL list", test_iter_create_NULL_list) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator create empty list", test_iter_create_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator create none empty list", test_iter_create_none_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator destroy", test_iter_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator has next NULL iter", test_iter_has_next_NULL_iter) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator has next emty iter", test_iter_has_next_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator has next", test_iter_has_next) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator next", test_iter_next) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator remove", test_iter_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator insert", test_iter_insert) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator reset", test_iter_reset) == NULL) ||
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
