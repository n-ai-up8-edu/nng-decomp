#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <unordered_map>
#include "mynng.h"
int main(int _ac, char** _av) {

  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }

  nng_t N;
  //N.load(_av[1]);
  N.load((char*)"nonogram-001.input");
  N.print_constraints();
  N.print_board();
  N.playout();
  N.print_board();
  N.print_board_id_and_size();
  
  return 0;
}

//// 1 voir les evidences
// nonogram-001.input
//           1
//     2 3 3 3 1
// 2         x x
// 1       x . .
// 3     x x x .
// 4   x x x x .
// 2 1 x x . x .


/// 2 voir les evidences
// nonogram-002.input
//       1
//     2 3 3 2 2
// 2     x
// 1     .
// 2     x
// 4     x
// 4     x

