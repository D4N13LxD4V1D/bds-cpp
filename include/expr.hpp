#ifndef EXPR_HPP
#define EXPR_HPP

#include <token.hpp>

#include <memory>
#include <variant>
#include <vector>

struct Expr {

  struct Assign {
    Token name;
    std::unique_ptr<Expr> value;

    Assign(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}
  };

  struct Binary {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
  };

  struct Call {
    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;

    Call(std::unique_ptr<Expr> callee, Token paren,
         std::vector<std::unique_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)),
          arguments(std::move(arguments)) {}
  };

  struct Get {
    std::unique_ptr<Expr> object;
    Token name;

    Get(std::unique_ptr<Expr> object, Token name)
        : object(std::move(object)), name(std::move(name)) {}
  };

  struct Grouping {
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
  };

  struct Literal {
    Token value;

    Literal(Token value) : value(std::move(value)) {}
  };

  struct Logical {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
  };

  struct Set {
    std::unique_ptr<Expr> object;
    Token name;
    std::unique_ptr<Expr> value;

    Set(std::unique_ptr<Expr> object, Token name, std::unique_ptr<Expr> value)
        : object(std::move(object)), name(std::move(name)),
          value(std::move(value)) {}
  };

  struct This {
    Token keyword;

    This(Token keyword) : keyword(std::move(keyword)) {}
  };

  struct Unary {
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}
  };

  struct Variable {
    Token name;

    Variable(Token name) : name(std::move(name)) {}
  };

  std::variant<Assign, Binary, Call, Get, Grouping, Literal, Logical, Set, This,
               Unary, Variable>
      expr;

  Expr(Assign expr) : expr(std::move(expr)) {}
  Expr(Binary expr) : expr(std::move(expr)) {}
  Expr(Call expr) : expr(std::move(expr)) {}
  Expr(Get expr) : expr(std::move(expr)) {}
  Expr(Grouping expr) : expr(std::move(expr)) {}
  Expr(Literal expr) : expr(std::move(expr)) {}
  Expr(Logical expr) : expr(std::move(expr)) {}
  Expr(Set expr) : expr(std::move(expr)) {}
  Expr(This expr) : expr(std::move(expr)) {}
  Expr(Unary expr) : expr(std::move(expr)) {}
  Expr(Variable expr) : expr(std::move(expr)) {}

  auto accept(const auto &visitor) const
      -> decltype(std::visit(visitor, expr)) {
    return std::visit(visitor, expr);
  }

  template <class T> auto get() -> T * { return &std::get<T>(expr); }
};

#endif // EXPR_HPP