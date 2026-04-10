CPP = gcc
FLAGS = -Wall -Wextra -O2
COMMAND = ${CPP} ${FLAGS}
# SRCPP = $(wildcard *.c)

main: main.c dispatcher.o command.o help.o exit.o
	${COMMAND} -o main $^

dispatcher.o: dispatcher.c
	${COMMAND} -c $^

command.o: command.c
	${COMMAND} -c $^

help.o: help.c
	${COMMAND} -c $^

exit.o: exit.c
	${COMMAND} -c $^

