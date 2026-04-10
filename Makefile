CPP = gcc
FLAGS = -Wall -Wextra -O2
# SRCPP = $(wildcard *.c)

main: main.c dispatcher.o command.o help.o exit.o
	${CPP} ${FLAGS} -o main $^

dispatcher.o: dispatcher.c
	${CPP} ${FLAGS} -c $^

command.o: command.c
	${CPP} ${FLAGS} -c $^

help.o: help.c
	${CPP} ${FLAGS} -c $^

exit.o: exit.c
	${CPP} ${FLAGS} -c $^

