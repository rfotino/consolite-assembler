#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
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
  Token(TokenType type, const std::string& data)
       : _type(type), _data(data) { }

  TokenType getType() const { return _type; }
  std::string getData() const { return _data; }
  size_t getSize() const { return _data.size(); }

 private:
  TokenType _type;
  std::string _data;
};

class Tokenizer {
 public:
  Tokenizer(const std::string& infile_name);
  void getLineOfTokens(std::vector<Token> &tokens);
  bool isEmpty() { return _input.eof() && 0 == _charsRead; }
  bool hasError() { return !_input; }
  void reset();

 private:
  bool _isValidLabel(const std::string& label);
  bool _isValidData(const std::string& text);
  char _getHexVal(const char& c);
  void _getData(const std::string& text, std::string& data);
  void _addToken(std::vector<Token>& tokens, const std::string& text);
  std::ifstream _input;
  int _charsRead;
};

#endif
