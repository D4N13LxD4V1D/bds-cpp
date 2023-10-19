#include <error.hpp>
#include <parser.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <variant>

std::map<Token::Type, std::string> expectedTokens = {
    {Token::Type::LEFT_PAREN, "'('"},
    {Token::Type::RIGHT_PAREN, "')'"},
    {Token::Type::LEFT_BRACE, "'{'"},
    {Token::Type::RIGHT_BRACE, "'}'"},
    {Token::Type::COMMA, "','"},
    {Token::Type::DOT, "'.'"},
    {Token::Type::MINUS, "'-'"},
    {Token::Type::PLUS, "'+'"},
    {Token::Type::SEMICOLON, "';'"},
    {Token::Type::SLASH, "'/'"},
    {Token::Type::STAR, "'*'"},
    {Token::Type::BANG, "'!'"},
    {Token::Type::BANG_EQUAL, "'!='"},
    {Token::Type::EQUAL, "'='"},
    {Token::Type::EQUAL_EQUAL, "'=='"},
    {Token::Type::GREATER, "'>'"},
    {Token::Type::GREATER_EQUAL, "'>='"},
    {Token::Type::LESS, "'<'"},
    {Token::Type::LESS_EQUAL, "'<='"},
    {Token::Type::AND, "'and'"},
    {Token::Type::ELSE, "'else'"},
    {Token::Type::FALSE, "'false'"},
    {Token::Type::FOR, "'for'"},
    {Token::Type::FN, "'fn'"},
    {Token::Type::IF, "'if'"},
    {Token::Type::LET, "'let'"},
    {Token::Type::OR, "'or'"},
    {Token::Type::PRINT, "'print'"},
    {Token::Type::RETURN, "'return'"},
    {Token::Type::TRUE, "'true'"},
    {Token::Type::WHILE, "'while'"},
    {Token::Type::IDENTIFIER, "identifier"},
    {Token::Type::STRING, "string"},
    {Token::Type::NUMBER, "number"},
    {Token::Type::END, "end of file"}};

Parser::Parser(std::string_view filename, std::vector<Token> tokens)
    : filename(filename), tokens(tokens) {}

auto Parser::parseTokens() -> std::vector<std::unique_ptr<Stmt>> {
  bool hadError = false;
  while (!isAtEnd()) {
    auto statement = declaration();
    if (!statement) {
      error(statement.error().type, statement.error().token,
            statement.error().args);
      hadError = true;
    }

    statements.push_back(std::move(*statement));
  }

  if (hadError)
    exit(65);

  return std::move(statements);
}

auto Parser::isAtEnd() -> bool { return peek().type == Token::Type::END; }

auto Parser::advance() -> Token {
  if (!isAtEnd())
    current++;
  return previous();
}

auto Parser::peek() -> Token { return tokens[current]; }

auto Parser::peekNext() -> Token {
  if (current + 1 >= tokens.size())
    return tokens[tokens.size() - 1];
  return tokens[current + 1];
}

auto Parser::previous() -> Token { return tokens[current - 1]; }

auto Parser::check(Token::Type type) -> bool {
  if (isAtEnd())
    return false;
  return peek().type == type;
}

auto Parser::match(std::initializer_list<Token::Type> types) -> bool {
  for (auto type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}

auto Parser::consume(Token::Type type) -> std::expected<Token, ParseError> {
  if (check(type))
    return advance();

  return std::unexpected(
      ParseError{Error::UnexpectedToken, peek(), {expectedTokens[type]}});
}

auto Parser::synchronize() -> void {
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

auto Parser::expression() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  return assignment();
}

auto Parser::assignment() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = logicalOr();
  if (!expr)
    return std::unexpected(expr.error());

  if (match({Token::Type::EQUAL})) {
    auto equals = previous();
    auto value = assignment();
    if (!value)
      return std::unexpected(value.error());

    if (auto *var = (*expr)->get<Expr::Variable>()) {
      Token name = var->name;
      return std::make_unique<Expr>(
          std::move(Expr::Assign(name, std::move(*value))));
    } else if (auto *get = (*expr)->get<Expr::Get>()) {
      return std::make_unique<Expr>(std::move(
          Expr::Set(std::move(get->object), get->name, std::move(*value))));
    }

    return std::unexpected(ParseError{Error::InvalidAssignment, peek(), {}});
  }

  return expr;
}

auto Parser::logicalOr() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = logicalAnd();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::OR})) {
    auto op = previous();
    auto right = logicalAnd();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Logical(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::logicalAnd() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = equality();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::AND})) {
    auto op = previous();
    auto right = equality();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Logical(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::equality() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = comparison();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::BANG_EQUAL, Token::Type::EQUAL_EQUAL})) {
    auto op = previous();
    auto right = comparison();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::comparison() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = addition();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::GREATER, Token::Type::GREATER_EQUAL,
                Token::Type::LESS, Token::Type::LESS_EQUAL})) {
    auto op = previous();
    auto right = addition();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::addition() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = multiplication();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::MINUS, Token::Type::PLUS})) {
    auto op = previous();
    auto right = multiplication();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::multiplication()
    -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = unary();
  if (!expr)
    return std::unexpected(expr.error());

  while (match({Token::Type::SLASH, Token::Type::STAR, Token::Type::MODULO})) {
    auto op = previous();
    auto right = unary();
    if (!right)
      return std::unexpected(right.error());

    expr = std::make_unique<Expr>(std::move(
        Expr::Binary(std::move(*expr), std::move(op), std::move(*right))));
  }

  return expr;
}

auto Parser::unary() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  if (match({Token::Type::BANG, Token::Type::MINUS})) {
    auto op = previous();
    auto right = unary();
    if (!right)
      return std::unexpected(right.error());

    return std::make_unique<Expr>(
        std::move(Expr::Unary(std::move(op), std::move(*right))));
  }

  return call();
}

auto Parser::call() -> std::expected<std::unique_ptr<Expr>, ParseError> {
  auto expr = primary();
  if (!expr)
    return std::unexpected(expr.error());

  while (true) {
    if (match({Token::Type::LEFT_PAREN})) {
      expr = finishCall(std::move(*expr));
    } else {
      break;
    }
  }

  return expr;
}

auto Parser::finishCall(std::unique_ptr<Expr> callee)
    -> std::expected<std::unique_ptr<Expr>, ParseError> {
  std::vector<std::unique_ptr<Expr>> arguments;

  if (!check(Token::Type::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255)
        return std::unexpected(ParseError{Error::TooManyArguments, peek(), {}});

      auto expr = expression();
      if (!expr)
        return std::unexpected(expr.error());
      arguments.push_back(std::move(*expr));
    } while (match({Token::Type::COMMA}));
  }

  auto paren = consume(Token::Type::RIGHT_PAREN);
  if (!paren)
    return std::unexpected(paren.error());

  return std::make_unique<Expr>(std::move(
      Expr::Call(std::move(callee), std::move(*paren), std::move(arguments))));
}

auto Parser::primary() -> std::expected<std::unique_ptr<Expr>, ParseError> {
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
    if (!expr)
      return std::unexpected(expr.error());

    auto paren = consume(Token::Type::RIGHT_PAREN);
    if (!paren)
      return std::unexpected(paren.error());

    return std::make_unique<Expr>(std::move(Expr::Grouping(std::move(*expr))));
  }

  return std::unexpected(
      ParseError{Error::UnexpectedToken, peek(), {"primary expression"}});
}

auto Parser::declaration() -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  if (match({Token::Type::FN})) {
    auto fn = function("function");
    if (!fn)
      synchronize();

    return fn;
  }

  if (match({Token::Type::LET})) {
    auto var = varDeclaration();
    if (!var)
      synchronize();

    return var;
  }

  if (match({Token::Type::FOR})) {
    auto forStmt = forStatement();
    if (!forStmt)
      synchronize();

    return forStmt;
  }

  if (match({Token::Type::IF})) {
    auto ifStmt = ifStatement();
    if (!ifStmt)
      synchronize();

    return ifStmt;
  }

  if (match({Token::Type::WHILE})) {
    auto whileStmt = whileStatement();
    if (!whileStmt)
      synchronize();

    return whileStmt;
  }

  if (match({Token::Type::PRINT})) {
    auto printStmt = printStatement();
    if (!printStmt)
      synchronize();

    return printStmt;
  }

  if (match({Token::Type::LEFT_BRACE})) {
    auto blockStmt = block();
    if (!blockStmt)
      synchronize();

    return blockStmt;
  }

  auto exprStmt = expressionStatement();
  if (!exprStmt)
    synchronize();

  return exprStmt;
}

auto Parser::varDeclaration()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto name = consume(Token::Type::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  std::unique_ptr<Expr> initializer = nullptr;

  if (match({Token::Type::EQUAL})) {
    auto expr = expression();
    if (!expr)
      return std::unexpected(expr.error());

    initializer = std::move(*expr);
  }

  auto end = consume(Token::Type::SEMICOLON);
  if (!end)
    return std::unexpected(end.error());

  return std::make_unique<Stmt>(
      std::move(Stmt::Var(std::move(*name), std::move(initializer))));
}

auto Parser::statement() -> std::expected<std::unique_ptr<Stmt>, ParseError> {
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

auto Parser::printStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto value = expression();
  if (!value)
    return std::unexpected(value.error());

  auto end = consume(Token::Type::SEMICOLON);
  if (!end)
    return std::unexpected(end.error());

  return std::make_unique<Stmt>(std::move(Stmt::Print(std::move(*value))));
}

auto Parser::expressionStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto expr = expression();
  if (!expr)
    return std::unexpected(expr.error());

  auto end = consume(Token::Type::SEMICOLON);
  if (!end)
    return std::unexpected(end.error());

  return std::make_unique<Stmt>(std::move(Stmt::Expression(std::move(*expr))));
}

auto Parser::block() -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  std::vector<std::unique_ptr<Stmt>> statements;

  while (!check(Token::Type::RIGHT_BRACE) && !isAtEnd()) {
    auto decl = declaration();
    if (!decl)
      return std::unexpected(decl.error());

    statements.push_back(std::move(*decl));
  }

  auto end = consume(Token::Type::RIGHT_BRACE);
  if (!end)
    return std::unexpected(end.error());

  return std::make_unique<Stmt>(std::move(Stmt::Block(std::move(statements))));
}

auto Parser::ifStatement() -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto begin = consume(Token::Type::LEFT_PAREN);
  if (!begin)
    return std::unexpected(begin.error());

  auto condition = expression();
  if (!condition)
    return std::unexpected(condition.error());

  auto end = consume(Token::Type::RIGHT_PAREN);
  if (!end)
    return std::unexpected(end.error());

  auto thenBranch = statement();
  if (!thenBranch)
    return std::unexpected(thenBranch.error());

  std::unique_ptr<Stmt> elseBranch = nullptr;

  if (match({Token::Type::ELSE})) {
    auto stmt = statement();
    if (!stmt)
      return std::unexpected(stmt.error());

    elseBranch = std::move(*stmt);
  }

  return std::make_unique<Stmt>(std::move(Stmt::If(
      std::move(*condition), std::move(*thenBranch), std::move(elseBranch))));
}

auto Parser::whileStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto begin = consume(Token::Type::LEFT_PAREN);
  if (!begin)
    return std::unexpected(begin.error());

  auto condition = expression();
  if (!condition)
    return std::unexpected(condition.error());

  auto end = consume(Token::Type::RIGHT_PAREN);
  if (!end)
    return std::unexpected(end.error());

  auto body = statement();
  if (!body)
    return std::unexpected(body.error());

  return std::make_unique<Stmt>(
      std::move(Stmt::While(std::move(*condition), std::move(*body))));
}

auto Parser::forStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  return nullptr;
}

auto Parser::breakStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  return nullptr;
}

auto Parser::function(std::string kind)
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  auto name = consume(Token::Type::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  auto begin = consume(Token::Type::LEFT_PAREN);
  if (!begin)
    return std::unexpected(begin.error());

  std::vector<Token> params;
  if (!check(Token::Type::RIGHT_PAREN)) {
    do {
      if (params.size() >= 255)
        return std::unexpected(
            ParseError{Error::TooManyParameters, previous(), {}});

      auto param = consume(Token::Type::IDENTIFIER);
      if (!param)
        return std::unexpected(param.error());

      params.push_back(std::move(*param));
    } while (match({Token::Type::COMMA}));
  }

  auto end = consume(Token::Type::RIGHT_PAREN);
  if (!end)
    return std::unexpected(end.error());

  auto body = statement();
  if (!body)
    return std::unexpected(body.error());

  return std::make_unique<Stmt>(std::move(
      Stmt::Function(std::move(*name), std::move(params), std::move(*body))));
}

auto Parser::returnStatement()
    -> std::expected<std::unique_ptr<Stmt>, ParseError> {
  return nullptr;
}