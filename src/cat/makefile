CC=gcc
CFLAGS= -Wall -Werror -Wextra
LDFLAGS=
SOURCES=
EXECUTABLE1= s21_cat
EXECUTABLE2= s21_cat_t
EXECUTABLEALL= $(EXECUTABLE1)

all: s21_cat

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE1)

test: s21_cat.c
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE1)
	sh s21_cat_test.sh

clean:
	-rm *.o $(EXECUTABLEALL)
