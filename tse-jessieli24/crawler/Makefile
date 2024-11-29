# Makefile for crawler.c
# Grabbed from https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes16/libraries
#
# Jessie Li
# CS 50, Fall 2022

C = ../common
L = ../libcs50
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = #-DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test valgrind clean

########### default ##################

all: crawler

########### crawler ##################

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

# crawler source dependencies
crawler.o: $L/webpage.h $C/pagedir.h
crawler.o: $L/mem.h $L/bag.h $L/hashtable.h 

test: crawler testing.sh
	bash -v testing.sh >& testing.out

valgrind: crawler testing.sh
	$(VALGRIND) ./testing.sh > valgrind.out

clean: 
	rm -f core
	rm -f $(LIB) *~ *.o
	crawler
	