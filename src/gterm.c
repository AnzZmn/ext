#include "../include/gterm.h"
#include "../include/hasht.h"
#include <curses.h>
#include <fcntl.h>
#include <ncurses.h>
#include <panel.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

// displays entries on the *win
void display(entry entries[], WINDOW *win) {

  if (!win) {
    printf("No active window");
    exit(1);
  }
  for (int y = 0; y < arr_size; y++) {

    if (y <= arr_size) {

      mvwprintw(win, y + y_offset_ent, x_offset_ent, "%s", entries[y].name);
      wrefresh(win);
    }
  }

  wmove(win, y_offset_ent, x_offset_ent);
  wrefresh(win);

  while ((chh = wgetch(win)) != 'q') {
    get_strokes(chh, win);
  }

  endwin();
}

// reads the strokes from wgetch and perform the up and down navigation between
// the entries
void get_strokes(int chh, WINDOW *win) {

  static int y, x;
  getyx(win, y, x);
  switch (chh) {
  case KEY_UP:
    if (y == y_offset_ent) {
      break;
    }
    mvprintw(y - 1, x - 1, ">");
    wmove(win, y - 1, x);
    break;
  case KEY_DOWN:
    if (y == arr_size) {
      break;
    }
    mvprintw(y + 1, x - 1, ">");
    wmove(win, y + 1, x);
    break;
  }
}
