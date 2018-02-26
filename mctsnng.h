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
  int node_id;        // identifiant unique
  nng_move_t move;    // le coup joué pour arriver là (coordonnées)
  int children_Wi;    // nombre de victoires (victoire=1, échec=0)
  int children_Ni;    // nombre de visites
  bool terminal;      // sous-arbre complètement exploré
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
  int nb_try;                         // nombre de descentes à partir de ce nœud
  int nb_terminal;                    // nœuds enfants complètement explorés
  std::vector<nng_node_info_t> infos; // stats des nœuds enfants
  nng_node_t() {
    nb_try = 0;
    nb_terminal = 0;
  }
};

struct nng_tree_t {
  int nb_nodes_alloc;  // nb d'itérations, nb de nœuds à prévoir dans l'arbre
  int nb_nodes;        // nb de noeuds dans l'arbre (utilisé pour node_id)
  bool solution_found; // indique si une solution est trouvée dans la branche
  int* descent_pid;    // parent id: T idx à chaque pas (size: descent_alloc)
  int* descent_cid;    // children id: infos idx à chq pas (size: descent_alloc)
  int descent_alloc;   // profondeur max d'exploration
  int descent_size;    // profondeur courante d'exploration
  nng_t root_board;    // sauvegarde de l'état initial
  nng_t mcts_board;    // position courante pendant la descente
  std::unordered_map<std::string, int> H; // table de transposition
  nng_node_t* T;       // racine de l'arbre UCT

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
    // si la branche a été complètement explorée en passant par un autre chemin
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
    // profondeur max atteinte
    if(_depth >= descent_alloc) {
      printf("ERROR : _depth >= descent_alloc in selection\n"); exit(0);
    }
    // on note le nœud courant
    descent_pid[_depth] = _node_id;
    descent_cid[_depth] = -1;
    // un nœud enfant jamais exploré, on arrête là
    if(T[_node_id].nb_try < mcts_board.get_nb_moves()) {
      descent_size = _depth+1;
      return true;
    }
    // sinon on sélectionne le meilleur enfant (non complètement exploré)
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
    descent_cid[_depth] = best_id; // on note l'enfant sélectionné
    //tous les enfants sont complètement explorés, le parent devrait être marqué
    if(all_terminal) {
      printf("ERROR selection all_terminal %d %d %d\n", _node_id, T[_node_id].nb_terminal, (int)T[_node_id].infos.size()); exit(0);
    }
    // on joue le coup pour passer à la position suivante
    mcts_board.play(T[_node_id].infos[best_id].move);
    std::string strboard =  mcts_board.mkH(); // calcul du hash de la position
    // recherche de la position dans la table de transpositions
    std::unordered_map<std::string, int>::iterator ii = H.find(strboard);
    if(ii != H.end()) {
      return selection(ii->second, _depth+1); // on continue la descente
    } else {
      descent_pid[_depth+1] = nb_nodes; // nouveau nœud sélectionné
      descent_cid[_depth+1] = -1;       // pas d'enfant sélectionné
      descent_size = _depth+2;          // nœud courant + enfant
      H[strboard] = nb_nodes;       // on place le nœud dans la table de transpo
      T[nb_nodes].infos.resize(mcts_board.get_nb_moves()); // initialisation
      nb_nodes++;                       // on prépare l'id pour le prochain nœud
    }
    return true;
  }
  void expansion() {
    int selection_id = descent_pid[descent_size-1];
    // si c'est un nouveau nœud, on termine l'initialisation
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
    // on choisi un des coups pas explorés
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
    // on joue le coup et on l'ajoute à la descente
    mcts_board.play(T[selection_id].infos[expansion_id].move);
    descent_cid[descent_size-1] = expansion_id;
  }
  int simulation() {
    int selection_id = descent_pid[descent_size-1];
    int expansion_id = descent_cid[descent_size-1];
    // si la position est terminale, on note directement (pas de nœud créé)
    if(mcts_board.terminal()) {
      T[selection_id].nb_terminal ++;
      T[selection_id].infos[expansion_id].terminal=true;
       return 0;
    }
    // sinon on fait un playout et on retourne le score
    mcts_board.playout();
    return mcts_board.score();
  }
  void backpropagate(int _score) {
    // on remonte chaque nœud
    for(int i = descent_size-1; i >= 0; i--) {
      int selection_id = descent_pid[i];
      int expansion_id = descent_cid[i];
      // on augmente le nombre de visites du parent et enfant
      T[selection_id].nb_try ++;
      T[selection_id].infos[expansion_id].children_Ni ++;
      // si tous les enfants sont terminaux, on remonte l'info
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
      // si on a trouvé le score max, on le note et on le signale globalement
      if(_score == 100) {
        T[selection_id].infos[expansion_id].children_Wi ++;
        solution_found = true;
      }
    }
  }
};
#endif /* MCTSNNG_H */
