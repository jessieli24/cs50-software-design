# Makefile for 'counters' module
#
# Adapted from Makefile for 'lib' module
# 
# Jessie Li, October 3, 2022
# CS 50, Fall 2022

PROG = counterstest
OBJS = counterstest.o counters.o ../lib/file.o ../lib/mem.o 
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

$(PROG).o: counters.h ../lib/file.h ../lib/mem.h
counters.o: counters.h 
../lib/file.o: ../lib/file.h
../lib/mem.o: ../lib/mem.h

.PHONY: test valgrind clean

# expects a file `test.counterkeys` to exist; it can contain any text.
test: $(PROG) test.counterkeys
	./$(PROG) < test.counterkeys

# expects a file `test.counterkeys` to exist; it can contain any text.
valgrind: $(PROG) test.counterkeys
	$(VALGRIND) ./$(PROG) < test.counterkeys

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core