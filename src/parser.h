/**
 * Consolite Assembler
 * Copyright (c) 2015 Robert Fotino, All Rights Reserved
 */

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <vector>
#include <fstream>
#include "tokenizer.h"

class Parser {
 public:
  Parser(Tokenizer *tokenizer, char *outfile_name)
    : _tokenizer(tokenizer), _outfileName(outfile_name) { }
  bool firstPass();
  bool secondPass();
  
 private:
  bool _validateInstruction(const std::vector<Token>& tokens,
                            const unsigned& line);
  Tokenizer *_tokenizer;
  std::map<std::string, unsigned> _labels;
  std::vector<std::pair<std::string, unsigned>> _labelRefs;
  std::string _outfileName;
  std::ofstream _output;
};

#endif
