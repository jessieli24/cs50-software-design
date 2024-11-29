/*
 * index.h - Header file for index.c
 *
 * "A module providing the data structure to represent the in-memory index,
 * and functions to read and write index files...Index is a hashtable keyed 
 * by word and storing counters as items. The counters is keyed by docID and 
 * stores a count of the number of occurrences of that word in the document 
 * with that ID" (Indexer Design Spec).
 * 
 * For additional details, see hashtable.h
 * 
 * Jessie Li
 * CS 50 October 2022
 */

#ifndef __INDEX_H
#define __INDEX_H

#include "hashtable.h"
#include "counters.h"

/*************** global types **************/
typedef hashtable_t index_t;  // opaque to users of the module

/**************** functions ****************/

/**************** index_new ****************/
/* Wrapper for hashtable_new
 *
 * Caller provides:
 *   number of slots to be used for the index
 * We return:
 *   pointer to the new index; NULL if error.
 * Caller is responsible for:
 *   later calling index_delete
 */
index_t* index_new(const int num_slots);

/*************** index_insert **************/
/* Wrapper for hashtable_insert; inserts key, counters pair into index 
 *
 * Caller provides:
 *   pointer to index, string for key, pointer to counters
 * We return:
 *   true if inserted,
 *   false otherwise
 * Notes:
 *   The key string is copied for use by index
 */
bool index_insert(index_t* index, const char* key, counters_t* ctrs);

/***************** index_find **************/
/* Wrapper for hashtable_find 
 *
 * Caller provides:
 *   pointer to index, string for key
 * We return:
 *   counters associated with key if found,
 *   NULL otherwise
 */
counters_t* index_find(index_t* index, const char* key);

/**************** index_save **************/
/* Saves index to output file 
 *
 * Caller provides:
 *   pointer to index, name of output file
 * We do: 
 *   exit non-zero if file could not be opened for writing
 *   nothing if NULL index
 *   output space-separated list of key, counters to file
 */
void index_save(index_t* index, const char* indexFilename);

/*************** index_delete **************/
/* Wrapper for hashtable_delete; deletes index
 *
 * Caller provides:
 *   index pointer
 *   
 * We do:
 *   if index==NULL, do nothing
 *   free index keys and counters
 */
void index_delete(index_t* index);

/**************** index_load ***************/
/* Load an index from an indexer-produced file
 *
 * Caller provides:
 *   string name of index file
 *   
 * We return:
 *   index specified in file
 *   NULL on error (empty or invalid file)
 */
index_t* index_load(const char* indexFilename);

#endif // __INDEX_H
