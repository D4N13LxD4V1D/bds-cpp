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

void error(Error type, const std::string args[]);

#endif // ERROR_HPP