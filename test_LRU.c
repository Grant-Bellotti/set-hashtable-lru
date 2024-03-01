#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru.h"

// prints to file
void stringPrint(FILE *fp, const char *key, void *item){
  fprintf(fp, "%s : %s\n", key, (char*)item);
}

// prints to file as well
void stringIterate(void* ptr, const char *key, void *item){
  FILE* fp = (FILE*) ptr;
  fprintf(fp, "%s : %s\n", key, (char*)item);
}

// test struct
typedef struct Record Record;
struct Record {
	char* value;
};

// constructor
Record *newRecord(char* v) {
	Record *newp;

	newp = (Record *) malloc(sizeof(Record));
	newp->value = v;
	return newp;
}

// destructor
void delRecord(Record *p) {
  free(p);
  p=NULL;
}


int main(){

  // test intialize
  lru_t *lru1 = lru_new(10);
  lru_t *lru2 = lru_new(10);

  // test insert with strings
  assert(lru_insert(lru1, "name", "Javier Hernandez"));
  assert(lru_insert(lru1, "goals", "1"));
  assert(lru_insert(lru1, "minutes", "483"));

  assert(lru_insert(lru2, "name", "Dejan Joveljic"));
  assert(lru_insert(lru2, "goals", "2"));
  assert(lru_insert(lru2, "minutes", "627"));
  assert(!lru_insert(lru2, "name", "Ricky Puig"));
  assert(!lru_insert(lru2, "where", NULL));

  printf("lru_insert() with string tests passed\n");

  // check inserted elements

  char* v1 = lru_find(lru1, "name");
  assert(strcmp(v1,"Javier Hernandez")==0);

  char* v2 = lru_find(lru2, "goals");
  assert(strcmp(v2,"2")==0);

  void* v3 = lru_find(lru1, "ronald");
  assert(v3==NULL);

  printf("lru_find() with string tests passed\n");

  FILE *fp = fopen("lruout.txt", "w");
  lru_print(lru1, fp, *stringPrint);
  fclose(fp);

  FILE *fp2 = fopen("lruout2.txt", "w");
  lru_iterate(lru1, fp2, *stringIterate);
  fclose(fp2);
  // compare output of lruout.txt and lruout2.txt; should be identical

  // delete
  lru_delete(lru1, NULL);
  lru_delete(lru2, NULL);
  // test insert with custom structure Record
  lru_t *dailyCheese = lru_new(10);

  Record* r1 = newRecord("cheddar cheese");
  Record* r2 = newRecord("brie");
	
  // insertion
  assert(lru_insert(dailyCheese, "monday", r1));
  assert(lru_insert(dailyCheese, "tuesday", newRecord("gouda")));
  assert(!lru_insert(dailyCheese, "monday", r2));

  //retrieval
  assert(lru_find(dailyCheese,"monday")==r1);
  assert(!lru_find(dailyCheese,"thursday"));

  printf("lru_insert() & lru_find() with Records passed\n");

  // check with valgrind
  // casting delRecord to be void delRecord(void*)
  lru_delete(dailyCheese, (void(*)(void*)) &delRecord );

  // testing that elements are deleted
  lru_t* smallLRU = lru_new(2);
  lru_insert(smallLRU, "one","1");
  lru_insert(smallLRU, "two","2");
  lru_insert(smallLRU, "three","3");
  assert(!lru_find(smallLRU,"one"));
  printf("LRU deleted old element succesfully\n");
	
  lru_delete(smallLRU, NULL);
  delRecord(r2);

}
