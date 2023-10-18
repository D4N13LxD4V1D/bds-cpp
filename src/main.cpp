#include <lexer.hpp>
#include <parser.hpp>
#include <printer.hpp>

#include <fstream>
#include <iostream>

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    std::cout << "Usage: bds [script]" << std::endl;
    exit(64);
  } else if (argc > 1) {
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
      std::cout << "Could not open file " << argv[1] << std::endl;
      exit(64);
    }

    std::string source{std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>()};

    Lexer lexer(argv[1], source);
    lexer.scanTokens();

    Parser parser(argv[1], lexer.tokens);
    parser.parseTokens();

    Printer printer;
    printer.print(std::move(parser.statements));
  } else {
    std::cout << "Usage: bds [script]" << std::endl;
    exit(64);
  }
}