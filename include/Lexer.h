//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_LEXER_H
#define MVSCRIPTCOMPILER_LEXER_H
#include <string>


class Lexer {
public:

};

// individual token
enum class TokenType {
    // Literals
    NUMBER, STRING, IDENTIFIER, BOOLEAN,

    // Keywords
    KW_IF, KW_THEN, KW_ELSEIF, KW_ELSE, KW_END,
    KW_WHILE, KW_DO, KW_FOR, KW_IN,
    KW_FUNCTION, KW_RETURN,
    KW_AND, KW_OR, KW_NOT,
    KW_LOCAL,
    KW_TRUE, KW_FALSE,

    // Operators
    ASSIGN, PLUS, MINUS, STAR, SLASH, PERCENT,
    EQ, NEQ, LT, GT, LE, GE,

    // Delimiters
    LPAREN, RPAREN, COMMA, SEMICOLON, DOT_DOT,

    // Special
    EOF_TOKEN, ERROR
};

struct Token {
    TokenType type;
    // optional
    std::string value;

    // for error reporting
    int line;
    int column;
};

#endif //MVSCRIPTCOMPILER_LEXER_H