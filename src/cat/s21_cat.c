#include "s21_cat.h"

int main(int argc, char *argv[]) {
  struct Flags myflags = {0, 0, 0, 0, 0, 0};

  read_options(argc, argv, &myflags);
  print_file_with_flags(argc, argv, myflags);

  return 0;
}

void read_options(int argc, char *argv[], struct Flags *myflags) {
  for (int i = 1; i < argc; i++) {
    int continue_flag = 0;
    if (argv[i][0] != '-') {
      continue_flag = 1;
    }

    if (argv[i][1] == '-' && !continue_flag) {
      if (strcmp(argv[i], "--number") == 0) {
        myflags->number = 1;
      } else if (strcmp(argv[i], "--number-nonblank") == 0) {
        myflags->number_nonblank = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        myflags->squeeze_blank = 1;
      }
      continue_flag = 1;
    }

    if (!continue_flag) parse_short_options(argv[i], myflags);
  }
}

void parse_short_options(const char *option, struct Flags *myflags) {
  if (strchr(option, 'n')) {
    myflags->number = 1;
  }
  if (strchr(option, 'b')) {
    myflags->number_nonblank = 1;
  }
  if (strchr(option, 'e')) {
    myflags->show_ends = 1;
    myflags->show_all = 1;
  }
  if (strchr(option, 'E')) {
    myflags->show_ends = 1;
  }
  if (strchr(option, 't') || strchr(option, 'T')) {
    myflags->show_tabs = 1;
  }
  if (strchr(option, 's')) {
    myflags->squeeze_blank = 1;
  }
}

void print_file_with_flags(int argc, char *argv[], struct Flags myflags) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        fprintf(stderr, "Cannot open this file: %s\n", argv[i]);
      } else {
        process_file(file, myflags);
        fclose(file);
      }
    }
  }
}

void process_file(FILE *file, struct Flags myflags) {
  char ch;
  int previous_was_blank = 0;
  int empty_line_count = 0;
  int start_of_line = 1;
  int line_number = 1;

  while ((ch = fgetc(file)) != EOF) {
    if (myflags.squeeze_blank) {
      if (blank_lines(ch, &empty_line_count, &previous_was_blank)) {
        continue;
      }
    }

    if (start_of_line) {
      line_numbers(ch, myflags, &start_of_line, &line_number);
    }
    char_output(ch, myflags, &start_of_line);
  }
}

int blank_lines(char ch, int *empty_line_count, int *previous_was_blank) {
  int skip_line = 0;
  if (ch == '\n') {
    (*empty_line_count)++;
  } else {
    *empty_line_count = 0;
  }

  if (*previous_was_blank && *empty_line_count > 2) {
    skip_line = 1;
  } else
    *previous_was_blank = (ch == '\n');

  return skip_line;
}

void line_numbers(char ch, struct Flags myflags, int *start_of_line,
                  int *line_number) {
  if (myflags.number && !myflags.number_nonblank) {
    printf("%6d\t", (*line_number)++);
  } else if (myflags.number_nonblank && ch != '\n') {
    printf("%6d\t", (*line_number)++);
  }
  *start_of_line = 0;
}

void char_output(char ch, struct Flags myflags, int *start_of_line) {
  if (ch == '\t' && myflags.show_tabs) {
    printf("^I");
  } else if (ch == '\n') {
    if (myflags.show_ends) {
      putchar('$');
    }
    putchar('\n');
    *start_of_line = 1;
  } else if (myflags.show_all && !isprint(ch) && ch != '\t' && ch != '\n') {
    printf("^%c", ch + 64);
  } else {
    putchar(ch);
  }
}