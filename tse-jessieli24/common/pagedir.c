/*
 * pagedir.c
 *
 * Jessie Li
 * CS 50 October 2022
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "pagedir.h"
#include "file.h"

/**************** make_path ****************/
FILE* open_file(const char* directory, const void* filename, 
                            const char* mode, const char filenametype) 
{
    int len = 200;
    char* path = mem_malloc_assert(len * sizeof(char), "path (makePath)");

    if (filenametype == 'i') {
        snprintf(path, len, "%s/%d", directory, *(int*) filename);
    }
    else if (filenametype == 's') {
        snprintf(path, len, "%s/%s", directory, (char*) filename);
    }

    FILE *fp = fopen(path, mode);
    mem_free(path);
    return fp;
}

/**************** pagedir_init ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory)
{   
    FILE* fp = open_file(pageDirectory, ".crawler", "w", 's');
    if (fp == NULL) return false;

    fclose(fp);
    return true;
}

/**************** pagedir_save ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    // check for null or invalid parameters
    if (page == NULL || pageDirectory == NULL || docID < 0) {
        fprintf(stderr, "null or invalid argument to pagedir_save");
        exit(1);
    }

    FILE *fp = open_file(pageDirectory, (void*) &docID, "w", 'i');
    if (fp == NULL) {
        fprintf(stderr, "unable to create or write to a file of form %s/%d", pageDirectory, docID);
        exit(1);
    }

    // write to file
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s\n", webpage_getHTML(page));
    fclose(fp);
}

/**************** pagedir_validate ****************/
/* see pagedir.h for description */
char* pagedir_validate(char* pageDirectory) 
{
    FILE *fp = open_file(pageDirectory, ".crawler", "r", 's');
    if (fp == NULL) {
        fprintf(stderr, "%s is not a crawler directory\n", pageDirectory);
        return NULL;
    }

    fclose(fp);
    return pageDirectory; 
} 

/***************** pagedir_load ***************/
/* see pagedir.h for description */
webpage_t* pagedir_load(const char* pageDirectory, const int docID)
{
    FILE *fp = open_file(pageDirectory, (void*) &docID, "r", 'i');
    if (fp == NULL) return NULL;

    char* url = file_readLine(fp); 
    int depth;
    fscanf(fp, "%d\n", &depth); 
    char *html = file_readUntil(fp, NULL); 
    webpage_t* page = mem_assert(webpage_new(url, depth, html), "page (pagedir_load)");

    fclose(fp);
    return page;
} 

/***************** pagedir_getUrl ***************/
/* see pagedir.h for description */
char* pagedir_getURL(const char* pageDirectory, const int docID) 
{
    FILE *fp = open_file(pageDirectory, (void*) &docID, "r", 'i');
    if (fp == NULL) return NULL;

    char* line = file_readLine(fp);
    fclose(fp);
    return line;
}