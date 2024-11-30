#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

int main() {
    // Example program
    std::string source = 
        "x = 5\n"
        "if x > 3\n"
        "  print x\n"
        "  while x > 0\n"
        "    print x\n"
        "    x = x - 1\n";

    try {
        // Create lexer and get tokens
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // Parse tokens into AST
        Parser parser(tokens);
        auto statements = parser.parse();

        // Execute the program
        Environment env;
        for (const auto& stmt : statements) {
            stmt->execute(env);
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}