//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Parser.h"


// constructor
Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0) {
}

ASTNode* Parser::parse() {
    while (currentToken < tokens.size()) {
        parseStatement();
    }
}

/* PARSERS */
// parsing dispatcher (decides which parsing function to call)
std::unique_ptr<Statement> Parser::parseStatement() {
    if (check(TokenType::KW_IF)) {
        return parseIf();
    }
}

std::unique_ptr<IfStatement> Parser::parseIf() {
    expect(TokenType::KW_IF);
    auto condition = parseExpression();

    expect(TokenType::KW_THEN);
    auto thenBlock = parseBlock();

    std::unique_ptr<BlockStatement> elseBlock;
    if (check(TokenType::KW_ELSE)) {
        advance();
        elseBlock = parseBlock();
    }

    expect(TokenType::KW_END);

    auto ifStatement = std::make_unique<IfStatement>();
    ifStatement->condition = std::move(condition);
    ifStatement->thenBlock = std::move(thenBlock);
    ifStatement->elseBlock = std::move(elseBlock);

    return ifStatement;
}

/* HELPERS*/
void Parser::advance() {
    currentToken++;
}

Token Parser::current() {
    return tokens[currentToken];
}

Token Parser::peek(int offset) {
    if (currentToken + offset < tokens.size()) {
        return tokens[currentToken + offset];
    } else {
        throw std::out_of_range("");
    }
}

void Parser::expect(TokenType type) {
    if (current().type == type) {
        advance();
    } else {
        throw new std::runtime_error("Expected token type");
    }
}

bool Parser::check(TokenType type) {
    return current().type == type;
}

