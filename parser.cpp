#include <iostream>
#include "defs.h"
#include "parser.h"

bool Parser::_validateInstruction(const std::vector<Token>& tokens,
                                  const unsigned& line) {
  const std::string opcode = tokens[0].getData();
  if ("NOP" == opcode) {
    if (1 < tokens.size()) {
      std::cerr << "Error: Invalid syntax, unknown token after "
                << opcode << " on line " << line << "." << std::endl;
      return false;
    }
  } else if ("INPUT" == opcode || "LOAD" == opcode || "MOV" == opcode ||
             "ADD" == opcode || "SUB" == opcode || "MUL" == opcode ||
             "DIV" == opcode || "AND" == opcode || "OR" == opcode ||
             "XOR" == opcode || "SHL" == opcode || "SHRA" == opcode ||
             "SHRL" == opcode || "CMP" == opcode || "TST" == opcode ||
             "PIXEL" == opcode) {
    if (3 != tokens.size() ||
        REGISTER != tokens[1].getType() ||
        REGISTER != tokens[2].getType()) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " DEST SRC on line "
                << line << "." << std::endl;
      return false;
    }
  } else if ("PUSH" == opcode || "POP" == opcode || "COLOR" == opcode ||
             "JMP" == opcode) {
    if (2 != tokens.size() ||
        REGISTER != tokens[1].getType()) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " REG on line "
                << line << "." << std::endl;
      return false;
    }
  } else if ("CALL" == opcode || "JMPI" == opcode || "JEQ" == opcode ||
             "JNE" == opcode || "JG" == opcode || "JGE" == opcode ||
             "JA" == opcode || "JAE" == opcode || "JL" == opcode ||
             "JLE" == opcode || "JB" == opcode || "JBE" == opcode ||
             "JO" == opcode || "JNO" == opcode || "JS" == opcode ||
             "JNS" == opcode) {
    if (2 != tokens.size() ||
        (DATA != tokens[1].getType() &&
         LABELREF != tokens[1].getType())) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " LABEL|ADDR on line "
                << line << "." << std::endl;
      return false;
    } else if (DATA == tokens[1].getType() && 2 < tokens[1].getSize()) {
      std::cerr << "Error: Address too large, max of 16 bits on line "
                << line << "." << std::endl;
      return false;
    } else if (LABELREF == tokens[1].getType()) {
      // Save the label reference for later validation against
      // the full list of label declarations.
      _labelRefs.push_back(make_pair(tokens[1].getData(), line));
    }
  } else if ("RET" == opcode) {
    if (2 < tokens.size() ||
        (2 == tokens.size() && DATA != tokens[1].getType())) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " [NUM] on line "
                << line << "." << std::endl;
      return false;
    } else if (2 < tokens[1].getSize()) {
      std::cerr << "Error: Data too large, max of 16 bits on line "
                << line << "." << std::endl;
      return false;
    }
  } else if ("LOADI" == opcode) {
    if (3 != tokens.size() ||
        REGISTER != tokens[1].getType() ||
        (DATA != tokens[2].getType() &&
         LABELREF != tokens[2].getType())) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " DEST LABEL|ADDR on line "
                << line << "." << std::endl;
      return false;
    } else if (DATA == tokens[2].getType() && 2 < tokens[2].getSize()) {
      std::cerr << "Error: Address too large, max of 16 bits on line "
                << line << "." << std::endl;
      return false;
    } else if (LABELREF == tokens[2].getType()) {
      // Save the label reference for later validation against
      // the full list of label declarations.
      _labelRefs.push_back(make_pair(tokens[2].getData(), line));
    }
  } else if ("MOVI" == opcode) {
    if (3 != tokens.size() ||
        REGISTER != tokens[1].getType() ||
        DATA != tokens[2].getType()) {
      std::cerr << "Error: Invalid syntax for " << opcode
                << ", expected " << opcode << " DEST NUM on line "
                << line << "." << std::endl;
      return false;
    } else if (2 < tokens[2].getSize()) {
      std::cerr << "Error: Data too large, max of 16 bits on line "
                << line << "." << std::endl;
      return false;
    }
  }
  return true;
}

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
    // If there's an unknown token, that's an error
    bool has_unknown = false;
    for (auto token = tokens.begin(); token != tokens.end(); token++) {
      if (UNKNOWN == token->getType()) {
        error = true;
        has_unknown = true;
        std::cerr << "Error: Unknown token '" << token->getData()
                  << "' on line " << line << "." << std::endl;
        break;
      }
    }
    // If there's an unknown token don't validate any further
    if (has_unknown) {
      continue;
    }
    // Validate based on lead token
    switch (tokens[0].getType()) {
    case OPCODE:
      // Validate further based on specific opcode
      if (_validateInstruction(tokens, line)) {
        byte_pos += 4;
      } else {
        error = true;
      }
      break;
    case REGISTER:
      error = true;
      std::cerr << "Error: Register '" << tokens[0].getData()
                << "' at start of instruction on line " << line
                << "." << std::endl;
      break;
    case DATA:
      // Make sure all tokens on this line are data, and
      // add their sizes to the byte offset at this point
      for (auto token = tokens.begin(); token != tokens.end(); token++) {
        if (DATA == token->getType()) {
          byte_pos += token->getSize();
        } else {
          error = true;
          std::cerr << "Error: Unknown token '" << token->getData()
               << "', expected data on line " << line
               << "." << std::endl;
          break;
        }
      }
      break;
    case LABELDECL:
      {
        const std::string label = tokens[0].getData();
        if (1 == tokens.size()) {
          if (_labels.find(label) == _labels.end()) {
            _labels[label] = byte_pos;
          } else {
            error = true;
            std::cerr << "Error: Duplicate label '" << label
                      << "' on line " << line << "." << std::endl;
          }
        } else {
          error = true;
          std::cerr << "Error: Invalid syntax, label '" << label
                    << "' must be by itself on line " << line
                    << "." << std::endl;
        }
      }
      break;
    case LABELREF:
      error = true;
      std::cerr << "Error: Unknown token '" << tokens[0].getData()
                << "' on line " << line << "." << std::endl;
      break;
    case UNKNOWN:
      break;
    }
  }

  if (!error) {
    // Check that all of the label references have a corresponding
    // label. If not, that is another error.
    for (auto ref = _labelRefs.begin(); ref != _labelRefs.end(); ref++) {
      std::string ref_label = ref->first;
      unsigned ref_line = ref->second;
      if (_labels.find(ref_label) == _labels.end()) {
        error = true;
        std::cerr << "Error: Could not find label for reference '"
                  << ref_label << "' on line " << ref_line
                  << "." << std::endl;
      }
    }
    // Check if the total size of the output will be too large,
    // which would be another error.
    if (MAX_OUTPUT_SIZE < byte_pos) {
      error = true;
      std::cerr << "Error: Output would be " << byte_pos
                << "bytes, which is larger than the maximum output size "
                << MAX_OUTPUT_SIZE << "." << std::endl;
    }
  }

  return !error;
}

bool Parser::secondPass() {
  // Reset the tokenizer to the start of the input
  // file so that we can do a second pass through it
  _tokenizer->reset();
  if (_tokenizer->hasError()) {
    std::cerr << "Error resetting input stream to start of file for 2nd pass."
              << std::endl;
    return false;
  }
  // Opens up the output file and checks that it succeeded
  _output.open(_outfileName);
  if (!_output.good()) {
    std::cerr << "Error opening file " << _outfileName
              << " for output." << std::endl;
    return false;
  }
  return true;
}
