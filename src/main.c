#include "../include/gterm.h"
#include "../include/hasht.h"
#include <asm-generic/errno-base.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *IGN_DIR_1 = ".";
const char *IGN_DIR_2 = "..";
// stores all the file entries
entry *new_entry = NULL;

int main(int argc, char *argv[]) {
  // the current working dir path
  char cwd[4096];
  getcwd(cwd, 4096);

  // Stores all the dirent props such as d_name
  struct dirent *cDirent;

  // The DIR pointer of the cwd
  DIR *cDir;

  entry entries[20] = {};
  // When no args are given along with the program
  // opens the explorer with the cwd as working dirent
  if (argc == 1) {
    cDir = opendir(cwd);
    while ((cDirent = readdir(cDir)) != NULL) {

      static int id = 0;

      printf("[%s]\n", cDirent->d_name);

      new_entry = malloc((arr_size + 1) * sizeof(entry));

      new_entry->id = id;

      // ignoring IGN_DIR_1 & IGN_DIR_2

      if (strcmp(cDirent->d_name, IGN_DIR_1) != 0 &&
          strcmp(cDirent->d_name, IGN_DIR_2) != 0) {

        new_entry->name = cDirent->d_name;

        DIR *ent = opendir(new_entry->name);

        if (ent == NULL && errno == ENOTDIR) {

          new_entry->is_dir = false;

        } else {
          new_entry->is_dir = true;
        }

        entries[arr_size++] = *new_entry;

        id++;
      }
    }

    new_win(std_height - h_offset, std_width - w_offset);
    WINDOW *win = search(win_id - 1);
    display(entries, win);
    return 0;
  }

  cDir = opendir(argv[1]);

  if (cDir == NULL) {
    printf("Cannot open directory %s\n", argv[1]);
    return 1;
  }

  closedir(cDir);
  return 0;
}
