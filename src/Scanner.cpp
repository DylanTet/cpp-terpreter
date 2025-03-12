#include <iostream>
#include <string>
#include <string_view>
#include <vector>

enum TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  STAR,
  DOT,
  COMMA,
  PLUS,
  MINUS,
  SEMICOLON,
  EQUAL,
  EQUAL_EQUAL,
  LESS_EQUAL,
  LESS,
  GREATER_EQUAL,
  GREATER,
  SLASH,
  EOF_TOKEN,
  BANG,
  BANG_EQUAL
};

struct Token {
  TokenType type;
  std::string lexeme;
  std::string literal;
  uint16_t line;

  std::string to_string(TokenType type) const {
    std::string res_string;
    switch (type) {
    case TokenType::LEFT_PAREN:
      return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
      return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
      return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
      return "RIGHT_BRACE";
    case TokenType::COMMA:
      return "COMMA";
    case TokenType::DOT:
      return "DOT";
    case TokenType::MINUS:
      return "MINUS";
    case TokenType::PLUS:
      return "PLUS";
    case TokenType::SEMICOLON:
      return "SEMICOLON";
    case TokenType::SLASH:
      return "SLASH";
    case TokenType::STAR:
      return "STAR";
    case TokenType::BANG_EQUAL:
      return "BANG_EQUAL";
    case TokenType::BANG:
      return "BANG";
    case TokenType::GREATER_EQUAL:
      return "GREATER_EQUAL";
    case TokenType::GREATER:
      return "GREATER";
    case TokenType::LESS:
      return "LESS";
    case TokenType::LESS_EQUAL:
      return "LESS_EQUAL";
    case TokenType::EQUAL:
      return "EQUAL";
    case TokenType::EQUAL_EQUAL:
      return "EQUAL_EQUAL";
      // case TokenType::IDENTIFIER:
      //   return "IDENTIFIER";
      // case TokenType::STRING:
      //   return "STRING";
      // case TokenType::NUMBER:
      //   return "NUMBER";
    case TokenType::EOF_TOKEN:
      return "EOF";
    default:
      return "UNKNOWN";
    }
  }
};

class Scanner {
public:
  Scanner(std::string_view content)
      : line(1), content(content), current(0), start(0), has_error(false) {}

  char peek_next() {
    if (at_content_end())
      return '\0';
    return content.at(current);
  }

  void add_token(TokenType token_type) {
    Token new_token = {.type = token_type,
                       .lexeme = content.substr(start, current - start),
                       .literal = "null",
                       .line = line};

    token_list.emplace_back(new_token);
  }

  void scan_token(char token) {
    switch (token) {
    case '(':
      add_token(LEFT_PAREN);
      break;

    case ')': {
      add_token(RIGHT_PAREN);
      break;
    }

    case '{': {
      add_token(LEFT_BRACE);
      break;
    }

    case '}': {
      add_token(RIGHT_BRACE);
      break;
    }

    case '*': {
      add_token(STAR);
      break;
    }

    case '.': {
      add_token(DOT);
      break;
    }

    case ',': {
      add_token(COMMA);
      break;
    }

    case '+': {
      add_token(PLUS);
      break;
    }

    case '-': {
      add_token(MINUS);
      break;
    }

    case ';': {
      add_token(SEMICOLON);
      break;
    }

    case '=': {
      if (content[current] == '=') {
        current++;
        add_token(EQUAL_EQUAL);
      } else {
        add_token(EQUAL);
      }
      break;
    }

    case '!': {
      if (content[current] == '=') {
        current++;
        add_token(BANG_EQUAL);
      } else {
        add_token(BANG);
      }
      break;
    }

    case '>': {
      if (content[current] == '=') {
        current++;
        add_token(GREATER_EQUAL);
      } else {
        add_token(GREATER);
      }
      break;
    }

    case '<': {
      if (content[current] == '=') {
        current++;
        add_token(LESS_EQUAL);
      } else {
        add_token(LESS);
      }
      break;
    }

    case '/': {
      if (content[current] == '/') {
        while (peek_next() != '\n' && !at_content_end())
          current++;
      } else {
        add_token(SLASH);
      }
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
      start = current;
      scan_token(content[current++]);
    }

    Token eof_token = {
        .type = EOF_TOKEN, .lexeme = "", .literal = "null", .line = line};
    token_list.emplace_back(eof_token);

    for (Token &token : token_list) {
      std::cout << token.to_string(token.type) << " " << token.lexeme << ' '
                << token.literal << '\n';
    }

    if (has_error)
      exit(65);
  }

private:
  std::vector<Token> token_list;
  std::string content;
  int current;
  int start;
  uint16_t line;
  bool has_error;
};
