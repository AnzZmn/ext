#include "../include/hasht.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <uthash.h>

// contains the list of active terminal windows struct
Node *windows[size] = {};

// produces hash of the given key
int hash(int key) { return key % size; }

// inserts the given WINDOW in *windows and maps the hash of the key to the
// WINDOW
int insert(int key, WINDOW *win) {
  int index = hash(key);

  Node *newNode = malloc(sizeof(Node));
  if (!newNode) {
    printf("Memory Allocation Failed!");
    return 1;
  }

  newNode->key = key;
  newNode->win = win;
  newNode->next = windows[index];

  windows[index] = newNode;
  return 0;
}

WINDOW *search(int key) {

  int index = hash(key);
  Node *temp = windows[index];
  while (temp) {
    if (temp->key == key) {
      return temp->win;
    }
    temp = temp->next;
  }
  return NULL;
}

int del_key(int key) {
  int index = hash(key);
  Node *temp = windows[index];
  Node *prev = NULL;
  while (temp) {
    if (temp->key == key) {
      if (prev) {
        prev->next = temp->next;
        return 0;
      } else {
        windows[index] = temp->next;
        return 0;
      }
      prev = temp;
      temp = temp->next;
    }
  }
  return 1;
}
