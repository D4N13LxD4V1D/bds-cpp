#include <error.hpp>

#include <map>

const std::map<Error::Type, std::string> errorMessages = {
    {Error::UnterminatedString, "Unterminated string"},
    {Error::UnterminatedComment, "Unterminated comment"},
    {Error::UnexpectedCharacter, "Unexpected character"},
    {Error::UnexpectedToken, "Unexpected token"},
    {Error::InvalidAssignment, "Invalid assignment"},
    {Error::TooManyArguments, "Too many arguments"},
    {Error::TooManyParameters, "Too many parameters"},
};

auto Error::print() -> void {
  auto [filename, source, row, column] = token.location;
  printf("\033[1;31mError:\033[0m %s at %s:%d:%d!",
         errorMessages.at(type).data(), filename.data(), row, column);

  for (auto arg : args)
    printf(" %s", arg.data());

  printf("\n");

  int n = printf("%4d ", row);
  printf("|    %s\n%*s|%4s", source.data(), n, "", "");
  n = column - token.lexeme.length() - 1;
  printf("%*s\033[1;31m^", n, "");
  n = token.lexeme.length() - 1;
  printf("%s\033[0m\n", n > 0 ? std::string(n, '~').data() : "");
}