/*
hashtable.c - implements a hashtable

usage: 
gcc -Wall -Wextra set.c hashtable.c test_hashtable.c -o test_hashtable
./test_hashtable

Grant Bellotti, 1939767, gbellott@ucsc.edu
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"
#include "set.h"

struct hashtable {
    set_t **bins;    // Array of sets
    int num_slots;   // Number of bins
};

long hash_function(const char *key, int num_bins) {
    long hash = 5381;
    int c;

    while ((c = *key++) != '\0') {
        hash = hash * 33 + c;
    }

    return hash % num_bins;
}

/* Create a new (empty) hashtable; return NULL if error. */
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL;
    }

    hashtable_t *ht = malloc(sizeof(hashtable_t));
    if (ht == NULL) {
        return NULL;
    }

    ht->num_slots = num_slots;
    ht->bins = malloc(num_slots * sizeof(set_t *));
    if (ht->bins == NULL) {
        free(ht);
        return NULL;
    }

    for (int i = 0; i < num_slots; i++) {
        ht->bins[i] = set_new();
        if (ht->bins[i] == NULL) {
            // Clean up on failure
            for (int j = 0; j < i; j++) {
                set_delete(ht->bins[j], NULL);
            }
            free(ht->bins);
            free(ht);
            return NULL;
        }
    }

    return ht;
}



/* Insert item, identified by key (string), into the given hashtable.
 * The key string is copied for use by the hashtable.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL) {
        return false;
    }

    int index = hash_function(key, ht->num_slots);
    return set_insert(ht->bins[index], key, item);
}

/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;
    }

    int index = hash_function(key, ht->num_slots);
    return set_find(ht->bins[index], key);
}

/* Print the whole table; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;
    } else if (ht == NULL) {
        printf("null\n");
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_print(ht->bins[i], fp, itemprint);
    }
}

/* Iterate over all items in the table; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->bins[i], arg, itemfunc);
    }
}

/* Delete the whole hashtable; ignore NULL ht.
 * Provide a function that will delete each item (may be NULL).
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->bins[i], itemdelete);
    }

    free(ht->bins);
    free(ht);
}

/* delete the last item in the hashtable to stay in the number on slots */
void hashtable_delete_first(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;
    }
    int index = ht->num_slots-1;
    set_delete_one(ht->bins[index], itemdelete);
}
