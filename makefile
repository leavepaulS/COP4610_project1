# COP4610 Project1 Makefile
# Santora, Usher, Xing

CC = gcc

shell: main.o
	$(CC) main.o -o shell

main.o: main.c utility.c 
	$(CC) -c main.c

clean:
	rm -rf *.o shell
