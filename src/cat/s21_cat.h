#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags {
  int number;
  int number_nonblank;
  int show_ends;
  int show_tabs;
  int show_all;
  int squeeze_blank;
};

void read_options(int argc, char *argv[], struct Flags *myflags);
void print_file_with_flags(int argc, char *argv[], struct Flags myflags);
void parse_short_options(const char *option, struct Flags *myflags);
void line_numbers(char ch, struct Flags myflags, int *start_of_line,
                  int *line_number);
int blank_lines(char ch, int *empty_line_count, int *previous_was_blank);
void char_output(char ch, struct Flags myflags, int *start_of_line);
void process_file(FILE *file, struct Flags myflags);