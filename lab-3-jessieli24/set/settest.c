/* 
 * settest.c - test program for CS50 set module
 *
 * usage: read names from stdin, each on one line
 *
 * uses bagtest.c and buggybag/bagsimple.c, plus edge cases
 * 
 * Jessie Li, October 2, 2022
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "../lib/file.h"

/***** local type definition *****/
// from buggybag/bagsimple.c
typedef struct student {
  char *name; 
  int age;  
} student_t; 

/***** function prototypes *****/
void student_print(FILE *fp, const char *key, void *item);
void student_delete(void *item);
void itemcount(void* arg, const char* key, void* item);

/* **************************************** */
int main() 
{
  set_t* set = NULL;            // a set
  int stucount = 0;             // number of names put in set
  int setcount = 0;             // number of names found in a set

  printf("Create set...\n");
  set = set_new();
  if (set == NULL) {
    fprintf(stderr, "set_new failed to create new set\n");
    return 1;
  }

  printf("\nPrinting empty set...\n");
  set_print(set, stdout, student_print);

  // set_insert edge cases
  printf("\n\nTest with null set, good item...\n");
  set_insert(NULL, "Dartmouth", "University");
  printf("Test with null key...\n");
  set_insert(set, NULL, "University"); 
  printf("Test with null item...\n");
  set_insert(set, "Dartmouth", NULL); 
  printf("Test with null set, null key, null item...\n");
  set_insert(NULL, NULL, NULL);

  printf("\nCount (should be zero): ");
  setcount = 0;
  set_iterate(set, &setcount, itemcount);
  printf("%d\n\n", setcount);

  // set_find edge cases
  printf("Testing set_find with a NULL set: ");
  if (set_find(NULL, "Dartmouth") == NULL) printf("NULL\n");
  printf("Testing set_find with a NULL key: ");
  if (set_find(set, NULL) == NULL) printf("NULL\n\n");

  // insert with set_insert, find keys with set_find
  int start_age = 16;
  while (!feof(stdin)) {
    char *name = file_readLine(stdin);
    if (name != NULL) {
      if (set_find(set, name) == NULL) {
        printf("'%s' is a new key\n", name);
      } else {
        printf("'%s' is already a key in the set\n", name);
      }
      student_t *stu = malloc(sizeof(student_t));
      if (stu != NULL) {
        stu->name = name; 
        stu->age = start_age++;
        if (set_insert(set, name, stu)) {
          stucount++;
          printf("set_insert inserted %s, %d\n", name, stu->age);
        } else {
          printf("set_insert failed to insert %s\n", name);
          free(name);
          free(stu);
        }
      }
    }
  }

  printf("\nCount (should be %d): ", stucount);
  setcount = 0;
  set_iterate(set, &setcount, itemcount);
  printf("%d\n", setcount);

  // testing set_print
  printf("\nThe set:\n");
  set_print(set, stdout, student_print);
  printf("\n");
  printf("\nPrinting NULL set...\n");
  set_print(NULL, stdout, student_print);
  printf("\n");
  printf("\nPrinting set with NULL file pointer...\n");
  set_print(set, NULL, student_print);
  printf("\n");
  printf("\nPrinting set with NULL itemprint...\n");
  set_print(set, stdout, NULL);
  printf("\n");

  printf("\ndelete the set...\n");
  set_delete(set, student_delete);

  return 0;
}

/***** helper functions from bugglebag/bagsimple.c and bagtest.c ******/
// print a student struct
void student_print(FILE *fp, const char* key, void *item)
{
	student_t *s = item; 
	if (s == NULL) {
		fprintf(fp, "(null)");
	}
	else {
		fprintf(fp, "(%s,%d)", key, s->age);
	}
}

// delete a student struct 
void student_delete(void *item)
{
	student_t *s = item; 
	if (s != NULL) {
		if (s->name != NULL) {
			free(s->name);
		}
		free(s); 
	}
}

// count the non-null items in the set.
void itemcount(void* arg, const char* key, void* item)
{
  int* nitems = arg;

  if (nitems != NULL && key != NULL && item != NULL)
    (*nitems)++;
}