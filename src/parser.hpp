#ifndef PARSER_HPP
#define PARSER_HPP

#include <error.hpp>
#include <expr.hpp>
#include <stmt.hpp>

#include <expected>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Parser {
  std::string filename;
  std::vector<Token> tokens;
  std::vector<std::unique_ptr<Stmt>> statements;

  int current = 0;

  auto isAtEnd() -> bool;
  auto advance() -> Token;
  auto peek() -> Token;
  auto peekNext() -> Token;
  auto previous() -> Token;
  auto check(Token::Type type) -> bool;
  auto match(std::initializer_list<Token::Type> types) -> bool;
  auto consume(Token::Type type) -> std::expected<Token, ParseError>;
  auto synchronize() -> void;

  auto expression() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto assignment() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto logicalOr() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto logicalAnd() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto equality() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto comparison() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto addition() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto multiplication() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto unary() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto call() -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto finishCall(std::unique_ptr<Expr> callee)
      -> std::expected<std::unique_ptr<Expr>, ParseError>;
  auto primary() -> std::expected<std::unique_ptr<Expr>, ParseError>;

  auto declaration() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto varDeclaration() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto statement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto printStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto expressionStatement()
      -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto block() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto ifStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto whileStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto forStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto breakStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto function(std::string kind)
      -> std::expected<std::unique_ptr<Stmt>, ParseError>;
  auto returnStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError>;

public:
  Parser(std::string_view filename, std::vector<Token> tokens);

  auto parseTokens()
      -> std::expected<std::vector<std::unique_ptr<Stmt>>, ParseError>;
};

#endif // PARSER_HPP