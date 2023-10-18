#include <error.hpp>
#include <parser.hpp>
#include <printer.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <variant>

Parser::Parser(std::string_view filename, std::vector<Token> tokens)
    : filename(filename), tokens(tokens) {}

void Parser::parseTokens() {
  while (!isAtEnd()) {
    statements.push_back(declaration());
  }
}

bool Parser::isAtEnd() { return peek().type == Token::Type::END; }

Token Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

Token Parser::peek() { return tokens[current]; }

Token Parser::peekNext() {
  if (current + 1 >= tokens.size())
    return tokens[tokens.size() - 1];
  return tokens[current + 1];
}

Token Parser::previous() { return tokens[current - 1]; }

bool Parser::check(Token::Type type) {
  if (isAtEnd())
    return false;
  return peek().type == type;
}

bool Parser::match(std::initializer_list<Token::Type> types) {
  for (auto type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}

Token Parser::consume(Token::Type type, std::string args[]) {
  if (check(type))
    return advance();

  error(Error::UnexpectedToken, args);

  __builtin_unreachable();
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == Token::Type::SEMICOLON)
      return;

    switch (peek().type) {
    case Token::Type::FN:
    case Token::Type::LET:
    case Token::Type::FOR:
    case Token::Type::IF:
    case Token::Type::WHILE:
    case Token::Type::PRINT:
    case Token::Type::RETURN:
      return;
    default:
      advance();
    }
  }
}

std::unique_ptr<Expr> Parser::expression() { return assignment(); }

std::unique_ptr<Expr> Parser::assignment() {
  auto expr = logicalOr();

  if (match({Token::Type::EQUAL})) {
    auto equals = previous();
    auto value = assignment();

    if (auto *var = expr->get<Expr::Variable>()) {
      Token name = var->name;
      return std::make_unique<Expr>(
          std::move(Expr::Assign(name, std::move(value))));
    } else if (auto *get = expr->get<Expr::Get>()) {
      return std::make_unique<Expr>(std::move(
          Expr::Set(std::move(get->object), get->name, std::move(value))));
    }

    std::string args[] = {};
    error(Error::UnexpectedToken, args);
  }

  return expr;
}

std::unique_ptr<Expr> Parser::logicalOr() {
  auto expr = logicalAnd();

  while (match({Token::Type::OR})) {
    auto op = previous();
    auto right = logicalAnd();
    expr = std::make_unique<Expr>(std::move(
        Expr::Logical(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
  auto expr = equality();

  while (match({Token::Type::AND})) {
    auto op = previous();
    auto right = equality();
    expr = std::make_unique<Expr>(std::move(
        Expr::Logical(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::equality() {
  auto expr = comparison();

  while (match({Token::Type::BANG_EQUAL, Token::Type::EQUAL_EQUAL})) {
    auto op = previous();
    auto right = comparison();
    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
  auto expr = addition();

  while (match({Token::Type::GREATER, Token::Type::GREATER_EQUAL,
                Token::Type::LESS, Token::Type::LESS_EQUAL})) {
    auto op = previous();
    auto right = addition();
    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::addition() {
  auto expr = multiplication();

  while (match({Token::Type::MINUS, Token::Type::PLUS})) {
    auto op = previous();
    auto right = multiplication();
    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::multiplication() {
  auto expr = unary();

  while (match({Token::Type::SLASH, Token::Type::STAR, Token::Type::MODULO})) {
    auto op = previous();
    auto right = unary();
    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(expr), std::move(op), std::move(right))));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::unary() {
  if (match({Token::Type::BANG, Token::Type::MINUS})) {
    auto op = previous();
    auto right = unary();
    return std::make_unique<Expr>(
        std::move(Expr::Unary(std::move(op), std::move(right))));
  }

  return call();
}

std::unique_ptr<Expr> Parser::call() {
  auto expr = primary();

  while (true) {
    if (match({Token::Type::LEFT_PAREN})) {
      expr = finishCall(std::move(expr));
    } else {
      break;
    }
  }

  return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee) {
  std::vector<std::unique_ptr<Expr>> arguments;

  if (!check(Token::Type::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        std::string args[] = {};
        error(Error::TooManyArguments, args);
      }
      arguments.push_back(expression());
    } while (match({Token::Type::COMMA}));
  }

  auto paren = consume(Token::Type::RIGHT_PAREN, {});

  return std::make_unique<Expr>(std::move(
      Expr::Call(std::move(callee), std::move(paren), std::move(arguments))));
}

std::unique_ptr<Expr> Parser::primary() {
  if (match({Token::Type::FALSE}))
    return std::make_unique<Expr>(std::move(Expr::Literal(previous())));
  if (match({Token::Type::TRUE}))
    return std::make_unique<Expr>(std::move(Expr::Literal(previous())));
  //   if (match({Token::Type::NIL}))
  //     return std::make_unique<Expr>(std::move(Expr::Literal(previous())));

  if (match({Token::Type::NUMBER, Token::Type::STRING}))
    return std::make_unique<Expr>(std::move(Expr::Literal(previous())));

  if (match({Token::Type::IDENTIFIER}))
    return std::make_unique<Expr>(std::move(Expr::Variable(previous())));

  if (match({Token::Type::LEFT_PAREN})) {
    auto expr = expression();
    consume(Token::Type::RIGHT_PAREN, {});
    return std::make_unique<Expr>(std::move(Expr::Grouping(std::move(expr))));
  }

  std::string args[] = {};
  error(Error::UnexpectedToken, args);

  __builtin_unreachable();
}

std::unique_ptr<Stmt> Parser::declaration() {
  //   try {
  if (match({Token::Type::FN}))
    return function("function");
  if (match({Token::Type::LET}))
    return varDeclaration();
  if (match({Token::Type::FOR}))
    return forStatement();
  if (match({Token::Type::IF}))
    return ifStatement();
  if (match({Token::Type::WHILE}))
    return whileStatement();
  if (match({Token::Type::PRINT}))
    return printStatement();
  if (match({Token::Type::LEFT_BRACE}))
    return block();

  return expressionStatement();
  //   } catch (ParseError &error) {
  //     synchronize();
  //     return nullptr;
  //   }
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
  auto name = consume(Token::Type::IDENTIFIER, {});
  std::unique_ptr<Expr> initializer = nullptr;

  if (match({Token::Type::EQUAL}))
    initializer = expression();

  consume(Token::Type::SEMICOLON, {});

  return std::make_unique<Stmt>(
      std::move(Stmt::Var(std::move(name), std::move(initializer))));
}

std::unique_ptr<Stmt> Parser::statement() {
  if (match({Token::Type::PRINT}))
    return printStatement();
  if (match({Token::Type::LEFT_BRACE}))
    return block();
  if (match({Token::Type::IF}))
    return ifStatement();
  if (match({Token::Type::WHILE}))
    return whileStatement();
  if (match({Token::Type::FOR}))
    return forStatement();
  if (match({Token::Type::BREAK}))
    return breakStatement();
  if (match({Token::Type::RETURN}))
    return returnStatement();

  return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement() {
  auto value = expression();
  consume(Token::Type::SEMICOLON, {});
  return std::make_unique<Stmt>(std::move(Stmt::Print(std::move(value))));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
  auto expr = expression();
  consume(Token::Type::SEMICOLON, {});
  return std::make_unique<Stmt>(std::move(Stmt::Expression(std::move(expr))));
}

std::unique_ptr<Stmt> Parser::block() {
  std::vector<std::unique_ptr<Stmt>> statements;

  while (!check(Token::Type::RIGHT_BRACE) && !isAtEnd()) {
    statements.push_back(declaration());
  }

  consume(Token::Type::RIGHT_BRACE, {});

  return std::make_unique<Stmt>(std::move(Stmt::Block(std::move(statements))));
}

std::unique_ptr<Stmt> Parser::ifStatement() {
  consume(Token::Type::LEFT_PAREN, {});
  auto condition = expression();
  consume(Token::Type::RIGHT_PAREN, {});

  auto thenBranch = statement();
  std::unique_ptr<Stmt> elseBranch = nullptr;

  if (match({Token::Type::ELSE}))
    elseBranch = statement();

  return std::make_unique<Stmt>(std::move(Stmt::If(
      std::move(condition), std::move(thenBranch), std::move(elseBranch))));
}

std::unique_ptr<Stmt> Parser::whileStatement() {
  consume(Token::Type::LEFT_PAREN, {});
  auto condition = expression();
  consume(Token::Type::RIGHT_PAREN, {});

  auto body = statement();

  return std::make_unique<Stmt>(
      std::move(Stmt::While(std::move(condition), std::move(body))));
}

std::unique_ptr<Stmt> Parser::forStatement() { return nullptr; }

std::unique_ptr<Stmt> Parser::breakStatement() { return nullptr; }

std::unique_ptr<Stmt> Parser::function(std::string kind) { return nullptr; }

std::unique_ptr<Stmt> Parser::returnStatement() { return nullptr; }