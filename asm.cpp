#include <iostream>
#include "tokenizer.h"
#include "parser.h"
using namespace std;

void usage(char *program_name) {
  cout << "Usage: " << program_name << " SRC DEST" << endl;
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
