#include <iostream>
#include "parser.h"
using namespace std;

bool Parser::firstPass() {
  if (_tokenizer->hasError()) {
    return false;
  }
  return true;
}

bool Parser::secondPass() {
  return true;
}
