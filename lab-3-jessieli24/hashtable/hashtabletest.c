/* 
 * hashtabletest.c - test program for CS50 hashtable module
 *
 * usage: read names from stdin, each on one line
 *
 * uses bagtest.c and buggybag/bagsimple.c, plus edge cases
 *
 * CS50, April 2019, 2021
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "../lib/file.h"

/***** local type definition *****/
typedef struct student {
	char *name; 
	int age;  
} student_t; 

/***** function prototypes from bugglebag/bagsimple.c and bagtest.c *****/
void student_print(FILE *fp, const char *key, void *item);
void student_delete(void *item);
void itemcount(void* arg, const char* key, void* item);

/* **************************************** */
int main() 
{
  hashtable_t* ht = NULL;       // hashtable
  int stucount = 0;             // number of names put in hashtable
  int htcount = 0;              // number of names found in a hashtable

  printf("Create hashtable...\n");
  ht = hashtable_new(7);
  if (ht == NULL) {
    fprintf(stderr, "hashtable_new failed to create new hashtable\n");
    return 1;
  }

  printf("\nPrinting empty hashtable...\n");
  hashtable_print(ht, stdout, student_print);

  // hashtable_insert edge cases
  printf("\nTest with null hashtable, good item...\n");
  hashtable_insert(NULL, "Dartmouth", "University");
  printf("Test with null key...\n");
  hashtable_insert(ht, NULL, "University"); 
  printf("Test with null item...\n");
  hashtable_insert(ht, "Dartmouth", NULL); 
  printf("Test with null hashtable, null key, null item...\n");
  hashtable_insert(NULL, NULL, NULL);

  printf("\nCount (should be zero): ");
  htcount = 0;
  hashtable_iterate(ht, &htcount, itemcount);
  printf("%d\n", htcount);

  // insert with hashtable_insert, find keys with hashtable_find
  int start_age = 16;
  while (!feof(stdin)) {
    char *name = file_readLine(stdin);
    if (name != NULL) {
      if (hashtable_find(ht, name) == NULL) {
        printf("'%s' is a new key\n", name);
      } else {
        printf("'%s' is already a key in the hashtable\n", name);
      }
      student_t *stu = malloc(sizeof(student_t));
      if (stu != NULL) {
        stu->name = name; 
        stu->age = start_age++;
        if (hashtable_insert(ht, name, stu)) {
            stucount++;
            printf("hashtable_insert inserted %s, %d\n", name, stu->age);
        } else {
          printf("hashtable_insert failed to insert %s\n", name);
          free(name);
          free(stu);
        }
      }
    }
  }

  printf("\nCount (should be %d): ", stucount);
  htcount = 0;
  hashtable_iterate(ht, &htcount, itemcount);
  printf("%d\n", htcount);

  // hashtable_find edge cases
  printf("\nTesting hashtable_find with NULL hashtable...got ");
  if (hashtable_find(NULL, "Dartmouth") == NULL) {
    printf("NULL\n");
  }
  printf("Testing hashtable_find with NULL key...got ");
  if (hashtable_find(ht, NULL) == NULL) {
    printf("NULL\n");
  }

  printf("\nThe hashtable:\n");
  hashtable_print(ht, stdout, student_print);
  printf("\n");
  printf("\nPrinting NULL hashtable...\n");
  hashtable_print(NULL, stdout, student_print);
  printf("\n");
  printf("\nPrinting hashtable with NULL file pointer...\n");
  hashtable_print(ht, NULL, student_print);
  printf("\n");
  printf("\nPrinting hashtable with NULL itemprint...\n");
  hashtable_print(ht, stdout, NULL);
  printf("\n");

  printf("\ndelete the hashtable...\n");
  hashtable_delete(ht, student_delete);

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