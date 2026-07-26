//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Compiler.h"
#include "AST.h"
#include <stdexcept>
#include <string>

// Constructor
Compiler::Compiler(ASTNode* Nodes) : AllNodes(Nodes) {};

// Main compile function
std::vector<Instruction> Compiler::compile(ASTNode* Nodes) {
  return instructions;
};


// Emit function
void Compiler::emit(ByteCode bc) {
  instructions.push_back(Instruction(bc));
};
void Compiler::emit(ByteCode bc, int operand) {
  instructions.push_back(Instruction(bc, operand));
};
void Compiler::emit(ByteCode bc, const std::string& strOperand) {
  instructions.push_back(Instruction(bc, strOperand));
};

// Tables funtions
int Compiler::addConstant(Value& v) {

};

int Compiler::resolveVariable(const std::string& name) {
  auto a = variableTable.find(name);
  if (a != variableTable.end()) {
    return a->second;
  }
  variableTable[name] = nextSlot;
  return nextSlot++;
};


// Compile routers functions
void Compiler::compileStatement(Statement* stmt) {
  if (auto* s = dynamic_cast<BlockStatement*>(stmt)) {
    compileBlock(s);
  } else if (auto* s = dynamic_cast<AssignmentStatement*>(stmt)) {
    compileAssignment(s);
  } else if (auto* s = dynamic_cast<IfStatement*>(stmt)) {
    compileIf(s);
  } else if (auto* s = dynamic_cast<WhileStatement*>(stmt)) {
    compileWhile(s);
  } else if (auto* s = dynamic_cast<ForStatement*>(stmt)) {
    compileFor(s);
  } else if (auto* s = dynamic_cast<FunctionStatement*>(stmt)) {
    compileFunction(s);
  } else if (auto* s = dynamic_cast<ExpressionStatement*>(stmt)) {
    compileExpressionStmt(s); // Needs remake!! (after all functions will be done)
  } else {
    throw std::runtime_error("Unknown statement type");
  }
};
void Compiler::compileExpession(Expression* expr) {
  if (auto* e = dynamic_cast<BinaryOpExpr*>(expr)) {
    compileBinaryOp(e);
  } else if (auto* e = dynamic_cast<LiteralExpr*>(expr)) {
    compileLiteral(e);
  } else if (auto* e = dynamic_cast<UnaryOpExpr*>(expr)) {
    compileUnaryOp(e);
  } else if (auto* e = dynamic_cast<IdentifierExpr*>(expr)) {
    compileIdentifier(e);
  } else if (auto* e = dynamic_cast<FunctionCallExpr*>(expr)) {
    compileCall(e);
  } else {
    throw std::runtime_error("Unknown expression type");
  }
};


// Statements functions
void Compiler::compileBlock(BlockStatement* block) {
  
};

void Compiler::compileAssignment(AssignmentStatement* stmt) {
  
};

void Compiler::compileIf(IfStatement* stmt) {

};

void Compiler::compileWhile(WhileStatement* stmt) {

};

void Compiler::compileFor(ForStatement* stmt) {

};

void Compiler::compileFunction(FunctionStatement* stmt) {

};

void Compiler::compileExpressionStmt(ExpressionStatement* stmt) {

};


// Expression functions
void Compiler::compileBinaryOp(BinaryOpExpr* expr) {

};

void Compiler::compileLiteral(LiteralExpr* expr) {

};

void Compiler::compileUnaryOp(UnaryOpExpr* expr) {
  
};

void Compiler::compileIdentifier(IdentifierExpr* expr) {

};

void Compiler::compileCall(FunctionCallExpr* expr) {

};
