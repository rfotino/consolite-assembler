#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>
#include <vector>

enum TokenType {
  OPCODE,
  REGISTER,
  DATA,
  LABELDECL,
  LABELREF,
  UNKNOWN
};

class Token {
 public:
  Token(TokenType type, char *data, size_t size);
  ~Token();

  TokenType getType() { return _type; }
  char * getData() { return _data; }
  size_t getSize() { return _size; }

 private:
  TokenType _type;
  char *_data;
  size_t _size;
};

class Tokenizer {
 public:
  Tokenizer(char *infile_name);
  void getLineOfTokens(std::vector<Token> &tokens);
  bool isEmpty() { return _input.eof(); }
  bool hasError() { return _error; }

 private:
  bool _isValidLabel(char const *label, const size_t label_size);
  bool _isValidData(char const *text, const size_t text_size);
  char _getHexVal(char c);
  void _getData(char const *text, const size_t text_size,
		char *data, size_t &data_size);
  void _addToken(std::vector<Token> &tokens, char *text);
  std::ifstream _input;
  bool _error;
};

#endif
