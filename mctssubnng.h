#ifndef MCTSSUBNNG_H
#define MCTSSUBNNG_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "subnng.h"
#define K_UCT 0.1

struct subnng_node_info_t {
  int node_id;
  int index_to_move; // -1 if noop
  int children_Wi;
  int children_Ni;
  bool terminal;
};

struct subnng_node_t {
  in nb_try;
  std::vector<subnng_node_info_t> infos;
  subnng_node_t() {
    nb_try = 0;
  }
};

struct subnng_tree_t {
  std::unordered_map<std::string, int> H;
  nng_node_t* T;
  int* descent_pid; // pid : parent id
  int* descent_cid; // cid : children id
  int descent_alloc;
  int descent_size;
  int nb_nodes_alloc;
  int nb_nodes;

  void selection(int _node_id, int _depth) {
  }
  void expansion() {
  }
  void backpropagate(int _score) {
  }
};

struct nng_subtrees_t {
  nng_t root_board;
  nng_t mcts_board;
  subnng_tree_t* SUB_TREE;
  nng_subtrees_t(int _nb_nodes_alloc, int _descent_alloc) {
  }
  void finalize() {
    delete[] SUB_TREE;
  }
  void selection(int _depth) {
  }
  void expansion() {
  }
  int simulation() {
    return mcts_board.score();
  }
  void backpropagate(int _score) {
  }
};
#endif /* MCTSSUBNNG_H */
