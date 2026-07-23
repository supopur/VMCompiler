cmake_minimum_required(VERSION 4.1)
project(MVScriptCompiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 26)

add_executable(MVScriptCompiler 
  src/main.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/CodeGen.cpp
  src/AST.cpp
)


target_include_directories(MVScriptCompiler PRIVATE include)
