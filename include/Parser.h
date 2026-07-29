//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_PARSER_H
#define MVSCRIPTCOMPILER_PARSER_H
#include "AST.h"
#include "Lexer.h"


class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode* parse();

private:
    const std::vector<Token>& tokens;
    size_t currentToken;

    //parsing functions
    std::unique_ptr<Statement> parseStatement();

    static BinaryOp tokenTypeToBinaryOp(TokenType op);

    std::unique_ptr<Expression> parseExpression(int minPrecedence = 0);
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Expression> parseUnary();

    std::unique_ptr<BlockStatement> parseBlock(); // if bodies etc...
    std::unique_ptr<IfStatement> parseIf();
    std::unique_ptr<WhileStatement> parseWhile();
    std::unique_ptr<ForStatement> parseFor();
    std::unique_ptr<FunctionStatement> parseFunction();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<ReturnStatement> parseReturn();


    //helpers
    void advance();
    void expect(TokenType type); // check if current token is of this type, if not throw err, if yes advance
    bool check(TokenType type); // same as expect, except it doesn't consume/advance
    bool match(std::vector<TokenType> matches, int offset = 0); // checks if current token (or offset) is any of given types
    Token current();
    Token peek(int offset = 1);
    static int getPrecedence(TokenType op);
    static bool isOperator(TokenType type);
};


#endif //MVSCRIPTCOMPILER_PARSER_H