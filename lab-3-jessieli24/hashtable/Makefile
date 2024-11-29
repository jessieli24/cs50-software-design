# Makefile for 'hashtable' module
# 
# Adapted from Makefile for 'lib' module
#
# Jessie Li, October 2022
# CS 50, Fall 2022

PROG = hashtabletest
OBJS = hashtabletest.o hashtable.o hash.o ../set/set.o ../lib/file.o ../lib/mem.o  
LIBS =

# uncomment the following to turn on verbose memory logging
# (comment or uncomment the following, then "make clean; make")
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
MAKE = make
CC = gcc
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG).o: hashtable.h ../lib/file.h
hashtable.o: hashtable.h ../set/set.h hash.h ../lib/mem.h
hash.o: hash.h 
../set/set.o: ../set/set.h ../lib/mem.h
../lib/file.o: ../lib/file.h
../lib/mem.o: ../lib/mem.h

.PHONY:  test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: $(PROG) test.names
	./$(PROG) < test.names

# expects a file `test.names` to exist; it can contain any text.
valgrind: $(PROG) test.names
	$(VALGRIND) ./$(PROG) < test.names


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core