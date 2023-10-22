#ifndef ERROR_HPP
#define ERROR_HPP

#include <token.hpp>

#include <string>
#include <vector>

enum class Error {
  UnterminatedString,
  UnterminatedComment,
  UnexpectedCharacter,
  UnexpectedToken,
  InvalidAssignment,
  TooManyArguments,
  TooManyParameters,
};

struct ParseError {
  Error type;
  Token token;
  std::vector<std::string> args;
};

auto error(ParseError e) -> void;

#endif // ERROR_HPP