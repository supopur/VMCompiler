//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Parser.h"
#include "AST.h"
#include "Lexer.h"

#include <iostream>
#include <memory>
#include <unordered_set>


// constructor
Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), currentToken(0) {
}

std::unique_ptr<ASTNode> Parser::parse() {
    auto programBody = std::make_unique<BlockStatement>();
    while (current().type != TokenType::EOF_TOKEN) {
        programBody->statements.push_back(parseStatement());
    }
    return programBody;
}

/* PARSERS */
// parsing dispatcher (decides which parsing function to call)
std::unique_ptr<Statement> Parser::parseStatement() {
    if (check(TokenType::KW_IF)) {
        return parseIf();
    } else if (check(TokenType::KW_WHILE)) {
        return parseWhile();
    } else if (check(TokenType::KW_FOR)) {
        return parseFor();
    } else if (check(TokenType::KW_FUNCTION)) {
        return parseFunction();
    } else if (check(TokenType::IDENTIFIER) && peek().type == TokenType::ASSIGN) {
        // variable assignment
        return parseAssignment();
    } else if (check(TokenType::KW_RETURN)) {
        return parseReturn();
    } else if (check(TokenType::IDENTIFIER) || check(TokenType::LPAREN)) {
        // Expression statement (function calls, etc)
        auto expr = parseExpression();
        auto exprStmt = std::make_unique<ExpressionStatement>();
        exprStmt->expression = std::move(expr);
        return exprStmt;
    } else if (check(TokenType::EOF_TOKEN)) {
        return nullptr;
    } else {
        std::cout << "Unexpected token: " << currentToken << std::endl;
        throw std::runtime_error("Expected statement");
    }
}

BinaryOp Parser::tokenTypeToBinaryOp(TokenType op) {
    switch (op) {
        // Arithmetic
        case TokenType::PLUS:    return BinaryOp::ADD;
        case TokenType::MINUS:   return BinaryOp::SUB;
        case TokenType::STAR:    return BinaryOp::MUL;
        case TokenType::SLASH:   return BinaryOp::DIV;
        case TokenType::PERCENT: return BinaryOp::MOD;

            // Comparison
        case TokenType::EQ:  return BinaryOp::EQ;
        case TokenType::NEQ: return BinaryOp::NEQ;
        case TokenType::LT:  return BinaryOp::LT;
        case TokenType::GT:  return BinaryOp::GT;
        case TokenType::LE:  return BinaryOp::LE;
        case TokenType::GE:  return BinaryOp::GE;

            // Logical
        case TokenType::KW_AND: return BinaryOp::AND;
        case TokenType::KW_OR:  return BinaryOp::OR;

        default:
            throw std::invalid_argument("TokenType is not a binary operator");
    }
}

std::unique_ptr<Expression> Parser::parseExpression(int minPrecedence) {
    auto left = parseUnary();

    while (isOperator(current().type) && getPrecedence(current().type) >= minPrecedence) {
        const TokenType op = current().type;
        const int opPrecedence = getPrecedence(op);

        advance(); // consume the operator

        auto right = parseExpression(opPrecedence + 1);

        auto binOp = std::make_unique<BinaryOpExpr>();
        binOp->op = tokenTypeToBinaryOp(op);
        binOp->left = std::move(left);
        binOp->right = std::move(right);
        left = std::move(binOp);
    }

    return left;
}

// atomic expressions, ones that can't be really broken down (2 + 2 + 5 - 10)
std::unique_ptr<Expression> Parser::parsePrimary() {
    switch (current().type) {
        case TokenType::STRING:
        case TokenType::BOOLEAN:
        case TokenType::KW_TRUE:
        case TokenType::KW_FALSE:
        case TokenType::NUMBER: {
            std::unique_ptr<LiteralExpr> literal = std::make_unique<LiteralExpr>();
            if (check(TokenType::NUMBER)) {
              literal->value = std::stod(current().value);
            } else if (check(TokenType::STRING)) {
              literal->value = current().value;
            } else if (check(TokenType::KW_TRUE) || check(TokenType::KW_FALSE)) {
              literal->value = (current().type == TokenType::KW_TRUE);
            }
            advance();
            return literal;
        }
        case TokenType::IDENTIFIER: {
            std::string name = current().value;
            advance();

            // check if it's a function call
            if (check(TokenType::LPAREN)) {
                advance();
                std::vector<std::unique_ptr<Expression> > args;

                while (!check(TokenType::RPAREN)) {
                    args.push_back(parseExpression());

                    if (check(TokenType::COMMA)) {
                        advance();
                    }
                }

                expect(TokenType::RPAREN);

                std::unique_ptr<FunctionCallExpr> functionCall = std::make_unique<FunctionCallExpr>();

                functionCall->name = name;
                functionCall->args = std::move(args);

                return functionCall;
            }

            // just an identifier/var
            std::unique_ptr<IdentifierExpr> identifier = std::make_unique<IdentifierExpr>();

            identifier->name = name;

            return identifier;
        }
        case TokenType::LPAREN: {
            // (1 + 2) * 2
            advance();
            std::unique_ptr<Expression> expression = parseExpression();
            expect(TokenType::RPAREN);
            return expression;
        }
        default: {
            std::cout << currentToken << std::endl;
            throw std::runtime_error("Expected expression");
        }
    }
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (current().type == TokenType::MINUS) {
        advance(); // consume operator

        auto operand = parseUnary();

        auto unary = std::make_unique<UnaryOpExpr>();
        unary->op = UnaryOp::NEGATE;
        unary->operand = std::move(operand);
        return unary;
    } else if (current().type == TokenType::KW_NOT) {
        advance(); // consume operator

        auto operand = parseUnary();

        auto unary = std::make_unique<UnaryOpExpr>();
        unary->op = UnaryOp::NOT;
        unary->operand = std::move(operand);
        return unary;
    } else {
        // no operator
        return parsePrimary();
    }
}

std::unique_ptr<BlockStatement> Parser::parseBlock() {
    auto block = std::make_unique<BlockStatement>();
    
    while (currentToken < tokens.size() &&
        current().type != TokenType::KW_END &&
        current().type != TokenType::KW_ELSE) {
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

std::unique_ptr<WhileStatement> Parser::parseWhile() {
    expect(TokenType::KW_WHILE);

    auto condition = parseExpression();

    expect(TokenType::KW_DO);
    auto doBlock = parseBlock();

    expect(TokenType::KW_END);

    auto whileStatement = std::make_unique<WhileStatement>();

    whileStatement->condition = std::move(condition);
    whileStatement->body = std::move(doBlock);

    return whileStatement;
}

std::unique_ptr<ForStatement> Parser::parseFor() {
    expect(TokenType::KW_FOR);

    // Get the loop variable
    std::string varName;
    if (check(TokenType::IDENTIFIER)) {
        varName = current().value;
        advance();
    } else {
        throw std::runtime_error("Expected identifier in for loop");
    }

    expect(TokenType::KW_IN);

    // Parse the start expression
    auto start = parseExpression();

    // Expect the dot dot operator
    expect(TokenType::DOT_DOT);

    // Parse the end expression
    auto end = parseExpression();

    expect(TokenType::KW_DO);

    // Parse the loop body
    auto body = parseBlock();

    expect(TokenType::KW_END);

    auto forStatement = std::make_unique<ForStatement>();
    forStatement->var = varName;
    forStatement->start = std::move(start);
    forStatement->end = std::move(end);
    forStatement->body = std::move(body);

    return forStatement;
}

std::unique_ptr<FunctionStatement> Parser::parseFunction() {
    expect(TokenType::KW_FUNCTION);

    std::string funcName = current().value;
    expect(TokenType::IDENTIFIER);

    expect(TokenType::LPAREN);

    std::vector<std::unique_ptr<Expression>> params;
    while (!check(TokenType::RPAREN)) {
        if (!check(TokenType::IDENTIFIER)) {
            throw std::runtime_error("Expected parameter name");
        }

        // Create IdentifierExpr for each parameter
        auto paramId = std::make_unique<IdentifierExpr>();
        paramId->name = current().value;
        params.push_back(std::move(paramId));

        advance();

        if (check(TokenType::COMMA)) {
            advance();
        }
    }

    expect(TokenType::RPAREN);

    auto body = parseBlock();

    expect(TokenType::KW_END);

    auto function = std::make_unique<FunctionStatement>();
    function->name = funcName;
    function->params = std::move(params);
    function->body = std::move(body);

    return function;
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

    auto value = parseExpression();  // Parse the right-hand side

    auto assignment = std::make_unique<AssignmentStatement>();
    assignment->name = varName;
    assignment->value = std::move(value);

    return assignment;
}

std::unique_ptr<ReturnStatement> Parser::parseReturn() {
    expect(TokenType::KW_RETURN);
    auto value = parseExpression();
    auto retStmt = std::make_unique<ReturnStatement>();
    retStmt->value = std::move(value);
    return retStmt;
}

/* HELPERS*/
void Parser::advance() {
    currentToken++;
}

 Token Parser::current() {
    if (currentToken >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input (missing 'end'?)");
    }
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

bool Parser::isOperator(const TokenType type) {
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
        throw std::runtime_error("Expected token type");
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
