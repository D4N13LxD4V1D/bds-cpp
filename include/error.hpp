#ifndef ERROR_HPP
#define ERROR_HPP

#include <token.hpp>

#include <string>
#include <vector>

struct Error {
  enum Type {
    UnterminatedString,
    UnterminatedComment,
    UnexpectedCharacter,
    UnexpectedToken,
    InvalidAssignment,
    TooManyArguments,
    TooManyParameters,
  } type;
  Token token;
  std::vector<std::string> args;

  auto print() -> void;
};

#endif // ERROR_HPP