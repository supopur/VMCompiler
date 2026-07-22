//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Lexer.h"

#include <unordered_map>

Lexer::Lexer(const std::string &input) : source(input), pos(0), line(1), col(0) {
}

std::vector<Token> Lexer::Tokenize() {
    // here we loop trough the whole source code, we "emit" tokens (add them to the tokens list)
    while (pos < source.length()) {
        char c = current();
        if (std::isspace(c)) {
            // skip whitespace
            advance();
        } else if (std::isdigit(c)) {
            // read the entire number then emit
            emit(readNumber());
        } else if (c == '-' && std::isdigit(peek())) {
            // negative number
            emit(readNumber());
        } else if (std::isalpha(c) || c == '_') {
            // read the entire word then emit
            emit(readIdentifier());
        } else if (current() == '"' || current() == '\'') {
            // string
            emit(readString());
        } else if (c == '=') {
            // logical operator
            if (peek() == '=') {
                emit(Token(TokenType::EQ, "==", line, col));
                advance();
                advance(); // and also skip the second =
            }
            // assignment
            else {
                emit(Token(TokenType::ASSIGN, "=", line, col));
                advance();
            }
        } else if (c == '!') {
            // neq
            if (peek() == '=') {
                emit(Token(TokenType::NEQ, "!=", line, col));
                advance();
                advance();
            }
        } else if (c == '>') {
            emit(Token(TokenType::GT, ">", line, col));
            advance();
        } else if (c == '<') {
            emit(Token(TokenType::LT, "<", line, col));
            advance();
        } else if (c == '+') {
            emit(Token(TokenType::PLUS, "+", line, col));
            advance();
        } else if (c == '*') {
            emit(Token(TokenType::STAR, "*", line, col));
            advance();
        } else if (c == '/') {
            emit(Token(TokenType::SLASH, "/", line, col));
            advance();
        } else if (c == '(') {
            emit(Token(TokenType::LPAREN, "(", line, col));
            advance();
        } else if (c == ')') {
            emit(Token(TokenType::RPAREN, ")", line, col));
        }
        else {
            throw std::runtime_error("Lexer::Tokenize()");
        }
    }
    return tokens;
}

Token Lexer::readNumber() {
    std::string numberStr;

    while (pos < source.length()) {
        //check if we reached the end of the int
        if (std::isspace(current())) {
            break;
        }
        numberStr += current();
        advance();
    }
    return Token(TokenType::NUMBER, numberStr, line, col);
}

Token Lexer::readString() {
    // we can have empty strings so we have to initialize
    std::string stringStr = "";

    // we always start at the ' or " sign so we skip this
    advance();

    while (pos < source.length()) {
        // string is completed/closed
        if (current() == '"' || current() == '\'') {
            if (!(peek(-1) == '\\')) {
                // escape character allowing ' or " to be stored in a string
                break;
            }
        }
        stringStr += current();
        advance();
    }
    return Token(TokenType::STRING, stringStr, line, col);
}

// variable names, function names and also keywords.
Token Lexer::readIdentifier() {
    std::string identifierStr = "";

    while (pos < source.length()) {
        if (std::isspace(current())) {
            // identifier is ended with a space
            break;
        }
        identifierStr += current();
        advance();
    }

    auto kwOrID = keywordOrIdentifier(identifierStr);

    if (kwOrID == TokenType::IDENTIFIER) {
        // identifier
        return Token(TokenType::IDENTIFIER, identifierStr, line, col);
    } else {
        // keyword
        return Token(kwOrID, identifierStr, line, col);
    }
}


// here we define how the keywords should look like in the source code
TokenType Lexer::keywordOrIdentifier(const std::string &text) {
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"if", TokenType::KW_IF},
        {"then", TokenType::KW_THEN},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE},
        {"do", TokenType::KW_DO},
        {"for", TokenType::KW_FOR},
        {"in", TokenType::KW_IN},
        {"and", TokenType::KW_AND},
        {"or", TokenType::KW_OR},
        {"not", TokenType::KW_NOT},
        {"true", TokenType::KW_TRUE},
        {"false", TokenType::KW_FALSE},
        {"end", TokenType::KW_END},
        {"func", TokenType::KW_FUNCTION},
    };

    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return it->second; // It's a keyword
    }
    return TokenType::IDENTIFIER; // It's just a variable/function name
}

// Helper to get the current character
char Lexer::current() {
    return source[pos];
}

// Helper to move to the next character
void Lexer::advance() {
    pos++;
}

char Lexer::peek(int offset) {
    if (pos + offset >= source.length()) {
        return '\0';
    }
    return source[pos + offset];
}

void Lexer::emit(const Token &token) {
    tokens.push_back(token);
}
