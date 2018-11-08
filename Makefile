CC = gcc
CFLAGS = -Wall -pedantic -std=c99
PROGRAM_NAME = test.out

all: framework
	@echo "Compiling Project"
	$(CC) *.c $(CFLAGS) -o $(PROGRAM_NAME) *.o
	rm *.o

framework:
	@echo "Compiling OOP Framework"
	$(CC) -c ./oop/*.c ./utilities/*.c $(CFLAGS)

clean:
	rm *.o