#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "tokenizer.h"

class Parser {
 public:
  Parser(Tokenizer *tokenizer, char *outfile_name)
    : _tokenizer(tokenizer), _output(outfile_name) { }
  bool firstPass();
  bool secondPass();
  
 private:
  Tokenizer *_tokenizer;
  std::ofstream _output;
};

#endif
