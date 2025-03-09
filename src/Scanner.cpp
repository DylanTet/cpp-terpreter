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
  Scanner() : line(1) {}
  void add_token(Token token) { token_list.emplace_back(token); }

  void scan_token(char token) {
    switch (token) {
    case '(': {
      Token new_token = {"(", "null", "LEFT_PAREN"};
      add_token(new_token);
      break;
    }

    case ')': {
      Token new_token = {")", "null", "RIGHT_PAREN"};
      add_token(new_token);
      break;
    }

    case '{': {
      Token new_token = {"{", "null", "LEFT_BRACE"};
      add_token(new_token);
      break;
    }

    case '}': {
      Token new_token = {"}", "null", "RIGHT_BRACE"};
      add_token(new_token);
      break;
    }

    case '*': {
      Token new_token = {"*", "null", "STAR"};
      add_token(new_token);
      break;
    }

    case '.': {
      Token new_token = {".", "null", "DOT"};
      add_token(new_token);
      break;
    }

    case ',': {
      Token new_token = {",", "null", "COMMA"};
      add_token(new_token);
      break;
    }

    case '+': {
      Token new_token = {"+", "null", "PLUS"};
      add_token(new_token);
      break;
    }

    case '-': {
      Token new_token = {"-", "null", "MINUS"};
      add_token(new_token);
      break;
    }

    case ';': {
      Token new_token = {";", "null", "SEMICOLON"};
      add_token(new_token);
      break;
    }

    default:
      std::cerr << "[line " << line << "] "
                << "Error: Unexpected character: " << token << '\n';
      has_error = true;
      break;
    }
  }

  void scan_tokens(std::string_view file_content) {
    // Reset the start of our current token
    for (char token : file_content) {
      scan_token(token);
    }

    Token eof_token = {"", "null", "EOF"};
    add_token(eof_token);

    for (Token &token : token_list) {
      std::cout << token.type << " " << token.lexeme << " " << token.literal
                << '\n';
    }

    if (has_error)
      exit(65);
  }

private:
  std::vector<Token> token_list;
  int line;
  bool has_error;
};
