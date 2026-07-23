//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_AST_H
#define MVSCRIPTCOMPILER_AST_H
#include <string>
#include <vector>
#include <memory>

// Base class for every node
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Binary operators
enum class BinaryOp {
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD,
    // Comparison
    EQ, NEQ, LT, GT, LE, GE,
    // Logical
    AND, OR
};

// Unary operators
enum class UnaryOp {
    NEGATE,  // -x
    NOT      // not x
};

/* BASE CLASSES */

class Expression : public ASTNode {
};

class Statement : public ASTNode {
};

/* EXPRESSIONS */

// Binary operation: left op right
class BinaryOpExpr : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryOp op;
};

// Unary operation: op operand
class UnaryOpExpr : public Expression {
public:
    std::unique_ptr<Expression> operand;
    UnaryOp op;
};

// Literal value: 15, "hello", true
class LiteralExpr : public Expression {
public:
    std::string value;
};

// Identifier: variable or function name
class IdentifierExpr : public Expression {
public:
    std::string name;
};

// Function call: func(arg1, arg2, ...)
class FunctionCallExpr : public Expression {
public:
    std::string name;
    std::vector<std::unique_ptr<Expression>> args;
};

/* STATEMENTS */

// Block of statements
class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
};

// Assignment: x = value
class AssignmentStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
};

// If statement: if condition then ifBlock [else elseBlock] end
class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> thenBlock;
    std::unique_ptr<BlockStatement> elseBlock;  // nullptr if no else
};

// While loop: while condition do whileBlock end
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
};

// For loop: for var in start..end do body end
class ForStatement : public Statement {
public:
    std::string var;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::unique_ptr<BlockStatement> body;
};

// Function definition: func name(params) body end
class FunctionStatement : public Statement {
public:
    std::string name;
    std::vector<std::string> params;
    std::unique_ptr<BlockStatement> body;
};

// Expression as statement (for function calls, etc)
class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
};

#endif //MVSCRIPTCOMPILER_AST_H