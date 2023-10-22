#ifndef LEXER_HPP
#define LEXER_HPP

#include <error.hpp>
#include <token.hpp>

#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class Lexer {
  std::string filename;
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
  auto newToken(Token::Type type, std::string_view lexeme) -> Token;
  auto scanToken() -> std::expected<std::optional<Token>, ParseError>;

public:
  Lexer(std::string_view filename, std::string_view source);

  auto scanTokens() -> std::expected<std::vector<Token>, ParseError>;
};

#endif // LEXER_HPP