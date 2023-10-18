#include <codegen.hpp>

void Compiler::compile(std::vector<std::unique_ptr<Stmt>> statements) {
  for (const auto &stmt : statements) {
    stmt->accept([this](const auto &s) { codegen(s); });
  }
}

llvm::Value *Compiler::codgen(const Expr::Assign &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Binary &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Call &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Get &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Grouping &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Literal &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Logical &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Set &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::This &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Unary &expr) { return nullptr; }

llvm::Value *Compiler::codgen(const Expr::Variable &expr) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Block &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Break &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Expression &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Function &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::If &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Print &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Return &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::Var &stmt) { return nullptr; }

llvm::Value *Compiler::codegen(const Stmt::While &stmt) { return nullptr; }
