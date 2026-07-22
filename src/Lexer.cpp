//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Lexer.h"

Lexer::Lexer(const std::string &input) {

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
        } else if (c == '-' && std::isdigit(peek())) { // negative number
            emit(readNumber());
        } else if (std::isalpha(c) || c == '_') {
            // read the entire word then emit
            emit(readIdentifier());
        } else if (c == '=') {
            // logical operator
            if (peek() == '=') {
                emit(Token(TokenType::EQ, "==", line, col));
            }
            // assignment
            else {
                emit(Token(TokenType::ASSIGN, "=", line, col));
            }
        } else {
            throw std::runtime_error("Lexer::Tokenize()");
        }
    }
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

// Helper to get the current character
char Lexer::current() {
    return source[pos];
}

// Helper to move to the next character
void Lexer::advance() {
    pos++;
}

char Lexer::peek(int offset) {
    return source[pos + offset];
}

void Lexer::emit(const Token &token) {
    tokens.push_back(token);
}
