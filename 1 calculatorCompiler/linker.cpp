#include "linker.hpp"
#include <fstream>
#include <stdexcept>

Linker::Linker() {
    // Add default symbols for common runtime functions
    symbolTable["_start"] = Symbol{"_start", 0x0, false};
    symbolTable["_printf"] = Symbol{"_printf", 0x0, true};
}

void Linker::addObjectFile(const std::vector<uint32_t>& code,
                          const std::vector<Symbol>& symbols,
                          const std::vector<Relocation>& fileRelocations) {
    // Store the object code
    objectCode.push_back(code);
    
    // Add symbols to symbol table
    for (const auto& sym : symbols) {
        symbolTable[sym.name] = sym;
    }
    
    // Store relocations
    relocations.insert(relocations.end(), 
                      fileRelocations.begin(), 
                      fileRelocations.end());
}

void Linker::createExecutable(const std::string& outputPath) {
    // Resolve all symbol addresses
    resolveSymbols();
    
    // Apply relocations
    applyRelocations();
    
    // Write Mach-O file
    writeMachOFile(outputPath);
}

void Linker::resolveSymbols() {
    uint64_t currentAddress = 0x100000000; // Standard macOS start address
    
    // Assign addresses to all symbols
    for (auto& [name, symbol] : symbolTable) {
        if (!symbol.isExternal) {
            symbol.address = currentAddress;
            currentAddress += sizeof(uint32_t); // Each instruction is 4 bytes
        }
    }
}

void Linker::applyRelocations() {
    // Apply each relocation
    for (const auto& reloc : relocations) {
        auto symbolIt = symbolTable.find(reloc.symbol);
        if (symbolIt == symbolTable.end()) {
            throw std::runtime_error("Undefined symbol: " + reloc.symbol);
        }
        
        // Calculate and apply the relocation
        uint64_t targetAddress = symbolIt->second.address;
        // ... apply relocation to the correct object code
    }
}

void Linker::writeMachOFile(const std::string& outputPath) {
    std::ofstream file(outputPath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create output file");
    }
    
    // Write Mach-O header
    // ... write proper Mach-O headers
    
    // Write segments and sections
    // ... write code and data segments
    
    // Write symbol table
    // ... write symbols
    
    // Write string table
    // ... write symbol names
    
    // Write the actual code
    for (const auto& code : objectCode) {
        file.write(reinterpret_cast<const char*>(code.data()),
                  code.size() * sizeof(uint32_t));
    }
}