# COP4610 Project1 Makefile
# Santora, Usher, Xing

CC = gcc

shell: main.o utility.o
	$(CC) main.o utility.o -o shell

main.o: main.c utility.c utility.h
	$(CC) -c main.c

clean:
	rm -rf *.o shell
