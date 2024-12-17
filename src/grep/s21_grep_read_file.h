#ifndef GREP_READ
#define GREP_READ

#include "s21_grep.h"

int read_options(int argc, char *argv[], struct Flags *myflags);
int handle_pattern_flag(char *argv[], int argc, int *i, int *j,
                        struct Flags *myflags, int *dont_read_flag);
int handle_file_flag(char *argv[], int argc, int *i, int *j,
                     struct Flags *myflags, int *dont_read_flag);
void set_default_pattern(int argc, char *argv[], struct Flags *myflags);
int read_patterns_by_file(const char *patternfile, struct Flags *myflags);

#endif