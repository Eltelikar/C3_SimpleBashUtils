#ifndef GREP_FILE
#define GREP_FILE

#include "s21_grep.h"

int grep_file(FILE *file, const char *filename, struct Flags myflags);
int compile_regex(regex_t *regex, const char *pattern, int ignore_case);
void free_regexes(regex_t *regexes, int pattern_count);
void only_matching(const char *line, regex_t *regexes, int pattern_count);
void print_line(const char *line, const char *filename, struct Flags myflags,
                int line_number);
int match_patterns(const char *line, regex_t *regexes, struct Flags myflags,
                   int *match, int *exclude_line);

#endif