#ifndef LEXER_HPP
#define LEXER_HPP

#include <error.hpp>
#include <token.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Lexer {
  std::string_view filename;
  std::string_view source;

  int start = 0;
  int current = 0;
  int row = 1;
  int column = 1;

  bool isAtEnd();
  char advance();
  char peek();
  char peekNext();
  std::string getline();
  std::string getline(int n);
  void addToken(Token::Type type, std::string_view lexeme);
  void scanToken();

public:
  std::vector<Token> tokens;

  Lexer(std::string_view filename, std::string_view source);

  void scanTokens();
};

#endif // LEXER_HPP