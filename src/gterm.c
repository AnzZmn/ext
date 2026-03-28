#include "../include/gterm.h"
#include "../include/files.h"
#include "../include/hasht.h"
#include <curses.h>
#include <fcntl.h>
#include <ncurses.h>
#include <panel.h>
#include <pty.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// id of the cuurent entry pointed
int curr_ent = 0;

// the size of the entries array
int arr_size = 0;

// x and y offsets of *win
int h_offset = 0;
int w_offset = 0;

// x,y offset of the entries
int x_offset_ent = 2;
int y_offset_ent = 1;

// stored the  height, width of standard screen (stdscr)
int std_height, std_width;

// defines the height and width of the *win
static int nw, nh;

// all the wgetch chars stored here
int chh;

int win_id = 0;

// initialize new terminal screen with ncurses
// gets size of stdscr assign it to the std props
void init_mainscr() {
  SCREEN *mainscr = newterm(NULL, stdout, stdin);
  if (!mainscr) {
    exit(1);
  }
}

// initializes *win
void new_win(int nh, int nw) {
  init_mainscr();
  getmaxyx(stdscr, std_height, std_width);
  nh = std_height - h_offset;
  nw = std_width - w_offset;
  WINDOW *win = newwin(nh, nw, 0, 0);
  box(win, ACS_VLINE, ACS_HLINE);
  noecho();
  keypad(win, TRUE);
  insert(win_id++, win);
};

void highlight_ent(char *str, WINDOW *win, bool dir, bool rm) {
  size_t charc = dir ? strlen(str) - 1 : strlen(str);
  chtype ch;
  int y, x;
  getyx(win, y, x);
  for (int i = 0; i < charc; i++) {
    ch = mvwinch(win, y, x + i);
    if (rm) {

      mvwaddch(win, y, x + i, ch & ~A_REVERSE);
    } else {

      mvwaddch(win, y, x + i, ch | A_REVERSE);
    }
  }
}

void color_ent() {}

// displays entries on the *win
void display(entry entries[], WINDOW *win) {

  if (!win) {
    printf("No active window");
    exit(1);
  }

  for (int y = 0; y < arr_size; y++) {

    if (y <= arr_size) {

      char *ent_buff = malloc(sizeof(char));
      strcpy(ent_buff, entries[y].name);
      if (entries[y].is_dir) {
        strcat(ent_buff, "/");
        entries[y].name = ent_buff;
      }

      mvwprintw(win, y + y_offset_ent, x_offset_ent, "%s", ent_buff);
      wrefresh(win);
    }
  }

  wmove(win, y_offset_ent, x_offset_ent);
  highlight_ent(entries[curr_ent].name, win, entries[curr_ent].is_dir, false);
  wmove(win, y_offset_ent, x_offset_ent);

  mvwprintw(win, y_offset_ent, x_offset_ent - 1, ">");

  wrefresh(win);

  while ((chh = wgetch(win)) != 'q') {
    get_strokes(chh, win, entries);
  }

  endwin();
}

// reads the strokes from wgetch and perform the up and down navigation between
// the entries

void get_strokes(int chh, WINDOW *win, entry entries[]) {

  ptr_pos *prev = malloc(sizeof(&prev));

  int y, x;

  getyx(win, prev->y, prev->x);
  x = prev->x;

  switch (chh) {
  case KEY_UP:

    if (prev->y == y_offset_ent) {
      break;
    }

    if (prev) {
      highlight_ent(entries[curr_ent].name, win, entries[curr_ent].is_dir,
                    true);
      mvwprintw(win, prev->y, prev->x - 1, " ");
      wrefresh(win);
    }
    mvwprintw(win, prev->y - 1, prev->x - 1, ">");
    wmove(win, prev->y - 1, x);
    curr_ent--;
    highlight_ent(entries[curr_ent].name, win, entries[curr_ent].is_dir, false);
    wmove(win, prev->y - 1, prev->x);

    prev->y = prev->y - 1;

    break;
  case KEY_DOWN:
    if (prev->y == arr_size) {

      break;
    }
    if (prev) {

      highlight_ent(entries[curr_ent].name, win, entries[curr_ent].is_dir,
                    true);
      mvwprintw(win, prev->y, prev->x - 1, " ");
      wrefresh(win);
    }

    mvwprintw(win, prev->y + 1, prev->x - 1, ">");
    wmove(win, prev->y + 1, x);
    curr_ent++;
    highlight_ent(entries[curr_ent].name, win, entries[curr_ent].is_dir, false);
    wmove(win, prev->y + 1, prev->x);

    prev->y = prev->y - 1;
    break;
  }
}
