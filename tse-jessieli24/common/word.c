/*
 * word.c
 *
 * Jessie Li
 * CS 50 October 2022
 */

#include <ctype.h>
#include "mem.h"
#include "word.h"

/**************** normalizeWord ****************/
/* Normalizes word in place */
void normalizeWord(char* word) {
    if (word == NULL) return;
    
    for (char* c = word; *c != '\0'; c++) {
        *c = tolower(*c);
    }
}
