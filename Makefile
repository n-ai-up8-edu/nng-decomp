CC=g++
CFLAGS=-std=c++11 -Wall -O3

##### NONOGRAM
#random solver, single problem
s1: mynng.h s1.cpp
	$(CC) $(CFLAGS) s1.cpp -o $@

#MCTS solver, single problem
s2: mynng.h mctsnng.h s2.cpp
	$(CC) $(CFLAGS) s2.cpp -o $@

#MCTS solver, subproblems
s3: mynng.h subnng.h mctssubnng.h s3.cpp
	$(CC) $(CFLAGS) s3.cpp -o $@

clean:
	rm -f s1 s2 s3
