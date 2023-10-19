#include <error.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <vector>

auto print(std::string_view source, int row, int column, int pointerLength)
    -> void {
  std::cout << std::format("{:>4} |    {}", row, source) << std::endl;
  for (int i = 0; i < std::format("{:>4} |    ", row).size(); i++)
    std::cout << " ";
  for (int i = 0; i < column - 2; i++)
    std::cout << " ";
  std::cout << "\033[1;31m";
  for (int i = 0; i < pointerLength; i++)
    std::cout << "^";
  std::cout << "\033[0m" << std::endl;
}

auto error(Error type, Token token, std::vector<std::string> args) -> void {
  std::string newArgs[] = {
      token.location.filename, std::to_string(token.location.row),
      std::to_string(token.location.column), token.location.source};
  std::copy(std::begin(newArgs), std::end(newArgs), std::begin(args));
  error(type, args.data());
}

auto error(Error type, const std::string args[]) -> void {
  std::cout << "\033[1;31mError:\033[0m ";
  switch (type) {
  case Error::UnterminatedString:
    std::cout << std::format("Unterminated string at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 1);
    break;
  case Error::UnterminatedComment:
    std::cout << std::format("Unterminated comment at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 2);
    break;
  case Error::UnexpectedCharacter:
    std::cout << std::format("Unexpected character at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 1);
    break;
  case Error::UnexpectedToken:
    std::cout << std::format("Unexpected token! Expected {} at {}:{}:{}!",
                             args[4], args[0], args[1], args[2])
              << std::endl;
    print(args[4], std::stoi(args[1]), std::stoi(args[2]), 1);
  case Error::InvalidAssignment:
    std::cout << std::format("Invalid assignment at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 1);
    break;
  case Error::TooManyArguments:
    std::cout << std::format("Too many arguments at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 1);
    break;
  case Error::TooManyParameters:
    std::cout << std::format("Too many parameters at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    print(args[3], std::stoi(args[1]), std::stoi(args[2]), 1);
    break;
  }
}