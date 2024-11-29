# CS50 TSE Indexer
## Implementation Specification

Core subset:

*  Data structures
*  Control flow: pseudo code for overall flow, and for each of the functions
*  Detailed function prototypes and their parameters
*  Error handling and recovery
*  Testing plan

## Data structures 

We use an `index` keyed by words, storing `counters` of (docID, count) pairs.
The `index` starts empty. 
The size of `index` is fixed at 200 in `indexer.c` but calculated from the number of lines in an index file in `indexertest.c`. 

## Control flow

The Indexer is implemented in `indexer.c` and tested with `indextest.c`.

### indexer

#### 1. main

Takes two parameters: 
* `pageDirectory` - crawler-produced directory of page files
* `indexFilename` - where to save index 

Validates parameters, calls `indexBuild`, `index_print`, and `index_delete`, then exits zero.

#### 2. indexBuild

"Builds an in-memory index from webpage files it finds in the pageDirectory" (Indexer Design Spec).

Pseudocode (from 10/17 activity solution):
```
    create a new index with 200 slots
    docID = 1
    load a webpage_t page with pagedir_load
    while page not NULL
      indexPage(index, page, docID)
      delete page
      load next page, ++docID
    return index
    
```

#### 3. indexPage

Adds words from `webpage` to the `index`.

Pseudocode (from 10/17 activity solution):
```
    get next word with webpage_getNextWord()
    while word not NULL
      normalizeWord(word) if length > 2
      get counters for word with index_find(index, word)
      if word not in index
          create new counters
          insert empty counters into index
      increment word count with counters_add(counters, docID)
      word = webpage_getNextWord()
    free(word)
    
```

### indextest

#### 1. main

Loads index from `oldIndex` filename, writes index it to a newly created `newIndex` file.

Takes two parameters: 
* `oldIndexFilename` - indexer-produced file
* `newIndexFilename` - where to save new index

## Other modules

### index

Wrapper for hashtable module. See Indexer Design Spec and 10/17 activity solution for more details. 

Pseudocode for `index_save` (based on 10/17 activity solution):
```
  open file for writing
  if file is NULL, exit non-zero
  if index not NULL, 
    pass index to hashtable_iterate with word_print
    
```
    
`word_print` prints word and calls count_iterate with count_print to print counters.

Pseudocode for `index_load` (based on 10/17 activity solution):
```
  open file for reading
  get wordcount with file_numLines
  if file is NULL or wordcount is 0, return NULL
  initialize word, id, count
  while there is a word left in file
    initialize counters
    while there are id,count pairs left in line
      set value of counter with id to count
    index word with counters
    free word
  close file
  return index
  
```

### word

Pseudocode for `normalizeWord`: 
```
    create pointer to head of string
    while pointer doesn't point to '\0'
      tolower(letter)
      move pointer one along with pointer++
      
```

### pagedir

`pagedir_validate` is like `pagedir_init`, but opens .crawler for writing. 

Pseudocode for `pagedir_load`:
```
    construct the pathname for the page file in pageDirectory
    open that file for reading
    get URL (first line)
    get depth (second line)
    get HTML (rest)
    initialize webpage with URL, depth, HTML
    close file
    return webpage
    
```
For additional information, see Crawler Implementation Spec.

### libcs50

Detailed in Crawler Implementation Spec.

## Function prototypes

### indexer

```c
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int id);

```

### pagedir

```c
char* pagedir_validate(char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

```

### index

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* key, counters_t* ctrs);
counters_t* index_find(index_t* index, const char* key);
void index_save(index_t* index, const char* indexFilename);
void index_delete(index_t* index);
index_t* index_load(const char* indexFilename);

```

### word

```c
void normalizeWord(char* word);

```

## Error handling and recovery (from Crawler Implementation Spec)

Problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

## Testing plan

Here is an implementation-specific testing plan.

### Integration/system testing (based on Crawler Implementation Spec)

We write a script `testing.sh` that invokes indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case (such Crawler output from `toscrape` at depth 1).
Third, runs over Crawler output from all three CS50 websites (`letters` at depths 0, 6, `toscrape` at depths 0,1,2, `wikipedia` at depths 0,1)
Run that script with `bash -v testing.sh` so the output of crawler is intermixed with the commands used to invoke the crawler.
Verify correct behavior by running indextest on the index produced by indexer and studying the output.


<a id="indexer-output"></a>
## Indexer Output

Below is a file produced by indexer when the program indexes one of our simple test page directories. 

Each line is a word followed by document ID, count pairs, indicating how many times that word appeared in a listed document. 
