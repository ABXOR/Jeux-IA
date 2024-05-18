all: board.o list.o list #nqueens

list.o: list.c list.h
	gcc -c list.c

board.o: board.c board.h
	gcc -c board.c

#nqueens: nqueens.c board.c list.c board.h list.h item.h
#	gcc -o nqueens nqueens.c board.c list.c

list: list.c list
	gcc -o list list.c
