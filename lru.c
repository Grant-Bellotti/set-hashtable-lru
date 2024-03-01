/*
lru.c - implements a lru

usage: 
gcc -Wall -Wextra set.c hashtable.c lru.c test_LRU.c -o test_LRU
./test_LRU

Grant Bellotti, 1939767, gbellott@ucsc.edu
*/
#include <stdlib.h>
#include <string.h>
#include "lru.h"
#include "hashtable.h"

typedef struct lru_node {
    char *key;
    void *item;
    struct lru_node *prev;
    struct lru_node *next;
} lru_node_t;

struct lru {
    int capacity;
    int size;
    lru_node_t *head;
    lru_node_t *tail;
    hashtable_t *hash_table;
};

/* Create a new (empty) LRU; return NULL if error. */
lru_t *lru_new(const int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    lru_t *lru = malloc(sizeof(lru_t));
    if (lru == NULL) {
        return NULL;
    }

    lru->capacity = capacity;
    lru->size = 0;
    lru->head = NULL;
    lru->tail = NULL;
    lru->hash_table = hashtable_new(capacity); // Assuming LRU_HASH_SIZE is defined

    if (lru->hash_table == NULL) {
        free(lru); // Free the LRU structure if hash_table allocation fails
        return NULL;
    }

    return lru;
}

// bring new node to the front when inserting
void add_to_front(lru_t *lru, const char *key, void *item) {
    lru_node_t *node = (lru_node_t *)malloc(sizeof(lru_node_t));
    node->key = strdup(key);
    node->item = item;
    node->prev = NULL;
    node->next = lru->head;

    if (lru->head != NULL) {
        lru->head->prev = node;
    }

    lru->head = node;

    if (lru->tail == NULL) {
        lru->tail = node;
    }

    lru->size++;
}

// remove last node
void remove_last_node(lru_t *lru, lru_node_t *tail) {
    if (tail == NULL) {
        return;
    }

    if (tail->prev != NULL) {
        tail->prev->next = tail->next;
    } else {
        lru->head = tail->next;
    }

    if (tail->next != NULL) {
        tail->next->prev = tail->prev;
    } else {
        lru->tail = tail->prev;
    }

    free(tail->key);
    free(tail);
}

/* Insert item, identified by key (string), into the given LRU.
 * The key string is copied for use by the LRU.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool lru_insert(lru_t *lru, const char *key, void *item) {
    if (lru == NULL || key == NULL || item == NULL) {
        return false;
    }

    if (lru->size >= lru->capacity) {
        remove_last_node(lru, lru->tail);
        hashtable_delete_first(lru->hash_table, NULL);
    }

    add_to_front(lru, key, item);

    return (hashtable_insert(lru->hash_table, key, item));
}

/* Return the item associated with the given key;
 * return NULL if LRU is NULL, key is NULL, key is not found.
 */
void *lru_find(lru_t *lru, const char *key) {
    if (lru == NULL || key == NULL) {
        return NULL;
    }

    void *item = hashtable_find(lru->hash_table, key);

    return item;
}

/* Print the whole LRU cache; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL lru.
 * Print a cache with no items if NULL itemprint.
 */
void lru_print(lru_t *lru, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (lru == NULL || fp == NULL) {
        return;
    }
    
    lru_node_t *current_node = lru->head;
    while (current_node != NULL) {
        if (itemprint != NULL) {
            itemprint(fp, current_node->key, current_node->item);
        }
        current_node = current_node->next;
    }
}

/* Iterate over all items in the cache; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If lru==NULL or itemfunc==NULL, do nothing.
 */
void lru_iterate(lru_t *lru, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (lru == NULL || itemfunc == NULL) {
        return;
    }

    lru_node_t *current_node = lru->head;
    while (current_node != NULL) {
        itemfunc(arg, current_node->key, current_node->item);
        current_node = current_node->next;
    }
}

/* Delete the whole LRU; ignore NULL lru.
 * Provide a function that will delete each item (may be NULL).
 */
void lru_delete(lru_t *lru, void (*itemdelete)(void *item)) {
    if (lru == NULL) {
        return;
    }

    hashtable_delete(lru->hash_table, itemdelete);
    free(lru);
}

