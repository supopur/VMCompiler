//
// Created by Matouš Smékal on 22.07.2026.
//

#ifndef MVSCRIPTCOMPILER_CODEGEN_H
#define MVSCRIPTCOMPILER_CODEGEN_H

#include "AST.h"
#include <string>
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
  AND, OR, NOT,
  // Bit operations
  BAND, BOR, BXOR, BNOT, SHL, SHR,
  // Jumps
  JUMP, JUMP_IF_FALSE, JUMP_IF_TRUE,
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

  std::vector<Instruction> compile(ASTNode* Nodes);

private:
  std::vector<Instruction> instructions;
  std::vector<Value> constants;

  ASTNode* AllNodes;

  // Emit overloads
  void emit(ByteCode bc);
  void emit(ByteCode bc, int operand);
  void emit(ByteCode bc, const std::string& strOperand);

};

#endif //MVSCRIPTCOMPILER_COMPILER_H
