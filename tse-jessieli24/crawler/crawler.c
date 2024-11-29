/*
 * crawler.c - "Crawls the web and retrieves webpages starting from a 'seed' URL. 
 * It parses the seed webpage, extracts any embedded URLs, then retrieves each 
 * of those pages, recursively, but limiting its exploration to a given 'depth'" 
 * (Requirements Spec)
 * 
 * Uses design from Design and Implementation Specs
 *
 * Jessie Li
 * CS 50 October 2022
 */

#include <stdlib.h>
#include "pagedir.h"
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "mem.h"

/********** local functions **********/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);
static bool alphtoint(const char *str, int *i); 

/*************************************/

/**************** logr ***************/
/* "Log one word (1-9 chars) about a given url" (Implementation Spec) */                                                                     
static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}

/************** alphtoint **************/
/* Parse integer from string */
static bool alphtoint(const char *str, int *i)
{
    char c; 
    if (sscanf(str, "%d%c", i, &c) == 1) return true;
    return false;
}
 
/************* parseArgs *************/
/* "Given arguments from the command line, 
 * extract them into the function parameters; 
 * return only if successful" (Implementation Spec). */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth) 
{
    // must provide 4 arguments
    if (argc != 4) exit(1);
    
    // parse URL
    *seedURL = normalizeURL(argv[1]);
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "seedURL: %s is invalid or not internal\n", argv[1]);
        exit(1);
    }
    // parse directory
    *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "unable to create a file of form %s/.crawler\n", *pageDirectory);
        exit(1);
    }
    // parse depth
    if (!alphtoint(argv[3], maxDepth) || *maxDepth > 10 || *maxDepth < 0) {
        fprintf(stderr, "maxDepth: %d is invalid or out of range\n", *maxDepth);
        exit(1);
    }
}

/*************** crawl ***************/
/* Crawl from seedURL to maxDepth, saving pages in pageDirectory */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{   
    // initialize hashtable pagesSeen
    hashtable_t *pagesSeen = hashtable_new(200);
    // initialize bag pagesToCrawl
    bag_t *pagesToCrawl = bag_new();
    // make a webpage for seedURL
    webpage_t *seed = webpage_new(seedURL, 0, NULL);

    // add seedURL to pagesSeen
    if (!hashtable_insert(pagesSeen, seedURL, "")) {
        // hashtable_insert checks if any parameter NULL
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    // add seed to the bag of webpages to crawl
    bag_insert(pagesToCrawl, seed);
    
    // explore
    int docID = 0; 
    for (webpage_t *page = bag_extract(pagesToCrawl); 
                    page != NULL; page = bag_extract(pagesToCrawl)) {
        // retrieve HTML 
        if (!webpage_fetch(page)) {
            fprintf(stderr, "couldn't fetch %s\n", webpage_getURL(page));
            exit(1);
        }
        logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
        // save page details
        pagedir_save(page, pageDirectory, ++docID);
        // if page not at maxDepth, scan it for more URLs
        if (webpage_getDepth(page) < maxDepth) {
            pageScan(page, pagesToCrawl, pagesSeen);
        }
        // delete the webpage 
        webpage_delete(page);
    }

    // clean up
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, webpage_delete);
}

/************** pageScan *************/
/* "Given a webpage, scan the given page to extract any links (URLs), 
 * ignoring non-internal URLs; for any URL not already seen before 
 * (i.e., not in the hashtable), add the URL to both the hashtable 
 * pages_seen and to the bag pages_to_crawl" (Implementation Spec). */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    logr("Scanning", webpage_getDepth(page), webpage_getURL(page));

    int pos = 0;
    for (char *next = webpage_getNextURL(page, &pos); 
                    next != NULL; next = webpage_getNextURL(page, &pos)) {
        char *url = normalizeURL(next);
        mem_free(next);
        logr("Found", webpage_getDepth(page), url);
        if (!isInternalURL(url)) {
            logr("IgnExtrn", webpage_getDepth(page), url);
            mem_free(url);
            continue;
        }
        // url is already in the table
        if (!hashtable_insert(pagesSeen, url, "")) {
            logr("IgnDupl", webpage_getDepth(page), url);
            mem_free(url);
            continue;
        }
        // url was not in the table, add to bag
        webpage_t *new = mem_assert(webpage_new(url, webpage_getDepth(page)+1, NULL), "new (pageScan)");
        bag_insert(pagesToCrawl, new);
        logr("Added", webpage_getDepth(page), webpage_getURL(new));
    }
}

int main(const int argc, char* argv[])
{
    char* seedURL; 
    char* pageDirectory;
    int maxDepth; 

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}
