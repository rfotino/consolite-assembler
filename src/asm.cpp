/**
 * Consolite Assembler
 * Copyright (c) 2015 Robert Fotino, All Rights Reserved
 */

#include <iostream>
#include "tokenizer.h"
#include "parser.h"

void usage(char *program_name) {
  std::cout << "Usage: " << program_name << " SRC DEST" << std::endl;
}

int main(int argc, char **argv) {
  if (3 != argc) {
    usage(argv[0]);
    return 1;
  }

  Tokenizer tokenizer(argv[1]);
  Parser parser(&tokenizer, argv[2]);
  if (!parser.firstPass() || !parser.secondPass()) {
    remove(argv[2]);
    return 1;
  }

  return 0;
}
