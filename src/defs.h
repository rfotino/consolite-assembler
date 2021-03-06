/**
 * Consolite Assembler
 * Copyright (c) 2015 Robert Fotino, All Rights Reserved
 */

#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <map>

#define MAX_OUTPUT_SIZE 65536

const static std::map<std::string, char> opcodes = {
  { "NOP",   0x00 },
  { "INPUT", 0x01 },
  { "CALL",  0x02 },
  { "RET",   0x03 },
  { "LOAD",  0x04 },
  { "LOADI", 0x05 },
  { "MOV",   0x06 },
  { "MOVI",  0x07 },
  { "PUSH",  0x08 },
  { "POP",   0x09 },
  { "ADD",   0x0A },
  { "SUB",   0x0B },
  { "MUL",   0x0C },
  { "DIV",   0x0D },
  { "AND",   0x0E },
  { "OR",    0x0F },
  { "XOR",   0x10 },
  { "SHL",   0x11 },
  { "SHRA",  0x12 },
  { "SHRL",  0x13 },
  { "CMP",   0x14 },
  { "TST",   0x15 },
  { "COLOR", 0x16 },
  { "PIXEL", 0x17 },
  { "STOR",  0x18 },
  { "STORI", 0x19 },
  { "TIME",  0x1A },
  { "TIMERST", 0x1B },
  { "RND",   0x1C },
  { "JMP",   0x30 },
  { "JMPI",  0x31 },
  { "JEQ",   0x32 },
  { "JNE",   0x33 },
  { "JG",    0x34 },
  { "JGE",   0x35 },
  { "JA",    0x36 },
  { "JAE",   0x37 },
  { "JL",    0x38 },
  { "JLE",   0x39 },
  { "JB",    0x3A },
  { "JBE",   0x3B },
  { "JO",    0x3C },
  { "JNO",   0x3D },
  { "JS",    0x3E },
  { "JNS",   0x3F }
};

const static std::map<std::string, unsigned char> registers = {
  { "SP",  0x0 },
  { "FP",  0x1 },
  { "A",   0x2 },
  { "B",   0x3 },
  { "C",   0x4 },
  { "D",   0x5 },
  { "E",   0x6 },
  { "F",   0x7 },
  { "G",   0x8 },
  { "H",   0x9 },
  { "I",   0xA },
  { "J",   0xB },
  { "K",   0xC },
  { "L",   0xD },
  { "M",   0xE },
  { "N",   0xF },
  { "R0",  0x0 },
  { "R1",  0x1 },
  { "R2",  0x2 },
  { "R3",  0x3 },
  { "R4",  0x4 },
  { "R5",  0x5 },
  { "R6",  0x6 },
  { "R7",  0x7 },
  { "R8",  0x8 },
  { "R9",  0x9 },
  { "R10", 0xA },
  { "R11", 0xB },
  { "R12", 0xC },
  { "R13", 0xD },
  { "R14", 0xE },
  { "R15", 0xF }
};

#endif
