/*
 * querier - "Reads the index produced by the Indexer and the page files 
 * produced by the Crawler, to interactively answer written queries 
 * entered by the user" (Lab 6 - Querier details).
 *
 * usage: 
 *      ./querier pageDirectory indexFilename
 * 
 * Jessie Li
 * CS 50 Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "counters.h"
#include "mem.h"
#include "pagedir.h"
#include "index.h"
#include "file.h"

/************ local types ************/
struct twocounters {counters_t *orSequence, *otherSequence;};
struct counter {int key, score;};

/* like wordlist_t from fuzzquery.c */
typedef struct {
    char* input;      // malloc'd line of input from stdin
    int nWords;       // number of words
    char** words;     // array of words
} query_t;


/************ functions ************/
/* general */
int fileno(FILE *stream);
int min(int a, int b);
static void prompt(void);
static void parseArgs(const int argc, char* argv[], 
                            char** pageDirectory, index_t** index);

/* query-processing */
query_t* loadQuery(char* line);
void printQuery(query_t* query);
bool isValidQuery(query_t* query);
counters_t* parseQuery(query_t* query, index_t* index);
void printResults(counters_t* results, char* pageDirectory);
void deleteQuery(query_t* query);

/* helper functions for parseQuery */
static void merge(counters_t** andSequence, counters_t** orSequence);
static void unite(counters_t* orSequence, counters_t* otherSequence);
static void unionFunction(void* arg, const int key, const int count);
static void intersect(counters_t* orSequence, counters_t* otherSequence);
static void intersectFunction(void *arg, const int key, int count);

/* helper functions for printResults */
static void countResults(void* arg, const int key, int count);
static void updateMax(void* arg, const int key, int score);

/***********************************/

/************ prompt ************/
/* From Lab 6 - Querier description
 * Print a prompt iff stdin is a tty (terminal)
 */
static void prompt(void)
{
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/************ parseArgs ************/
/* Parse and validate command-line arguments */
static void parseArgs(const int argc, char* argv[], 
                            char** pageDirectory, index_t** index)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", argv[0]);
        exit(1);
    }

    *pageDirectory = mem_assert(pagedir_validate(argv[1]), "pageDirectory");
    *index = mem_assert(index_load(argv[2]), "index");
}

/************ loadQuery ************/
/* Based on wordlist_load from fuzzquery.c
 * Tokenizes string and saves tokens (words) to an array in a query_t
 * 
 * Caller provides:
 *  line    malloc'd input string
 *
 * Return:
 *  query_t query with tokenized string,
 *  NULL if error
 */
query_t* loadQuery(char* line) 
{
    // line cannot be empty or NULL
    if (line == NULL || strlen(line) == 0) {
        return NULL;
    }
    
    // hold words in an array; number of words < length of line
    char** words = mem_calloc_assert(strlen(line), sizeof(char*), "words[]");

    // wrap array of words in a query struct
    query_t* query = mem_malloc_assert(sizeof(query_t), "query");
    query->input = line;
    query->words = words;
    query->nWords = 0;

    // loop over line
    for (char *curr = line, *prev = NULL; *curr != '\0'; prev = curr, curr++) {
        // if curr is a letter
        if (isalpha(*curr)) {
            // make curr lowercase
            *curr = tolower(*curr);
            // if prev is NULL or a letter or '\0'
            if (prev == NULL || isspace(*prev) || *prev == '\0') {
                // make a pointer in words to where curr points
                words[query->nWords++] = curr;
            } 
        // else if curr is a space
        } else if (isspace(*curr)) {
            // if prev is not NULL and a letter, put '\0'
            if (prev != NULL && isalpha(*prev)) *curr = '\0';
        // curr is neither a letter nor a space
        } else {
            fprintf(stderr, "Error: bad character '%c' in query\n", *curr);
            deleteQuery(query);
            return NULL;
        }
    }
    
    return query;
}

/************ printQuery ************/
/* Print clean query
 */
void printQuery(query_t* query)
{
    if (query == NULL) return;

    printf("Query: ");
    char** words = query->words;
    int nWords = query->nWords;
    for (int w = 0; w < nWords; w++) {
        printf("%s ", words[w]);
    }
    printf("\n");
}

/************ isValidQuery ************/
/* Validate the basic structure: 
 *      first and last words cannot be operators
 *      two operators cannot be adjacent
 * 
 * If valid, proceed;
 * otherwise print error
 */
bool isValidQuery(query_t* query) {
    if (query == NULL) return false;

    char** words = query->words;
    for (int i = 0; i < query->nWords; i++) {
        // word is and/or
        if (strcmp(words[i], "or") == 0 || strcmp(words[i], "and") == 0) {
            // and/or is first
            if (i == 0) {
                fprintf(stderr, "Error: '%s' cannot be first\n", words[i]);
                return false;
            }
            // and/or is last
            if (i == query->nWords-1) {
                fprintf(stderr, "Error: '%s' cannot be last\n", words[i]);
                return false;
            }
            // word adjacent to and/or is also and/or
            if (strcmp(words[i+1], "or") == 0 || strcmp(words[i+1], "and") == 0) {
                fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
                return false;
            }
        } 
    }
    return true;
}

/************ parseQuery ************/
/* From 10/24 activity solution
 * Parses query and gathers results into counters of doc id, score pairs
 * 
 * Returns: 
 *  pointer to counters_t of results
 *  NULL if error or no documents match query
 */
counters_t* parseQuery(query_t* query, index_t* index) {
    if (query == NULL || index == NULL) return NULL;

    counters_t *andSequence = NULL; // holds result of current AND operations
    counters_t *orSequence = NULL;  // accumulates the final result combining OR sequences
    bool shortCircuit = false;

    char** words = query->words;
    for (int i = 0; i < query->nWords; i++) {
        // if this word is 'or'
        if (strcmp(words[i], "or") == 0) {
            merge(&andSequence, &orSequence);
            shortCircuit = false;
            continue;
        }
        // NULL match in this 'and' sequence
        if (shortCircuit) continue;
        // 'and' is the same as ' ' between words
        if (strcmp(words[i], "and") == 0) continue;
        // this word is not an operator
        counters_t* match = index_find(index, words[i]);
        if (match == NULL) {
            // shortCircuit rest in this 'and' sequence
            shortCircuit = true;
            // reset andSequence
            if (andSequence != NULL) {
                counters_delete(andSequence);
                andSequence = NULL;
            }
        } else { // found a match
            if (andSequence == NULL) { // first match
                andSequence = mem_assert(counters_new(), "andSequence (parseQuery)");
                unite(andSequence, match);
            } else { // not first match, intersect
                intersect(andSequence, match);
            }
        }
    }
    merge(&andSequence, &orSequence);
    return orSequence;
}

/************ merge ************/
/* From 10/24 activity solution */
static void merge(counters_t** andSequence, counters_t** orSequence) 
{
    if (*andSequence != NULL) {
        if (*orSequence == NULL) *orSequence = mem_assert(counters_new(), "orSequence (merge)");
        // push temporary result andSequence to final result orSequence
        unite(*orSequence, *andSequence);
        counters_delete(*andSequence);
        // also reset andSequence
        *andSequence = NULL;
    }
}

/************ unite ************/
/* From UNION in 10/24 activity solution */
static void unite(counters_t* orSequence, counters_t* otherSequence) 
{
    // union orSequence and otherSequence by adding counts for matching keys
    // store results in orSequence
    counters_iterate(otherSequence, orSequence, unionFunction);
}

/************ unionFunction ************/
/* From 10/24 activity solution */
static void unionFunction(void* arg, const int key, const int count)
{
    // update arg counters 
    int wordCount = counters_get((counters_t*) arg, key); // returns 0 if key not found
    counters_set((counters_t*) arg, key, count + wordCount);
}

/************ intersect ************/
/* From 10/24 activity solution */
void intersect(counters_t* orSequence, counters_t* otherSequence)
{
    // intersect orSequence and otherSequence by taking of counts for matching keys
    // store results in orSequence
    struct twocounters args = {orSequence, otherSequence};
    counters_iterate(orSequence, (void*) &args, intersectFunction);
}

/************ intersectFunction ************/
/* From 10/24 activity solution */
void intersectFunction(void *arg, const int key, int count) 
{
    struct twocounters* twocounters = (struct twocounters*) arg;
    counters_set(twocounters->orSequence, key, min(count, counters_get(twocounters->otherSequence, key)));
}

/************ min ************/
/* Return mininum of two integers */
int min(int a, int b) 
{
    if (a > b) return  b; else return a;
}

/************ printResults ************/
void printResults(counters_t* results, char* pageDirectory) 
{
    if (pageDirectory == NULL) return;

    // no matches
    if (results == NULL) {
        printf("No documents matched.\n");
        return;
    }

    int nResults = 0;
    counters_iterate(results, (void*) &nResults, countResults);
    // print the number of matching documents
    if (nResults == 0) {
        printf("No documents matched.\n");
        return;
    }
    printf("Matches %d documents (ranked)\n", nResults);

    // save key, score of doc with max score
    struct counter max = {0, 0};
    counters_iterate(results, (void*) &max, updateMax);
    while (max.score != 0) {
        // make a webpage for this doc
        char* url = mem_assert(pagedir_getURL(pageDirectory, max.key), "page");
        // print this doc
        printf("score\t%d doc\t%d: %s\n", max.score, max.key, url);
        // clean and reset
        max.score = 0;
        mem_free(url);
        counters_set(results, max.key, 0);
        counters_iterate(results, (void*) &max, updateMax);
    }
}

/************ countResults ************/
static void countResults(void* arg, const int key, int count) 
{   
    int* nResults = (int*) arg;
    if (count != 0) (*nResults)++;
}

/************ updateMax ************/
static void updateMax(void* arg, const int key, int score)
{
    struct counter* max = (struct counter*) arg;
    if (score > max->score) {
        max->key = key;
        max->score = score;
    }
}

/************ deleteQuery ************/
/* Partially based on wordlist_delete in fuzzquery.c */
void deleteQuery(query_t* query)
{
  if (query == NULL) return;

  mem_free(query->input);   // was allocated by file_readLine()
  mem_free(query->words);
  mem_free(query);
}

int main(const int argc, char* argv[])
{
    char* pageDirectory;
    index_t* index = NULL;
    char* input;
    counters_t* results;

    parseArgs(argc, argv, &pageDirectory, &index);

    while(!feof(stdin)) {
        prompt();
        // read input from stdin
        input = file_readLine(stdin);
        // load the query
        query_t* query = loadQuery(input);
        // something wrong with input
        if (query == NULL) {
            continue;
        }
        // print the clean query
        printQuery(query);
        // validate the query
        if (!isValidQuery(query)) {
            deleteQuery(query);
            continue;
        }
        // parse the query
        results = parseQuery(query, index);
        // print the results
        printResults(results, pageDirectory);
        printf("-----------------------------------------------\n");
        // clean up
        counters_delete(results);
        deleteQuery(query);
    }
    // clean up
    index_delete(index);
    return 0;
}