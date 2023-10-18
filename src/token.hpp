#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>
#include <string>
#include <variant>

struct Token {
  enum class Type {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    COLON,
    SLASH,
    STAR,
    QUESTION,
    MODULO,
    BANG,
    EQUAL,
    GREATER,
    LESS,

    // Two character tokens
    BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER_EQUAL,
    LESS_EQUAL,
    PLUS_PLUS,
    MINUS_MINUS,
    ARROW,

    // Literals
    IDENTIFIER,
    NUMBER,
    STRING,

    // Keywords
    AND,
    BREAK,
    DO,
    ELSE,
    ENUM,
    FALSE,
    FOR,
    FN,
    IF,
    IMPL,
    LET,
    MUT,
    OR,
    PRINT,
    RETURN,
    SELF,
    SKIP,
    STRUCT,
    TRUE,
    WHILE,

    // End of file
    END
  } type;
  std::string lexeme;

  int row;
  int column;
};

#endif // TOKEN_HPP