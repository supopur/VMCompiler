#include <fstream>
#include <iostream>
#include <vector>
#include "../include/Lexer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Bad usage of files as arguments\n";
        return 1;
    }

    std::ifstream programFile(argv[1]);

    if (!programFile.is_open()) {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    Lexer lexer(programFile);

    std::vector<Token> tokens = lexer.Tokenize();

    // Print tokens to stdout
    std::cout << "=== TOKENS ===" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (size_t i = 0; i < tokens.size(); i++) {
        const Token &token = tokens[i];

        std::cout << "Token #" << i << std::endl;
        std::cout << "  Type:   " << static_cast<int>(token.type) << std::endl;
        std::cout << "  Value:  \"" << token.value << "\"" << std::endl;
        std::cout << "  Line:   " << token.line << std::endl;
        std::cout << "  Column: " << token.column << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Total tokens: " << tokens.size() << std::endl;

    return 0;
}
