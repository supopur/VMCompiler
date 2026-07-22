#include <iostream>
#include "../include/Lexer.h"

int main() {
    // Example Lua-like code snippet
    std::string sourceCode = R"(
        func add(a, b)
            return a + b
        end

        if x == 10 then
            print("x is 10")
        else
            print("x is not 10")
        end

        for i in 1..10 do
            x = i * 2
        end
    )";

    try {
        // Create lexer with source code
        Lexer lexer(sourceCode);

        // Tokenize the source code
        std::vector<Token> tokens = lexer.Tokenize();

        // Print tokens to stdout
        std::cout << "=== TOKENS ===" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (size_t i = 0; i < tokens.size(); i++) {
            const Token& token = tokens[i];

            std::cout << "Token #" << i << std::endl;
            std::cout << "  Type:   " << static_cast<int>(token.type) << std::endl;
            std::cout << "  Value:  \"" << token.value << "\"" << std::endl;
            std::cout << "  Line:   " << token.line << std::endl;
            std::cout << "  Column: " << token.column << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Total tokens: " << tokens.size() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}