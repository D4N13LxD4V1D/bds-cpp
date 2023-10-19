#include <lexer.hpp>

#include <map>

std::map<char, Token::Type> singleTokens = {
    {'(', Token::Type::LEFT_PAREN},   {')', Token::Type::RIGHT_PAREN},
    {'{', Token::Type::LEFT_BRACE},   {'}', Token::Type::RIGHT_BRACE},
    {'[', Token::Type::LEFT_BRACKET}, {']', Token::Type::RIGHT_BRACKET},
    {';', Token::Type::SEMICOLON},    {',', Token::Type::COMMA},
    {'.', Token::Type::DOT},          {'-', Token::Type::MINUS},
    {'+', Token::Type::PLUS},         {'/', Token::Type::SLASH},
    {'*', Token::Type::STAR},         {'%', Token::Type::MODULO},
    {'!', Token::Type::BANG},         {'=', Token::Type::EQUAL},
    {'<', Token::Type::LESS},         {'>', Token::Type::GREATER},
    {'?', Token::Type::QUESTION},     {':', Token::Type::COLON},
};

std::map<std::string_view, Token::Type> doubleTokens = {
    {"!=", Token::Type::BANG_EQUAL},    {"==", Token::Type::EQUAL_EQUAL},
    {">=", Token::Type::GREATER_EQUAL}, {"<=", Token::Type::LESS_EQUAL},
    {"++", Token::Type::PLUS_PLUS},     {"--", Token::Type::MINUS_MINUS},
    {"->", Token::Type::ARROW},
};

std::map<std::string_view, Token::Type> keywords = {
    {"and", Token::Type::AND},       {"break", Token::Type::BREAK},
    {"do", Token::Type::DO},         {"else", Token::Type::ELSE},
    {"enum", Token::Type::ENUM},     {"false", Token::Type::FALSE},
    {"for", Token::Type::FOR},       {"fn", Token::Type::FN},
    {"if", Token::Type::IF},         {"impl", Token::Type::IMPL},
    {"let", Token::Type::LET},       {"mut", Token::Type::MUT},
    {"or", Token::Type::OR},         {"print", Token::Type::PRINT},
    {"return", Token::Type::RETURN}, {"self", Token::Type::SELF},
    {"skip", Token::Type::SKIP},     {"struct", Token::Type::STRUCT},
    {"true", Token::Type::TRUE},     {"while", Token::Type::WHILE}};

Lexer::Lexer(std::string_view filename, std::string_view source)
    : filename(filename), source(source) {
  tokens = std::vector<Token>();
}

auto Lexer::scanTokens() -> std::vector<Token> {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }
  addToken(Token::Type::END, "EOF");

  return tokens;
}

auto Lexer::isAtEnd() -> bool { return current >= source.size(); }

auto Lexer::advance() -> char {
  if (isAtEnd())
    return '\0';
  current++;
  column++;
  return source[current - 1];
}

auto Lexer::peek() -> char {
  if (isAtEnd())
    return '\0';
  return source[current];
}

auto Lexer::peekNext() -> char {
  if (current + 1 >= source.size())
    return '\0';
  return source[current + 1];
}

auto Lexer::getline() -> std::string { return getline(row); }

auto Lexer::getline(int n) -> std::string {
  int line = 1;
  int start = 0;
  int end = 0;
  for (int i = 0; i < source.size(); i++) {
    if (source[i + 1] == '\0') {
      end = i + 1;
      break;
    }
    if (source[i] == '\n') {
      line++;
      if (line == n) {
        start = i + 1;
      } else if (line == n + 1) {
        end = i;
        break;
      }
    }
  }
  return std::string{source.substr(start, end - start)};
}

auto Lexer::addToken(Token::Type type, std::string_view lexeme) -> void {
  tokens.push_back(Token(type, std::string{lexeme}, filename, getline(), row,
                         column - lexeme.size()));
}

auto Lexer::scanToken() -> void {
  std::string possibleDoubleToken{peek(), peekNext()};
  if (doubleTokens.find(possibleDoubleToken) != doubleTokens.end()) {
    current += 2;
    addToken(doubleTokens[possibleDoubleToken], possibleDoubleToken);
    return;
  }

  char c = advance();
  switch (c) {
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    row++;
    column = 1;
    break;
  case '#':
    while (peek() != '\n' && !isAtEnd())
      advance();
    break;
  case '/': {
    switch (peek()) {
    case '/':
      while (peek() != '\n' && !isAtEnd())
        advance();
      break;
    case '*': {
      int oldRow = row;
      int oldColumn = column;

      while (!isAtEnd()) {
        if (peek() == '\n') {
          row++;
          column = 1;
        }
        if (peek() == '*' && peekNext() == '/') {
          advance();
          advance();
          break;
        }
        advance();
        if (isAtEnd()) {
          std::string args[] = {filename, std::to_string(oldRow),
                                std::to_string(oldColumn), getline(oldRow)};
          error(Error::UnterminatedComment, args);
        }
      }
    } break;
    default:
      addToken(Token::Type::SLASH, "/");
      break;
    }
  } break;
  default:
    if (singleTokens.find(c) != singleTokens.end()) {
      addToken(singleTokens[c], std::string{1, c});
    } else if (isalpha(c) || c == '_') {
      while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
        advance();

      std::string_view text = source.substr(start, current - start);
      if (keywords.find(text) != keywords.end()) {
        addToken(keywords[text], text);
      } else {
        addToken(Token::Type::IDENTIFIER, text);
      }
    } else if (isdigit(c)) {
      while (isdigit(peek()))
        advance();

      if (peek() == '.') {
        advance();
        while (isdigit(peek()))
          advance();
      }

      std::string_view text = source.substr(start, current - start);
      addToken(Token::Type::NUMBER, text);
    } else if (c == '"') {
      int oldRow = row;
      int oldColumn = column;

      while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
          row++;
          column = 1;
        }
        advance();
      }

      if (isAtEnd()) {
        std::string args[] = {filename, std::to_string(oldRow),
                              std::to_string(oldColumn), getline(oldRow)};
        error(Error::UnterminatedString, args);
      }

      advance();

      std::string_view text = source.substr(start + 1, current - start - 2);
      addToken(Token::Type::STRING, text);
    } else {
      std::string args[] = {filename, std::to_string(row),
                            std::to_string(column), getline()};
      error(Error::UnexpectedCharacter, args);
    }
    break;
  }
}