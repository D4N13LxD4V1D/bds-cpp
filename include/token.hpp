#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

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
    QUOTE,
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

    // Special tokens
    ERROR,
    END
  } type;
  std::string lexeme;

  struct {
    std::string filename;
    std::string source;
    int row;
    int column;
  } location;

  Token(Type type, std::string lexeme, std::string filename, std::string source,
        int row, int column)
      : type(type), lexeme(lexeme), location({filename, source, row, column}) {}
};

#endif // TOKEN_HPP