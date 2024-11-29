/*
 * index.c - wrapper for hashtable module, based on activity solution from 10/17
 * 
 * Jessie Li
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "index.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/************* local functions *************/
static void word_print(void* arg, const char* key, void* item);
static void count_print(void* arg, const int key, const int count);
static void count_delete(void* item);

/**************** index_new ****************/
index_t* index_new(const int num_slots)
{
    return (index_t*) hashtable_new(num_slots);
}

/*************** index_insert **************/
bool index_insert(index_t* index, const char* key, counters_t* ctrs) 
{
    return hashtable_insert((hashtable_t*) index, key, (void*) ctrs);
}

/***************** index_find **************/
counters_t* index_find(index_t* index, const char* key)
{
    return (counters_t*) hashtable_find((hashtable_t*) index, key);
}

/***************** index_save **************/
void index_save(index_t* index, const char* indexFilename)
{
    FILE* fp = fopen(indexFilename, "w");
    
    if (fp == NULL) {
        fprintf(stderr, "couldn't open %s\n", indexFilename);
        exit(1);
    }

    if (index != NULL) {
        hashtable_iterate((hashtable_t*) index, (void*) fp, word_print);
        fclose(fp);
    }
}

/***************** word_print **************/
/* Helper function for index_save */
static void word_print(void* arg, const char* key, void* item)
{
    FILE* fp = (FILE*) arg; 
    fprintf(fp, "%s ", key);
    counters_iterate(item, arg, count_print);
    fprintf(fp, "\n");
}

/***************** count_print **************/
/* Helper function for word_print */
static void count_print(void* arg, const int key, const int count)
{
	fprintf((FILE*) arg, "%d %d ", key, count);
}

/*************** index_delete **************/
void index_delete(index_t* index)
{
    hashtable_delete((hashtable_t*) index, count_delete);
}

/************** count_delete ***************/
/* Helper function for index_delete */
static void count_delete(void* item) 
{   
    if (item != NULL) counters_delete((counters_t*) item);
}

/**************** index_load ***************/
index_t* index_load(const char* indexFilename) 
{
    FILE* fp = fopen(indexFilename, "r");
    int wordcount = file_numLines(fp);

    if (fp == NULL || wordcount == 0) {
        fprintf(stderr, "file %s is invalid or empty\n", indexFilename);
        return NULL;
    }

    index_t* index = mem_assert(index_new(wordcount/2+1), "index (index_load)");
    char* word;
    int id, count; 
    while ((word = file_readWord(fp)) != NULL) {
        counters_t* ctrs = mem_assert(counters_new(), "ctrs (index_load)");
        while (fscanf(fp, "%d %d ", &id, &count) == 2) {
            counters_set(ctrs, id, count);
        }
        index_insert(index, word, ctrs);
        mem_free(word);
    }
    fclose(fp);
    return index;
}