# ACE3
## Spec
16 Operators, 16 Registers

4 Bit Opcodes, 12 Bit Operands
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
+ `0000 / PC` Program Counter (12 bits)
+ `0001 / IR` Instruction Register (16 bits)
+ `0010 / MAR` Memory Address Register (12 bits)
+ `0011 / MBR` Memory Buffer Register (16 bits)
+ `0100 / RA` Return Address Register (12 bits)
+ `0101 / INREG` Input Register (16 bits)
+ `0110 / OUTREG` Output Register (16 bits)
+ `0111 / T1` Temporary Register 1 (16 bits)
+ `1001 / T2` Temporary Register 2 (16 bits)
+ `1010 / S1` Saved Register 1 (16 bits)
+ `1011 / S2` Saved Register 2 (16 bits)
+ `1100 / SP` Stack Pointer (12 bits)
+ `1101 / FP` Frame Pointer (12 bits)
+ `1110 / FR` Flag Register (5 bits)
+ `1111 / PR` Pseudoinstruction Register (16 bits)

## Flag Register
+ 0: Halt
+ 1: Overflow
+ 2: Input
+ 3-4: Exit codes

### Exit codes
+ 0: Ok
+ 1: Segfault

## Instructions

### `0000 / HALT` Halt
Halts the program.

---
### `0001 / JUMP` Jump
Jumps to the given address.

**Operand:** 12 bit address.

---
### `0010 / SKC` Skip on Condition
Skips the next instruction if the contents of the given register are greater than 0.

**Operand:** One 4 bit register address, padded with 0s.

---
### `0011 / LOAD` Load
Loads the contents of the given address into the given register. Can accept immediate values.

**Operands:** One 12 bit value, one 16 bit value (from the next word in memory).

**Operand A:** 4 bit register address, padded with 0s. May have a 1 in the first bit.

**Operand B:** 12 bit memory address, padded with 0s. If the first bit of operand A is 1, then treated as a 16 bit immediate value.

---
### `0110 / OUT` Output
Places the contents of the output register into the output queue.

**Operand:** One bit indicating whether or not to print, padded with 0s.

---
### `0111 / MOVE` Move
Moves values from one register to another.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1000 / ADD` Add
Adds the values of two registers, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1001 / MUL` Multiply
Multiplies the values of two registers, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1010 / DIV` Divide
Divides the value of the first register by the value of the second register, rounding down, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1011 / AND` And
Ands each bit of two registers, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1100 / OR` Or
Ors each bit of two registers, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1101 / NOT` Not
Inverts each bit of a given register.

**Operand:** 4 bit register address, padded with 0s.

---
### `1110 / SHL` Shift Left
Shifts the first register left by a number of bits equal to the value in the second register, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.

---
### `1111 / SHR` Shift Right
Shifts the first register right by a number of bits equal to the value in the second register, storing the result in the first register.

**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address, padded with 0s.

**Operand B:** 4 bit register address.