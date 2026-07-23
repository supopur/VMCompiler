//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Lexer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

//Lexer::Lexer(const std::string &input) : source(input), pos(0), line(1), col(0) {}

Lexer::Lexer(std::ifstream &file) : file(file), pos(0), line(1), col(0) {
}

// Characters that cannot be part of an identifier or number
bool Lexer::isInvalidForIdentifier(char c) {
    static const std::string invalidChars = "(){}[],;:.=<>!+-*/%&|^~?@#$`\\\"' \t\n\r";
    return invalidChars.find(c) != std::string::npos;
}

std::vector<Token> Lexer::Tokenize() {
    std::cout << "zavolano";
    while(std::getline(file, currentLine)) {
        pos = 0;
        line = lineNumber;
        lineNumber++;
        while (pos < currentLine.length()) {
            char c = current();
  
            if (std::isspace(c)) {
                advance();
            } else if (std::isdigit(c)) {
                emit(readNumber());
            } else if (c == '-' && pos + 1 < currentLine.length() && std::isdigit(peek())) {
                emit(readNumber());
            } else if (std::isalpha(c) || c == '_') {
                emit(readIdentifier());
            } else if (c == '"' || c == '\'') {
                emit(readString());
            } else if (c == '=') {
                if (peek() == '=') {
                    emit(Token(TokenType::EQ, "==", line, col));
                    advance();
                    advance();
                } else {
                    emit(Token(TokenType::ASSIGN, "=", line, col));
                    advance();
                }
            } else if (c == '!') {
                if (peek() == '=') {
                    emit(Token(TokenType::NEQ, "!=", line, col));
                    advance();
                    advance();
                } else {
                    advance();
                }
            } else if (c == '<') {
                if (peek() == '=') {
                    emit(Token(TokenType::LE, "<=", line, col));
                    advance();
                    advance();
                } else {
                    emit(Token(TokenType::LT, "<", line, col));
                    advance();
                }
            } else if (c == '>') {
                if (peek() == '=') {
                    emit(Token(TokenType::GE, ">=", line, col));
                    advance();
                    advance();
                } else {
                    emit(Token(TokenType::GT, ">", line, col));
                    advance();
                }
            } else if (c == '+') {
                emit(Token(TokenType::PLUS, "+", line, col));
                advance();
            } else if (c == '-') {
                emit(Token(TokenType::MINUS, "-", line, col));
                advance();
            } else if (c == '*') {
                emit(Token(TokenType::STAR, "*", line, col));
                advance();
            } else if (c == '/') {
                emit(Token(TokenType::SLASH, "/", line, col));
                advance();
            } else if (c == '%') {
                emit(Token(TokenType::PERCENT, "%", line, col));
                advance();
            } else if (c == '(') {
                emit(Token(TokenType::LPAREN, "(", line, col));
                advance();
            } else if (c == ')') {
                emit(Token(TokenType::RPAREN, ")", line, col));
                advance();
            } else if (c == ',') {
                emit(Token(TokenType::COMMA, ",", line, col));
                advance();
            } else if (c == ';') {
                emit(Token(TokenType::SEMICOLON, ";", line, col));
                advance();
            } else if (c == '.' && peek() == '.') {
                emit(Token(TokenType::DOT_DOT, "..", line, col));
                advance();
                advance();
            } else {
                throw std::runtime_error(std::string("Unknown character: ") + c);
        }
    }
  }
  return tokens;
}

Token Lexer::readNumber() {
    std::string numberStr;

    while (pos < currentLine.length() && !isInvalidForIdentifier(current())) {
        numberStr += current();
        advance();
    }
    return Token(TokenType::NUMBER, numberStr, line, col);
}


Token Lexer::readString() {
    // we can have empty strings so we have to initialize
    std::string stringStr = "";
    char quote = current();
    advance();

    while (pos < currentLine.length()) {
        if (current() == quote && peek(-1) != '\\') { // escape character
            break;
        }
        stringStr += current();
        advance();
    }
    if (pos < currentLine.length()) advance(); // skip closing quote

    return Token(TokenType::STRING, stringStr, line, col);
}


// variable names, function names and also keywords.
Token Lexer::readIdentifier() {
    std::string identifierStr = "";

    while (pos < currentLine.length() && !isInvalidForIdentifier(current())) {
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
        {"func", TokenType::KW_FUNCTION}
    };

    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return it->second; // It's a keyword
    }
    return TokenType::IDENTIFIER; // It's just a variable/function name
}

// Helper to get the current character
char Lexer::current() {
    return currentLine[pos];
}

// Helper to move to the next character
void Lexer::advance() {
    pos++;
}

char Lexer::peek(int offset) {
    if (pos + offset >= currentLine.length()) {
        return '\0';
    }
    return currentLine[pos + offset];
}

void Lexer::emit(const Token &token) {
    tokens.push_back(token);
}
