#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

enum class Error {
  UnterminatedString,
  UnterminatedComment,
  UnexpectedCharacter,
};

void error(Error type, const std::string args[]);

#endif // ERROR_HPP