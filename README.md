# How to build

    To build the freq-count program and all the necessary run: make compile_fc
    For building different kind of libraries seperately just run:make compile_linked_list,
     make compile_hash_table,
     make compile_iterator.
     To run all the tests run: make test
     Remember to run: make clean between testing.

     The line coverage and branch coverage using gcov for:
      hash_table.c have a lines executed:99,32% of 148
      linked_list.c have a lines executed:98,58% of 211

# Documentation

    Failure handeling:
        In the hasthable are handeled by using a struct option that states that if the operation was succesfull or not and then the value of the elem_t if the operation was succsesfull.

        In the linked_list we are using enum ioopm_status with different error codes indicating the different kinds of errors. The actual value of the element are returned by using an outpoint-pointer. The same failure handeling in the iterators.

    Assumptions about datastructures:
       In the hastable we are using three different kinds of data structures, the first one is struct entry that contains key, value and next entry, the second data structure is the hastable itself, containing an array of buckets the, size of the hastable and three function pointers that are the hash function, key equal function and value equal function that are used for hashing and equality comparisons. The third data structure is the eq_args_t that are used for passing the equality function and target element in some functions.

# Initial Profiling Results

_Top 3_
**inputfile small.txt**
top 3 functions: 1. string_eq  
 Calls:129
Self Time: 0.00 seconds 2. find_previous_entry_for_key
Calls:81
Self Time: 0.00 seconds 3. string_sum_hash
Calls:81
Self Time: 0.00 seconds

**inputfile 10k-words.txt**
top 3 functions: 1. find_previous_entry_for_key  
 Calls:20199
Self Time: 0.01 seconds 2. string_eq
Calls:141528
Self Time: 0.00 seconds 3. string_sum_hash
Calls:20199
Self Time: 0.00 seconds
**inputfile 16k-words.txt**
top 3 functions: 1. find_previous_entry_for_key  
 Calls:38354
Self Time: 0.05 seconds 2. string_eq
Calls:3083092
Self Time: 0.00 seconds 3. string_sum_hash
Calls:38354
Self Time: 0.00 seconds
**inputfile 1k-long-words.txt**
top 3 functions: 1. string_eq  
 Calls:5475
Self Time: 0.00 seconds 2. find_previous_entry_for_key
Calls:2023
Self Time: 0.00 seconds 3. string_sum_hash
Calls:2023
Self Time: 0.00 seconds

All these functions are written by us.

_Consistensy_
In small.txt and 1k-long-words.txt, string_eq and find_previous_entry_for_key appear together.
Wheras in the larger files, find_previous_entry_for_key becomes more prominent, indicating that function calls depend on input size and complexity.

_Trends_
As input size increases, the number of calls to find_previous_entry_for_key and string_eq also rises significantly.
string_eq is critical for longer words, showing a trend towards more intensive string operations.

_Expectations_
Results align with the expectations of function performance related to input size.

**Based on these results, do you see a way to make your program go faster?**
Answer: No need as it is so fast. But by somehow optimizing find_previous_entry_for_key.
