#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct Token {
  std::string lexeme;
  std::string literal;
  std::string type;
};

class Scanner {
public:
  void add_token(Token token) { token_list.emplace_back(token); }

  void scan_token(char token) {
    switch (token) {
    case '(': {
      Token new_token = {"(", "", "LEFT_PAREN"};
      add_token(new_token);
      break;
    }

    case ')': {
      Token new_token = {")", "", "RIGHT_PAREN"};
      add_token(new_token);
      break;
    }

    default:
      std::cout << "Unknown token.\n";
    }
  }

  void scan_tokens(std::string_view file_content) {
    // Reset the start of our current token
    for (char token : file_content) {
      scan_token(token);
    }

    Token eof_token = {"", "", "EOF"};
    add_token(eof_token);

    for (Token &token : token_list) {
      std::cout << token.type << " " << token.lexeme << " null\n";
    }
  }

private:
  std::vector<Token> token_list;
};
