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
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parseBinary(int precedence); // precedence = priority

    std::unique_ptr<BlockStatement> parseBlock();
    std::unique_ptr<IfStatement> parseIf();
    std::unique_ptr<WhileStatement> parseWhile();
    std::unique_ptr<ForStatement> parseFor();
    std::unique_ptr<FunctionStatement> parseFunction();


    //helpers
    void advance();
    void expect(TokenType type); // check if current token is of this type, if not throw err, if yes advance
    bool check(TokenType type); // same as expect, except it doesn't consume/advance
    // todo implmenet
    //bool match() // checks if current token is any of given types
    Token current();
    Token peek(int offset = 1);

};


#endif //MVSCRIPTCOMPILER_PARSER_H