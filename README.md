# TBTTBSQSA (The Bigger Than Tiny But Still Quite Small Architecture)
### Notes
+ Any references to 'padding' refer to the act of filling in the bits to the left of a value.
+ Assembly code for this architecture is __not__ case sensitive.

## Spec
+ Von Neumann Architecture
+ 16 Bit Words
+ Word Addressable
+ 12 Bit Addresses, 4096 Words in Memory
+ 16 Instructions
+ 16 Registers
+ Single Word Instructions
+ 4 Bit Opcodes, 12 Bit Operands

## Assembling Programs
Programs can be assembled by running `Assembler` as follows:

```
Assembler <file_name> <output_file_name>
```

The assembler will output a binary file containing the assembled program, which can be given as input to the virtual machine.

## Using The Virtual Machine
The programs can be ran on the virtual machine (`TBTTBSQSA`) as follows:

```
TBTTBSQSA <mode> <loading_flags>
```

### Modes
There are three modes that the virtual machine can be ran in:

+ `dm` (Disassemble Mode): Runs the disassembler on the loaded program
+ `rm` (Run Mode): Runs the virtual machine on the loaded program
+ `drm` (Disassemble + Run Mode): Disassembles and runs the loaded program

### Loading Flags
Different flags can be used to specify how the virtual machine loads a program:
+ `-d` or `--default`: Load the default program
+ `-c` or `--console`: Load a program from stdin
+ `-f <filename>` or `--file <filename>`: Load a program from a specified file

### Disassembly
When ran in disassemble mode, the virtual machine will prompt the user to input an output filename. The disassembler will differentiate between instructions and data based which words in the program are executed as instructions. Programs are statically disassembled, but jumps to labels and jumps to `$ra` are tracked to determine what is data and what is an instruction.

The disassembler will be able to disassemble any valid program such that it can be reassembled and ran, but will not be able to distinguish between instructions and data in every case.

## Using The Architecture

### Loading A Program
The machine uses a bootloader to load programs, which is written into the end of memory automatically when the VM is initialised. The bootloader will read in programs up to 3/4 of the memory space in size, by loading them, a word at a time, from the input queue. Programs are loaded into memory location 1 (as 0 contains an instruction to jump to the bootloader). Once the program is loaded in, the stack and frame pointers are set to be equal to the first free memory location (i.e. not containing program data).

### Program Execution
Programs are executed starting at the first line, which is assumed to be an instruction. If data is entered as the first line of a program, that will be executed as an instruction. They will then run until a halt instruction is reached, printing data to the console and getting user input through `IN` and `OUT` when needed. Programs may also halt when a fault occurs. When a program halts it will do so with an exit code corresponding to what caused the halt (exit codes are listed below).

### The Stack
The `$fp` (frame pointer) and `$sp` (stack pointer) registers indicate the bounds of the current stack frame in memory. `$fp` should always point to the first memory location in the current stack frame, while `$sp` should always point to the memory location after the end of the stack frame (the top of the stack). When a program is initialised, `$fp` and `$sp` will both be equal to the first free memory location after the program. The stack must be managed by the programmer.

### Subroutines
The convention for this architecture is that saved registers (`$s1`-`$s5`), the stack and frame pointers (`$sp` and `$fp`), and the return address (`$ra`) are callee-saved, and the temporary registers (`$t1`-`$t4`) are caller-saved. All subroutines should begin by creating a stack frame and storing all the callee-saved register values on the stack frame. This frame can then be deconstructed upon completion of the subroutine, and the thread can then jump to `$ra`. This is done so by using the instruction `jump 0xfff`, which will jump to `$ra`.

## Operators
+ `0000 / HALT` Halt
+ `0001 / JUMP` Jump
+ `0010 / SKC` Skip on Condition
+ `0011 / LOAD` Load
+ `0100 / STORE` Store
+ `0101 / IN` Input
+ `0110 / OUT` Output
+ `0111 / MOVE` Move
+ `1000 / ADD` Add
+ `1001 / MUL` Multiply
+ `1010 / DIV` Divide
+ `1011 / AND` And
+ `1100 / OR` Or
+ `1101 / NOT` Not
+ `1110 / SHL` Shift Left
+ `1111 / SHR` Shift Right

## Registers
+ `0000 / $pc` Program Counter (12 bits)
+ `0001 / $ir` Instruction Register (16 bits)
+ `0010 / $ra` Return Address Register (12 bits)
+ `0011 / $sp` Stack Pointer (12 bits)
+ `0100 / $fp` Frame Pointer (12 bits)
+ `0101 / $t1` Temporary Register 1 (16 bits)
+ `0110 / $t2` Temporary Register 2 (16 bits)
+ `0111 / $t3` Temporary Register 3 (16 bits)
+ `1000 / $t4` Temporary Register 4 (16 bits)
+ `1001 / $s1` Saved Register 1 (16 bits)
+ `1010 / $s2` Saved Register 2 (16 bits)
+ `1011 / $s3` Saved Register 3 (16 bits)
+ `1100 / $s4` Saved Register 4 (16 bits)
+ `1101 / $s5` Saved Register 5 (16 bits)
+ `1110 / $pr` Pseudoinstruction Register (16 bits)
+ `1111 / $fr` Flag Register (5 bits)

## Flag Register
+ 0: Halt
+ 1: Overflow
+ 2: Input
+ 3-4: Exit codes

### Exit codes
+ 0: Ok
+ 1: Segfault
+ 2: Illegal register access

## Instructions

### `0000 / HALT` Halt
Halts the program.

#### _Example_
`halt` → `0x0000`

---
### `0001 / JUMP` Jump
Jumps to the given address/label, or $ra if the address is `0xfff`.

#### _Assembly_
**Operand:** The label to jump to. If the value `0xfff` is given instead of a label, then will jump to the value of `$ra`.

#### _Machine Code_
**Operand:** 12 bit address to jump to. If the address is given as `0xfff`, then will jump to the value of `$ra`.

#### _Examples_
`jump loop` → `0x10a1`

`jump 0xfff` → `0x1fff`

---
### `0010 / SKC` Skip on Condition
Skips the next instruction if the contents of the given register are greater than 0.

#### _Assembly_
**Operand:** A register to test conditionally If the register contains a value greater than 0, the next instruction will be skipped.

#### _Machine Code_
**Operand:** A 4 bit register address, padded with 0s.

#### _Examples_
`skc $t1` → `0x2005`

`skc $s4` → `0x200c`

---
### `0011 / LOAD` Load
Loads into the first register either the value of the memory address contained in the second register, or an immediate value.

#### _Assembly_
**Operands:** Two registers / One register and a signed 7 bit immediate value.

**Operand A:** A register to load the value into.

**Operand B:** A register containing the memory address to load from, or an immediate value that is representable with 7 bit two's complement.

#### _Machine Code_
**Operands:** One 4 bit value, one 1 bit value, one 7 bit value.

**Operand A:** 4 bit register address.

**Operand B:** A single bit indicating if the following operand should be interpreted as a 0-padded register address or a signed 7 bit immediate.

**Operand C:** 4 bit register address (of register containing memory address), padded with 0s. If operand B is 1, then the operand is treated as a 7 bit signed immediate value.

#### _Examples_
`load $s1 $t3` → `0x3907`

`load $t4 12` → `0x388c`

`load $s3 0xf` → `0x3b8f`

`load $s5 -4` → `0x3dfc`

---
### `0100 / STORE` Store
Loads the contents of the second register into the memory address given in the first register.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** A register containing the memory address to store the value of the second register in.

**Operand B:** A register containing the value to be stored in the address given in the first register.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address (of register containing memory address), padded with 0s.

**Operand B:** 4 bit register address containing value to be written to memory.

#### _Examples_
`store $t1 $t2` → `0x4056`

`store $fp $sp` → `0x4043`

---
### `0101 / IN` Input
Places the next word from the input queue into the given register. If there is nothing in the input queue, the machine will prompt the user to input space-separated input values. If the input queue becomes empty upon taking input, the input flag will be set to 0. Otherwise, it will be set to 1.

#### _Assembly_
**Operand:** The register to place the input value in.

#### _Machine Code_
**Operand:** One 4 bit register address to store the input value, padded with 0s.

#### _Examples_
`in $s1` → `0x5009`

`in $t3` → `0x5007`

---
### `0110 / OUT` Output
Places the contents of the given register into the output queue. If the second operand is 1, then the output queue will be flushed and printed to the console.

#### _Assembly_
**Operands:** One register, one single bit.

**Operand A:** A register to place the value of into the output queue.

**Operand B:** 1 bit flag indicating whether or not to print, padded with 0s.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 1 bit flag indicating whether or not to print, padded with 0s.

**Operand B:** 4 bit register address to place the value of into the output queue.

#### _Examples_
`out $s1 0` → `0x6009`

`out $t3 1` → `0x6017`

---
### `0111 / MOVE` Move
Moves a value from the second register to the first.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register to move the value into.

**Operand B:** The register to move the value from.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address to move the value into, padded with 0s.

**Operand B:** 4 bit register address to move the value from.

#### _Examples_
`move $t2 $t1` → `0x7065`

`move $s1 $s4` → `0x709c`

---
### `1000 / ADD` Add
Adds the values of two registers, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. 

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the add to be applied to.

**Operand B:** The register containing the value to be added to the value of the first register.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the add to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value to be added to the value of the first register.

#### _Examples_
`add $t1 $t1` → `0x8055`

`add $sp $s1` → `0x8039`

---
### `1001 / MUL` Multiply
Multiplies the values of two registers, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. 

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the multiply to be applied to.

**Operand B:** The register containing the value that the value of the first register will be multiplied by.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the multiply to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be multiplied by.

#### _Examples_
`mul $t4 $t1` → `0x9085`

`mul $s2 $s2` → `0x90aa`

---
### `1010 / DIV` Divide
Divides the value of the first register by the value of the second register, rounding down, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. If a divide by zero occurs, the virtual machine will exit with an error.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the divide to be applied to.

**Operand B:** The register containing the value that the value of the first register will be divided by.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the divide to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be divided by.

#### _Examples_
`div $t3 $t2` → `0xa076`

`div $s5 $s2` → `0xa0da`

---
### `1011 / AND` And
Ands each bit of two registers, storing the result in the first register.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the and to be applied to.

**Operand B:** The register containing the value that will be anded with the value of the first register.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the and to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that will be anded with the value of the first register.

#### _Examples_
`and $t1 $s3` → `0xb05b`

`and $s1 $s2` → `0xb09a`

---
### `1100 / OR` Or
Ors each bit of two registers, storing the result in the first register.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the or to be applied to.

**Operand B:** The register containing the value that will be or'd with the value of the first register.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the or to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that will be or'd with the value of the first register.

#### _Examples_
`or $s1 $s2` → `0xc09a`

`or $s4 $t1` → `0xc0c5`

---
### `1101 / NOT` Not
Inverts each bit of a given register.

#### _Assembly_
**Operand:** The register for the not to be applied to.

#### _Machine Code_
**Operand:** 4 bit register address for the not to be applied to, padded with 0s.

#### _Examples_
`not $t4` → `0xd008`

`not $s3` → `0xd00b`

---
### `1110 / SHL` Shift Left
Shifts the first register left by a number of bits equal to the value in the second register, storing the result in the first register.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the shift to be applied to.

**Operand B:** The register containing the value that the value of the first register will be shifted by.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the shift to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be shifted by.

#### _Examples_
`shl $t1 $s4` → `0xe05c`

`shl $s2 $t2` → `0xe0a6`

---
### `1111 / SHR` Shift Right
Shifts the first register right by a number of bits equal to the value in the second register, storing the result in the first register.

#### _Assembly_
**Operands:** Two registers.

**Operand A:** The register for the shift to be applied to.

**Operand B:** The register containing the value that the value of the first register will be shifted by.

#### _Machine Code_
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the shift to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be shifted by.

#### _Examples_
`shr $s1 $t1` → `0xe095`

`shr $s5 $t4` → `0xe0d8`

---
## Labels
Labels are used by referencing them in `jump` or `load` instructions, and placing them somewhere in the program with a colon.

The label `loop` may be used by placing `loop:` on its own line somewhere in the program, and using `jump loop` or `load $[some register] loop`.

A jump to a label will cause the instruction after the label to execute next. Loading a label will place the equivalent memory address in the register.

### Jump Example
```asm
load $t0 10
load $t1 -1

loop:
out $t0 0
add $t0 $t1
skc $t0
jump loop

out $t0 1
halt
```

This will produce an output as follows:

`10 9 8 7 6 5 4 3 2 1`

### Load Example
```asm
load $t1 dataSegment
load $t2 $t1
out $t2 1
halt

dataSegment:
100
```

This will produce an output as follows:

`100`

---
## Comments
Comments can be placed at the end of any line with a `#` symbol.

### Example
```asm
add $t1 $t2 # This is a comment!
halt
# This is also a comment!
```

---
## Full Progam Example
Here is a sample program that takes a user inputted number and displays it in binary:

```asm
in $s1

move $s2 $s1
load $t1 1
load $s3 0

#count bits in number
print_binary_count_bits_loop:
shr $s2 $t1
add $s3 $t1
skc $s2
jump print_binary_done_bits
jump print_binary_count_bits_loop

print_binary_done_bits:

jump reverse_number

move $s1 $t1
load $t4 1
load $t3 -1

#print binary
print_binary_loop:
add $s3 $t3
skc $s3
jump print_binary_done
move $t2 $t1
and $t2 $t4
out $t2 0
shr $t1 $t4
jump print_binary_loop

print_binary_done:
out $t1 1

halt

reverse_number:

#create stack frame
load $t1 1
store $sp $fp
move $fp $sp

add $sp $t1
store $sp $ra
add $sp $t1
store $sp $s1
add $sp $t1
store $sp $s2
add $sp $t1
store $sp $s3
add $sp $t1
store $sp $s4
add $sp $t1
store $sp $s5
add $sp $t1

#subroutine body
load $s5 0
load $s4 -15
load $t1 0
load $t3 1

#main loop
reverse_number_loop:
shl $t1 $t3
move $t2 $s1
and $t2 $t3
or $t1 $t2
shr $s1 $t3
add $s4 $t3
skc $s1
jump reverse_number_done
jump reverse_number_loop
reverse_number_done:

#deconstruct stack frame
load $t4 -1
add $sp $t4
load $s5 $sp
add $sp $t4
load $s4 $sp
add $sp $t4
load $s3 $sp
add $sp $t4
load $s2 $sp
add $sp $t4
load $s1 $sp
add $sp $t4
load $ra $sp
add $sp $t4
load $fp $sp

#jump with $ra
jump 0xfff
```