#include <iostream>
#include "tokenizer.h"
using namespace std;

Tokenizer::Tokenizer(char *infile_name): _input(infile_name), _error(false) {
  if (!_input) {
    cerr << "Failed to open input file '" << infile_name << "'." << endl;
    _error = true;
  }
}
