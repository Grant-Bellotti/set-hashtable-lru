#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct hashtable hashtable_t;

hashtable_t *hashtable_new(const int num_slots);

bool hashtable_insert(hashtable_t *ht, const char *key, void *item);

void *hashtable_find(hashtable_t *ht, const char *key);

void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item));

void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item));

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item));

void hashtable_delete_first(hashtable_t *ht, void (*itemdelete)(void *item));

#endif // HASHTABLE_H_
