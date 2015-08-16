#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "tokenizer.h"

class Parser {
 public:
  Parser(Tokenizer *tokenizer, char *outfile_name)
    : _tokenizer(tokenizer), _outfileName(outfile_name) { }
  bool firstPass();
  bool secondPass();
  
 private:
  Tokenizer *_tokenizer;
  std::string _outfileName;
  std::ofstream _output;
};

#endif
