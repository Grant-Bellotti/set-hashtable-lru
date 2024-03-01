#ifndef LRU_H_
#define LRU_H_

#include <stdbool.h>
#include <stdio.h>

// Forward declaration of lru_t (opaque structure)
typedef struct lru lru_t;

lru_t *lru_new(const int capacity);

bool lru_insert(lru_t *lru, const char *key, void *item);

void *lru_find(lru_t *lru, const char *key);

void lru_print(lru_t *lru, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item));

void lru_iterate(lru_t *lru, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) );

void lru_delete(lru_t *lru, void (*itemdelete)(void *item));

#endif // LRU_H_
