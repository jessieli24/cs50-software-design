/* 
 * hashtable.c - implements a hashtable
 *
 * Jessie Li, October 1, 2022
 * CS 50, Fall 2022
 */

#include <string.h>
#include "hashtable.h"
#include "../lib/mem.h"
#include "../set/set.h"
#include "hash.h"

/**************** global types ****************/
typedef struct hashtable {
    int size;
    set_t** table;
} hashtable_t;

/**************** functions ****************/

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable; based on bag_new from bag.c */
hashtable_t* hashtable_new(const int num_slots)
{
    if (num_slots <= 0) return NULL;

    // pointer to new hashtable
    hashtable_t *ht = mem_malloc(sizeof(hashtable_t));

    // return NULL if error
    if (ht == NULL) return NULL;

    // allocate memory
    ht->table = mem_calloc(num_slots, sizeof(set_t*));
    // out of memory
    if (ht->table == NULL) {
        mem_free(ht);
        return NULL;
    }

    // initialize empty sets 
    for (int i = 0; i < num_slots; i++) {
        (ht->table)[i] = set_new();
        // out of memory
        if ((ht->table)[i] == NULL) {
            // clean up and return NULL
            for (int j = 0; j < i; j++) mem_free((ht->table)[j]);
            mem_free(ht->table);
            mem_free(ht);
            return NULL;
        }
    }

    ht->size = num_slots;
    return ht;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable. */
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
    if (ht == NULL || key == NULL || item == NULL) return false;

    // calculate hashkey 
    unsigned long hashkey = hash_jenkins(key, ht->size);

    // try to insert key
    return set_insert((ht->table)[hashkey], key, item); 
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key. */
void* hashtable_find(hashtable_t* ht, const char* key) 
{
    if (ht == NULL) return NULL;

    // calculate hashkey
    unsigned long hashkey = hash_jenkins(key, ht->size);

    // try to find key and return pointer to corresponding item
    return set_find((ht->table)[hashkey], key);
}

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item. */
void hashtable_print(hashtable_t* ht, FILE* fp, 
                     void (*itemprint)(FILE* fp, const char* key, void* item) )
{
    if (fp == NULL) return;

    if (ht == NULL) {
        fprintf(fp, "(null)");
        return;
    }

    for (int i = 0; i < ht->size; i++) {
        // print this slot
        set_print((ht->table)[i], fp, itemprint); 
        fputs("\n", fp);
    }
}

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order. */
void hashtable_iterate(hashtable_t* ht, void* arg,
                       void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (ht == NULL) return;
    
    for (int i = 0; i < ht->size; i++) {
        set_iterate((ht->table)[i], arg, itemfunc);
    }
}

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item. */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) ) 
{
    if (ht == NULL) return;

    for (int i = 0; i < ht->size; i++) {
        set_delete((ht->table)[i], itemdelete);
    }
    mem_free(ht->table);
    mem_free(ht);
}