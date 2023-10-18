#include <codegen.hpp>

#include <iostream>

auto Compiler::compile(std::vector<std::unique_ptr<Stmt>> statements) -> void {
  for (const auto &stmt : statements) {
    stmt->accept([this](const auto &s) { codegen(s); });
  }
}

auto Compiler::codegen(const Expr &expr) -> llvm::Value * {
  return expr.accept([this](const auto &e) { return codegen(e); });
}

auto Compiler::codegen(const Stmt &stmt) -> llvm::Value * {
  return stmt.accept([this](const auto &s) { return codegen(s); });
}

auto Compiler::codegen(const Expr::Assign &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Binary &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Call &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Get &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Grouping &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Literal &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Logical &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Set &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::This &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Unary &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Expr::Variable &expr) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Block &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Break &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Expression &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Function &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::If &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Print &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Return &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::Var &stmt) -> llvm::Value * {
  return nullptr;
}

auto Compiler::codegen(const Stmt::While &stmt) -> llvm::Value * {
  return nullptr;
}
