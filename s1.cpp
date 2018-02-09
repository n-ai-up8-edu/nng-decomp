#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sys/time.h>
#include "mynng.h"

int main(int _ac, char** _av) {
  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }
  srand(1);
  nng_t N;
  //N.load(_av[1]);
  N.load((char*)"decomp/nonogram5x5_1_game.txt");
  N.print_problem_info();
  N.print_board();
  struct timeval i_time;
  struct timeval f_time;
  gettimeofday (&i_time, 0);
  for(int i = 0; i < 10000000; i++) {
    nng_t NN;
    NN.copy(N);
    NN.playout();
    if(NN.score() == 100) {
      gettimeofday (&f_time, 0);
      float time = ((float)(f_time.tv_sec - i_time.tv_sec)) +
        ((float)(f_time.tv_usec - i_time.tv_usec))/1000000.0;
      printf("solved after %d iterations time %.2f\n", i, time);
      NN.print_board();
      //NN.print_board_info();
      break;
    }
  }
  return 0;
}


