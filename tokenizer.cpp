#include <iostream>
#include <cctype>
#include <string.h>
#include "defs.h"
#include "tokenizer.h"
using namespace std;

Token::Token(TokenType type, char *data, size_t size)
            : _type(type), _size(size) {
  _data = new char[_size];
  memcpy(_data, data, _size);
}

Token::~Token() {
  delete[] _data;
}

Tokenizer::Tokenizer(char *infile_name)
                    : _input(infile_name), _error(false) {
  if (!_input) {
    cerr << "Failed to open input file '" << infile_name << "'." << endl;
    _error = true;
  }
}

bool Tokenizer::_isValidLabel(char const *label, const size_t label_size) {
  // Labels must have nonzero length and cannot start with a digit
  if (0 == label_size || isdigit(label[0])) {
    return false;
  }
  // All characters in label must be either alphanumeric or
  // the underscore character.
  for (size_t i = 0; i < label_size; i++) {
    if ('_' != label[i] && !isalnum(label[i])) {
      return false;
    }
  }
  return true;
}

bool Tokenizer::_isValidData(char const *text, const size_t text_size) {
  // Data must start with '0x' and must be followed by at least
  // one digit.
  if (text_size < 3 || '0' != text[0] || 'x' != text[1]) {
    return false;
  }
  // Data after the initial '0x' must be hexadecimal
  for (size_t i = 2; i < text_size; i++) {
    if (-1 == _getHexVal(text[i])) {
      return false;
    }
  }
  return true;
}

char Tokenizer::_getHexVal(char c) {
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

void Tokenizer::_getData(char const *text, const size_t text_size,
			 char *data, size_t &data_size) {
  data_size = (text_size - 1) / 2;
  size_t i;
  if (text_size & 1) {
    data[0] = _getHexVal(text[2]);
    i = 3;
  } else {
    data[0] = (_getHexVal(text[2]) << 4) | _getHexVal(text[3]);
    i = 4;
  }
  for (int j = 1; i < text_size; i += 2, j++) {
    data[j] = (_getHexVal(text[i]) << 4) | _getHexVal(text[i+1]);
  }
}

void Tokenizer::_addToken(vector<Token> &tokens, char *text) {
  size_t text_size = strlen(text);
  if (opcodes.find(text) != opcodes.end()) {
    // The token is an opcode
    Token t(OPCODE, text, text_size + 1);
    tokens.push_back(t);
  } else if (registers.find(text) != registers.end()) {
    // The token is a register
    Token t(REGISTER, text, text_size + 1);
    tokens.push_back(t);
  } else if (_isValidData(text, text_size)) {
    // The token is data
    char data[MAXLINESIZE];
    size_t data_size;
    _getData(text, text_size, data, data_size);
    Token t(DATA, data, data_size);
    tokens.push_back(t);
  } else if (':' == text[text_size - 1] &&
	     _isValidLabel(text, text_size - 1)) {
    // The token ends in :, it is a label declaration
    text[text_size - 1] = '\0';
    Token t(LABELDECL, text, text_size);
    tokens.push_back(t);
  } else if (_isValidLabel(text, text_size)) {
    // The token is a label reference
    Token t(LABELREF, text, text_size + 1);
    tokens.push_back(t);
  } else {
    // The token is of an unknown type
    Token t(UNKNOWN, text, text_size);
    tokens.push_back(t);
  }
}

void Tokenizer::getLineOfTokens(vector<Token> &tokens) {
  if (_error) {
    return;
  }

  char line[MAXLINESIZE+1];
  size_t line_size;
  _input.getline(line, MAXLINESIZE+1);
  line_size = _input.gcount();
  tokens.clear();

  size_t start = 0;
  for (size_t end = 0; end <= line_size; end++) {
    if (end == line_size || ';' == line[end] || ' ' == line[end]) {
      // We are on a token boundary
      size_t token_size = end - start - 1;
      if (0 != token_size) {
	char token_text[sizeof(line)];
	memcpy(token_text, &line[start], token_size);
	token_text[token_size] = '\0';
	_addToken(tokens, token_text);
      }
      start = end;
    }
    // If we're at the start of a comment, end tokenization
    if (end != line_size && ';' == line[end]) {
      break;
    }
  }
}
