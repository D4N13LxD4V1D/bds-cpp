#ifndef STMT_HPP
#define STMT_HPP

#include <expr.hpp>
#include <token.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct Stmt {

  struct Block {
    std::vector<std::unique_ptr<Stmt>> statements;

    Block(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
  };

  struct Break {
    Token keyword;

    Break(Token keyword) : keyword(std::move(keyword)) {}
  };

  struct Expression {
    std::unique_ptr<Expr> expression;

    Expression(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
  };

  struct Function {
    Token name;
    std::vector<Token> params;
    std::unique_ptr<Stmt> body;

    Function(Token name, std::vector<Token> params, std::unique_ptr<Stmt> body)
        : name(std::move(name)), params(std::move(params)),
          body(std::move(body)) {}
  };

  struct If {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
       std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
  };

  struct Print {
    std::unique_ptr<Expr> expression;

    Print(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
  };

  struct Return {
    Token keyword;
    std::unique_ptr<Expr> value;

    Return(Token keyword, std::unique_ptr<Expr> value)
        : keyword(std::move(keyword)), value(std::move(value)) {}
  };

  struct Var {
    Token name;
    std::unique_ptr<Expr> initializer;

    Var(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
  };

  struct While {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
  };

  std::variant<Block, Break, Expression, Function, If, Print, Return, Var,
               While>
      stmt;

  template <typename T> Stmt(T &&stmt) : stmt(std::forward<T>(stmt)) {}

  auto accept(const auto &visitor) const
      -> decltype(std::visit(visitor, stmt)) {
    return std::visit(visitor, stmt);
  }
};

#endif // STMT_HPP
