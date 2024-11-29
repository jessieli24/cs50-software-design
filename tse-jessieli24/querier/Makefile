# Makefile for querier.c
# Based on Crawler Makefile from  https://www.cs.dartmouth.edu/~tjp/cs50/notes/notes16/libraries
#
# Jessie Li
# CS 50, Fall 2022

C = ../common
L = ../libcs50
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

FLAGS = -DDEBUG #-DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test clean

########### default ##################

all: querier

########### querier ##################

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

# indexer source dependencies
querier.o: $C/index.h $C/word.h $C/pagedir.h
querier.o: $L/counters.h $L/mem.h $L/file.h

###### fuzzquery (CS50 version) ######

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

# fuzzquery source dependencies
fuzzquery.o: $L/file.h $L/mem.h
#######################################

test: querier fuzzquery testing.sh
	bash -v ./testing.sh

clean: 
	rm -f core
	rm -f $(LIB) *~ *.o
	querier
	fuzzquery
