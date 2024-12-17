#include "s21_grep_read_file.h"

int read_options(int argc, char *argv[], struct Flags *myflags) {
  int error_flag = 0;

  for (int i = 1; i < argc && !error_flag; i++) {
    int dont_read_flag = 0;
    for (int j = 0; argv[i][j] != '\0' && !error_flag && argv[i][0] == '-' &&
                    !dont_read_flag;
         j++) {
      switch (argv[i][j]) {
        case 'i':
          myflags->ignore_case = 1;
          break;

        case 'v':
          myflags->invert_match = 1;
          break;

        case 'c':
          myflags->count = 1;
          break;

        case 'l':
          myflags->list_files = 1;
          break;

        case 'n':
          myflags->line_number = 1;
          break;

        case 'h':
          myflags->without_cmp_files = 1;
          break;

        case 's':
          myflags->silence = 1;
          break;

        case 'o':
          myflags->only_matching = 1;
          break;

        case 'e':
          if (handle_pattern_flag(argv, argc, &i, &j, myflags,
                                  &dont_read_flag)) {
            error_flag = 1;
          }
          break;

        case 'f':
          if (handle_file_flag(argv, argc, &i, &j, myflags, &dont_read_flag)) {
            error_flag = 1;
          }
          break;
      }
    }
  }

  if (!error_flag && myflags->pattern_flag == 0 &&
      myflags->patterns_by_file == 0) {
    set_default_pattern(argc, argv, myflags);
  }

  return error_flag;
}

int handle_pattern_flag(char *argv[], int argc, int *i, int *j,
                        struct Flags *myflags, int *dont_read_flag) {
  int error = 0;
  myflags->pattern_flag = 1;
  if (argv[*i][*j + 1] != '\0') {
    myflags->patterns[myflags->pattern_count++] = strdup(argv[*i] + *j + 1);
    *dont_read_flag = 1;
  } else if (*i + 1 < argc && argv[*i + 1][0] != '-') {
    myflags->patterns[myflags->pattern_count++] = strdup(argv[++(*i)]);
  } else
    error = 1;
  return error;
}

int handle_file_flag(char *argv[], int argc, int *i, int *j,
                     struct Flags *myflags, int *dont_read_flag) {
  int error = 0;
  myflags->patterns_by_file = 1;
  if (argv[*i][*j + 1] != '\0') {
    error = read_patterns_by_file(argv[*i] + *j, myflags);
    *dont_read_flag = 1;
  } else if (*i + 1 < argc && argv[*i + 1][0] != '-' && !error) {
    error = read_patterns_by_file(argv[++(*i)], myflags);
  } else
    error = 1;

  return error;
}

void set_default_pattern(int argc, char *argv[], struct Flags *myflags) {
  int break_flag = 0;
  for (int i = 1; i < argc && !break_flag; i++) {
    if (strchr(argv[i], '-') == NULL) {
      myflags->patterns[0] = strdup(argv[i]);
      myflags->pattern_count++;
      break_flag = 1;
    }
  }
}

int read_patterns_by_file(const char *patternfile, struct Flags *myflags) {
  int error = 0;
  FILE *file = fopen(patternfile, "r");
  if (file == NULL) {
    if (!myflags->silence) {
      fprintf(stderr, "Cannot open this file to read patterns: %s\n",
              patternfile);
    }
    error = 1;
  }

  if (!error) {
    char pattern[256];
    while (fgets(pattern, sizeof(pattern), file) && !error) {
      pattern[strcspn(pattern, "\n")] = '\0';
      if (myflags->pattern_count < 1024) {
        myflags->patterns[myflags->pattern_count++] = strdup(pattern);
      } else {
        if (!myflags->silence) {
          fprintf(stderr, "Too many patterns specified. Max allowed is %d.\n",
                  1024);
        }
        error = 1;
      }
    }
    fclose(file);
  }
  return error;
}
