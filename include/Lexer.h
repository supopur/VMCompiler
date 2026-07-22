//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_LEXER_H
#define MVSCRIPTCOMPILER_LEXER_H
#include <string>
#include <vector>

// individual token
enum class TokenType {
    // Literals
    NUMBER, STRING, IDENTIFIER, BOOLEAN,

    // Keywords
    KW_IF, KW_THEN, KW_ELSE, KW_END,
    KW_WHILE, KW_DO, KW_FOR, KW_IN,
    KW_FUNCTION,
    KW_AND, KW_OR, KW_NOT,
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
    // seems like it is optional but is actually required!!
    std::string value;

    // for error reporting
    int line;
    int column;
};

class Lexer {
public:
    // Constructor which takes source code
    Lexer(const std::string &input);

    // make it run
    std::vector<Token> Tokenize();

private:
    // current position in terms of the sourcecode, this is used for error reporting and is useless for the lexer...
    int line, col;
    // the current CHARACTER which we are currently reading, (we do not split the string upfront)
    size_t pos;

    //sc
    std::string source;

    std::vector<Token> tokens;

    Token readNumber();
    Token readString();
    Token readIdentifier();
    TokenType keywordOrIdentifier(const std::string& text);

    // helper to get the current character (from pos)
    char current();
    // move to the next character
    void advance();
    // see what character is after a given offset relative from current position
    char peek(int offset = 1);
    //basically create the token
    void emit(const Token &token);
};

#endif //MVSCRIPTCOMPILER_LEXER_H
