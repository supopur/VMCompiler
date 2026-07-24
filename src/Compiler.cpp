//
// Created by Matouš Smékal on 22.07.2026.
//

#include "../include/Compiler.h"
#include "AST.h"

// Constructor
Compiler::Compiler(ASTNode* Nodes) {};

std::vector<Instruction> compile(ASTNode* Nodes) {

};

void Compiler::emit(ByteCode bc) {
  instructions.push_back(Instruction(bc));
};

void Compiler::emit(ByteCode bc, int operand) {
  instructions.push_back(Instruction(bc, operand));
};

void Compiler::emit(ByteCode bc, const std::string& strOperand) {
  instructions.push_back(Instruction(bc, strOperand));
};

