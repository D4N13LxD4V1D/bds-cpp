#ifndef PARSER_HPP
#define PARSER_HPP

#include <error.hpp>
#include <expr.hpp>
#include <stmt.hpp>

#include <expected>
#include <memory>
#include <string>
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
  auto consume(Token::Type type) -> std::expected<Token, Error>;
  auto synchronize() -> void;

  auto expression() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto assignment() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto logicalOr() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto logicalAnd() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto equality() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto comparison() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto addition() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto multiplication() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto unary() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto call() -> std::expected<std::unique_ptr<Expr>, Error>;
  auto finishCall(std::unique_ptr<Expr> callee)
      -> std::expected<std::unique_ptr<Expr>, Error>;
  auto primary() -> std::expected<std::unique_ptr<Expr>, Error>;

  auto declaration() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto varDeclaration() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto statement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto printStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto expressionStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto block() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto ifStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto whileStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto forStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto breakStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto function(std::string kind)
      -> std::expected<std::unique_ptr<Stmt>, Error>;
  auto returnStatement() -> std::expected<std::unique_ptr<Stmt>, Error>;

public:
  Parser(std::string_view filename, std::vector<Token> tokens);

  auto parseTokens()
      -> std::expected<std::vector<std::unique_ptr<Stmt>>, Error>;
};

#endif // PARSER_HPP