


# Makefile för inlupp1 projekt

# ----------------------------------------
# Variabler
# ----------------------------------------



# Standardmål: bygg bibliotek och tester
all: compile_hash_table compile_linked_list compile_iterator

compile_linked_list: linked_list.o linked_list_tests.o
	gcc -Wall -g linked_list.o linked_list_tests.o -I/usr/local/include -L/usr/local/lib -o linked_list_test -lcunit

compile_hash_table: hash_table.o hash_table_tests.o linked_list.o
	gcc -Wall -g hash_table.o hash_table_tests.o linked_list.o -I/usr/local/include -L/usr/local/lib -o hash_table_test -lcunit

compile_iterator: iterator.o iterator_tests.o linked_list.o
	gcc -Wall -g iterator.o iterator_tests.o linked_list.o -I/usr/local/include -L/usr/local/lib -o iterator_test -lcunit

compile_fc: freq-count.o hash_table.o linked_list.o iterator.o
	gcc -Wall -pg freq-count.o hash_table.o linked_list.o iterator.o -I/usr/local/include -L/usr/local/lib -o freq-count -lcunit


freq-count.o: freq-count.c
	gcc -Wall -pg -g -c freq-count.c -o freq-count.o


# Pattern rule för att kompilera .c till .o
%.o: %.c
	gcc -Wall -g -c $< -o $@


test_hash_table: compile_hash_table
	./hash_table_test

test_linked_list: compile_linked_list
	./linked_list_test

test_iterator: compile_iterator
	./iterator_test

test: all
	./hash_table_test
	./linked_list_test
	./iterator_test

ht_test: all
		./hash_table_test
# Rensa upp byggda filer
clean:
	rm -rf *.o *.gcda *.gcno *.gcov *.d *.out massif.out.* cachegrind.out.* hash_table_test linked_list_test iterator_test freq-count

# Inkludera beroendefiler
-include $(DEPS)

# ----------------------------------------
# Specialmål
# ----------------------------------------

.PHONY: all clean
