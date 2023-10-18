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
  std::vector<Token> tokens;

  int start = 0;
  int current = 0;
  int row = 1;
  int column = 1;

  auto isAtEnd() -> bool;
  auto advance() -> char;
  auto peek() -> char;
  auto peekNext() -> char;
  auto getline() -> std::string;
  auto getline(int n) -> std::string;
  auto addToken(Token::Type type, std::string_view lexeme) -> void;
  auto scanToken() -> void;

public:

  Lexer(std::string_view filename, std::string_view source);

  auto scanTokens() -> std::vector<Token>;
};

#endif // LEXER_HPP