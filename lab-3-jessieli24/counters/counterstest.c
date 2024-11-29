/* 
 * counterstest.c - test program for CS50 counters module
 *
 * usage: read names from stdin, each on one line
 *
 * uses most of bagtest.c, plus additional edge cases
 *
 * CS50, April 2019, 2021
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "../lib/file.h"

static void countadded(void* arg, const int key, const int count);
static void itemcount(void* arg, const int key, const int count);

/* **************************************** */
int main() 
{
  counters_t* counters = NULL;            // counters
  int key = -1;                           // a key in the counters
  int keyadds = 0;                        // number of keys entered
  int added = 0;                          // cumulative counts across all keys
  int counterscount = 0;                  // number of keys

  printf("Create first counters...\n");
  counters = counters_new();
  if (counters == NULL) {
    fprintf(stderr, "counters_new failed to create new counters\n");
    return 1;
  }

  printf("\nPrinting empty counters...\n");
  counters_print(counters, stdout);

  // counters_add edge cases
  printf("\nTest with null counters, good key...\n");
  counters_add(NULL, 1);
  printf("test with negative key...\n");
  counters_add(counters, -1); 
  printf("test with null counters, negative key...\n");
  counters_add(NULL, -1);

  printf("\nNumber of keys (should be zero): ");
  counterscount = 0;
  counters_iterate(counters, &counterscount, itemcount);
  printf("%d\n", counterscount);

  printf("\nTesting counters_add and counters_get...\n");
  // read lines from stdin
  keyadds = 0;
  while (!feof(stdin)) {
    scanf("%d", &key);
    printf("key %d -- before: %d, ", key, counters_get(counters, key));
    if (counters_add(counters, key) != 0) {
        keyadds++;
    } 
    printf("after: %d\n", counters_get(counters, key));
  }

  // assumes counters_set wasn't used to modify counts before this point
  printf("\nTotal adds (should be %d): ", keyadds);
  added  = 0;
  counters_iterate(counters, &added, countadded);
  printf("%d\n", added);
  printf("\nNumber of keys: ");
  counterscount = 0;
  counters_iterate(counters, &counterscount, itemcount);
  printf("%d\n", counterscount);

  printf("\nThe counters:\n");
  counters_print(counters, stdout);
  printf("\n");

  printf("\nTesting counters_set...\n");
  printf("with a key in counters (set %d to 10): ", key);
  printf("%s\n", counters_set(counters, key, 10) ? "true" : "false");
  // assume 999 is not a key
  printf("with a key not in counters (set 999 to 10): ");
  printf("%s\n", counters_set(counters, 999, 10) ? "true" : "false");
  printf("with a negative key (set -1 to 10): ");
  printf("%s\n", counters_set(counters, -1, 10) ? "true" : "false");
  printf("with a negative count(set %d to -10): ", key);
  printf("%s\n", counters_set(counters, key, -10) ? "true" : "false");
  printf("with a NULL counters: ");
  printf("%s\n", counters_set(NULL, key, 10) ? "true" : "false");

  // testing counters_print
  printf("\nThe counters:\n");
  counters_print(counters, stdout);
  printf("\n");
  printf("\nPrinting NULL counters...\n");
  counters_print(NULL, stdout);
  printf("\n");
  printf("\nPrinting counters with NULL file pointer...\n");
  counters_print(counters, NULL);
  printf("\n");

  printf("\ndelete the counters...\n");
  counters_delete(counters);

  return 0;
}

/***** helper functions ******/
// sum the total counts of counters in counter set
static void countadded(void* arg, const int key, const int count)
{
  int* nitems = arg;

  if (nitems != NULL && key >= 0 && count >= 0)
    (*nitems) += count;
}

// count the non-null counters in the counter set; from bagtest.c
static void itemcount(void* arg, const int key, const int count)
{
  int* nitems = arg;

  if (nitems != NULL && key >= 0 && count >= 0)
    (*nitems)++;
}
