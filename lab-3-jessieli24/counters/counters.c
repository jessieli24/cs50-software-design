/* 
 * counters.c - implements a counter set; assumes integer keys
 * 
 * Jessie Li, October 1, 2022
 * CS 50, Fall 2022
 */

#include <stddef.h>

#include "counters.h"
#include "../lib/mem.h"

/************** local types ***************/
typedef struct counter {
    int key;
    int count;
    struct counter *next;
} counter_t;

/************** global types **************/
typedef struct counters {
    struct counter *head;
} counters_t;

/************* local functions ************/
/* not visible outside this file */
static counter_t* counter_new(int key);

/************** counters_new **************/
/* Create a new (empty) counter structure; based on bag_new from bag.c */
counters_t* counters_new(void) 
{
    // pointer to new counterset
    counters_t *ctrs = mem_malloc(sizeof(counters_t));
    
    // return NULL if error
    if (ctrs == NULL) return NULL;

    // initialize to empty 
    ctrs->head = NULL;
    return ctrs;
}

/************** counter_new ***************/
/* Create a new counter with key and initalize to 1; based on treenode_new from tree.c */
static counter_t* counter_new(int key)
{
    // pointer to new counter
    counter_t *ctr = mem_malloc(sizeof(counter_t));
    // return NULL if error
    if (ctr == NULL || key < 0) return NULL;
    
    // initialize counter
    ctr->key = key;
    ctr->count = 1;
    ctr->next = NULL;
    return ctr; 
}

/************** counters_add **************/
/* Increment the counter indicated by key. */
int counters_add(counters_t* ctrs, const int key) 
{
    // if ctrs is NULL or key is negative, return 0
    if (ctrs == NULL || key < 0) return 0;

    // look for key
    for (counter_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        // if key found, increment counter by 1 and return counter's new value
        if (curr->key == key) return ++curr->count;
    }

    // key doesn't exist, create counter and initialize to 1
    counter_t *new = counter_new(key);
    if (new == NULL) return 0;

    // insert counter into counter set at head
    new->next = ctrs->head;
    ctrs->head = new;

    // new counter initalized to 1
    return 1;
}

/*************** counters_get ***************/
/* Return current value of counter associated with the given key. */
int counters_get(counters_t* ctrs, const int key)
{
    // if ctrs is NULL or key is negative, return 0
    if (ctrs == NULL || key < 0) return 0;

    // look for key
    for (counter_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        // key found, return count
        if (curr->key == key) return curr->count;
    }

    // key not found
    return 0;
}

/*************** counters_set ***************/
/* Set the current value of counter associated with the given key. */
bool counters_set(counters_t* ctrs, const int key, const int count)
{
    if (ctrs == NULL || key < 0 || count < 0) return false;

    // look for key
    for (counter_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        // key found, set count
        if (curr->key == key) {
            curr->count = count;
            return true;
        }
    }

    // key not found, make new counter
    counter_t *new = counter_new(key);
    // out of memory
    if (new == NULL) return false;

    // initialize to given value
    new->count = count;

    // insert counter into counter set at head
    new->next = ctrs->head;
    ctrs->head = new;
    return true;
}

/************** counters_print **************/
/* Print all counters; based on bag_print from bag.c */
void counters_print(counters_t* ctrs, FILE* fp)
{
    if (fp == NULL) {
        return;
    }
    if (ctrs == NULL) {
        fprintf(fp, "(null)");
        return;
    }

    fputc('{', fp);
    for (counter_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        // print this counter
        fprintf(fp, "%d=%d", curr->key, curr->count);
        fputs(",", fp);
    }
    fputs("}\n", fp);
}

/************* counters_iterate *************/
/* Iterate over all counters in the set; based on bag_iterate from bag.c */
void counters_iterate(counters_t* ctrs, void* arg, 
                      void (*itemfunc)(void* arg, const int key, const int count))
{
    if (ctrs == NULL || itemfunc == NULL) return;

    // call itemfunc on each item
    for (counter_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        (*itemfunc)(arg, curr->key, curr->count);
    }
}

/************* counters_delete **************/
/* Delete the whole counterset */
/* Based on bag_delete from bag.c and main from names5-free.c */
void counters_delete(counters_t* ctrs)
{
    if (ctrs == NULL) return;

    counter_t *next = NULL;
    for (counter_t *curr = ctrs->head; curr != NULL; curr = next) {
        next = curr->next;     
        mem_free(curr);
    }
    mem_free(ctrs);
}