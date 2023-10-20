# Assembly-Code-Execution-Framework

1. Purpose

This project is intended to understand the instructions of a 
assembly language and how to simulate the execution of the resulting
machine code representation of a program.

2. Problem

In this project I will write a behavioral simulator for the machine code
created using the assembler code provided.  This simulator will read in a text file 
consisting of LC3100 machine code instructions (represented as decimal values),
and execute the program, then display the values of register files and memory
after each instruction is completed. Running any reasonable length program will
generate a large amount of output, but it will make debugging easier.

3. LC3100 Instruction-Set Architecture

The LC3100 is very simple, but it is general
enough to solve complex problems. 

The LC3100 is an 8-register, 32-bit computer.  All addresses are
word-addresses.  The LC3100 has 65536 words of memory.  By assembly-language
convention, register 0 will always contain 0 (i.e. the machine will not enforce
this, but no assembly-language program should ever change register 0 from its
initial value of 0).

There are 4 instruction formats (bit 0 is the least-significant bit).  Bits
31-25 are unused for all instructions, and should always be 0.

R-type instructions (add, nand):
    bits 24-22: opcode
    bits 21-19: reg A
    bits 18-16: reg B
    bits 15-3:  unused (should all be 0)
    bits 2-0:   destReg

I-type instructions (lw, sw, beq):
    bits 24-22: opcode
    bits 21-19: reg A
    bits 18-16: reg B
    bits 15-0:  offsetField (an 16-bit, 2's complement number with a range of
		    -32768 to 32767)

O-type instructions (halt, noop):
    bits 24-22: opcode
    bits 21-0:  unused (should all be 0)

-------------------------------------------------------------------------------
Table 1: Description of Machine Instructions
-------------------------------------------------------------------------------
Assembly language 	Opcode in binary		Action
name for instruction	(bits 24, 23, 22)
-------------------------------------------------------------------------------
add (R-type format)	000 			add contents of regA with
						contents of regB, store
						results in destReg.

nand (R-type format)	001			nand contents of regA with
						contents of regB, store
						results in destReg.

lw (I-type format)	010			load regB from memory. Memory
						address is formed by adding
						offsetField with the contents of
						regA.

sw (I-type format)	011			store regB into memory. Memory
						address is formed by adding
						offsetField with the contents of
						regA.

beq (I-type format)	100			if the contents of regA and
						regB are the same, then branch
						to the address PC+1+offsetField,
						where PC is the address of the
						beq instruction.

xxx (O-type format)	101			Unused for this assignment.

halt (O-type format)	110			increment the PC (as with all
						instructions), then halt the
						machine (let the simulator
						notice that the machine
						halted).

noop (O-type format)	111			do nothing.
-------------------------------------------------------------------------------

4. Assembly Code

After the optional label is white space.  Then follows the instruction field,
where the instruction can be any of the assembly-language instruction names
listed in the above table.

    R-type instructions (add, nand) instructions require 3 fields: field0
    is regA, field1 is regB, and field2 is destReg.

    I-type instructions (lw, sw, beq) require 3 fields: field0 is regA, field1
    is regB, and field2 is either a numeric value for offsetField or a symbolic
    address.  Numeric offsetFields can be positive or negative; symbolic
    addresses are discussed below.

    O-type instructions (noop and halt) require no fields.

Symbolic addresses refer to labels.  For lw or sw instructions, the assembler
should compute offsetField to be equal to the address of the label.  This could
be used with a zero base register to refer to the label, or could be used with
a non-zero base register to index into an array starting at the label.  For beq
instructions, the assembler should translate the label into the numeric
offsetField needed to branch to that label.

After the last used field comes more white space, then any comments.  The
comment field ends at the end of a line.  Comments are vital to creating
understandable assembly-language programs, because the instructions themselves
are rather cryptic.

In addition to LC3100 instructions, an assembly-language program may contain
directions for the assembler. The only assembler directive we will use is .fill
(note the leading period). .fill tells the assembler to put a number into the
place where the instruction would normally be stored. .fill instructions use
one field, which can be either a numeric value or a symbolic address.  For
example, ".fill 32" puts the value 32 where the instruction would normally be
stored.  .fill with a symbolic address will store the address of the label.
In the example below, ".fill start" will store the value 2, because the label
"start" is at address 2.

The assembler makes two passes over the assembly-language program. In the
first pass, it will calculate the address for every symbolic label.  Assume
that the first instruction is at address 0.  In the second pass, it will
generate a machine-language instruction (in decimal) for each line of assembly
language.  For example, here is an assembly-language program (that counts down
from 5, stopping when it hits 0).
