//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Compiler.h"
#include "AST.h"
#include <cstddef>
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
  for (size_t i = 0; i < constantTable.size(); i++) {
    if (constantTable[i] == v) {
      return static_cast<int>(i);
    }
  }
  constantTable.push_back(v);
  return static_cast<int>(constantTable.size() - 1);
};

int Compiler::resolveVariable(const std::string& name) {
  auto a = variableTable.find(name);
  if (a != variableTable.end()) {
    return a->second;
  }
  variableTable[name] = nextSlot;
  return nextSlot++;
};


ByteCode Compiler::binaryOpToByteCode(BinaryOp op) {
  switch(op) {
    case BinaryOp::ADD: return ByteCode::ADD;
    case BinaryOp::SUB: return ByteCode::SUB;
    case BinaryOp::MUL: return ByteCode::MUL;
    case BinaryOp::DIV: return ByteCode::DIV;
    case BinaryOp::MOD: return ByteCode::MOD;
    case BinaryOp::EQ: return ByteCode::EQ;
    case BinaryOp::NEQ: return ByteCode::NEQ;
    case BinaryOp::LT: return ByteCode::LT;
    case BinaryOp::GT: return ByteCode::GT;
    case BinaryOp::LE: return ByteCode::LE;
    case BinaryOp::GE: return ByteCode::GE;
    case BinaryOp::AND: return ByteCode::AND;
    case BinaryOp::OR: return ByteCode::OR;
  }
}

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
    compileExpressionStmt(s); 
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
  for (auto& stmt : block->statements) {
    compileStatement(stmt.get());
  }
};

void Compiler::compileAssignment(AssignmentStatement* stmt) {
  
};

void Compiler::compileIf(IfStatement* stmt) {
  compileExpession(stmt->condition.get());

  size_t jumpToElsePos = instructions.size();
  emit(ByteCode::JUMP_IF_FALSE, 0);

  compileStatement(stmt->thenBlock.get());

  if (stmt->elseBlock) {
    size_t jumpToElsePos = instructions.size();
    emit(ByteCode::JUMP, 0);

    instructions[jumpToElsePos].operand = instructions.size();
    compileStatement(stmt->elseBlock.get());
    instructions[jumpToElsePos].operand = instructions.size();
  } else {
    instructions[jumpToElsePos].operand = instructions.size();
  }
};

void Compiler::compileWhile(WhileStatement* stmt) {

};

void Compiler::compileFor(ForStatement* stmt) {

};

void Compiler::compileFunction(FunctionStatement* stmt) {

};

void Compiler::compileExpressionStmt(ExpressionStatement* stmt) {
  compileExpession(stmt->expression.get());
  emit(ByteCode::POP);
};


// Expression functions
void Compiler::compileBinaryOp(BinaryOpExpr* expr) {
    compileExpession(expr->left.get());
    compileExpession(expr->right.get());
    emit(binaryOpToByteCode(expr->op));
};

void Compiler::compileLiteral(LiteralExpr* expr) {
  int idx = addConstant(expr->value);
  emit(ByteCode::LOAD, idx);
};

void Compiler::compileUnaryOp(UnaryOpExpr* expr) {
  compileExpession(expr->operand.get());
  if (expr->op == UnaryOp::NOT) {
    emit(ByteCode::NOT);
  } else if (expr->op == UnaryOp::NEGATE) {
    emit(ByteCode::NEGATE);
  } else {
    throw std::runtime_error("Unknown unary operator");
  }
};

void Compiler::compileIdentifier(IdentifierExpr* expr) {
  int slot = resolveVariable(expr->name);
  emit(ByteCode::LOAD, slot);
};

void Compiler::compileCall(FunctionCallExpr* expr) {
  for (auto& arg : expr->args) {
    compileExpession(arg.get());
  }
  emit(ByteCode::CALL, expr->name);
};
