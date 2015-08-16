#include <iostream>
#include "defs.h"
#include "parser.h"

bool Parser::firstPass() {
  _tokenizer->reset();
  if (_tokenizer->hasError()) {
    return false;
  }

  bool error = false;
  unsigned line = 0;
  unsigned byte_pos = 0;
  std::vector<Token> tokens;
  while (true) {
    // Get the next line of tokens
    _tokenizer->getLineOfTokens(tokens);
    line++;
    // If there is an error or the tokenizer is
    // empty, break out of the loop
    if (_tokenizer->isEmpty()) {
      line--;
      break;
    }
    if (_tokenizer->hasError()) {
      error = true;
      std::cerr << "Error reading from input file on line "
		<< line << "." << std::endl;
      break;
    }
    // If it's an empty line, do nothing
    if (0 == tokens.size()) {
      continue;
    }
    // Validate based on lead token
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
      switch (it->getType()) {
      case OPCODE:
	std::cout << "Found OPCODE on line " << line
		  << "." << std::endl;
	break;
      case REGISTER:
	std::cout << "Found REGISTER on line " << line
		  << "." << std::endl;
	break;
      case DATA:
	byte_pos += it->getSize();
	std::cout << "Found DATA on line " << line
		  << "." << std::endl;
	break;
      case LABELDECL:
	std::cout << "Found LABEL DECLARATION on line " << line
		  << "." << std::endl;
	break;
      case LABELREF:
	std::cout << "Found LABEL REFERENCE on line " << line
		  << "." << std::endl;
	break;
      case UNKNOWN:
	std::cout << "Found UNKNOWN TOKEN on line " << line
		  << "." << std::endl;
	break;
      }
    }
  }

  std::cout << "Read " << line << " lines from input file." << std::endl;

  return !error;
}

bool Parser::secondPass() {
  _tokenizer->reset();
  if (_tokenizer->hasError()) {
    std::cerr << "Error resetting input stream to start of file for 2nd pass."
	      << std::endl;
    return false;
  }
  return true;
}
