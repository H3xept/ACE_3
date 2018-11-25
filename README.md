# TBTTBSQSA (The Bigger Than Tiny But Still Quite Small Architecture)
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
+ `0010 / RA` Return Address Register (12 bits)
+ `0011 / SP` Stack Pointer (12 bits)
+ `0100 / FP` Frame Pointer (12 bits)
+ `0101 / T1` Temporary Register 1 (16 bits)
+ `0110 / T2` Temporary Register 2 (16 bits)
+ `0111 / T3` Temporary Register 3 (16 bits)
+ `1000 / T4` Temporary Register 4 (16 bits)
+ `1001 / S1` Saved Register 1 (16 bits)
+ `1010 / S2` Saved Register 2 (16 bits)
+ `1011 / S3` Saved Register 3 (16 bits)
+ `1100 / S4` Saved Register 4 (16 bits)
+ `1101 / S5` Saved Register 5 (16 bits)
+ `1110 / PR` Pseudoinstruction Register (16 bits)
+ `1111 / FR` Flag Register (5 bits)

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
Loads the contents of the given memory address into the given register. Can accept immediate values.

**Operands:** One 12 bit value, one 16 bit value (from the next word in memory).

**Operand A:** 4 bit register address, padded with 0s. May have a 1 in the first bit.

**Operand B:** 4 bit register address (of register containing memory address), padded with 0s. If the padding of operand A contains 1, then treated as a 16 bit immediate value.

---
### `0100 / STORE` Store
Loads the contents of the second register into the address given in the first register.

**Operands:** One 4 bit value, one 4 bit value.

**Operand A:** 4 bit register address (of register containing memory address), padded with 0s.

**Operand B:** 4 bit register address.

---
### `0101 / IN` Input
Places the next word from the input queue into the given register.

**Operand:** One 4 bit register address, padded with 0s.

---
### `0110 / OUT` Output
Places the contents of the given register into the output queue.

**Operands:** One 1 bit value, one 4 bit value. indicating whether or not to print, padded with 0s.

**Operand A:** 1 bit flag indicating whether or not to print, padded with 0s.

**Operand B:** 4 bit register address.

---
### `0111 / MOVE` Move
Moves values from the second register to the first.

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