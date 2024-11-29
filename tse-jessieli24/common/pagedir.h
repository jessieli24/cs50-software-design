/*
 * pagedir.h - Header file for pagedir.c
 *
 * "Handles the pagesaver mentioned in the design (writing a page to the pageDirectory), 
 * and marking it as a Crawler-produced pageDirectory" (Implementation Spec).
 * 
 * Jessie Li
 * CS 50 October 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"

/**************** open_file *****************/
/* Opens file specified by directory, filename, and mode
 * 
 * Caller provides:
 *  directory       directory to open
 *  filename        file to open
 *  mode            mode passed to fopen
 *  filenametype    'i' for integer or 's' for char* string
 * 
 * Returns: 
 *  FILE pointer if file opened successfully
 *  NULL otherwise
 * 
 * Note: 
 *  Assumes that file pathname (i.e. "directory/filename") at most 200 characters long
 */
FILE* open_file(const char* directory, const void* filename, 
                            const char* mode, const char filenametype);
/**************** pagedir_init ****************/
/* Construct the pathname for the .crawler file in that directory
 * 
 * Caller provides:
 *  pageDirectory   directory to store crawler files
 * 
 * Returns: 
 *  true if opened and closed a .crawler file for writing,
 *  false otherwise
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Prints contents of the webpage into a file called docID in pageDirectory
 * 
 * Caller provides:
 *  page            pointer to a webpage_t
 *  pageDirectory   directory to store page file
 *  docID           integer id for page file
 * 
 * Assumptions:
 *  docID is at most 4 digits long
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/************** pagedir_validate **************/
/* Verify whether pageDirectory is a Crawler-produced directory
 * 
 * Caller provides:
 *  pageDirectory   name of directory
 * 
 * Returns: 
 *  pageDirectory if opened and closed a .crawler file for reading,
 *  NULL otherwise
 */
char* pagedir_validate(char* pageDirectory);

/***************** pagedir_load ***************/
/* Load a page from a file (docID) in pageDirectory
 * 
 * Caller provides:
 *  pageDirectory   name of directory
 *  docID           integer id of page file
 * 
 * Returns: 
 *  webpage loaded from page file,
 *  NULL if error
 * 
 * Caller is responsible for: 
 *  later calling webpage_delete on the page returned
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/***************** pagedir_getURL ***************/
/* Get URL of a page from a file (docID) in pageDirectory
 * 
 * Caller provides:
 *  pageDirectory   name of directory
 *  docID           integer id of page file
 * 
 * Returns: 
 *  string url,
 *  NULL if error
 * 
 * Caller is responsible for: 
 *  later calling free on the string returned
 */
char* pagedir_getURL(const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H