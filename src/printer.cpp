#include <expr.hpp>
#include <printer.hpp>

#include <iostream>
#include <string>

void print(std::vector<Stmt> statements) {
  Printer printer;
  for (auto &statement : statements) {
    std::cout << statement.accept(printer) << std::endl;
  }
}

std::string Printer::visit(Expr::Assign &expr) { return ""; }
std::string Printer::visit(Expr::Binary &expr) { return ""; }
std::string Printer::visit(Expr::Call &expr) { return ""; }
std::string Printer::visit(Expr::Get &expr) { return ""; }
std::string Printer::visit(Expr::Grouping &expr) { return ""; }
std::string Printer::visit(Expr::Literal &expr) { return ""; }
std::string Printer::visit(Expr::Logical &expr) { return ""; }
std::string Printer::visit(Expr::Set &expr) { return ""; }
std::string Printer::visit(Expr::This &expr) { return ""; }
std::string Printer::visit(Expr::Unary &expr) { return ""; }
std::string Printer::visit(Expr::Variable &expr) { return ""; }

std::string Printer::visit(Stmt::Block &stmt) { return ""; }
std::string Printer::visit(Stmt::Break &stmt) { return ""; }
std::string Printer::visit(Stmt::Expression &stmt) { return ""; }
std::string Printer::visit(Stmt::Function &stmt) { return ""; }
std::string Printer::visit(Stmt::If &stmt) { return ""; }
std::string Printer::visit(Stmt::Print &stmt) { return ""; }
std::string Printer::visit(Stmt::Return &stmt) { return ""; }
std::string Printer::visit(Stmt::Var &stmt) { return ""; }
std::string Printer::visit(Stmt::While &stmt) { return ""; }