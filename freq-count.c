// freq-count.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
    elem_t key = { .ptrValue = word };

    // Check if the word is already in the hash table
    option_t opt = ioopm_hash_table_lookup(ht, key);

    if (opt.success)
    {
        // Key exists, increment the frequency
        int freq = opt.value.intValue;
        ioopm_hash_table_insert(ht, key, (elem_t){ .intValue = freq + 1 });
    }
    else
    {
        // Key does not exist, insert it with frequency 1
        char *word_copy = strdup(word);
        if (!word_copy)
        {
            fprintf(stderr, "Failed to allocate memory for word\n");
            exit(EXIT_FAILURE);
        }
        ioopm_hash_table_insert(ht, (elem_t){ .ptrValue = word_copy }, (elem_t){ .intValue = 1 });
    }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return;
    }

    char *buf = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&buf, &len, f)) != -1)
    {
        for (char *word = strtok(buf, Delimiters);
             word != NULL;
             word = strtok(NULL, Delimiters))
        {
            process_word(word, ht);
        }
        free(buf);
        buf = NULL;
    }

    free(buf); // Free any residual buffer
    fclose(f);
}

size_t string_sum_hash(elem_t e)
{
    char *str = e.ptrValue;
    size_t result = 0;
    while (*str != '\0')
    {
        result += *str;
        str++;
    }
    return result;
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp(e1.ptrValue, e2.ptrValue) == 0);
}

void free_keys(elem_t key, elem_t *value, void *extra)
{
    (void)value; // Unused parameter
    (void)extra; // Unused parameter
    free(key.ptrValue);
}

int main(int argc, char *argv[])
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, string_eq, NULL);

    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            process_file(argv[i], ht);
        }

        // Get the keys as a list
        ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);

        // Get the number of keys
        size_t keys_count;
        ioopm_status_t status = ioopm_linked_list_size(keys_list, &keys_count);
        if (status != IOOPM_SUCCESS)
        {
            fprintf(stderr, "Failed to get size of keys list\n");
            exit(EXIT_FAILURE);
        }

        // Allocate an array for the keys
        char **keys_array = malloc(keys_count * sizeof(char *));
        if (!keys_array)
        {
            fprintf(stderr, "Failed to allocate memory for keys array\n");
            exit(EXIT_FAILURE);
        }

        // Iterate over the list and fill the array
        ioopm_list_iterator_t *iter = ioopm_iterator_create(keys_list);
        if (!iter)
        {
            fprintf(stderr, "Failed to create iterator\n");
            exit(EXIT_FAILURE);
        }

        size_t index = 0;
        bool has_next;
        status = ioopm_iterator_has_next(iter, &has_next);
        while (status == IOOPM_SUCCESS && has_next)
        {
            elem_t current;
            status = ioopm_iterator_next(iter, &current);
            if (status != IOOPM_SUCCESS)
            {
                fprintf(stderr, "Iterator next failed\n");
                exit(EXIT_FAILURE);
            }
            keys_array[index] = current.ptrValue; // Store the key
            index++;

            status = ioopm_iterator_has_next(iter, &has_next);
        }

        ioopm_iterator_destroy(iter);

        // Sort the keys
        sort_keys(keys_array, keys_count);

        // Print the frequencies
        for (size_t i = 0; i < keys_count; ++i)
        {
            char *key = keys_array[i];
            option_t opt = ioopm_hash_table_lookup(ht, (elem_t){ .ptrValue = key });
            if (opt.success)
            {
                int freq = opt.value.intValue;
                printf("%s: %d\n", key, freq);
            }
        }

        // Free allocated memory
        free(keys_array);
        ioopm_linked_list_destroy(keys_list);

        // Free the keys stored in the hash table
        ioopm_hash_table_apply_to_all(ht, free_keys, NULL);

        // Destroy the hash table
        ioopm_hash_table_destroy(ht);
    }
    else
    {
        puts("Usage: freq-count file1 ... filen");
    }

    return 0;
}
