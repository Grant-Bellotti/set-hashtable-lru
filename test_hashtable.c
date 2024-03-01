#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

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
  hashtable_t *hashtable1 = hashtable_new(5);
  hashtable_t *hashtable2 = hashtable_new(5);

  // test insert with strings
  assert(hashtable_insert(hashtable1, "name", "Javier Hernandez"));
  assert(hashtable_insert(hashtable1, "goals", "1"));
  assert(hashtable_insert(hashtable1, "minutes", "483"));

  assert(hashtable_insert(hashtable2, "name", "Dejan Joveljic"));
  assert(hashtable_insert(hashtable2, "goals", "2"));
  assert(hashtable_insert(hashtable2, "minutes", "627"));
  assert(!hashtable_insert(hashtable2, "name", "Ricky Puig"));
  assert(!hashtable_insert(hashtable2, "where", NULL));

  printf("hashtable_insert() with string tests passed\n");

  // check inserted elements

  char* v1 = hashtable_find(hashtable1, "name");
  assert(strcmp(v1,"Javier Hernandez")==0);

  char* v2 = hashtable_find(hashtable2, "goals");
  assert(strcmp(v2,"2")==0);

  void* v3 = hashtable_find(hashtable1, "ronald");
  assert(v3==NULL);

  printf("hashtable_find() with string tests passed\n");

  FILE *fp = fopen("hashtableout.txt", "w");
  hashtable_print(hashtable1, fp, *stringPrint);
  fclose(fp);

  FILE *fp2 = fopen("hashtableout2.txt", "w");
  hashtable_iterate(hashtable1, fp2, *stringIterate);
  fclose(fp2);
  // compare output of hashtableout.txt and hashtableout2.txt; should be identical

  // delete
  hashtable_delete(hashtable1, NULL);
  hashtable_delete(hashtable2, NULL);
	
  // test insert with custom structure Record
  hashtable_t *dailyCheese = hashtable_new(5);

  Record* r1 = newRecord("cheddar cheese");
  Record* r2 = newRecord("brie");
	
  // insertion
  assert(hashtable_insert(dailyCheese, "monday", r1));
  assert(hashtable_insert(dailyCheese, "tuesday", newRecord("gouda")));
  assert(!hashtable_insert(dailyCheese, "monday", r2));

  //retrieval
  assert(hashtable_find(dailyCheese,"monday")==r1);
  assert(!hashtable_find(dailyCheese,"thursday"));

  printf("hashtable_insert() & hashtable_find() with Records passed\n");

  // check with valgrind
  // casting delRecord to be void delRecord(void*)
  hashtable_delete(dailyCheese, (void(*)(void*)) &delRecord );
  delRecord(r2);

}
