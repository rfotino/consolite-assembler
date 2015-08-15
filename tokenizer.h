#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>

class Tokenizer {
 public:
  Tokenizer(char *infile_name);
  bool hasError() { return _error; }

 private:
  std::ifstream _input;
  bool _error;
};

#endif
