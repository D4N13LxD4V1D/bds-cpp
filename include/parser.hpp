#ifndef PARSER_HPP
#define PARSER_HPP

#include <expr.hpp>
#include <stmt.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Parser {
  std::string_view filename;
  std::vector<Token> tokens;

  int current = 0;

  bool isAtEnd();
  Token advance();
  Token peek();
  Token peekNext();
  Token previous();
  bool check(Token::Type type);
  bool match(std::initializer_list<Token::Type> types);
  Token consume(Token::Type type, std::string args[]);
  void synchronize();

  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> assignment();
  std::unique_ptr<Expr> logicalOr();
  std::unique_ptr<Expr> logicalAnd();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> addition();
  std::unique_ptr<Expr> multiplication();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> call();
  std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);
  std::unique_ptr<Expr> primary();

  std::unique_ptr<Stmt> declaration();
  std::unique_ptr<Stmt> varDeclaration();
  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> printStatement();
  std::unique_ptr<Stmt> expressionStatement();
  std::unique_ptr<Stmt> block();
  std::unique_ptr<Stmt> ifStatement();
  std::unique_ptr<Stmt> whileStatement();
  std::unique_ptr<Stmt> forStatement();
  std::unique_ptr<Stmt> breakStatement();
  std::unique_ptr<Stmt> function(std::string kind);
  std::unique_ptr<Stmt> returnStatement();

public:
  std::vector<std::unique_ptr<Stmt>> statements;

  Parser(std::string_view filename, std::vector<Token> tokens);

  void parseTokens();
};

#endif // PARSER_HPP