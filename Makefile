CC=g++
CFLAGS=-std=c++11 -Wall -O3

##### NONOGRAM
#random solver, single problem
s1: mynng.h s1.cpp
	$(CC) $(CFLAGS) s1.cpp -o $@

#MCTS solver, single problem
s2: mynng.h mctsnng.h s2.cpp
	$(CC) $(CFLAGS) s2.cpp -o $@

