#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <expr.hpp>
#include <stmt.hpp>

#include <memory>
#include <vector>

struct Printer {
  auto print(std::vector<std::unique_ptr<Stmt>> statements) -> void;

  auto to_string(const Expr &expr) -> std::string;
  auto to_string(const Stmt &stmt) -> std::string;

  auto to_string(const Expr::Assign &expr) -> std::string;
  auto to_string(const Expr::Binary &expr) -> std::string;
  auto to_string(const Expr::Call &expr) -> std::string;
  auto to_string(const Expr::Get &expr) -> std::string;
  auto to_string(const Expr::Grouping &expr) -> std::string;
  auto to_string(const Expr::Literal &expr) -> std::string;
  auto to_string(const Expr::Logical &expr) -> std::string;
  auto to_string(const Expr::Set &expr) -> std::string;
  auto to_string(const Expr::This &expr) -> std::string;
  auto to_string(const Expr::Unary &expr) -> std::string;
  auto to_string(const Expr::Variable &expr) -> std::string;

  auto to_string(const Stmt::Block &stmt) -> std::string;
  auto to_string(const Stmt::Break &stmt) -> std::string;
  auto to_string(const Stmt::Expression &stmt) -> std::string;
  auto to_string(const Stmt::Function &stmt) -> std::string;
  auto to_string(const Stmt::If &stmt) -> std::string;
  auto to_string(const Stmt::Print &stmt) -> std::string;
  auto to_string(const Stmt::Return &stmt) -> std::string;
  auto to_string(const Stmt::Var &stmt) -> std::string;
  auto to_string(const Stmt::While &stmt) -> std::string;
};

#endif // PRINTER_HPP