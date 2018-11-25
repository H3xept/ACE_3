CC = gcc
CFLAGS = -Wall -pedantic -std=c99
PROGRAM_NAME = TBTTBSQSA
ASSEMBLER_NAME = Assembler

all: framework disasm
	@echo "Compiling Project"
	$(CC) *.c ./src/VirtualMachine/*.c $(CFLAGS) -o $(PROGRAM_NAME) *.o
	rm *.o

asm: framework
	@echo "Compiling Assembler"
	$(CC) ./src/Assembler/*.c $(CFLAGS) -o asm.out *.o
	rm *.o

disasm: framework
	@echo "Compiling Disassembler"
	$(CC) -c ./src/Disassembler/*.c $(CFLAGS)

framework:
	@echo "Compiling OOP Framework"
	$(CC) -c ./src/oop/*.c ./src/utilities/*.c $(CFLAGS)

clean:
	@echo "Cleanup..."
	rm *.o