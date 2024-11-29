# Hashtable: Testing
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from *Testing* section in `README.md` for the bag module.

The `hashtabletest.c` program reads names from stdin, creates student items, and puts each name, student pair into a hashtable. 

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.

### Functionality 

The following are tested in `hashtabletest.c`:

* Creating a new hashtable with `hashtable_new`
* Inserting key, item pairs with `hashtable_insert`
    * New keys
    * Duplicate keys
* Finding keys in hashtable with `hashtable_find`
    * Keys added
    * Keys not yet added
* Counting nodes in a hashtable with `hashtable_iterate`
* Printing hashtables with `hashtable_print`
    * Empty hashtables
    * Non-empty hashtables

Additional error and edge cases are also tested, including NULL parameters. 

### Input

Provided by `test.names`.

```bash
Alice
Bob
Alice
Charlie
Mickey
Mickey
Mouse
Minnie
Donald
Snoopy
Alice
Goofy

```

### Output

As seen in `testing.out`.

```bash
gcc -Wall -pedantic -std=c11 -ggdb    -c -o hashtabletest.o hashtabletest.c
gcc -Wall -pedantic -std=c11 -ggdb    -c -o hashtable.o hashtable.c
gcc -Wall -pedantic -std=c11 -ggdb    -c -o hash.o hash.c
gcc -Wall -pedantic -std=c11 -ggdb  hashtabletest.o hashtable.o hash.o ../set/set.o ../lib/file.o ../lib/mem.o  -o hashtabletest
./hashtabletest < test.names
Create hashtable...

Printing empty hashtable...
{}
{}
{}
{}
{}
{}
{}

Test with null hashtable, good item...
Test with null key...
Test with null item...
Test with null hashtable, null key, null item...

Count (should be zero): 0
'Alice' is a new key
hashtable_insert inserted Alice, 16
'Bob' is a new key
hashtable_insert inserted Bob, 17
'Alice' is already a key in the hashtable
hashtable_insert failed to insert Alice
'Charlie' is a new key
hashtable_insert inserted Charlie, 19
'Mickey' is a new key
hashtable_insert inserted Mickey, 20
'Mickey' is already a key in the hashtable
hashtable_insert failed to insert Mickey
'Mouse' is a new key
hashtable_insert inserted Mouse, 22
'Minnie' is a new key
hashtable_insert inserted Minnie, 23
'Donald' is a new key
hashtable_insert inserted Donald, 24
'Snoopy' is a new key
hashtable_insert inserted Snoopy, 25
'Alice' is already a key in the hashtable
hashtable_insert failed to insert Alice
'Goofy' is a new key
hashtable_insert inserted Goofy, 27

Count (should be 9): 9

Testing hashtable_find with NULL hashtable...got NULL
Testing hashtable_find with NULL key...got NULL

The hashtable:
{}
{(Donald,24),(Mickey,20),(Charlie,19),}
{}
{(Snoopy,25),(Bob,17),}
{(Minnie,23),(Alice,16),}
{(Goofy,27),(Mouse,22),}
{}


Printing NULL hashtable...
(null)

Printing hashtable with NULL file pointer...


Printing hashtable with NULL itemprint...
{}
{}
{}
{}
{}
{}
{}


delete the hashtable...

```