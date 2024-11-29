/* 
 * set.c - implements a set
 *
 * Jessie Li, September 30, 2022
 * CS 50, Fall 2022
 */

#include <string.h>
#include "set.h"
#include "../lib/mem.h"

/**************** local types ****************/
/* based on treenode struct from tree.c */
typedef struct setnode {
    char *key;                  // key for this node
    void *item;                 // pointer to item for this node
    struct setnode *next;       // pointer to next node
} setnode_t;

/************** global types *****************/
typedef struct set {
    struct setnode *head;       // head of the set
} set_t; 

/************* local functions ***************/
/* not visible outside of this file */
static setnode_t* setnode_new(const char *key, void *item);

/************* global functions **************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** setnode_new ****************/
/* Create a new node to store key and item; based on treenode_new from tree.c */
static setnode_t* setnode_new(const char *key, void *item)
{
    // pointer to new setnode
    setnode_t *setnode = mem_malloc(sizeof(setnode_t));
    // return NULL if error
    if (setnode == NULL || key == NULL || item == NULL) return NULL;
    
    // intialize with key
    setnode->key = mem_malloc(strlen(key) + 1);

    // if error, clean up and return NULL 
    if (setnode->key == NULL) {
        mem_free(setnode);
        return NULL;
    } 
    // else initialize setnode
    strcpy(setnode->key, key);
    setnode->item = item;
    setnode->next = NULL;
    return setnode;
}

/******************* set_new ******************/
/* Create a new (empty) set; based on bag_new from bag.c */
set_t* set_new(void) 
{
    // pointer to new set
    set_t *set = mem_malloc(sizeof(set_t));

    // return NULL if error
    if (set == NULL) return NULL;

    // initialize to empty 
    set->head = NULL;
    return set;
}

/***************** set_insert *****************/
/* Insert item, identified by a key (string), into the given set; based on bag_new from bag.c */
bool set_insert(set_t* set, const char* key, void* item)
{
    // if any parameter is NULL, return false
    if (set == NULL || key == NULL || item == NULL) return false;

    // look for key
    setnode_t *node = set_find(set, key);
    // key already exists
    if (node != NULL) return false;

    // create node
    setnode_t *new = setnode_new(key, item);
    if (new == NULL) return false;

    // insert at head
    new->next = set->head;
    set->head = new;
    return true;
}

/****************** set_find ******************/
/* Return the item associated with the given key. */
void* set_find(set_t* set, const char* key) 
{
    // if any parameters are NULL, return NULL
    if (set == NULL || key == NULL) return NULL;

    // look for key
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
        // if key found, return pointer to node with key
        if (strcmp(key,  node->key) == 0) return node->item;
    }

    // key not found
    return NULL;
}

/****************** set_print ******************/
/* Print the whole set; based on bag_print from bag.c */
void set_print(set_t* set, FILE* fp, 
                void (*itemprint)(FILE* fp, const char* key, void* item) )
{
    if (fp == NULL) return;
    
    if (set == NULL) {
        fprintf(fp, "(null)");
        return;
    }

    fputc('{', fp);
    if (itemprint != NULL) {
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            // print this node 
            (*itemprint)(fp, node->key, node->item); 
            fputc(',', fp);
        }
    }
    fputc('}', fp);
}

/***************** set_iterate *****************/
/* Iterate over the set, calling a function on each item; based on bag_iterate from bag.c */
void set_iterate(set_t* set, void* arg,
                 void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (set == NULL || itemfunc == NULL) return;

    // call itemfunc on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
        (*itemfunc)(arg, node->key, node->item);
    }
}

/***************** set_delete *****************/
/* Delete set, calling a delete function on each item */
/* Based on bag_delete from bag.c and main from names5-free.c */
void set_delete(set_t* set, void (*itemdelete)(void* item) )
{
    if (set == NULL) return;

    setnode_t *next = NULL; 
    for (setnode_t *node = set->head; node != NULL; node = next) {
        next = node->next;
        // delete item if possible
        if (itemdelete != NULL) (*itemdelete)(node->item);     
        if (node->key != NULL) mem_free(node->key);
        mem_free(node);
    }
    mem_free(set);
}
