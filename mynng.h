#ifndef MYNNG_H
#define MYNNG_H
#include <cstdio>
#include <cstdlib>
#define WHITE 0
#define BLACK 1
char* cboard = (char*)".X";

struct nng_move_t {
  int line; int col;
};
#define MAX_CONSTRAINTS 5
#define MAX_LINES 10
#define MAX_COLS 10
struct nng_t {
  int nbl;
  int nbc;
  int c_lines[MAX_LINES][MAX_CONSTRAINTS];
  int nb_c_lines[MAX_LINES];
  int c_cols[MAX_COLS][MAX_CONSTRAINTS];
  int nb_c_cols[MAX_COLS];
  int sum_c_lines;

  int max_lines[MAX_LINES];
  int max_cols[MAX_COLS];

  int board_lines_id[MAX_LINES][MAX_COLS];
  int board_cols_id[MAX_LINES][MAX_COLS];
  int board_lines_size[MAX_COLS];
  int board_cols_size[MAX_LINES];
  
  int board[MAX_LINES][MAX_COLS];
  int nb_val_set;
  
  void init(int _nbl, int _nbc) {
    nbl = _nbl; nbc = _nbc;
    for(int i = 0; i < nbl; i++)
      for(int j = 0; j < nbc; j++) {
	board[i][j] = WHITE;
	board_lines_id[i][j] = -1;
	board_cols_id[i][j] = -1;
      }
    for(int i = 0; i < nbc; i++) board_lines_size[i] = 0;
    for(int i = 0; i < nbl; i++) board_cols_size[i] = 0;
    nb_val_set = 0;
  }
  void set_max() {
    for(int i = 0; i < nbl; i++) {
      max_lines[i] = 0;
      for(int j = 0; j < nb_c_lines[i]; j++) {
	if(c_lines[i][j] > max_lines[i]) max_lines[i] = c_lines[i][j];
      }
    }
    for(int i = 0; i < nbc; i++) {
      max_cols[i] = 0;
      for(int j = 0; j < nb_c_cols[i]; j++) {
	if(c_cols[i][j] > max_cols[i]) max_cols[i] = c_cols[i][j];
      }
    }
  }
  void set_line_id(int _lid) {
    for(int i = 0; i < nbc; i++) board_lines_id[_lid][i] = -1;
    int curr_id = 0;
    int curr_val = board[_lid][0];
    board_lines_id[_lid][0] = curr_id;
    int best_cols_size = 0;
    int curr_cols_size = 0;
    if(board[_lid][0] == BLACK) {
      best_cols_size = 1;
      curr_cols_size = 1;
    }
    for(int i = 1; i < nbc; i++) {
      if(board[_lid][i] == curr_val) {
	board_lines_id[_lid][i] = curr_id;
	curr_cols_size ++;
      } else {
	if(board[_lid][i] == WHITE)
	  if(best_cols_size < curr_cols_size)
	    best_cols_size = curr_cols_size;
	curr_val = board[_lid][i];
	++curr_id;
	board_lines_id[_lid][i] = curr_id;
	curr_cols_size = 1;
      }
    }
    board_lines_size[_lid] = best_cols_size;
  }
  void set_col_id(int _cid) {
    for(int i = 0; i < nbl; i++) board_cols_id[i][_cid] = -1;
    int curr_id = 0;
    int curr_val = board[0][_cid];
    board_cols_id[0][_cid] = curr_id;
    int best_lines_size = 0;
    int curr_lines_size = 0;
    if(board[0][_cid] == BLACK) {
      best_lines_size = 1;
      curr_lines_size = 1;
    }
    for(int i = 1; i < nbl; i++) {
      if(board[i][_cid] == curr_val) {
	board_cols_id[i][_cid] = curr_id;
	curr_lines_size ++;
      } else {
	if(board[i][_cid] == WHITE)
	  if(best_lines_size < curr_lines_size)
	    best_lines_size = curr_lines_size;
	curr_val = board[i][_cid];
	++curr_id;
	board_cols_id[i][_cid] = curr_id;
	curr_lines_size = 1;
      }
    }
    board_cols_size[_cid] = best_lines_size;
  }
  void print_board() {
    printf("sum_c_lines: %d\n", sum_c_lines);
    printf("nb_val_set: %d\n", nb_val_set);
    for(int i = 0; i < nbl; i++) {
      for(int j = 0; j < nbc; j++) {
	printf("%c ", cboard[board[i][j]]);
      }
      printf("\n");
    }
  }
  void print_board_id_and_size(){
    printf("lines_id __ cols_id : \n");
    for(int i = 0; i < nbl; i++) {
      for(int j = 0; j < nbc; j++) {
	if(board_lines_id[i][j]!=-1) printf(" ");
	printf("%d ", board_lines_id[i][j]);
      }
      printf("__   ");
      for(int j = 0; j < nbc; j++) {
	if(board_cols_id[i][j]!=-1) printf(" ");
	printf("%d ", board_cols_id[i][j]);
      }
      printf("\n");
    }
    printf("lines_size : ");
    for(int i = 0; i < nbc; i++) printf("%d ", board_lines_size[i]);
    printf("\ncols_size : ");
    for(int i = 0; i < nbl; i++) printf("%d ", board_cols_size[i]);
    printf("\n");
  }
  void print_constraints() {
    printf("lines %d : ", nbl);
    for(int i = 0; i < nbl; i++) {
      printf("(");
      for(int j = 0; j < nb_c_lines[i]; j++) {
	printf("%d ", c_lines[i][j]);
      }
      printf(") ");
    }
    printf("\ncols %d : ", nbc);
    for(int i = 0; i < nbc; i++) {
      printf("(");
      for(int j = 0; j < nb_c_cols[i]; j++) {
	printf("%d ", c_cols[i][j]);
      }
      printf(") ");
    }
    printf("\n");
    printf("max_lines: ");
    for(int i = 0; i < nbl; i++)
      printf("%d ", max_lines[i]);
    printf("\n");
    printf("max_cols: ");
    for(int i = 0; i < nbc; i++)
      printf("%d ", max_cols[i]);
    printf("\n");
  }
  void load(char* _file) {
    FILE* fp;
    if ((fp = fopen(_file,"r")) == 0) {
      fprintf(stderr, "fopen %s error\n", _file); return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t linesize = getline(&line, &len, fp);
    linesize=linesize; // to avoid a warning
    int new_nbl; int new_nbc;
    int max_cl; int max_cc;
    sscanf(line, "%d %d %d %d", &new_nbl, &new_nbc, &max_cl, &max_cc);
    init(new_nbl, new_nbc);
    nbl = 0; nbc = 0;
    if(max_cl > 3 || max_cc > 3) {
      printf("ERROR : max_cl > 3 || max_cc > 3\n"); exit(0);
    }
    int mode = 0; /* 0:nil 1:lines 2:cols */
    while ((read = getline(&line, &len, fp)) != -1) {
      if(strncmp(line, "lines", 5) == 0) { mode = 1; }
      else if(strncmp(line, "cols", 4) == 0) { mode = 2; }
      else {
	int vals[3];
	int nbvals = 0;
	if(sscanf(line, "%d %d %d", &vals[0], &vals[1], &vals[2]) == 3) nbvals=3;
	else if(sscanf(line, "%d %d", &vals[0], &vals[1]) == 2) nbvals=2;
	else if(sscanf(line, "%d", &vals[0]) == 1) nbvals=1;
	if(mode == 1) {
	  nb_c_lines[nbl] = nbvals;
	  for(int i = 0; i < nbvals; i++) {
	    c_lines[nbl][i] = vals[i];
	    sum_c_lines += vals[i];
	  }
	  nbl++;
	}
	if(mode == 2) {
	  nb_c_cols[nbc] = nbvals;
	  for(int i = 0; i < nbvals; i++) c_cols[nbc][i] = vals[i];
	  nbc++;
	}
      }
    }
    free(line);
    fclose(fp);
    if(nbl != new_nbl || nbc != new_nbc) {
      fprintf(stderr, "ERROR  : nbl %d nbc %d ... new_nbl %d new_nbc %d\n", nbl, nbc, new_nbl, new_nbc);
      exit(0);
    }
    set_max();
  }
  nng_move_t get_rand_move() {
    nng_move_t ret;
    int r = ((int)rand())%((nbl*nbc)-nb_val_set);
    for(int i = 0; i < nbl; i++) {
      for(int j = 0; j < nbc; j++) {
	if(r == 0 && board[i][j] == WHITE) {
	  ret.line = i; ret.col = j; return ret;
	}
	if(board[i][j] == WHITE) r--;
      }
    }
    printf("ERROR : NO RAND MOVE\n"); exit(0);
    return ret;
  }
  void play(nng_move_t _m) {
    board[_m.line][_m.col] = BLACK;
    set_line_id(_m.line);
    set_col_id(_m.col);
    nb_val_set++;
  }
  // GGP-like endgame
  // if the longest group is longer than the longest constraint
  // (in a row or column)
  bool terminal() {
    /* for(int i = 0; i < nbl; i++)  */
    /*   if(terminal_line(i)) return true; */
    /* for(int j = 0; j < nbc; j++)  */
    /*   if(terminal_col(i)) return true;	 */
    return false;
  }
  /* bool terminal_line(int _lid) { */
  /*   static int gid[MAX_COLS]; */
  /*   static int nb_gid = 0; */
  /*   static int count_ON = 0; */
  /*   for(int i = 0; i < MAX_COLS; i++) gid[i] = -1; */
  /*   if(board[_lid].col[0] == BLACK) { gid[0] = 0; nb_gid = 1; count_ON = 1; }; */
  /*   for(int i = 1; i < MAX_COLS; i++) { */
  /*     if(count_ON == 1) { */
  /* 	if(board[_lid].col[i] == BLACK) { gid[i] = nb_gid-1; } */
  /* 	else { nb_gid++; count_ON = 0; } */
  /*     } else { */
  /* 	if(board[_lid].col[i] == BLACK) { gid[i] = nb_gid; nb_gid++; count_ON = 1; } */
  /*     } */
  /*   } */
  /*   prnitf("gid: "); */
  /*   for(int i = 0; i < MAX_COLS; i++)  */
  /*     printf("%d ", gid[i]); */
  /*   prnitf("\n"); */
  /* } */

  void playout() {
    for(int i  =0; i < 5; i++) {
      nng_move_t m = get_rand_move();
      play(m);
    }
  }
  bool terminal_col(int _cid) {
    return true;
  }
  
};
#endif /* MYNNG_H */