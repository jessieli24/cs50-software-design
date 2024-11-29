# CS50 TSE Querier
## Implementation Specification
Based on Crawler Implementation Specification.

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use three main data structures: a 'query' storing words in a user-supplied search query, a 'counters' of results (or documents that match the query), and an 'index' of words for a pageDirectory.

Aside from these, I found it helpful to create two secondary structures, 'twocounters' and 'counter' to aid processes that manipulate 'counters' objects.

## Control flow

The Querier is implemented in one file `querier.c`.

### 0. Local types and function prototypes

#### querier

```c
/* local types */
struct twocounters {counters_t *orSequence, *otherSequence;};
struct counter {int key, score;};

/* like wordlist_t from fuzzquery.c */
typedef struct {
    char* input;      // malloc'd line of input from stdin
    int nWords;       // number of words
    char** words;     // array of words
} query_t;

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
```

#### pagedir

```c
char* pagedir_validate(char* pageDirectory);
char* pagedir_getURL(const char* pageDirectory, const int docID);
```

#### index

```c
index_t* index_load(const char* indexFilename);
void index_delete(index_t* index);
```

See `libcs50` for additional functions used in this program.  

### 1. Primary functions

#### main

The `main` function reads input from stdin, calls functions to process the query and print the results, then exits zero.

Pseudocode: 

    parse the command line, validate parameters, initialize other modules
    index_load, with pageDirectory

    while stdin not EOF:
        read line
        loadQuery
        printQuery to print a clean query
        isValidQuery to ensure that the clean query has a valid structure
        parseQuery with query and index to gather a counters of results
        printResults
        deleteQuery

    index_delete
    return 0

#### loadQuery

Tokenize the query and ensures that it only contains letters and spaces.

Pseudocode:

    create an array of pointers to words, char** words
    create a new 'query' object
    loop over the line with one pointer to current char, one pointer to previous
        if current is a letter,
            lowercase
            if previous is NULL, space, or '\0'
                this is the beginning of a word, store a pointer in words
        if current is a space,
            if previous is a letter,
                this is the end of a word, put '\0' here
        if current is neither a letter nor a space,
            this is an invalid character, print error and return NULL
    return query

#### parseQuery

Finds documents in the pageDirectory that match the query.

Pseudocode (taken from 10/24 activity solution):

    // Create two counters initially set to NULL:
    counters_t *andSequence // holds result of current AND operations
    counters_t *orSequence  // accumulates the final result combining OR sequences
    shortCircuit = false    // stop early if AND sequence will fail

    Loop over all words in query
        if word == OR 
        // orSequence holds final result, merge in andSequence seen so far
        // think of stepping back after previously stepping away to calculate AND
        MERGE(andSequence, orSequence) 
        shortCicuit = false  //reset shortCircuit
        continue
        if shortCircuit 
        // at least one word returned no results, so AND sequence will fail
        // skip the words until see OR and shortCircuit is reset
         continue 
        if word == AND
        continue to next word // same as if AND wasn't there
        else found non operator word 
        // think of stepping away to calculate AND sequence
        get pages for this word with match = index_find(word)
        if match == NULL  // no pages contain this query word
            shortCircuit = true // AND will fail for next word, so skip it and others until see OR
            if andSequence != NULL 
               // because this word returned no pages, drop everything AND'ed so far
               delete andSequence
               andSequence = NULL
        else  // found counters for this word in index
            if andSequence == NULL // this is the first word of AND sequence
               andSequence = new counters
               UNION(andSequence, match)  //essentially copies match to andSequence
            else // not first word in AND sequence
               INTERSECT(andSequence, match) //intersect this word's pages 

    MERGE(andSequence, orSequence)
    return orSequence

#### printResults

Print set of documents in decreasing order by score.

Pseudocode:

    print the number of results
    create a new 'counter' object
    set max score to 0
    iterate through results to find max-scoring document
        print max-scoring document (score, document id, url)
        set that document's score to 0
        reset max score to 0

### 2. Secondary (helper) functions

* `fileno` - provided by stdio
* `min` - returns minimum of two integers
* `prompt` - print prompt 'Query?' if stdin is read from keyboard
* `parseArgs` - validate pageDirectory and load index from indexFilename
* `printQuery` - print clean query
* `isValidQuery` - verifies the basic structure of the query, returning true if valid, false otherwise
* `deleteQuery` - delete query object

Helper functions for `parseArgs`:
* `merge` - calls unite on 'and' and 'or' sequences 
* `unite` - takes the union of two sequences by calling `counters_iterate`
* `unionFunction` - helper function for `unite`, sums counts (scores) of keys (document ids) from both counters
* `intersect` - takes the intersection of two sequences by calling `counters_iterate`
* `intersectFunction` - helper function for `intersect`, takes the mininum count (score) of a key between two counters

Helper functions for `printResults`:
* `countResults` - counts the number of elements in a counters of results
* `updateMax`- updates max score

## Other modules

<a id="pagedir"></a>
### pagedir

We use a re-usable module `pagedir.c` in `../common` to handle the page operations. Here, `pagedir_validate` is used to validate a pageDirectory and a new function, `pagedir_getURL`, extracts a url from a page in the pageDirectory. 

Pseudocode for `pagedir_getURL`:

    construct the pathname for the page file in pageDirectory
    open the file for reading; on error, return NULL.
    read the first line, close the file, and return url.


### libcs50

We leverage the modules of libcs50, most notably `counters` and `file`.
See that directory for module interfaces.

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

This code uses defensive-programming tactics to catch and terminate bad queries. If an error occurs while processing a query, the querier stops, prints an error message, cleans up, and asks for another query. 

## Testing plan

Here is an implementation-specific testing plan.

### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments and stdin inputs.
First, we call the querier on a small pageDirectory/index and pass it many valid and erroneous arguments to test basic functionality.
Second, we perform some additional tests on a moderate-sized pageDirectory/index (such as `toscrape` at depth 1) and run with valgrind over some of these. 
Third, fuzz-test over all three CS50 websites.
Run that script with `bash -v testing.sh` so the output of crawler is intermixed with the commands used to invoke the crawler.
Verify correct behavior by studying the output and comparing with example outputs provided in the assignment page.


<a id="crawler-output"></a>
## Crawler Output

Below is an example of output produced by the querier (run with inputs generated by fuzzquery):

```
./fuzzquery $tse/toscrape-index-2 5 0 | ./querier $tse/toscrape-depth-2 $tse/toscrape-index-2
./fuzzquery: generating 5 queries from 13563 words
Query: inthe and quarters 
No documents matched.
-----------------------------------------------
Query: hangs or visited kahneman or beneath shopping 
Matches 3 documents (ranked)
score	1 doc	171: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/night-shift-night-shift-1-20_335/index.html
score	1 doc	536: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-last-painting-of-sara-de-vos_259/index.html
score	1 doc	569: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-last-mile-amos-decker-2_754/index.html
-----------------------------------------------
Query: nationally holery or predicts 
Matches 1 documents (ranked)
score	1 doc	246: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-grand-design_405/index.html
-----------------------------------------------
Query: endlessly axell conduct or christine or curdled or sorbonne 
Matches 2 documents (ranked)
score	2 doc	357: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/a-piece-of-sky-a-grain-of-rice-a-memoir-in-four-meditations_878/index.html
score	1 doc	367: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/orchestra-of-exiles-the-story-of-bronislaw-huberman-the-israel-philharmonic-and-the-one-thousand-jews-he-saved-from-nazi-horrors_337/index.html
-----------------------------------------------
Query: endowment or cosmic lover sketchbook and priest or bfed 
Matches 2 documents (ranked)
score	1 doc	28: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/olio_984/index.html
score	1 doc	20: http://cs50tse.cs.dartmouth.edu/tse
```
