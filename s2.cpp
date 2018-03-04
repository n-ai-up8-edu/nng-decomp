#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sys/time.h>
#include "mctsnng.h"

int main(int _ac, char** _av) {
  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }
  srand(time(NULL));
  nng_t root_board;
  //root_board.load(_av[1]);
  root_board.load((char*)"decomp/nonogram5x5_game_Nsoluce");
  root_board.print_problem_info();
  root_board.print_board();
  struct timeval i_time;
  struct timeval f_time;
  gettimeofday (&i_time, 0);
  int nb_iterations = 10000000;
  int max_depth = 25;
  nng_tree_t T(nb_iterations, max_depth, root_board);
  for(int i = 0; i < nb_iterations; i++) {
    T.selection();
    // printf("selection:");
    // for (int j = 0; j < T.descent_size; j++)
    //   printf(" %d", T.descent_pid[j]);
    // printf("\n");
    T.expansion();
    int score = T.simulation();
    T.backpropagate(score);
    if(T.solution_found) {
      gettimeofday (&f_time, 0);
      float time = ((float)(f_time.tv_sec - i_time.tv_sec)) +
        ((float)(f_time.tv_usec - i_time.tv_usec))/1000000.0;
      printf("solved after %d iterations time %.2f\n", i, time);
      T.mcts_board.print_board();
      break;
    }
  }
  return 0;
}

