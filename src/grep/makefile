CC=gcc
CFLAGS= -Wall -Werror -Wextra
LDFLAGS=
SOURCES=
EXECUTABLE1= s21_grep
EXECUTABLEALL= $(EXECUTABLE1)

all: s21_grep

s21_grep: s21_grep.c s21_grep_read_file.c s21_grep_file.c
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE1)

test: s21_grep.c s21_grep_read_file.c s21_grep_file.c
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE1)
	sh s21_grep_test.sh

clean:
	-rm *.o $(EXECUTABLEALL)

