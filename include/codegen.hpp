#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <expr.hpp>
#include <stmt.hpp>

#include <memory>
#include <variant>

#include <llvm/IR/Value.h>

struct Compiler {
  llvm::Value *codgen(const Expr::Assign &expr);
  llvm::Value *codgen(const Expr::Binary &expr);
  llvm::Value *codgen(const Expr::Call &expr);
  llvm::Value *codgen(const Expr::Get &expr);
  llvm::Value *codgen(const Expr::Grouping &expr);
  llvm::Value *codgen(const Expr::Literal &expr);
  llvm::Value *codgen(const Expr::Logical &expr);
  llvm::Value *codgen(const Expr::Set &expr);
  llvm::Value *codgen(const Expr::This &expr);
  llvm::Value *codgen(const Expr::Unary &expr);
  llvm::Value *codgen(const Expr::Variable &expr);

  llvm::Value *codegen(const Stmt::Block &stmt);
  llvm::Value *codegen(const Stmt::Break &stmt);
  llvm::Value *codegen(const Stmt::Expression &stmt);
  llvm::Value *codegen(const Stmt::Function &stmt);
  llvm::Value *codegen(const Stmt::If &stmt);
  llvm::Value *codegen(const Stmt::Print &stmt);
  llvm::Value *codegen(const Stmt::Return &stmt);
  llvm::Value *codegen(const Stmt::Var &stmt);
  llvm::Value *codegen(const Stmt::While &stmt);

  void compile(std::vector<std::unique_ptr<Stmt>> statements);
};

#endif // CODEGEN_HPP