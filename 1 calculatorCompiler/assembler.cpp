#include "assembler.hpp"
#include <regex>
#include <stdexcept>

uint32_t Assembler::assembleLine(const std::string& line) {
    // Skip empty lines and labels
    if (line.empty() || line[0] == '.' || line[0] == '_') {
        return 0;
    }
    
    // Extract instruction
    std::string instr = line.substr(0, line.find(' '));
    
    // Route to correct assembly function
    if (instr == "mov") {
        return assembleMov(line);
    }
    else if (instr == "add" || instr == "sub" || 
             instr == "mul" || instr == "sdiv") {
        return assembleArithmetic(line);
    }
    else if (instr == "ldr") {
        return assembleLoad(line);
    }
    else if (instr == "str") {
        return assembleStore(line);
    }
    
    throw std::runtime_error("Unknown instruction: " + instr);
}

uint32_t Assembler::assembleMov(const std::string& line) {
    // Match pattern: mov xN, #immediate
    std::regex pattern(R"(mov x(\d+),\s*#(\d+))");
    std::smatch matches;
    
    if (std::regex_search(line, matches, pattern)) {
        int rd = std::stoi(matches[1]); // Destination register
        int imm = std::stoi(matches[2]); // Immediate value
        
        // ARM64 MOV immediate encoding
        uint32_t instruction = 0xD2800000; // Base opcode for MOV
        instruction |= (rd & 0x1F);        // Add register number
        instruction |= ((imm & 0xFFFF) << 5); // Add immediate value
        
        return instruction;
    }
    
    throw std::runtime_error("Invalid MOV instruction format");
}

uint32_t Assembler::assembleArithmetic(const std::string& line) {
    // Match pattern: op xN, xM, xK
    std::regex pattern(R"((\w+)\s+x(\d+),\s*x(\d+),\s*x(\d+))");
    std::smatch matches;
    
    if (std::regex_search(line, matches, pattern)) {
        std::string op = matches[1];
        int rd = std::stoi(matches[2]); // Destination register
        int rn = std::stoi(matches[3]); // First source register
        int rm = std::stoi(matches[4]); // Second source register
        
        uint32_t instruction;
        if (op == "add") {
            instruction = 0x8B000000; // Base opcode for ADD
        }
        else if (op == "sub") {
            instruction = 0xCB000000; // Base opcode for SUB
        }
        else if (op == "mul") {
            instruction = 0x9B007C00; // Base opcode for MUL
        }
        else if (op == "sdiv") {
            instruction = 0x9AC00C00; // Base opcode for SDIV
        }
        else {
            throw std::runtime_error("Unknown arithmetic operation");
        }
        
        instruction |= (rd & 0x1F);        // Add destination register
        instruction |= ((rn & 0x1F) << 5); // Add first source register
        instruction |= ((rm & 0x1F) << 16); // Add second source register
        
        return instruction;
    }
    
    throw std::runtime_error("Invalid arithmetic instruction format");
}

std::vector<uint32_t> Assembler::assemble(const std::vector<std::string>& lines) {
    std::vector<uint32_t> machineCode;
    
    for (const auto& line : lines) {
        if (!line.empty() && line[0] != '.' && line[0] != '_') {
            uint32_t instruction = assembleLine(line);
            if (instruction != 0) {
                machineCode.push_back(instruction);
            }
        }
    }
    
    return machineCode;
}

uint32_t Assembler::assembleLoad(const std::string& line) {
    // Match pattern: ldr xN, [sp], #imm
    std::regex pattern(R"(ldr x(\d+),\s*\[sp\],\s*#(\d+))");
    std::smatch matches;
    
    if (std::regex_search(line, matches, pattern)) {
        int rt = std::stoi(matches[1]);    // Target register
        int imm = std::stoi(matches[2]);   // Immediate offset
        
        // ARM64 LDR immediate post-indexed encoding
        uint32_t instruction = 0xF8400400;  // Base opcode for LDR
        instruction |= (rt & 0x1F);         // Add target register
        instruction |= ((imm/8 & 0x1FF) << 12); // Add immediate value (scaled)
        
        return instruction;
    }
    
    throw std::runtime_error("Invalid LDR instruction format");
}

uint32_t Assembler::assembleStore(const std::string& line) {
    // Match pattern: str xN, [sp, #-imm]!
    std::regex pattern(R"(str x(\d+),\s*\[sp,\s*#-(\d+)\]!)");
    std::smatch matches;
    
    if (std::regex_search(line, matches, pattern)) {
        int rt = std::stoi(matches[1]);    // Source register
        int imm = std::stoi(matches[2]);   // Immediate offset
        
        // ARM64 STR immediate pre-indexed encoding
        uint32_t instruction = 0xF8000C00;  // Base opcode for STR
        instruction |= (rt & 0x1F);         // Add source register
        instruction |= ((imm/8 & 0x1FF) << 12); // Add immediate value (scaled)
        
        return instruction;
    }
    
    throw std::runtime_error("Invalid STR instruction format");
}