#include "s21_grep.h"

#include "s21_grep_file.h"
#include "s21_grep_read_file.h"

int main(int argc, char *argv[]) {
  struct Flags myflags = {0};
  int not_file_idx = 0;
  int error = 0;

  error = read_options(argc, argv, &myflags);

  for (int i = 1; i < argc && !error; i++) {
    int not_file = 0;
    int continue_flag = 0;

    if ((!myflags.pattern_flag || !myflags.patterns_by_file) && i == 1) {
      not_file = 1;
    }
    if (skip_by_flag(argv[i])) {
      not_file_idx = i + 1;
    }
    if (myflags.pattern_flag == 0 && myflags.patterns_by_file == 0 &&
        strcmp(argv[i], myflags.patterns[0]) == 0) {
      not_file = 1;
    }

    if ((not_file == 0 && argv[i][0] != '-') && not_file_idx != i) {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        if (!myflags.silence) {
          fprintf(stderr, "Cannot open file: %s\n", argv[i]);
        }
        continue_flag = 1;
      }
      if (!continue_flag) {
        if (grep_file(file, argv[i], myflags)) error = 1;
        fclose(file);
      }
    }
  }

  free_pattenrs(myflags.patterns, myflags.pattern_count);

  return error;
}

int skip_by_flag(char *arg) {
  int skip_flag = 0;
  if (arg[0] == '-') {
    for (int i = 0; arg[i] != '\0'; i++) {
      if ((arg[i] == 'e' || arg[i] == 'f') && arg[i + 1] == '\0') {
        skip_flag = 1;
      }
    }
  }
  return skip_flag;
}

void free_pattenrs(char *patterns[], int pattern_count) {
  for (int i = 0; i < pattern_count; i++) {
    free(patterns[i]);
  }
}
