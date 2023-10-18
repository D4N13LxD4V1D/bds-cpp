#include <printer.hpp>

#include <format>
#include <iostream>

auto Printer::print(std::vector<std::unique_ptr<Stmt>> statements) -> void {
  for (const auto &stmt : statements) {
    stmt->accept(
        [this](const auto &s) { std::cout << to_string(s) << std::endl; });
  }
}

auto Printer::to_string(const Expr &expr) -> std::string {
  return expr.accept([this](const auto &e) { return to_string(e); });
}

auto Printer::to_string(const Stmt &stmt) -> std::string {
  return stmt.accept([this](const auto &s) { return to_string(s); });
}

auto Printer::to_string(const Expr::Assign &expr) -> std::string {
  return std::format("(assign {} {})", expr.name.lexeme,
                     to_string(*expr.value));
}

auto Printer::to_string(const Expr::Binary &expr) -> std::string {
  return std::format("(binary {} {} {})", expr.op.lexeme, to_string(*expr.left),
                     to_string(*expr.right));
}

auto Printer::to_string(const Expr::Call &expr) -> std::string {
  std::string args;
  for (const auto &arg : expr.arguments)
    args += to_string(*arg) + " ";

  return std::format("(call {} {} {})", to_string(*expr.callee),
                     expr.paren.lexeme, args);
}

auto Printer::to_string(const Expr::Get &expr) -> std::string {
  return std::format("(get {} {})", to_string(*expr.object), expr.name.lexeme);
}

auto Printer::to_string(const Expr::Grouping &expr) -> std::string {
  return std::format("(grouping {})", to_string(*expr.expression));
}

auto Printer::to_string(const Expr::Literal &expr) -> std::string {
  return std::format("(literal {})", expr.value.lexeme);
}

auto Printer::to_string(const Expr::Logical &expr) -> std::string {
  return std::format("(logical {} {} {})", expr.op.lexeme,
                     to_string(*expr.left), to_string(*expr.right));
}

auto Printer::to_string(const Expr::Set &expr) -> std::string {
  return std::format("(set {} {} {})", to_string(*expr.object),
                     expr.name.lexeme, to_string(*expr.value));
}

auto Printer::to_string(const Expr::This &expr) -> std::string {
  return std::format("(this {})", expr.keyword.lexeme);
}

auto Printer::to_string(const Expr::Unary &expr) -> std::string {
  return std::format("(unary {} {})", expr.op.lexeme, to_string(*expr.right));
}

auto Printer::to_string(const Expr::Variable &expr) -> std::string {
  return std::format("(variable {})", expr.name.lexeme);
}

auto Printer::to_string(const Stmt::Block &stmt) -> std::string {
  std::string statements;
  for (const auto &statement : stmt.statements)
    statements += to_string(*statement) + " ";

  return std::format("(block {})", statements);
}

auto Printer::to_string(const Stmt::Break &stmt) -> std::string {
  return std::format("(break {})", stmt.keyword.lexeme);
}

auto Printer::to_string(const Stmt::Expression &stmt) -> std::string {
  return std::format("(expression {})", to_string(*stmt.expression));
}

auto Printer::to_string(const Stmt::Function &stmt) -> std::string {
  std::string params;
  for (const auto &param : stmt.params)
    params += param.lexeme + " ";

  std::string body;
  for (const auto &statement : stmt.body)
    body += to_string(*statement) + " ";

  return std::format("(function {} {} {})", stmt.name.lexeme, params, body);
}

auto Printer::to_string(const Stmt::If &stmt) -> std::string {
  return std::format("(if {} {} {})", to_string(*stmt.condition),
                     to_string(*stmt.thenBranch), to_string(*stmt.elseBranch));
}

auto Printer::to_string(const Stmt::Print &stmt) -> std::string {
  return std::format("(print {})", to_string(*stmt.expression));
}

auto Printer::to_string(const Stmt::Return &stmt) -> std::string {
  return std::format("(return {} {})", stmt.keyword.lexeme,
                     to_string(*stmt.value));
}

auto Printer::to_string(const Stmt::Var &stmt) -> std::string {
  return std::format("(var {} {})", stmt.name.lexeme,
                     to_string(*stmt.initializer));
}

auto Printer::to_string(const Stmt::While &stmt) -> std::string {
  return std::format("(while {} {})", to_string(*stmt.condition),
                     to_string(*stmt.body));
}
