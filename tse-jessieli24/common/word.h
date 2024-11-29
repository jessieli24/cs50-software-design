/*
 * word.h - Header file for word.c
 *
 * "A module providing a function to normalize a word" (Indexer Design Spec).
 * 
 * Jessie Li
 * CS 50 October 2022
 */

#ifndef __WORD_H
#define __WORD_H

/**************** normalizeWord ****************/
/* Normalizes word in place
 *
 * Caller provides:
 *    word: string containing absolute word to normalize
 */
void normalizeWord(char* word);

#endif // __WORD_H