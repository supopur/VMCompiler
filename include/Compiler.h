//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_COMPILER_H
#define MVSCRIPTCOMPILER_COMPILER_H

#include "AST.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

enum class ByteCode {
  // Math 
  ADD, SUB, MUL, DIV, MOD,
  // Comparison
  LT, LE, GT, GE, EQ, NEQ,
  // Variables operations
  LOAD, STORE,
  // Stack manipulation
  POP, DUP, PUSH_CONST,
  // Logic
  AND, OR, NOT, NEGATE,
  // Bit operations
  BAND, BOR, BXOR, BNOT, SHL, SHR,
  // Jumps
  JUMP, JUMP_IF_FALSE,
  // Functions
  RETURN, CALL,
  // Housekeeping
  HALT
};

// Whole line in the file, argument can be NUMBER, STRING or BOOL (via Value)
struct Instruction {
  ByteCode bytecode;
  int operand = 0;
  std::string strOperand;

  Instruction(ByteCode bc) : bytecode(bc) {};
  Instruction(ByteCode bc, int op) : bytecode(bc), operand(op) {};
  Instruction(ByteCode bc, const std::string& strOp) : bytecode(bc), strOperand(strOp) {};
};

using Value = std::variant<double, std::string, bool>;

class Compiler {
public:
  // Constructor
  Compiler(ASTNode* Nodes);

  // Main compile function
  std::vector<Instruction> compile();

  // Makes the output file
  void serialize(const std::string& filename);

private:
  // All generated instructions
  std::vector<Instruction> instructions;

  // Constants table
  std::vector<Value> constantTable;

  // Variable table and slot
  std::unordered_map<std::string, int> variableTable;
  int nextSlot = 0;

  std::unordered_map<std::string, size_t> functionTable;

  // Input nodes
  ASTNode* AllNodes;

  // --- Helper functions ---

  // Emit function and overloads
  void emit(ByteCode bc);
  void emit(ByteCode bc, int operand);
  void emit(ByteCode bc, const std::string& strOperand);

  // Adds constant to the constant table
  int addConstant(const Value& v);

  // Checks slot in varible table
  int resolveVariable(const std::string& name);

  // Map function for BinaryOp -> ByteCode
  ByteCode binaryOpToByteCode(BinaryOp op);

  // Main compile routers functions
  void compileStatement(Statement* stmt);
  void compileExpession(Expression* expr);

  // Compile functions - statements
  void compileBlock(BlockStatement* block);
  void compileAssignment(AssignmentStatement* stmt);
  void compileIf(IfStatement* stmt);
  void compileWhile(WhileStatement* stmt);
  void compileFor(ForStatement* stmt);
  void compileFunction(FunctionStatement* stmt);
  void compileExpressionStmt(ExpressionStatement* stmt);

  // Compile functions - expressions
  void compileBinaryOp(BinaryOpExpr* expr);
  void compileLiteral(LiteralExpr* expr);
  void compileUnaryOp(UnaryOpExpr* expr);
  void compileIdentifier(IdentifierExpr* expr);
  void compileCall(FunctionCallExpr* expr);

};

#endif //MVSCRIPTCOMPILER_COMPILER_H
