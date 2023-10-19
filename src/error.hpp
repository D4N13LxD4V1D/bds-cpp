#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <token.hpp>

enum class Error {
  UnterminatedString,
  UnterminatedComment,
  UnexpectedCharacter,
  UnexpectedToken,
  InvalidAssignment,
  TooManyArguments,
  TooManyParameters,
};

auto error(Error type, const std::string args[]) -> void;
auto error(Error type, Token token, std::vector<std::string> args) -> void;

#endif // ERROR_HPP