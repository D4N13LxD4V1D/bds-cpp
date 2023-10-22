#include <error.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <vector>

const std::map<Error, std::string> errorMessages = {
    {Error::UnterminatedString, "Unterminated string"},
    {Error::UnterminatedComment, "Unterminated comment"},
    {Error::UnexpectedCharacter, "Unexpected character"},
    {Error::UnexpectedToken, "Unexpected token"},
    {Error::InvalidAssignment, "Invalid assignment"},
    {Error::TooManyArguments, "Too many arguments"},
    {Error::TooManyParameters, "Too many parameters"},
};

auto error(ParseError e) -> void {
  auto [type, token, args] = e;
  auto [filename, source, row, column] = token.location;
  std::cout << "\033[1;31mError:\033[0m " << errorMessages.at(type) << " at "
            << filename << ":" << row << ":" << column << "!";

  for (auto arg : args)
    std::cout << " " << arg;

  std::cout << std::endl;

  int n = printf("%4d ", row);
  printf("|    %s\n%*s|%4s", source.data(), n, "", "");
  n = column - token.lexeme.length() - 1;
  printf("%*s\033[1;31m^", n, "");
  n = token.lexeme.length() - 1;
  printf("%s\033[0m\n", n > 0 ? std::string(n, '~').data() : "");
}