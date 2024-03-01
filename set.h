#ifndef SET_H_
#define SET_H_

#include <stdbool.h>
#include <stdio.h>

// Define the set structure
typedef struct set set_t;

/* Create a new (empty) set; return NULL if error. */
set_t* set_new(void);

bool set_insert(set_t *set, const char *key, void *item);

void *set_find(set_t *set, const char *key);

void set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item));

void set_iterate(set_t *set, void *arg, 
                 void (*itemfunc)(void *arg, const char *key, void *item));

void set_delete(set_t *set, void (*itemdelete)(void *item));

void set_delete_one(set_t *set, void (*itemdelete)(void *item));

#endif // SET_H_
