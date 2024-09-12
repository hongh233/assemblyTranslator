# Assembly Translator

A Binary Translator from X Instruction Set to x86-64 Assembly

## Project Overview
This project implements a binary translator called Xtra that converts binary code written in the simplified X instruction set to x86-64 assembly code. The X instruction set is based on a 16-bit architecture with 16 general-purpose registers and two special registers: the Program Counter (PC) and the Status register (F).

The translator reads binary X code, decodes the instructions, and outputs the equivalent x86-64 assembly code, allowing it to run on a modern x86-64 machine. The project is divided into two parts:

1. Simple Xtra: Translates basic X instructions.
2. Full Xtra: Extends the translator to handle more complex instructions like jumps, calls, and memory operations.

## Features
1. Binary to Assembly Translation: Converts the X instruction set to x86-64 assembly.
2. Support for Simple and Complex Instructions: Includes translation for arithmetic, logic, flow control, and memory operations.
3. Debug Mode: Supports std and cld instructions to enable or disable debug mode.
4. Program Stack: Manages the program stack using register r15 (X) mapped to %rsp (x86).
5. Register Mapping: Maps 16-bit X registers to corresponding 64-bit x86 registers.
6. Dynamic Assembly Generation: Outputs assembly code to be compiled and run on x86-64 systems.

---

## Usage
To build the project, ensure you have `gcc` installed and run the following command:
``` bash
make
```

This will generate the `xas` (assembler) and `xtra` (translator) executables.

### Running the Translator
You can run the translator using the `runit.sh` script, which automates the process of assembling, translating, and running X assembly files:
``` bash
./runit.sh <xas file>
```

For example, to assemble and translate `example.xas`, you can run:

```bash
./runit.sh example.xas
```

### Command-Line Options
The translator takes one argument:
```bash
./xtra <xo file>
```
Where `<xo file>` is the binary file generated by the X assembler (e.g., `example.xo`).

### Output
The translator outputs x86-64 assembly code to standard output. You can redirect the output to a file for further compilation:
```bash
./xtra example.xo > example.s
```

### Testing
A `runtest.sh` script is provided to test your implementation. To run all tests:
```bash
./runtest.sh
```
To run a specific test, provide the test number:
```bash
./runtest.sh 01
```

## X Instruction Set
The X instruction set consists of 16-bit instructions divided into four categories:
0-Operand Instructions: Operate without any operands (e.g., `ret`, `cld`, `std`).
1-Operand Instructions: Operate on a single register or immediate value (e.g., `push`, `pop`, `inc`, `neg`).
2-Operand Instructions: Operate on two registers (e.g., `add`, `sub`, `mul`, `mov`).
Extended Instructions: Use an additional word to store an immediate or memory address (e.g., `jmp`, `call`, `loadi`).

## Debugging
When the `std` instruction is encountered, the translator enters debug mode. In this mode, after each instruction is translated, the state of the registers is output using the `debug` function. Debug mode can be disabled using the `cld` instruction.

---

## Appendix

### Register Mapping
The following table shows how the 16-bit X registers are mapped to x86-64 registers:

| X Register | x86-64 Register |
|------------|-----------------|
| r0         | %rax            |
| r1         | %rbx            |
| r2         | %rcx            |
| r3         | %rdx            |
| r4         | %rsi            |
| r5         | %rdi            |
| r6         | %r8             |
| r7         | %r9             |
| r8         | %r10            |
| r9         | %r11            |
| r10        | %r12            |
| r11        | %r13            |
| r12        | %r14            |
| F          | %r15            |
| r14        | %rbp            |
| r15        | %rsp            |

### 0-Operand Instructions

| Mnemonic      | Encoding          | Description                              | Function                                  |
|---------------|-------------------|------------------------------------------|-------------------------------------------|
| ret           | 00000001 0        | Return from a procedure call.            | PC ← memory[r15] r15 ← r15 + 2            |
| cld           | 00000010 0        | Stop debug mode                          | See Debug Mode below.                     |
| std           | 00000011 S 0      | Start debug mode                         | See Debug Mode below.                     |

### 1-Operand Instructions

| Mnemonic      | Encoding          | Description                                          | Function                                |
|---------------|-------------------|------------------------------------------------------|-----------------------------------------|
| neg rD        | 01000001 D 0      | Negate register rD.                                  | rD ← −rD                                |
| not rD        | 01000010 D 0      | Logically negate register rD.                        | rD ← !rD                                |
| inc rD        | 01001000 D 0      | Increment rD.                                        | rD ← rD + 1                             |
| dec rD        | 01001001 D 0      | Decrement rD.                                        | rD ← rD − 1                             |
| push rS       | 01000011 S 0      | Push register rS onto the pro- gram stack.           | r15 ← r15 − 2 memory[r15] ← rS          |
| pop rD        | 01000100 D 0      | Pop value from stack into register rD.               | rD ← memory[r15] r15 ← r15 + 2          |
| out rS        | 01000111 S 0      | Output character in rS to stdout.                    | output ← rS (see below)                 |
| br L          | 01100001 L        | Branch relative to label L if condition bit is true. | if F & 0x0001 == 0x001: PC ← PC + L − 2 |
| jr L          | 01100010 L        | Jump relative to label L.                            | PC ← PC + L − 2                         |

### 2-Operand Instructions

| Mnemonic      | Encoding          | Description                                                       | Function                                                  |
|---------------|-------------------|-------------------------------------------------------------------|-----------------------------------------------------------|
| add rS, rD    | 10000001 S D      | Add register rS to register rD.                                   | rD ← rD + rS                                              |
| sub rS, rD    | 10000010 S D      | Subtract register rS from register rD.                            | rD ← rD − rS                                              |
| mul rS, rD    | 10000011 S D      | Multiply register rD by register rS.                              | rD ← rD * rS                                              |
| and rS, rD    | 10000101 S D      | And register rS with register rD.                                 | rD ← rD & rS                                              |
| or rS, rD     | 10000110 S D      | Or register rS with register rD.                                  | `rD ← rD \| rS`                                           |
| xor rS, rD    | 10000111 S D      | Xor register rS with register rD.                                 | rD ← rD ^ rS                                              |
| test rS1, rS2 | 10001010 S D      | Set condition flag to true if and only if rS1 ∧ rS2 is not 0.     | `if rS1 & rS2 != 0: F ← F \| 0x0001 else: F ← F & 0xFFFE` |
| cmp rS1, rS2  | 10001011 S D      | Set condition flag to true if and only if rS1 < rS2.              | `if rS1 < rS2: F ← F  \| 0x0001 else: F ← F & 0xFFFE`     |
| equ rS1, rS2  | 10001100 S D      | Set condition flag to true if and only if rS1 == rS2.             | `if rS1 == rS2: F ← F \| 0x0001 else: F ← F & 0xFFFE`     |
| mov rS, rD    | 10001101 S D      | Copy register rS to register rD.                                  | rD ← rS                                                   |
| load rS, rD   | 10001110 S D      | Load word into register rD from memory pointed to by register rS. | rD ← memory[rS]                                           |
| stor rS, rD   | 10001111 S D      | Store word from register rS to memory at address in register rD.  | memory[rD] ← rS                                           |
| loadb rS, rD  | 10010000 S D      | Load byte into register rD from memory pointed to by register rS. | rD ← (byte)memory[rS]                                     |
| storb rS, rD  | 10010001 S D      | Store byte from register rS to memory at address in register rD.  | (byte)memory[rD] ← rS                                     |

### Extended Instructions

| Mnemonic      | Encoding          | Description                                         | Function                                           |
|---------------|-------------------|-----------------------------------------------------|----------------------------------------------------|
| jmp L         | 11000001 0        | Absolute jump to label L.                           | PC ← memory[PC]                                    |
| call L        | 11000010 0        | Absolute call to label L.                           | r15 ← r15 − 2 memory[r15] ← PC + 2 PC ← memory[PC] |
| loadi V, rD   | 11100001 D 0      | Load immediate value or address V into register rD. | rD ← memory[PC] PC ← PC + 2                        |

### Rest Instruction Set Table

| Instruction   | Description                                                       |
|---------------|-------------------------------------------------------------------|
| ret           | Return from a procedure call.                                     |
| br L          | Branch relative to label L if condition bit is true.              |
| jr L          | Jump relative to label L.                                         |
| jmp L         | Absolute jump to label L.                                         |
| call L        | Absolute call to label L.                                         |
| load rS, rD   | Load word into register rD from memory pointed to by register rS. |
| stor rS, rD   | Store word from register rS to memory at address in register rD.  |
| loadb rS, rD  | Load byte into register rD from memory pointed to by register rS. |
| storb rS, rD  | Store byte from register rS to memory at address in register rD.  |
| out rS        | Output character in rS to stdout.                                 |



