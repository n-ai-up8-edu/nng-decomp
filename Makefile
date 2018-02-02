CC=g++
CFLAGS=-std=c++11 -Wall -O3

##### NONOGRAM
#MCTS solver, single problem
s1: mynng.h s1.cpp
	$(CC) $(CFLAGS) s1.cpp -o $@

