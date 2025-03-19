#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

enum TokenType {
  CLASS,
  PRINT,
  NIL,
  SUPER,
  ELSE,
  RETURN,
  THIS,
  VAR,
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
  BANG_EQUAL,
  STRING,
  NUMBER,
  IDENTIFIER,
  AND,
  FOR,
  IF,
  TRUE,
  FALSE,
  WHILE,
  FUN,
  OR,

};

struct Token {
  TokenType type;
  std::string lexeme;
  std::string literal;
  uint16_t line;

  std::string to_string(TokenType type) const {
    std::string res_string;
    switch (type) {
    case TokenType::RETURN:
      return "RETURN";
    case TokenType::SUPER:
      return "SUPER";
    case TokenType::CLASS:
      return "CLASS";
    case TokenType::PRINT:
      return "PRINT";
    case TokenType::NIL:
      return "NIL";
    case TokenType::ELSE:
      return "ELSE";
    case TokenType::VAR:
      return "VAR";
    case TokenType::AND:
      return "AND";
    case TokenType::FOR:
      return "FOR";
    case TokenType::IF:
      return "IF";
    case TokenType::TRUE:
      return "TRUE";
    case TokenType::FALSE:
      return "FALSE";
    case TokenType::WHILE:
      return "WHILE";
    case TokenType::FUN:
      return "FUN";
    case TokenType::OR:
      return "OR";
    case TokenType::IDENTIFIER:
      return "IDENTIFIER";
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
    case TokenType::STRING:
      return "STRING";
    case TokenType::NUMBER:
      return "NUMBER";
    case TokenType::EOF_TOKEN:
      return "EOF";
    case TokenType::THIS:
      return "THIS";
    default:
      return "UNKNOWN";
    }
  }
};

class Scanner {
public:
  Scanner(std::string_view content)
      : line(1), content(content), current(0), start(0), has_error(false) {}

  char peek() {
    if (at_content_end())
      return '\0';
    return content.at(current);
  }

  char peek_next() {
    if (current + 1 >= content.length())
      return '\0';

    return content.at(current + 1);
  }

  bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool is_alpha_num(char c) { return (is_alpha(c) || is_digit(c)); }

  void handle_identifier() {
    while (is_alpha_num(peek())) {
      current++;
    }

    std::string current_text = content.substr(start, current - start);
    TokenType word_type;
    if (keywords.find(current_text) != keywords.end()) {
      word_type = keywords.find(current_text)->second;
    } else {
      word_type = TokenType::IDENTIFIER;
    }

    add_token(word_type);
  }

  bool is_digit(char c) { return c >= '0' && c <= '9'; }

  void handle_number() {
    while (is_digit(peek()) && !at_content_end())
      current++;

    if (peek() == '.' && is_digit(peek_next())) {
      current++;

      while (is_digit(peek()) && !at_content_end())
        current++;
    }

    std::string sub_num = content.substr(start, current - start);
    size_t decimal = sub_num.find('.');
    int decimal_places = 0;
    if (decimal != std::string::npos) {
      decimal_places = sub_num.length() - decimal - 1;
    }

    double value = std::stod(sub_num);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimal_places) << value;
    std::string formatted_num = oss.str();
    size_t decimal_pos = formatted_num.find('.');

    if (decimal_pos == std::string::npos) {
      formatted_num += ".0";
    } else {
      // Remove trailing zeros, but keep at least one digit after decimal
      size_t last_non_zero = formatted_num.find_last_not_of('0');

      // If the last non-zero character is the decimal point itself
      if (last_non_zero == decimal_pos) {
        // Keep one zero after decimal
        formatted_num = formatted_num.substr(0, decimal_pos + 2);
      } else {
        // Keep until the last non-zero digit
        formatted_num = formatted_num.substr(0, last_non_zero + 1);
      }
    }

    add_token(NUMBER, formatted_num);
  }

  void add_token(TokenType token_type) { add_token(token_type, "null"); }

  template <typename T> void add_token(TokenType type, T value) {
    std::string lexeme = content.substr(start, current - start);
    Token new_token = {
        .type = type, .lexeme = lexeme, .literal = value, .line = line};

    token_list.emplace_back(new_token);
  }

  void handle_string() {
    while (peek() != '"' && !at_content_end()) {
      if (peek_next() == '\n')
        line++;

      current++;
    }

    if (at_content_end()) {
      std::cerr << "["
                << "line " << line << "] Error: "
                << "Unterminated string.\n";
      has_error = true;
      return;
    }

    // Move past the closing "
    current++;

    std::string value = content.substr(start + 1, (current - start) - 2);
    add_token(STRING, value);
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
        while (peek() != '\n' && !at_content_end())
          current++;
      } else {
        add_token(SLASH);
      }
      break;
    }

    case ' ':
    case '\r':
    case '\t':
      break;
    case '\n':
      line++;
      break;

    case '"':
      handle_string();
      break;

    default:
      if (is_digit(token)) {
        handle_number();
      } else if (is_alpha(token)) {
        handle_identifier();
      } else {
        std::cerr << "[line " << line << "] "
                  << "Error: Unexpected character: " << token << '\n';
        has_error = true;
        break;
      }
    }
  }

  bool at_content_end() { return current >= content.length(); }

  void scan_tokens() {
    while (!at_content_end()) {
      start = current;
      scan_token(content[current++]);
    }

    Token eof_token = {TokenType::EOF_TOKEN, "", "null", line};
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

  std::unordered_map<std::string, TokenType> keywords = {
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"for", TokenType::FOR},       {"fun", TokenType::FUN},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE},
  };
};
