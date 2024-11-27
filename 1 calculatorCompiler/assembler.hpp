#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <map>

class Assembler {
public:
    // Converts a single instruction to machine code
    uint32_t assembleLine(const std::string& line);
    
    // Assembles full program
    std::vector<uint32_t> assemble(const std::vector<std::string>& lines);

private:
    // Different instruction formats
    uint32_t assembleMov(const std::string& line);
    uint32_t assembleArithmetic(const std::string& line);
    uint32_t assembleLoad(const std::string& line);
    uint32_t assembleStore(const std::string& line);
    
    // Helper functions
    int parseRegister(const std::string& reg);
    int parseImmediate(const std::string& imm);
};