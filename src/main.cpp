#include <lexer.hpp>
#include <parser.hpp>
#include <printer.hpp>

#include <fstream>
#include <iostream>

auto main(int argc, const char *argv[]) -> int {
  if (argc == 2) {
    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cout << "Could not open file " << filename << std::endl;
      return 1;
    }

    std::string source{std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>()};

    Lexer lexer(filename, source);
    auto tokens = lexer.scanTokens();
    if (!tokens) {
      tokens.error().print();
      return 1;
    }

    Parser parser(filename, *tokens);
    auto statements = parser.parseTokens();
    if (!statements) {
      statements.error().print();
      return 1;
    }

    Printer printer;
    printer.print(std::move(*statements));
  } else {
    std::cout << "Usage: bds [script]" << std::endl;
    return 1;
  }

  return 0;
}