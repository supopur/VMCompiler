//
// Created by Matouš Smékal on 30.07.2026.
// Source: claude.ai
//

#ifndef MVSCRIPTCOMPILER_EXCEPTIONTYPES_H
#define MVSCRIPTCOMPILER_EXCEPTIONTYPES_H

#include <stdexcept>
#include <string>

class CompilerException : public std::runtime_error {
public:
    // Base exception class for all compiler errors with line and column info
    CompilerException(const std::string& msg, int line = -1, int col = -1)
        : std::runtime_error(msg), line(line), col(col) {}

    int getLine() const { return line; }
    int getColumn() const { return col; }

private:
    int line, col;
};

// Variable errors
class UndefinedVariableError : public CompilerException {
public:
    // Thrown when code references a variable that has not been declared
    UndefinedVariableError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class VariableShadowingWarning : public CompilerException {
public:
    // Thrown when a variable declaration shadows an outer scope variable
    VariableShadowingWarning(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Function errors
class UndefinedFunctionError : public CompilerException {
public:
    // Thrown when calling a function that has not been defined
    UndefinedFunctionError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class DuplicateFunctionError : public CompilerException {
public:
    // Thrown when defining a function with the same name as an existing function
    DuplicateFunctionError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class ArgumentCountError : public CompilerException {
public:
    // Thrown when a function call has the wrong number of arguments
    ArgumentCountError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class MissingReturnError : public CompilerException {
public:
    // Thrown when a non void function has execution paths that do not return
    MissingReturnError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Type errors
class TypeMismatchError : public CompilerException {
public:
    // Thrown when operands of incompatible types are used in an operation
    TypeMismatchError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class InvalidUnaryOperationError : public CompilerException {
public:
    // Thrown when a unary operator is applied to an incompatible operand type
    InvalidUnaryOperationError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class IncomparableTypesError : public CompilerException {
public:
    // Thrown when comparison operators are used on incompatible types
    IncomparableTypesError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class InvalidLoopBoundsError : public CompilerException {
public:
    // Thrown when for loop bounds are not numeric types
    InvalidLoopBoundsError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Assignment errors
class InvalidAssignmentTargetError : public CompilerException {
public:
    // Thrown when assignment is attempted to something that cannot be assigned to
    InvalidAssignmentTargetError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Control flow errors
class BreakOutsideLoopError : public CompilerException {
public:
    // Thrown when a break statement is used outside of a loop
    BreakOutsideLoopError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class ContinueOutsideLoopError : public CompilerException {
public:
    // Thrown when a continue statement is used outside of a loop
    ContinueOutsideLoopError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class UnreachableCodeError : public CompilerException {
public:
    // Thrown when code is detected that cannot be reached due to prior returns
    UnreachableCodeError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Arithmetic errors
class DivisionByZeroError : public CompilerException {
public:
    // Thrown when division by a zero literal is detected at compile time
    DivisionByZeroError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Recursion errors
class InfiniteRecursionError : public CompilerException {
public:
    // Thrown when a function only calls itself with no other control flow
    InfiniteRecursionError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Constant errors
class InvalidConstantError : public CompilerException {
public:
    // Thrown when an invalid or non constant value is added to the constant table
    InvalidConstantError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class ConstantTableError : public CompilerException {
public:
    // Thrown when the constant table becomes corrupted or overflows
    ConstantTableError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Internal errors
class InternalCompilerError : public CompilerException {
public:
    // Thrown when the compiler encounters an unexpected internal state
    InternalCompilerError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

class NullPointerError : public CompilerException {
public:
    // Thrown when a null pointer is dereferenced during compilation
    NullPointerError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

// Index errors
class IndexOutOfBoundsError : public CompilerException {
public:
    // Thrown when an array index literal is outside the bounds of a literal array
    IndexOutOfBoundsError(const std::string& msg, int line = -1, int col = -1)
        : CompilerException(msg, line, col) {}
};

#endif //MVSCRIPTCOMPILER_EXCEPTIONTYPES_H