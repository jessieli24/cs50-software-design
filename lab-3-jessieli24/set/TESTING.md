# Set: Testing
## Jessie Li, CS 50 Lab 3, Fall 2022
Adapted from *Testing* section in `README.md` for the bag module.

The `settest.c` program reads names from stdin, creates student items, and puts each name, student pair into a set. 

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.

### Functionality 

The following are tested in `settest.c`:

* Creating a new set with `set_new`
* Inserting key, item pairs with `set_insert`
    * New keys
    * Duplicate keys
* Finding keys in set with `set_find`
    * Keys added to set
    * Keys not yet added to set
* Counting nodes in a set with `set_iterate`
* Printing sets with `set_print`
    * Empty sets
    * Non-empty sets

Additional error and edge cases are also tested, including NULL parameters. 

### Input

Provided by `test.names`.

```bash
Alice
Wonderland
Alice
Girl
Mickey
Mouse
Minnie
Mouse

```

### Output

As seen in `testing.out`.

```bash
./settest < test.names
Create set...

Printing empty set...
{}

Test with null set, good item...
Test with null key...
Test with null item...
Test with null set, null key, null item...

Count (should be zero): 0

Testing set_find with a NULL set: NULL
Testing set_find with a NULL key: NULL

'Alice' is a new key
set_insert inserted Alice, 16
'Wonderland' is a new key
set_insert inserted Wonderland, 17
'Alice' is already a key in the set
set_insert failed to insert Alice
'Girl' is a new key
set_insert inserted Girl, 19
'Mickey' is a new key
set_insert inserted Mickey, 20
'Mouse' is a new key
set_insert inserted Mouse, 21
'Minnie' is a new key
set_insert inserted Minnie, 22
'Mouse' is already a key in the set
set_insert failed to insert Mouse

Count (should be 6): 6

The set:
{(Minnie,22),(Mouse,21),(Mickey,20),(Girl,19),(Wonderland,17),(Alice,16),}

Printing NULL set...
(null)

Printing set with NULL file pointer...


Printing set with NULL itemprint...
{}

delete the set...

```