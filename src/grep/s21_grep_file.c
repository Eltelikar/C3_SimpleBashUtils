#include "s21_grep_file.h"

int grep_file(FILE *file, const char *filename, struct Flags myflags) {
  char line[1024];
  int line_number = 0;
  int match_count = 0;
  int printed_filename = 0;

  regex_t regexes[1024];
  int error = 0;

  for (int i = 0; i < myflags.pattern_count && !error; i++) {
    if (!compile_regex(&regexes[i], myflags.patterns[i], myflags.ignore_case)) {
      fprintf(stderr, "Failed to compile regex for pattern: %s\n",
              myflags.patterns[i]);
      error = 1;
    }
  }

  while (fgets(line, sizeof(line), file) && !error) {
    line_number++;
    int match = 0;
    int exclude_line = 0;

    if (myflags.count) {
      myflags.only_matching = 0;
    }

    if (match_patterns(line, regexes, myflags, &match, &exclude_line)) {
      error = 1;
    }

    if (!myflags.only_matching && match && !error) {
      match_count++;
      if (myflags.list_files && !printed_filename) {
        printf("%s\n", filename);
        printed_filename = 1;
      }

      if (!myflags.count && !myflags.list_files) {
        print_line(line, filename, myflags, line_number);
      }
    }

    if (myflags.only_matching && !myflags.count && !error) {
      only_matching(line, regexes, myflags.pattern_count);
    }
  }

  if (myflags.count && !myflags.list_files && !error) {
    printf("%d\n", match_count);
  }

  free_regexes(regexes, myflags.pattern_count);
  return error;
}

int match_patterns(const char *line, regex_t *regexes, struct Flags myflags,
                   int *match, int *exclude_line) {
  for (int i = 0; i < myflags.pattern_count; i++) {
    int result = regexec(&regexes[i], line, 0, NULL, 0);
    if (!myflags.invert_match && result == 0) {
      *match = 1;
      break;
    } else if (myflags.invert_match && result == 0) {
      *exclude_line = 1;
      break;
    }
  }

  if (myflags.invert_match && *exclude_line == 0) {
    *match = 1;
  }
  return 0;
}

void print_line(const char *line, const char *filename, struct Flags myflags,
                int line_number) {
  if (myflags.multifile && !myflags.without_cmp_files) {
    printf("%s:", filename);
  }
  if (myflags.line_number) {
    printf("%d:", line_number);
  }
  printf("%s", line);
}

void only_matching(const char *line, regex_t *regexes, int pattern_count) {
  for (int i = 0; i < pattern_count; i++) {
    regmatch_t match_pos;
    const char *current_line = line;

    while (regexec(&regexes[i], current_line, 1, &match_pos, 0) == 0) {
      printf("%.*s\n", (int)(match_pos.rm_eo - match_pos.rm_so),
             current_line + match_pos.rm_so);
      current_line += match_pos.rm_eo;
    }
  }
}

void free_regexes(regex_t *regexes, int pattern_count) {
  for (int i = 0; i < pattern_count; i++) {
    regfree(&regexes[i]);
  }
}

int compile_regex(regex_t *regex, const char *pattern, int ignore_case) {
  int flags = REG_EXTENDED;
  int result = 1;
  if (ignore_case) {
    flags |= REG_ICASE;
  }
  int reg = regcomp(regex, pattern, flags);
  if (reg != 0) {
    result = 0;
  }
  return result;
}