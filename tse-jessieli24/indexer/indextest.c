/*
 * indextest.c - "a unit test for the index module; it reads an index file 
 * into the internal index data structure, then writes the index out to a 
 * new index file" (Indexer Design Spec)
 * 
 * Jessie Li
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include "index.h"
#include "mem.h"

int main(const int argc, char* argv[]) 
{
    index_t* index = mem_assert(index_load(argv[1]), "couldn't load index");
    index_save(index, argv[2]);
    index_delete(index);
    
    return 0;
}
