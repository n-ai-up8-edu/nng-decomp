#ifndef MCTSNNG_H
#define MCTSNNG_H
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "mynng.h"
#define K_UCT 0.1

struct nng_node_info_t {
  int node_id;
  nng_move_t move;
  int children_Wi;
  int children_Ni;
  bool terminal;
  nng_node_info_t() {
    move.line = -1; move.col = -1;
    children_Wi = 0;
    children_Ni = 0;
    terminal = false;
  }
  void print() {
    printf("move %d %d -- Wi %d -- Ni %d -- terminal %d\n", move.line, move.col, children_Ni, children_Wi, (int)terminal);
  }
};

struct nng_node_t {
  int nb_try;
  int nb_terminal;
  std::vector<nng_node_info_t> infos;
  nng_node_t() {
    nb_try = 0;
    nb_terminal = 0;
  }
};

struct nng_tree_t {
  int nb_nodes_alloc;
  int nb_nodes;
  bool solution_found;
  int* descent_pid; // pid : parent id
  int* descent_cid; // cid : children id
  int descent_alloc;
  int descent_size;
  nng_t root_board;
  nng_t mcts_board;
  std::unordered_map<std::string, int> H;
  nng_node_t* T;

  nng_tree_t(int _nb_nodes_alloc, int _descent_alloc, nng_t _root_board) {
    root_board = _root_board;
    nb_nodes_alloc = _nb_nodes_alloc;
    descent_alloc = _descent_alloc;
    T = new nng_node_t[nb_nodes_alloc];
    descent_pid = new int[descent_alloc];
    descent_cid = new int[descent_alloc];
    T[0].infos.resize(root_board.get_nb_moves());
    nb_nodes = 1;
    solution_found = false;
  }
  ~nng_tree_t() {
    delete[] T;
    delete[] descent_pid;
    delete[] descent_cid;
  }
  void print(int _id) {
    printf("node %p\n", &T[_id]);  
    for(nng_node_info_t i : T[_id].infos) i.print();
  }
  void selection() {
    while(true) {
      mcts_board = root_board;
      descent_size = 0;
      if(selection(0, 0)) break;
    }
  }
  bool selection(int _node_id, int _depth) {
    if(T[_node_id].nb_terminal == (int)T[_node_id].infos.size()) {
      // terminal info is missing on parent node due to hashtable
      // => backpropagate terminal info and restart selection
      if(_depth == 0) {
	printf("[selection] problem WITHOUT SOLUTION ?\n");
	exit(0);
      }
      T[descent_pid[_depth-1]].infos[descent_cid[_depth-1]].terminal = true;
      T[descent_pid[_depth-1]].nb_terminal ++;
      return false;
    }
    if(_depth >= descent_alloc) {
      printf("ERROR : _depth >= descent_alloc in selection\n"); exit(0);
    }
    descent_pid[_depth] = _node_id;
    descent_cid[_depth] = -1;
    if(T[_node_id].nb_try < mcts_board.get_nb_moves()) {
      descent_size = _depth+1;
      return true;
    }
    int best_id = 0;
    double best_score = 0.0;
    bool all_terminal = true;
    for(int i = 0; i < (int)T[_node_id].infos.size(); i++) {
      if(T[_node_id].infos[i].terminal == false) {
        all_terminal = false;
        double a = ((double) T[_node_id].infos[i].children_Wi ) / T[_node_id].infos[i].children_Ni;
        double b = log((double) T[_node_id].nb_try) / T[_node_id].infos[i].children_Ni;
        double score = a + K_UCT * b;
        if(score > best_score) {
          best_id = i;
          best_score = score;
        }
      }
    }
    descent_cid[_depth] = best_id;
    if(all_terminal) {
      printf("ERROR selection all_terminal %d %d %d\n", _node_id, T[_node_id].nb_terminal, (int)T[_node_id].infos.size()); exit(0);
    }
    mcts_board.play(T[_node_id].infos[best_id].move);
    std::string strboard =  mcts_board.mkH();
    std::unordered_map<std::string, int>::iterator ii = H.find(strboard);
    if(ii != H.end()) {
      return selection(ii->second, _depth+1);
    } else {
      descent_pid[_depth+1] = nb_nodes;
      descent_cid[_depth+1] = -1;
      descent_size = _depth+2;
      H[strboard] = nb_nodes;
      T[nb_nodes].infos.resize(mcts_board.get_nb_moves());
      nb_nodes++;
    }
    return true;
  }
  void expansion() {
    int selection_id = descent_pid[descent_size-1];
    if(T[selection_id].nb_try == 0) {
      std::vector<nng_move_t> mm = mcts_board.get_all_moves();
      if(mm.size() != T[selection_id].infos.size()) {
	printf("ERROR mm.size() %d != T[selection_id].infos.size() %d\n",
	       (int)mm.size(), (int)T[selection_id].infos.size());
	exit(0);
      }
      for(int i = 0; i < (int)mm.size(); i++) {
	T[selection_id].infos[i].move.line = mm[i].line;
	T[selection_id].infos[i].move.col = mm[i].col;
      }
    }
    int nb_moves = ((int)T[selection_id].infos.size()) - T[selection_id].nb_try;
    int rmove = ((int)rand())%nb_moves;
    int expansion_id = 0;
    for(int i = 0; i < (int)T[selection_id].infos.size(); i++) {
      if(T[selection_id].infos[i].children_Ni == 0) {
        if(rmove == 0) {
          expansion_id = i; break;
        }
        rmove--;
      }
    }
    mcts_board.play(T[selection_id].infos[expansion_id].move);
    descent_cid[descent_size-1] = expansion_id;
  }
  int simulation() {
    int selection_id = descent_pid[descent_size-1];
    int expansion_id = descent_cid[descent_size-1];
    if(mcts_board.terminal()) {
      T[selection_id].nb_terminal ++;
      T[selection_id].infos[expansion_id].terminal=true;
       return 0;  
    }
    mcts_board.playout();
    return mcts_board.score();
  }
  void backpropagate(int _score) {
    for(int i = descent_size-1; i >= 0; i--) {
      int selection_id = descent_pid[i];
      int expansion_id = descent_cid[i];
      T[selection_id].nb_try ++;
      T[selection_id].infos[expansion_id].children_Ni ++;
      if(T[selection_id].nb_terminal == (int)T[selection_id].infos.size()) {
	if(i == 0) {
	  printf("[backpropagate] problem WITHOUT SOLUTION ?\n");
	  exit(0);
	}
	int prev_selection_id = descent_pid[i-1];
	int prev_expansion_id = descent_cid[i-1];
	T[prev_selection_id].nb_terminal ++;
	T[prev_selection_id].infos[prev_expansion_id].terminal = true;
      }
      if(_score == 100) {
        T[selection_id].infos[expansion_id].children_Wi ++;
        solution_found = true;
      }
    }
  }
};
#endif /* MCTSNNG_H */
