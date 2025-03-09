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
  Scanner(std::string_view content) : line(1), content(content), current(0) {}
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

    case '=': {
      Token new_token;
      if (content[current] == '=') {
        new_token.type = "EQUAL_EQUAL";
        new_token.lexeme = "==";
        new_token.literal = "null";
        current++;
      } else {
        new_token.type = "EQUAL";
        new_token.lexeme = "=";
        new_token.literal = "null";
      }
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

  bool at_content_end() { return current >= content.size(); }

  void scan_tokens() {
    while (!at_content_end()) {
      scan_token(content[current++]);
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
  std::string content;
  int current;
  int line;
  bool has_error;
};
