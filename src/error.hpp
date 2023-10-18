#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

enum class Error {
  UnterminatedString,
  UnterminatedComment,
  UnexpectedCharacter,
  UnexpectedToken,
  InvalidAssignmentTarget,
  TooManyArguments,
};

auto error(Error type, const std::string args[]) -> void;

#endif // ERROR_HPP