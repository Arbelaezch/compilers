#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include "assembler.hpp"
#include "linker.hpp"
#include <iostream>
#include <fstream>

int main() {
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "exit") break;
        
        try {
            // Lexical analysis
            Lexer lexer(input);
            auto tokens = lexer.tokenize();
            
            // Parsing
            Parser parser(tokens);
            auto expr = parser.parse();
            
            // Code generation
            CodeGenerator codegen;
            expr->generateCode(codegen);
            
            // Split assembly into lines
            std::string assembly = codegen.getCode();
            std::vector<std::string> lines;
            std::stringstream ss(assembly);
            std::string line;
            while (std::getline(ss, line)) {
                if (!line.empty()) {
                    lines.push_back(line);
                }
            }
            
            // Assembly
            Assembler assembler;
            std::vector<uint32_t> machineCode = assembler.assemble(lines);
            
            // // Output machine code (for demonstration)
            // std::cout << "\nMachine code:\n";
            // for (uint32_t instruction : machineCode) {
            //     std::cout << std::hex << instruction << std::dec << "\n";
            // }
            // Create symbols for our code
        
            std::vector<Symbol> symbols = {
                {"_main", 0, false}  // Our entry point
            };
            
            // Create relocations if we need any
            std::vector<Relocation> relocations;
            
            // Create linker and add our object code
            Linker linker;
            linker.addObjectFile(machineCode, symbols, relocations);
            
            // Create executable
            linker.createExecutable("calculator");
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    
    return 0;
}