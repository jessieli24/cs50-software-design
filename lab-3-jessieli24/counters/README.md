# Counters
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from `README.md` for the bag module.

### Usage

The *counters* module, defined in `counters.h` and implemented in `counters.c`, implements a set of `const int*` _key_, `int` _count_ pairs, and exports the following functions:

```c
counters_t* counters_new(void);
int counters_add(counters_t* ctrs, const int key);
int counters_get(counters_t* ctrs, const int key);
bool counters_set(counters_t* ctrs, const int key, const int count);
void counters_print(counters_t* ctrs, FILE* fp);
void counters_iterate(counters_t* ctrs, void* arg, 
                      void (*itemfunc)(void* arg, 
                                       const int key, const int count));
void counters_delete(counters_t* ctrs);
```

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `counters.h` - the interface
* `counters.c` - the implementation
* `counterstest.c` - unit test driver
* `test.counterkeys` - test data
* `testing.out` - result of `make test &> testing.out`
* `TESTING.md` - testing summary

### Compilation

To compile, simply `make counters.o`.