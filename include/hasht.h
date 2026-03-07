#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASH_TABLE
#define HASH_TABLE

#define size 10

typedef struct Node {
  int key;
  WINDOW *win;
  struct Node *next;
} Node;

extern Node *windows[size];

WINDOW *search(int key);

int hash(int key);

int insert(int key, WINDOW *win);

int del_key(int key);

#endif
