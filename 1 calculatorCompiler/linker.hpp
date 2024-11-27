#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdint>

struct Symbol {
    std::string name;
    uint64_t address;
    bool isExternal;
};

struct Relocation {
    uint64_t offset;    // Where to apply the relocation
    std::string symbol; // Symbol to link to
    uint32_t type;      // Type of relocation
};

class Linker {
public:
    Linker();
    
    // Add an object file to be linked
    void addObjectFile(const std::vector<uint32_t>& code,
                      const std::vector<Symbol>& symbols,
                      const std::vector<Relocation>& relocations);
                      
    // Add a library to link against
    void addLibrary(const std::string& libraryPath);
    
    // Create final executable
    void createExecutable(const std::string& outputPath);

private:
    // Object code from all files
    std::vector<std::vector<uint32_t>> objectCode;
    
    // Symbol table
    std::map<std::string, Symbol> symbolTable;
    
    // Relocation entries
    std::vector<Relocation> relocations;
    
    // Helper functions
    void resolveSymbols();
    void applyRelocations();
    void writeMachOFile(const std::string& outputPath);
};