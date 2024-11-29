# Makefile for common library
#
# Jessie Li
# CS 50, Fall 2022

# object files, and the target library
OBJS = pagedir.o index.o word.o 
L = ../libcs50
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $L/webpage.h $L/mem.h
index.o: index.h $L/counters.h $L/hashtable.h $L/mem.h
word.c: word.h $L/mem.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o