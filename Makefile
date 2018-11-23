CC = gcc
CFLAGS = -Wall -pedantic -std=c99
PROGRAM_NAME = test.out

all: framework disasm
	@echo "Compiling Project"
	$(CC) *.c ./VirtualMachine/*.c $(CFLAGS) -o $(PROGRAM_NAME) *.o
	rm *.o

asm: framework
	@echo "Compiling Assembler"
	$(CC) ./Assembler/*.c $(CFLAGS) -o asm.out *.o
	rm *.o

disasm: framework
	@echo "Compiling Disassembler"
	$(CC) -c ./Disassembler/*.c $(CFLAGS)

framework:
	@echo "Compiling OOP Framework"
	$(CC) -c ./oop/*.c ./utilities/*.c $(CFLAGS)

clean:
	@echo "Cleanup..."
	rm *.o