#ifndef MAIN_GREP
#define MAIN_GREP

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags {
  int ignore_case;
  int invert_match;
  int count;
  int list_files;
  int line_number;
  int without_cmp_files;
  int silence;
  int patterns_by_file;
  int only_matching;
  int multifile;
  int pattern_count;
  int pattern_flag;
  char *patterns[1024];
};

int skip_by_flag(char *arg);
void free_pattenrs(char *patterns[], int pattern_count);

#endif