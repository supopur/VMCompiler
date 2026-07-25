//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Parser.h"

#include <unordered_set>


// constructor
Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0) {
}

ASTNode* Parser::parse() {
    auto* programBody = new BlockStatement();
    while (currentToken < tokens.size()) {
        programBody->statements.push_back(parseStatement());
    }
    return programBody;
}

/* PARSERS */
// parsing dispatcher (decides which parsing function to call)
std::unique_ptr<Statement> Parser::parseStatement() {
    if (check(TokenType::KW_IF)) {
        return parseIf();
    } else if (check(TokenType::IDENTIFIER) && peek().type == TokenType::ASSIGN) { // variable assignment
        return parseAssignment();
    }
}

std::unique_ptr<Expression> Parser::parseExpression(int minPrecedence) {
    // start with the first one
    std::unique_ptr<BinaryOpExpr> left = std::make_unique<BinaryOpExpr>();

    left->left = std::move(parseUnary());


    int precedence = 0;

    while (isOperator(current().type) && precedence >= minPrecedence) {
        const TokenType op = current().type;
        const int opPrecedence = getPrecedence(op);

        advance(); // consume the operator

        std::unique_ptr<Expression> right = parseExpression(opPrecedence);

        left = std::make_unique<BinaryOpExpr>();
        left->left = std::move(left);
        left->right = std::move(right);
    }

    return left;
}

// atomic expressions, ones that can't be really broken down (2 + 2 + 5 - 10)
std::unique_ptr<Expression> Parser::parsePrimary() {

}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (current().type == TokenType::MINUS) {
        advance(); // consume operator

        std::unique_ptr<LiteralExpr> operand = std::make_unique<LiteralExpr>();

        operand->value = parseUnary(); // recursive! -(-5) works

        std::unique_ptr<UnaryOpExpr> unary = std::make_unique<UnaryOpExpr>();

        unary->op = UnaryOp::NEGATE;
        unary->operand = std::move(operand);
    } else if (current().type == TokenType::KW_NOT) {
        advance(); // consume operator

        std::unique_ptr<LiteralExpr> operand = std::make_unique<LiteralExpr>();

        operand->value = parseUnary(); // recursive! -(-5) works

        std::unique_ptr<UnaryOpExpr> unary = std::make_unique<UnaryOpExpr>();

        unary->op = UnaryOp::NOT;
        unary->operand = std::move(operand);
    } else {
        // no operator
        return parsePrimary();
    }
}

std::unique_ptr<BlockStatement> Parser::parseBlock() {
    auto block = std::make_unique<BlockStatement>();

    while (current().type != TokenType::KW_END) {
        block->statements.push_back(parseStatement());
    }
    return block;
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

std::unique_ptr<Statement> Parser::parseAssignment() {
    std::string varName;
    if (check(TokenType::IDENTIFIER)) {
        varName = current().value;
        advance();
    } else {
        throw std::runtime_error("Expected identifier");
    }
    expect(TokenType::ASSIGN);



    // we have a statement instead of a single literal
    // if (match(operatorTokens,1)) {
    //     throw std::runtime_error("oops not implemented");
    // } else if (match(literalTokens)) {
    //     // the actual value
    //     std::unique_ptr<LiteralExpr> literal = std::make_unique<LiteralExpr>();
    //     // asign the value
    //     literal->value = current().value;
    //
    //     // the entire statement (x = 5)
    //     std::unique_ptr<AssignmentStatement> assignment = std::make_unique<AssignmentStatement>();
    //     // the var name (x)
    //     assignment->name = varName;
    //     // it's value (5)
    //     assignment->value = std::move(literal);
    //     return assignment;
    // } else {
    //     throw std::runtime_error("invalid token type");
    // }
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

int Parser::getPrecedence(const TokenType op) {
    switch (op) {
        case TokenType::KW_OR: return 1;
        case TokenType::KW_AND: return 2;
        case TokenType::NEQ:
        case TokenType::EQ: return 3;
        case TokenType::LT:
        case TokenType::GT:
        case TokenType::LE:
        case TokenType::GE:
            return 4;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 5;
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::PERCENT:
            return 6;
        default:
            return 0;
    }
}

bool isOperator(const TokenType type) {
    static const std::unordered_set<TokenType> operators = {
        TokenType::PLUS, TokenType::MINUS, TokenType::STAR, TokenType::SLASH, TokenType::PERCENT,
        TokenType::EQ, TokenType::NEQ, TokenType::LT, TokenType::GT, TokenType::LE, TokenType::GE,
        TokenType::KW_AND, TokenType::KW_OR, TokenType::KW_NOT
    };
    return operators.count(type) > 0;
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

bool Parser::match(std::vector<TokenType> matches, int offset) {
    if (std::find(matches.begin(), matches.end(), peek(offset).type) != matches.end()) {
        return true;
    }
    return false;
}

