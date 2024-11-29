# Hashtable
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from `README.md` for the bag module.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements a hashtable of `const char*` _key_, `void*` _item_ pairs, and exports the following functions:

```c
hashtable_t* hashtable_new(void);
bool hashtable_insert(hashtable_t* ht, const char* key, void* item);
void* hashtable_find(hashtable_t* ht, const char* key);
void hashtable_print(hashtable_t* ht, FILE* fp, 
            void (*itemprint)(FILE* fp, const char* key, void* item) );
void hashtable_iterate(hashtable_t* ht, void* arg,
            void (*itemfunc)(void* arg, const char* key, void* item) );
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) );
```

### Assumptions

Cannot insert two keys with the same `item` (`hashtable_delete` will try to free the same item twice).

### Files

* `Makefile` - compilation procedure
* `hashtable.h` - the interface
* `hashtable.c` - the implementation
* `hash.h` - hash interface
* `hash.c` - hash implementation
* `hashtabletest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`
* `TESTING.md` - testing summary

### Compilation

To compile, simply `make hashtable.o`.