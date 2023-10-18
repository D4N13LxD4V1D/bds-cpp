#include <error.hpp>

#include <algorithm>
#include <format>
#include <iostream>

void error(Error type, const std::string args[]) {
  std::cout << "\033[1;31mError:\033[0m ";
  switch (type) {
  case Error::UnterminatedString:
    std::cout << std::format("Unterminated string at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    std::cout << std::format("{:>4} |    {}", args[1], args[3]) << std::endl;
    for (int i = 0; i < std::format("{:>4} |    ", args[1]).size(); i++)
      std::cout << " ";
    for (int i = 0; i < std::stoi(args[2]) - 2; i++)
      std::cout << " ";
    std::cout << "\033[1;31m^\033[0m" << std::endl;
    break;
  case Error::UnterminatedComment:
    std::cout << std::format("Unterminated comment at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    for (int i = 0, line = std::stoi(args[1]); i < args[3].size(); i++) {
      std::cout << std::format("{:>4} |    {}", line++, args[3][i]);
      while (args[3][i] != '\n' && i < args[3].size()) {
        i++;
        std::cout << args[3][i];
      }
    }
    std::cout << std::endl;
    for (int i = 0; i < std::format("{:>4} |    ", args[1]).size(); i++)
      std::cout << " ";
    for (int i = 0; i < std::stoi(args[2]) - 2; i++)
      std::cout << " ";
    std::cout << "\033[1;31m^^\033[0m" << std::endl;
    break;
  case Error::UnexpectedCharacter:
    std::cout << std::format("Unexpected character at {}:{}:{}!", args[0],
                             args[1], args[2])
              << std::endl;
    std::cout << std::format("{:>4} |    {}", args[1], args[3]) << std::endl;
    for (int i = 0; i < std::format("{:>4} |    ", args[1]).size(); i++)
      std::cout << " ";
    for (int i = 0; i < std::stoi(args[2]) - 2; i++)
      std::cout << " ";
    std::cout << "\033[1;31m^\033[0m" << std::endl;
    break;
  default:
    std::cout << "Unknown error!" << std::endl;
  }
  exit(1);
}