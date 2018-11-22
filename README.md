# ACE3
### Note
Any references to 'padding' refer to the act of filling in the bits to the left of a value.

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

#### __Example__
`halt` -> `0x0000`

---
### `0001 / JUMP` Jump
Jumps to the given address/label, or $ra if the address is 0xfff.

#### __Assembly__
**Operand:** The label to jump to. If the value 0xfff is given instead of a label, then will jump to the value of $ra.

#### __Machine Code__
**Operand:** 12 bit address to jump to. If the address is given as 0xfff, then will jump to the value of $ra.

#### __Examples__
`jump loop` -> `0x10a1`

`jump 0xfff` -> `0x1fff`

---
### `0010 / SKC` Skip on Condition
Skips the next instruction if the contents of the given register are greater than 0.

#### __Assembly__
**Operand:** A register to test conditionally If the register contains a value greater than 0, the next instruction will be skipped.

#### __Machine Code__
**Operand:** A 4 bit register address, padded with 0s.

#### __Examples__
`skc $t1` -> `0x2005`

`skc $s4` -> `0x200c`

---
### `0011 / LOAD` Load
Loads into the first register either the value of the memory address contained in the second register, or an immediate value.

#### __Assembly__
**Operands:** Two registers / One register and a signed 7 bit immediate value.

**Operand A:** A register to load the value into.

**Operand B:** A register containing the memory address to load from, or an immediate value that is representable with 7 bit two's complement.

#### __Machine Code__
**Operands:** One 4 bit value, one 1 bit value, one 7 bit value.

**Operand A:** 4 bit register address.

**Operand B:** A single bit indicating if the following operand should be interpreted as a 0-padded register address or a signed 7 bit immediate.

**Operand C:** 4 bit register address (of register containing memory address), padded with 0s. If operand B is 1, then the operand is treated as a 7 bit signed immediate value.

#### __Examples__
`load $s1 $t3` -> `0x3907`

`load $t4 12` -> `0x388c`

`load $s3 0xf` -> `0x3b8f`

`load $s5 -4` -> `0x3dfc`

---
### `0100 / STORE` Store
Loads the contents of the second register into the memory address given in the first register.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** A register containing the memory address to store the value of the second register in.

**Operand B:** A register containing the value to be stored in the address given in the first register.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address (of register containing memory address), padded with 0s.

**Operand B:** 4 bit register address containing value to be written to memory.

#### __Examples__
`store $t1 $t2` -> `0x4056`

`store $fp $sp` -> `0x4043`

---
### `0101 / IN` Input
Places the next word from the input queue into the given register. If there is nothing in the input queue, the machine will prompt the user to input space-separated input values.

#### __Assembly__
**Operand:** The register to place the input value in.

#### __Machine Code__
**Operand:** One 4 bit register address to store the input value, padded with 0s.

#### __Examples__
`in $s1` -> `0x5009`

`in $t3` -> `0x5007`

---
### `0110 / OUT` Output
Places the contents of the given register into the output queue. If the second operand is 1, then the output queue will be flushed and printed to the console.

#### __Assembly__
**Operands:** One register, one single bit.

**Operand A:** A register to place the value of into the output queue.

**Operand B:** 1 bit flag indicating whether or not to print, padded with 0s.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 1 bit flag indicating whether or not to print, padded with 0s.

**Operand B:** 4 bit register address to place the value of into the output queue.

#### __Examples__
`out $s1 0` -> `0x6009`

`out $t3 1` -> `0x6017`

---
### `0111 / MOVE` Move
Moves a value from the second register to the first.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register to move the value into.

**Operand B:** The register to move the value from.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address to move the value into, padded with 0s.

**Operand B:** 4 bit register address to move the value from.

#### __Examples__
`move $t2 $t1` -> `0x7065`

`move $s1 $s4` -> `0x709c`

---
### `1000 / ADD` Add
Adds the values of two registers, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. 

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the add to be applied to.

**Operand B:** The register containing the value to be added to the value of the first register.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the add to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value to be added to the value of the first register.

#### __Examples__
`add $t1 $t1` -> `0x8055`

`add $sp $s1` -> `0x8039`

---
### `1001 / MUL` Multiply
Multiplies the values of two registers, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. 

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the multiply to be applied to.

**Operand B:** The register containing the value that the value of the first register will be multiplied by.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the multiply to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be multiplied by.

#### __Examples__
`mul $t4 $t1` -> `0x9085`

`mul $s2 $s2` -> `0x90aa`

---
### `1010 / DIV` Divide
Divides the value of the first register by the value of the second register, rounding down, storing the result in the first register. Sets the overflow flag to 1 if an overflow occurs, and 0 otherwise. If a divide by zero occurs, the virtual machine will exit with an error.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the divide to be applied to.

**Operand B:** The register containing the value that the value of the first register will be divided by.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the divide to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be divided by.

#### __Examples__
`div $t3 $t2` -> `0xa076`

`div $s5 $s2` -> `0xa0da`

---
### `1011 / AND` And
Ands each bit of two registers, storing the result in the first register.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the and to be applied to.

**Operand B:** The register containing the value that will be anded with the value of the first register.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the and to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that will be anded with the value of the first register.

#### __Examples__
`and $t1 $s3` -> `0xb05b`

`and $s1 $s2` -> `0xb09a`

---
### `1100 / OR` Or
Ors each bit of two registers, storing the result in the first register.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the or to be applied to.

**Operand B:** The register containing the value that will be or'd with the value of the first register.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the or to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that will be or'd with the value of the first register.

#### __Examples__
`or $s1 $s2` -> `0xc09a`

`or $s4 $t1` -> `0xc0c5`

---
### `1101 / NOT` Not
Inverts each bit of a given register.

#### __Assembly__
**Operand:** The register for the not to be applied to.

#### __Machine Code__
**Operand:** 4 bit register address for the not to be applied to, padded with 0s.

#### __Examples__
`not $t4` -> `0xd008`

`not $s3` -> `0xd00b`

---
### `1110 / SHL` Shift Left
Shifts the first register left by a number of bits equal to the value in the second register, storing the result in the first register.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the shift to be applied to.

**Operand B:** The register containing the value that the value of the first register will be shifted by.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the shift to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be shifted by.

#### __Examples__
`shl $t1 $s4` -> `0xe05c`

`shl $s2 $t2` -> `0xe0a6`

---
### `1111 / SHR` Shift Right
Shifts the first register right by a number of bits equal to the value in the second register, storing the result in the first register.

#### __Assembly__
**Operands:** Two registers.

**Operand A:** The register for the shift to be applied to.

**Operand B:** The register containing the value that the value of the first register will be shifted by.

#### __Machine Code__
**Operands:** One 8 bit value, one 4 bit value.

**Operand A:** 4 bit register address for the shift to be applied to, padded with 0s.

**Operand B:** 4 bit register address containing the value that the value of the first register will be shifted by.

#### __Examples__
`shr $s1 $t1` -> `0xe095`

`shr $s5 $t4` -> `0xe0d8`