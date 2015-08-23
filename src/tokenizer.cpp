#include <iostream>
#include <cctype>
#include <string.h>
#include "defs.h"
#include "tokenizer.h"

Tokenizer::Tokenizer(const std::string& infile_name)
                    : _input(infile_name), _charsRead(0) {
  if (!_input) {
    std::cerr << "Failed to open input file '" << infile_name
              << "'." << std::endl;
  }
}

bool Tokenizer::_isValidLabel(const std::string& label) {
  // Labels must have nonzero length and cannot start with a digit
  if (0 == label.size() || isdigit(label[0])) {
    return false;
  }
  // All characters in label must be either alphanumeric or
  // the underscore character.
  for (size_t i = 0; i < label.size(); i++) {
    if ('_' != label[i] && !isalnum(label[i])) {
      return false;
    }
  }
  return true;
}

bool Tokenizer::_isValidData(const std::string& text) {
  // Data must start with '0x' and must be followed by at least
  // one digit.
  if (text.size() < 3 || '0' != text[0] || 'x' != text[1]) {
    return false;
  }
  // Data after the initial '0x' must be hexadecimal
  for (size_t i = 2; i < text.size(); i++) {
    if (-1 == _getHexVal(text[i])) {
      return false;
    }
  }
  return true;
}

char Tokenizer::_getHexVal(const char& c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('a' <= c && c <= 'f') {
    return 10 + c - 'a';
  } else if ('A' <= c && c <= 'F') {
    return 10 + c - 'A';
  } else {
    return -1;
  }
}

void Tokenizer::_getData(const std::string& text, std::string& data) {
  size_t i;
  if (text.size() & 1) {
    data.push_back(_getHexVal(text[2]));
    i = 3;
  } else {
    data.push_back((_getHexVal(text[2]) << 4) | _getHexVal(text[3]));
    i = 4;
  }
  for (; i < text.size(); i += 2) {
    data.push_back((_getHexVal(text[i]) << 4) | _getHexVal(text[i+1]));
  }
}

void Tokenizer::_addToken(std::vector<Token>& tokens,
                          const std::string& text) {
  // Find the type of token
  if (opcodes.find(text) != opcodes.end()) {
    // The token is an opcode
    Token t(OPCODE, text);
    tokens.push_back(t);
  } else if (registers.find(text) != registers.end()) {
    // The token is a register
    Token t(REGISTER, text);
    tokens.push_back(t);
  } else if (_isValidData(text)) {
    // The token is data
    std::string data;
    _getData(text, data);
    Token t(DATA, data);
    tokens.push_back(t);
  } else if (':' == text[text.size() - 1] &&
             _isValidLabel(text.substr(0, text.size() - 1))) {
    // The token ends in :, it is a label declaration
    std::string label = text.substr(0, text.size() - 1);
    Token t(LABELDECL, label);
    tokens.push_back(t);
  } else if (_isValidLabel(text)) {
    // The token is a label reference
    Token t(LABELREF, text);
    tokens.push_back(t);
  } else {
    // The token is of an unknown type
    Token t(UNKNOWN, text);
    tokens.push_back(t);
  }
}

void Tokenizer::getLineOfTokens(std::vector<Token> &tokens) {
  if (hasError()) {
    return;
  }

  std::string line;
  std::getline(_input, line);
  _charsRead = line.size();
  tokens.clear();

  size_t start = 0;
  for (size_t end = 0; end <= line.size(); end++) {
    if (end == line.size() || ';' == line[end] || ' ' == line[end]) {
      // We are on a token boundary
      size_t token_size = end - start;
      if (0 != token_size) {
        std::string token_text = line.substr(start, token_size);
        _addToken(tokens, token_text);
      }
      start = end + 1;
    }
    // If we're at the start of a comment, end tokenization
    if (end != line.size() && ';' == line[end]) {
      break;
    }
  }
}

void Tokenizer::reset() {
  _input.clear();
  _input.seekg(0, std::ios::beg);
}
