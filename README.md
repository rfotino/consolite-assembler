# Consolite Assembler

This program takes in a text file written in assembly language, and writes out
binary instructions to another file. The binary instructions can then be
directly executed by the Consolite microprocessor or an emulator.

## What Is Consolite?

Consolite (coming from "Console Lite") is the name I've given to my design of a
hobbyist microprocessor and associated toolchain. Consolite's instruction set
architecture (ISA) has assembly level instructions for setting colors, drawing
pixels on the screen, and receiving input. This makes it suitable for writing
games, which is what makes it a "console".

### Consolite Specs

* Display: 256 x 192 pixels, 8-bit color
* Registers: 16 general purpose, instruction pointer, color, flags
* Memory: 64KiB main memory, 48KiB video memory

## Building and Dependencies

This version of the assembler is written in C++, and uses only the standard
library. To build the project just type `make`.

## Usage

`./asm SRC DEST`

### Limitations

* The assembler can't read from standard input because it needs two passes
through the assembly code, the first for error checking and the other for
converting to binary. 
* The assembler can't take in multiple source files. You will have to do
something like `cat FILE1 FILE2 FILE3 > FILE4 && ./asm FILE4 DEST` to
achieve this.
* The maximum output filesize is limited to 64KiB, because Consolite must
load the entire binary into main memory before it can start executing.
