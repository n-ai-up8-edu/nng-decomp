#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <unordered_map>
#include "mynng.h"

int main(int _ac, char** _av) {
  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }
  srand(1);
  nng_t N;
  //N.load(_av[1]);
  N.load((char*)"nonogram-001.input");
  N.print_problem_info();
  N.print_board();
  for(int i = 0; i < 10000000; i++) {
    nng_t NN;
    NN.copy(N);
    NN.playout();
    if(NN.score() == 100) {
      printf("solved after %d iterations:\n", i);
      NN.print_board();
      //NN.print_board_info();
      break;
    }
  }
  return 0;
}


