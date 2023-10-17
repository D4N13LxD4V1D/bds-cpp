#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <expr.hpp>
#include <stmt.hpp>

#include <iostream>
#include <variant>

struct Printer {
  std::string print(const Expr::Assign &expr) { return ""; }
  std::string print(const Expr::Binary &expr) { return ""; }
  std::string print(const Expr::Call &expr) { return ""; }
  std::string print(const Expr::Get &expr) { return ""; }
  std::string print(const Expr::Grouping &expr) { return ""; }
  std::string print(const Expr::Literal &expr) { return ""; }
  std::string print(const Expr::Logical &expr) { return ""; }
  std::string print(const Expr::Set &expr) { return ""; }
  std::string print(const Expr::This &expr) { return ""; }
  std::string print(const Expr::Unary &expr) { return ""; }
  std::string print(const Expr::Variable &expr) { return ""; }

  std::string print(const Stmt::Block &stmt) { return ""; }
  std::string print(const Stmt::Break &stmt) { return "this is a break"; }
  std::string print(const Stmt::Expression &stmt) { return ""; }
  std::string print(const Stmt::Function &stmt) { return ""; }
  std::string print(const Stmt::If &stmt) { return ""; }
  std::string print(const Stmt::Print &stmt) { return ""; }
  std::string print(const Stmt::Return &stmt) { return ""; }
  std::string print(const Stmt::Var &stmt) { return ""; }
  std::string print(const Stmt::While &stmt) { return ""; }

  void print(std::vector<Stmt> statements) {
    for (const auto &stmt : statements) {
      stmt.accept(
          [this](const auto &s) { std::cout << print(s) << std::endl; });
    }
  }
};

#endif // PRINTER_HPP