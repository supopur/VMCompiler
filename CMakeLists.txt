cmake_minimum_required(VERSION 4.1)
project(MVScriptCompiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(MVScriptCompiler src/main.cpp
        src/lexer.cpp
        include/lexer.h
        src/parser.cpp
        include/parser.h
        src/codegen.cpp
        include/codegen.h
        src/ast.cpp
        include/ast.h)
