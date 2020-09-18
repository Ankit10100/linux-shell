# Define required macros here
SHELL = /bin/bash

CC = g++
CFLAG =-g

.PHONY: clean all

all:
	${CC} ${CFLAG} my_shell.cpp -o shell -lreadline

clean:
	rm -f *.o *.o

