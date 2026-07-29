//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Compiler.h"
#include "AST.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <fstream>
#include <cstdint>

// Constructor
Compiler::Compiler(ASTNode* Nodes) : AllNodes(Nodes) {};

// Main compile function
std::vector<Instruction> Compiler::compile() {
  auto* rootStatemnt = dynamic_cast<Statement*>(AllNodes);
  if (!rootStatemnt) {
    throw std::runtime_error("Root AST node is not a statement");
  }

  compileStatement(rootStatemnt);

  emit(ByteCode::HALT);

  return instructions;
};

void Compiler::serialize(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }

    // header
    out.write("MVBC", 4);

    int32_t version = 1;
    int32_t variableCount = nextSlot;
    int32_t constantCount = static_cast<int32_t>(constantTable.size());
    int32_t instructionCount = static_cast<int32_t>(instructions.size());

    out.write(reinterpret_cast<const char*>(&version), sizeof(version));
    out.write(reinterpret_cast<const char*>(&variableCount), sizeof(variableCount));
    out.write(reinterpret_cast<const char*>(&constantCount), sizeof(constantCount));
    out.write(reinterpret_cast<const char*>(&instructionCount), sizeof(instructionCount));

    // constants
    for (const auto& c : constantTable) {
        uint8_t typeTag = static_cast<uint8_t>(c.index()); // variant index: 0=double,1=string,2=bool
        out.write(reinterpret_cast<const char*>(&typeTag), sizeof(typeTag));

        if (auto* d = std::get_if<double>(&c)) {
            out.write(reinterpret_cast<const char*>(d), sizeof(double));
        } else if (auto* s = std::get_if<std::string>(&c)) {
            int32_t len = static_cast<int32_t>(s->size());
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(s->data(), len);
        } else if (auto* b = std::get_if<bool>(&c)) {
            uint8_t val = *b ? 1 : 0;
            out.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }

    // instructions
    for (const auto& instr : instructions) {
        uint8_t opcode = static_cast<uint8_t>(instr.bytecode);
        out.write(reinterpret_cast<const char*>(&opcode), sizeof(opcode));

        int32_t operand = instr.operand;
        out.write(reinterpret_cast<const char*>(&operand), sizeof(operand));

        uint8_t hasStr = instr.strOperand.empty() ? 0 : 1;
        out.write(reinterpret_cast<const char*>(&hasStr), sizeof(hasStr));

        if (hasStr) {
            int32_t len = static_cast<int32_t>(instr.strOperand.size());
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(instr.strOperand.data(), len);
        }
    }

    out.close();
}

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
int Compiler::addConstant(const Value& v) {
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
  } else if (auto* s = dynamic_cast<ReturnStatement*>(stmt)) {
    // TODO IMPLEMENT
    //compileReturn(s);
  } else if (auto* s = dynamic_cast<ExpressionStatement*>(stmt)) {
    compileExpressionStmt(s);
  } else {
    throw std::runtime_error("Unknown statement type");
  }
}

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
  compileExpession(stmt->value.get());
  int slot = resolveVariable(stmt->name);
  emit(ByteCode::STORE, slot);
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
  size_t loopStart = instructions.size();

  compileExpession(stmt->condition.get());

  size_t jumpToEndPos = instructions.size();
  emit(ByteCode::JUMP_IF_FALSE, 0);

  compileStatement(stmt->body.get());

  emit(ByteCode::JUMP, loopStart);

  instructions[jumpToEndPos].operand = instructions.size();
};

void Compiler::compileFor(ForStatement* stmt) {
  compileExpession(stmt->start.get());
  int slot = resolveVariable(stmt->var);
  emit(ByteCode::STORE, slot);

  size_t loopStart = instructions.size();

  emit(ByteCode::LOAD, slot);
  compileExpession(stmt->end.get());
  emit(ByteCode::LE);

  size_t jumpToEndPos = instructions.size();
  emit(ByteCode::JUMP_IF_FALSE, 0);

  compileStatement(stmt->body.get());

  emit(ByteCode::LOAD, slot);
  int oneIdx = addConstant(1.0);
  emit(ByteCode::PUSH_CONST, oneIdx);
  emit(ByteCode::ADD);
  emit(ByteCode::STORE, slot);

  emit(ByteCode::JUMP, loopStart);

  instructions[jumpToEndPos].operand = instructions.size();
};

void Compiler::compileFunction(FunctionStatement* stmt) {
  size_t jumpOverPos = instructions.size();
  emit(ByteCode::JUMP, 0);

  size_t functionStart = instructions.size();
  functionTable[stmt->name] = functionStart;

  for (const auto& paramExpr : stmt->params) {
    if (auto* idExpr = dynamic_cast<IdentifierExpr*>(paramExpr.get())) {
      resolveVariable(idExpr->name);
    } else {
      throw std::runtime_error("Function parameter must be an identifier");
    }
  }

  compileStatement(stmt->body.get());

  emit(ByteCode::RETURN);

  instructions[jumpOverPos].operand = instructions.size();
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
  emit(ByteCode::CALL, static_cast<int>(functionTable[expr->name]));
};
