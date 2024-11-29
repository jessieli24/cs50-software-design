# Common directory
This directory will hold code used by multiple modules in the Tiny Search Engine (TSE).

## Usage

To build `common.a`, run `make`. 
To clean up, run `make clean`.

## Overview 

 * `pagedir.c` - saves pages produced by Crawler

## Assumptions

* `pagedir_save` in `pagedir.c` - docIDs are at most 4 digits long
