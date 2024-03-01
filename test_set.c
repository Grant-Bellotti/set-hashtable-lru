#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
 
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
  set_t *set1 = set_new();
  set_t *set2 = set_new();

  // test insert with strings
  assert(set_insert(set1, "name", "Javier Hernandez"));
  assert(set_insert(set1, "goals", "1"));
  assert(set_insert(set1, "minutes", "483"));

  assert(set_insert(set2, "name", "Dejan Joveljic"));
  assert(set_insert(set2, "goals", "2"));
  assert(set_insert(set2, "minutes", "627"));
  assert(!set_insert(set2, "name", "Ricky Puig"));
  assert(!set_insert(set2, "where", NULL));

  printf("set_insert() with string tests passed\n");

  // check inserted elements

  char* v1 = set_find(set1, "name");
  assert(strcmp(v1,"Javier Hernandez")==0);

  char* v2 = set_find(set2, "goals");
  assert(strcmp(v2,"2")==0);

  void* v3 = set_find(set1, "ronald");
  assert(v3==NULL);

  printf("set_find() with string tests passed\n");

  FILE *fp = fopen("setout.txt", "w");
  set_print(set1, fp, *stringPrint);
  fclose(fp);

  FILE *fp2 = fopen("setout2.txt", "w");
  set_iterate(set1, fp2, *stringIterate);
  fclose(fp2);
  // compare output of setout.txt and setout2.txt; should be identical

  // delete
  set_delete(set1, NULL);
  set_delete(set2, NULL);
	
  // test insert with custom structure Record
  set_t *dailyCheese = set_new();

  Record* r1 = newRecord("cheddar cheese");
  Record* r2 = newRecord("brie");
	
  // insertion
  assert(set_insert(dailyCheese, "monday", r1));
  assert(set_insert(dailyCheese, "tuesday", newRecord("gouda")));
  assert(!set_insert(dailyCheese, "monday", r2));

  //retrieval
  assert(set_find(dailyCheese,"monday")==r1);
  assert(!set_find(dailyCheese,"thursday"));

  printf("set_insert() & set_find() with Records passed\n");

  // check with valgrind
  // casting delRecord to be void delRecord(void*)
  set_delete(dailyCheese, (void(*)(void*)) &delRecord );
  delRecord(r2);

}
