/*
 * indexer.c - "program that reads the document files produced by the TSE crawler, 
 * builds an index, and writes that index to a file" (Indexer Design Spec)
 * 
 * Jessie Li
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "webpage.h"
#include "mem.h"

/******* local functions ********/
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);

/********** indexBuild **********/
/* Builds an in-memory index from files in pageDirectory */
static index_t* indexBuild(const char* pageDirectory) 
{
    index_t* index = mem_assert(index_new(200), "new index (indexBuild)"); 
    
    webpage_t *page = pagedir_load(pageDirectory, 1);
    // pageDirectory/1 must exist
    if (page == NULL) {
        fprintf(stderr, "%s is empty\n", pageDirectory);
        exit(1);
    }

    for (int id = 1; page != NULL; ) {
        indexPage(index, page, id);
        webpage_delete(page);
        page = pagedir_load(pageDirectory, ++id);
    }

    return index;
}

/*********** indexPage **********/
/* Adds words from webpage doc to the index */
static void indexPage(index_t* index, webpage_t* page, const int id) 
{
    // from webpage_getNextWord (webpage.h)
    int pos = 0;
    char* word; 
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        // skip trivial
        if (strlen(word) > 2) {
            // normalize
            normalizeWord(word);
            // key in index? 
            counters_t* ctrs = index_find(index, word);
            // create new ctrs for word if word not in index
            if (ctrs == NULL) {
                ctrs = mem_assert(counters_new(), "ctrs (indexPage)");
                index_insert(index, word, ctrs);
            }
            counters_add(ctrs, id);                         
        }
        mem_free(word);
    } 
}

int main(const int argc, char* argv[]) 
{
    // parse command line, validate parameters
    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", argv[0]);
        return 1;
    } 

    // pageDirectory
    char* pageDirectory = pagedir_validate(argv[1]);
    if (pageDirectory == NULL) return 1;

    // indexFilename
    char* indexFilename = argv[2];

    // build, save, then clean
    index_t* index = indexBuild(pageDirectory);
    index_save(index, indexFilename);
    index_delete(index);

    return 0;
}