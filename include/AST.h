//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_AST_H
#define MVSCRIPTCOMPILER_AST_H
#include <string>
#include <vector>
#include <memory>
#include <variant>

// Custom value type for ByteCode
using Value = std::variant<double, std::string, bool>;

// Base class for every node
class ASTNode {
public:
    explicit ASTNode(int lineNumber) : lineNumber(lineNumber) {}
    virtual ~ASTNode() = default;
    int lineNumber;
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
    using ASTNode::ASTNode;

};

class Statement : public ASTNode {
    using ASTNode::ASTNode;
};

/* EXPRESSIONS */

// Binary operation: left op right
class BinaryOpExpr : public Expression {
    using Expression::Expression;
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryOp op;
};

// Literal value: 15, "hello", true
class LiteralExpr : public Expression {
    using Expression::Expression;
public:
    Value value;
};

// Unary operation: op operand
class UnaryOpExpr : public Expression {
    using Expression::Expression;
public:
    std::unique_ptr<Expression> operand;
    UnaryOp op;
};

// Identifier: variable or function name
class IdentifierExpr : public Expression {
    using Expression::Expression;
public:
    std::string name;
};

// Function call: func(arg1, arg2, ...)
class FunctionCallExpr : public Expression {
    using Expression::Expression;
public:
    std::string name;
    std::vector<std::unique_ptr<Expression>> args;
};

/* STATEMENTS */

// Block of statements
class BlockStatement : public Statement {
    using Statement::Statement;
public:
    std::vector<std::unique_ptr<Statement>> statements;
};

class ReturnStatement : public Statement {
    using Statement::Statement;
    public:
    std::unique_ptr<Expression> value;
};

// Assignment: x = value
class AssignmentStatement : public Statement {
    using Statement::Statement;
public:
    std::string name;
    std::unique_ptr<Expression> value;
};

// If statement: if condition then ifBlock [else elseBlock] end
class IfStatement : public Statement {
    using Statement::Statement;
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> thenBlock;
    std::unique_ptr<BlockStatement> elseBlock;  // nullptr if no else
};

// While loop: while condition do whileBlock end
class WhileStatement : public Statement {
    using Statement::Statement;
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
};

// For loop: for var in start..end do body end
class ForStatement : public Statement {
    using Statement::Statement;
public:
    std::string var;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::unique_ptr<BlockStatement> body;
};

// Function definition: func name(params) body end
class FunctionStatement : public Statement {
    using Statement::Statement;
public:
    std::string name;
    std::vector<std::unique_ptr<Expression>> params;
    std::unique_ptr<BlockStatement> body;
};

///@brief On statements/interrupts (on button1.press do body end)
class OnStatement : public Statement {
    using Statement::Statement;
public:
    ///@brief The peripheral source, for example BUTTON1, CAN_RX, DMA...
    std::string source;
    ///@brief The action/event to listen for, for example NEW_MESSAGE, PRESS, RELEASE...
    std::string event;
    std::unique_ptr<BlockStatement> body;
};

// Expression as statement (for function calls, etc)
class ExpressionStatement : public Statement {
    using Statement::Statement;
public:
    std::unique_ptr<Expression> expression;
};

#endif //MVSCRIPTCOMPILER_AST_H
