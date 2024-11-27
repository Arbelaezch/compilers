#pragma once
#include <string>
#include <sstream>
#include <memory>

/*
For numbers, simply load them into x0.

For binary operations, 
    1. Generates code for right operand
    2. Saves result to stack
    3. Generates code for left operand
    4. Retrieves right result from stack
    5. Performs operation

2+3*4 would generate:
    mov x0, #4       // Load 4
    str x0, [sp, #-16]!
    mov x0, #3       // Load 3
    ldr x1, [sp], #16
    mul x0, x0, x1   // 3 * 4
    str x0, [sp, #-16]!
    mov x0, #2       // Load 2
    ldr x1, [sp], #16
    add x0, x0, x1   // 2 + (3 * 4)

*/

class CodeGenerator {
public:
    CodeGenerator() : label_count(0) {}
    
    // Get the generated assembly code
    std::string getCode() const { return output.str(); }
    
    // Generate a unique label
    std::string newLabel() { 
        return "L" + std::to_string(label_count++); 
    }
    
    // Add a line of assembly code
    void emit(const std::string& line) {
        output << "    " << line << "\n";
    }

private:
    std::stringstream output;
    int label_count;
};