# Makefile for indexer.c and indextest.c
# Based on crawler Makefile from  https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes16/libraries
#
# Jessie Li
# CS 50, Fall 2022

C = ../common
L = ../libcs50
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

FLAGS = #-DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test valgrind clean

########### default ##################

all: indexer indextest

########### indexer ##################

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@


# indexer source dependencies
indexer.o: $C/index.h $C/word.h $C/pagedir.h
indexer.o: $L/webpage.h $L/mem.h 

# indextest source dependencies
indextest.o: $C/index.h $L/mem.h 

#######################################

test: indexer indextest testing.sh
	bash -v ./testing.sh >& testing.out

 valgrind: indexer indextest testing.sh
	$(VALGRIND) ./testing.sh > valgrind.out

clean: 
	rm -f core
	rm -f $(LIB) *~ *.o
	indexer
	indextest
