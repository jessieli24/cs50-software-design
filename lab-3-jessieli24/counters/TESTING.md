# Counters: Testing
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from *Testing* section in `README.md` for the bag module.

The `counterstest.c` program reads integers from stdin and adds them to a counter set; it tracks a counter for each integer key. 

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.

### Functionality 

The following are tested in `counterstest.c`:

* Creating a new set with `counters_new`
* Adding keys and tracking their counts with `counters_add`
    * New keys
    * Keys previously added
* Retrieving counts of keys with `counters_get`
    * Keys added 
    * Keys not yet added
* Setting counter values with `counters_set`
    * New keys
    * Existing keys
* Counting keys and total adds with `counters_iterate`
* Printing counters with `counters_print`
    * Empty counter sets
    * Non-empty counter sets

Additional error and edge cases are also tested, including NULL parameters. 

### Input

Provided by `test.counterkeys`.

```bash
1
2
3
-2
2
1
4
7
1
5

```

### Output

As seen in `testing.out`.

```bash
./counterstest < test.counterkeys
Create first counters...

Printing empty counters...
{}

Test with null counters, good key...
test with negative key...
test with null counters, negative key...

Number of keys (should be zero): 0

Testing counters_add and counters_get...
key 1 -- before: 0, after: 1
key 2 -- before: 0, after: 1
key 3 -- before: 0, after: 1
key -2 -- before: 0, after: 0
key 2 -- before: 1, after: 2
key 1 -- before: 1, after: 2
key 4 -- before: 0, after: 1
key 7 -- before: 0, after: 1
key 1 -- before: 2, after: 3
key 5 -- before: 0, after: 1
key 5 -- before: 1, after: 2

Total adds (should be 10): 10

Number of keys: 6

The counters:
{5=2,7=1,4=1,3=1,2=2,1=3,}


Testing counters_set...
with a key in counters (set 5 to 10): true
with a key not in counters (set 999 to 10): true
with a negative key (set -1 to 10): false
with a negative count(set 5 to -10): false
with a NULL counters: false

The counters:
{999=10,5=10,7=1,4=1,3=1,2=2,1=3,}


Printing NULL counters...
(null)

Printing counters with NULL file pointer...


delete the counters...

```