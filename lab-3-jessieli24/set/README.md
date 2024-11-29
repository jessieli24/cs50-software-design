# Set
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from `README.md` for the bag module.

### Usage

The *set* module, defined in `set.h` and implemented in `set.c`, implements a set of `const char*` _key_, `void*` _item_ pairs, and exports the following functions:

```c
set_t* set_new(void);
bool set_insert(set_t* set, const char* key, void* item);
void* set_find(set_t* set, const char* key);
void set_print(set_t* set, FILE* fp, 
            void (*itemprint)(FILE* fp, const char* key, void* item) );
void set_iterate(set_t* set, void* arg,
            void (*itemfunc)(void* arg, const char* key, void* item) );
void set_delete(set_t* set, void (*itemdelete)(void* item) );
```

### Assumptions

Cannot insert two keys with the same `item` (`set_delete` will try to free the same item twice).

### Files

* `Makefile` - compilation procedure
* `set.h` - the interface
* `set.c` - the implementation
* `settest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`
* `TESTING.md` - testing summary

### Compilation

To compile, simply `make set.o`.
