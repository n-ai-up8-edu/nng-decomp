#ifndef SUBNNG_H
#define SUBNNG_H
#include <cstdio>
#include <cstdlib>
#include "mynng.h"
#include <vector>
#include <unordered_map>

struct subnng_t {
  int boardsize; // !!! ALLOC SIZE
  int* board;
  int* index;
  int nb_index;
  int nb_val_set;

  subnng_t(int _alloc){
    boardsize = _alloc;
    board = new int[boardsize];
    index = new int[boardsize];
  }
  void init() {
    for(int i = 0; i < boardsize; i++) board[i] = WHITE;
    nb_index=0;
    nb_val_set=0;
  }
  void add_index(int _val) {
    index[nb_index] = _val;
    nb_index++;
  }
  int get_nb_moves() {
    return nb_index-nb_val_set;
  }
  nng_move_t get_move(int _id, nng_t _N) {
    nng_move_t mm;
    int id = _id;
    for(int i = 0; i < nb_index; i++) {
      if(id == 0 && board[i] == WHITE) {
	mm.line = index[i]/_N.nbc;
	mm.col = index[i]%_N.nbc;
	return mm;
      }
      if(board[i] == WHITE) id--;
    }
    printf("ERROR : NO %d MOVE\n", _id); exit(0);
    return mm;
  }
  std::vector<nng_move_t> get_all_moves(nng_t _N){
    std::vector<nng_move_t> ret;
    for(int i = 0; i < nb_index; i++) {
      if(board[i] == WHITE) {
	nng_move_t mm;
	mm.line = index[i]/_N.nbc;
	mm.col = index[i]%_N.nbc;
	ret.push_back(mm);
      }
    }
    return ret;
  }
};

struct nng_decomposition_t {
  std::unordered_map<int, int> sub_id; // move index --> SUB[i]
  subnng_t* SUB;
  int sub_size = 0;
  void load(char* _file) {
    FILE* fp;
    if ((fp = fopen(_file,"r")) == 0) {
      fprintf(stderr, "fopen %s error\n", _file); return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t linesize = getline(&line, &len, fp);
    linesize=linesize+0; // to avoid a warning
    int nb_subproblems;
    sscanf(line, "%d", &nb_subproblems);
    while ((read = getline(&line, &len, fp)) != -1) {
      printf("reading %s\n", line);
    }
    free(line);
    fclose(fp);
  }
};
nng_decomposition_t decomp;
#endif /* SUBNNG_T */
