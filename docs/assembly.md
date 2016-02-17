# Consolite Assembly

Consolite's assembly language is the input format required for the
assembler. Details are given here about the constructs of the assembly
language and the usage of its instructions.

## Comments

Comments in Consolite assembly start with a semicolon. From the first
occurrence of `;` to the end of the line is ignored. For example:

```asm
ADD A B           ; I'm a comment! I don't affect the code.
```

## Labels

Labels in Consolite assembly refer to the address of the instruction
or data that follows. A label starts with a letter or an underscore and
is followed by zero or more alphanumeric or underscore characters.
A *label declaration* ends in a colon and is on a line by itself,
whereas a *label reference* is used as part of an instruction. For
example:

```asm
label:             ; This is a label declaration
ADD A C
CMP A B
JNE label          ; This is a label reference.
```

Label declarations are often used at the start of functions or loops
so that you can jump to an address by name instead of giving the
assembler a literal numeric address that you have to update manually
when the code changes. The assembler finds labels on the first pass
and calculates their addresses, then substitues in the address value
on the second pass. For example:

```asm
MOVI A 0x0         ; Each instruction is 4 bytes, so the ADD
MOVI B 0x1         ; instruction that 'label' points to starts
MOVI C 0x5         ; on address 0xc.
label:
ADD A B
CMP A C
JB label           ; This will be replaced with 'JB 0xc' by the assembler.
```

## Data

The only type of data that Consolite assembly can currently parse is hexadecimal.
Consolite is Big Endian, so the value `0x1234` will be translated as
the byte `0x12` followed by the byte `0x34`. Data can be used in some
instructions, and they usually have a 16-bit or 8-bit limit. For example:

```asm
MOVI A 0x1234      ; MOVI can take up to a 16-bit value.
RET 0x12           ; RET can only take an 8-bit value.
```

Data can also be declared on a line by itself, and will be padded with
trailing zero bytes to meet 4-byte alignment. You can prefix data with
a label for using something like named global variables. For example:

```asm
LOADI A data       ; Load the first 16 bits of 'data' into register A.
MOVI C 0x1
ADD A C            ; Add 1 to register A.
STORI A data       ; Store the modified value.

data:              ; A label for referencing the start of the data
0x0024 0x3244 0xbc993211 0x4  ; Multiple data declarations on one line
```

## Registers

There are 16 addressable registers in Consolite. These are `SP`, the stack
pointer; `FP`, the frame pointer; and `A` through `N`, 14 general purpose
registers. These can also be referred to as `R0` through `R15`, where
`R0` is `SP`, `R1` is `FP`, and `R2` through `R15` are `A` through `N`.
The former format is preferred, however.

Each of these registers is 16 bits. Since there is a limit of 64KiB of main
memory, each register can store an absolute address to any byte in memory.

There is also an instruction pointer register, which is only accessible
through the `CALL` and `RET` instructions. The instruction pointer only has
14 bits since it must be 4-byte aligned. Jumping to an address that is
not 4-byte aligned will cause the address to be truncated to 14 bits.

The color register is write-only, and stores the value of the current
color to be drawn to the screen with the `PIXEL` instruction. To set the
current color, use `COLOR <REG>`. Since the color is only 8-bit, the
lower 8 bits in the provided register will be used. The high 8 bits
are ignored.

The flags register is not directly accessible, but is set by arithmetic and
bitwise instructions. The values of the flags are used in variations of the
`JMP` instrution, detailed later under [Instructions](#instructions). There
are four flags:

* Overflow flag (OF), used to signify signed arithmetic overflow or underflow.
* Carry flag (CF), used to signify unsigned arithmetic overflow or underflow.
* Zero flag (ZF), used to signify the result of the last instruction was zero.
* Sign flag (SF), used to signify the result of the last instruction was
  negative.

## <a name="instructions"></a>Instructions

Consolite assembly currently has 45 instructions. This section details
the syntax and effects of each instruction. All DATA values are 16-bit
unless otherwise indicated.

### Load and Store

* `MOV REG1 REG2` - Copies the data in `REG2` to `REG1`.
* `MOVI REG1 LABEL|DATA` - Copies the the address of `LABEL` or the literal
`DATA` to `REG1`.
* `LOAD REG1 REG2` - Loads the 2 bytes stored at location `REG2` and stores them
in `REG1`.
* `LOADI REG1 LABEL|DATA` - Loads the 2 bytes stored at the address given
by `LABEL|DATA` and stores them in `REG1`.
* `STOR REG1 REG2` - Stores the value of `REG1` to the address of `REG2` in
main memory.
* `STORI REG1 LABEL|DATA` - Stores the value of `REG1` to the literal address
given by `LABEL|DATA`.
`LABEL|DATA` and stores it in `REG1`.
* `PUSH REG` - Pushes the value of `REG` onto the stack and increments the stack
pointer.
* `POP REG` - Pops the value off the top of the stack and stores it in `REG`
and decrements the stack pointer.

### Arithmetic

* `ADD REG1 REG2` - Stores the value of `REG1 + REG2` in `REG1`.
* `SUB REG1 REG2` - Stores the value of `REG1 - REG2` in `REG1`.
* `MUL REG1 REG2` - Stores the value of `REG1 * REG2` in `REG1`.
* `DIV REG1 REG2` - Stores the value of `REG1 / REG2` in `REG1`.

### Bitwise

* `AND REG1 REG2` - Stores the value of `REG1 & REG2` in `REG1`.
* `OR REG1 REG2` - Stores the value of `REG1 | REG2` in `REG1`.
* `XOR REG1 REG2` - Stores the value of `REG1 ^ REG2` in `REG1`.
* `SHL REG1 REG2` - Stores the value of `REG1 << REG2` in `REG1`.
* `SHRA REG1 REG2` - Stores the value of `REG1 >> REG2` in `REG1`, where `>>`
is an arithmetic right shift, meaning it will sign-extend `REG1`.
* `SHRL REG1 REG2` - Stores the value of `REG1 >> REG2` in `REG1`, where `>>`
is a logical right shift, meaning it will not sign-extend `REG1`.

### Graphics

* `COLOR REG` - Sets the color register as the lower 8 bits of `REG`.
See the [Colors](#colors) section below for information on the color format.
* `PIXEL REG1 REG2` - Draws a pixel on the screen with the current value
of the color register. The lower 8 bits of `REG1` are the x-coordinate, and
the lower 8 bits of `REG2` are the y-coordinate.

### Input

* `INPUT REG1 REG2` - Stores in `REG1` the status of the input with input_id
stored in `REG2`.

### Timing

* `TIME REG` - Stores the time (in milliseconds) since the last `TIMERST`
instruction in `REG`.
* `TIMERST` - Resets the timing counter to zero.

### Control Flow

The names for the jump instructions assume that the `CMP` instruction was
used to set flags. For example, if we are testing for less than or equal to:

```asm
MOVI A 0x1
MOVI B 0x3
CMP A B
JLE some_label         ; Will jump if A <= B
```

* `CALL LABEL|DATA` - Pushes the instruction pointer onto the stack and
jumps to the address given by `LABEL|DATA`.
* `RET [DATA]` - Pops the instruction pointer off the stack and jumps to
it, optionally popping `DATA` more bytes off the stack. `DATA` is a maximum
of 8 bits.
* `CMP REG1 REG2` - Does the operation `REG1 - REG2` and discards the
result, but sets flags.
* `TST REG1 REG2` - Does the operation `REG1 & REG2` and discards the
result, but sets flags.
* `JMP REG` - Unconditional jump to the address stored in `REG`.
* `JMPI LABEL|DATA` - Unconditional jump to the address given by `LABEL|DATA`.
* `JEQ LABEL|DATA` - Jump to the address given by `LABEL|DATA` if the zero
flag is set.
* `JNE LABEL|DATA` - Jump if the zero flag is not set.
* `JG LABEL|DATA` - 'Jump greater than', jump if the zero flag is not set and
the sign flag equals the overflow flag.
* `JGE LABEL|DATA` - 'Jump greater than or equal to', jump if the sign flag
equals the overflow flag.
* `JA LABEL|DATA` - 'Jump if above', jump if the carry flag is not set and
the zero flag is not set.
* `JAE LABEL|DATA` - 'Jump if above or equal to', jump if the carry flag is
not set.
* `JL LABEL|DATA` - 'Jump less than', jump if the sign flag does not equal
the overflow flag.
* `JLE LABEL|DATA` - 'Jump less than or equal to', jump if the sign flag does
not equal the overflow flag or if the zero flag is set.
* `JB LABEL|DATA` - 'Jump if below', jump if the carry flag is set.
* `JBE LABEL|DATA` - 'Jump if below or equal to', jump if the carry flag is
set or the zero flag is set.
* `JO LABEL|DATA` - 'Jump on overflow', jump if the overflow flag is set.
* `JNO LABEL|DATA` - 'Jump on no overflow', jump if the overflow flag is not
set.
* `JS LABEL|DATA` - 'Jump on sign flag', jump if the sign flag is set.
* `JNS LABEL|DATA` - 'Jump on no sign flag', jump if the sign flag is not set.

### Miscellaneous

* `NOP` - Does nothing for one cycle.
* `RND REG` - Gets 16 random bits and stores them in `REG`.

## <a name="colors"></a>Colors

Consolite has 8-bit color in an RGB format. The first three bits are
red, the middle three bits are green, and the lower three bits are blue.
For example, `111 000 00` would be fully red, `000 111 00` would be fully
green, and `000 000 11` would be fully blue.
