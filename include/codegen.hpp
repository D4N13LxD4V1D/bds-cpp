#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <expr.hpp>
#include <stmt.hpp>

#include <memory>
#include <variant>
#include <vector>

#include <llvm/IR/Value.h>

struct Compiler {
  auto compile(std::vector<std::unique_ptr<Stmt>> statements) -> void;

  auto codegen(const Expr &expr) -> llvm::Value *;
  auto codegen(const Stmt &stmt) -> llvm::Value *;

  auto codegen(const Expr::Assign &expr) -> llvm::Value *;
  auto codegen(const Expr::Binary &expr) -> llvm::Value *;
  auto codegen(const Expr::Call &expr) -> llvm::Value *;
  auto codegen(const Expr::Get &expr) -> llvm::Value *;
  auto codegen(const Expr::Grouping &expr) -> llvm::Value *;
  auto codegen(const Expr::Literal &expr) -> llvm::Value *;
  auto codegen(const Expr::Logical &expr) -> llvm::Value *;
  auto codegen(const Expr::Set &expr) -> llvm::Value *;
  auto codegen(const Expr::This &expr) -> llvm::Value *;
  auto codegen(const Expr::Unary &expr) -> llvm::Value *;
  auto codegen(const Expr::Variable &expr) -> llvm::Value *;

  auto codegen(const Stmt::Block &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Break &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Expression &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Function &stmt) -> llvm::Value *;
  auto codegen(const Stmt::If &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Print &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Return &stmt) -> llvm::Value *;
  auto codegen(const Stmt::Var &stmt) -> llvm::Value *;
  auto codegen(const Stmt::While &stmt) -> llvm::Value *;
};

#endif // CODEGEN_HPP