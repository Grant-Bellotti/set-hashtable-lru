/*
set.c - implements a set

usage: 
gcc -Wall -Wextra set.c test_set.c -o test_set
./test_set

Grant Bellotti, 1939767, gbellott@ucsc.edu
*/
#include <stdlib.h>
#include <string.h>
#include "set.h"

typedef struct setnode {
    char *key;
    void *item;
    struct setnode *next;
} setnode_t;

struct set {
    setnode_t *head;
};

/* Create a new (empty) set; return NULL if error. */
set_t* set_new(void) {
    set_t *new_set = malloc(sizeof(set_t));
    if (new_set == NULL) {
        return NULL;
    }
    new_set->head = NULL;
    return new_set;
}

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL || set_find(set, key)) {
        return false;
    } 

    setnode_t *new_node = malloc(sizeof(setnode_t));
    new_node->key = strdup(key);
    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;
    return true;
}

/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL;
    }

    setnode_t *current_node = set->head;
    while (current_node != NULL) {
        if (strcmp(current_node->key, key) == 0) {
            return current_node->item; 
        }
        current_node = current_node->next;
    }

    return NULL;
}

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;
    } else if (set == NULL) {
        printf("null\n");
        return;
    }

    setnode_t *current_node = set->head;
    while (current_node != NULL) {
        if (itemprint != NULL) {
            itemprint(fp, current_node->key, current_node->item);
        }
        current_node = current_node->next;
    }
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return;
    }

    setnode_t *current_node = set->head;
    while (current_node != NULL) {
        itemfunc(arg, current_node->key, current_node->item);
        current_node = current_node->next;
    }
}

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return;
    }

    setnode_t *current_node = set->head;
    while (current_node != NULL) {
        setnode_t *next_node = current_node->next;
        if (itemdelete != NULL) {
            itemdelete(current_node->item);
        }
        free(current_node->key);
        free(current_node);
        current_node = next_node;
    }
}

/* delete last item in set */
void set_delete_one(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return;
    }

    setnode_t *current_node = set->head;
    setnode_t *prev_node = current_node;
    while (current_node->next != NULL) {
        prev_node = current_node;
        current_node = current_node->next;
    }
    if (itemdelete != NULL) {
        itemdelete(current_node->item);
        prev_node->next = current_node->next;
    }
    free(current_node->key);
    free(current_node);
}
