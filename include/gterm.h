#include <ncurses.h>
#include <panel.h>
#include <stdbool.h>

#ifndef G_TERM_H
#define G_TERM_H

typedef struct {
  int id;
  char *name;
  bool is_dir;
} entry;

typedef struct {
  int x;
  int y;
} ptr_pos;

extern int win_id;

extern int arr_size;

extern int std_height;
extern int std_width;

extern int h_offset;
extern int w_offset;

extern int x_offset_ent;
extern int y_offset_ent;

void display(entry entries[], WINDOW *win);

void new_win(int nh, int nw);

void init_mainscr();

void get_strokes(int chh, WINDOW *win, entry entries[]);

#endif
