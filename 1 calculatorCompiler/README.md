# Calculator Compiler

## Overview

This program implements a basic compiler that processes arithmetic expressions. It demonstrates the fundamental phases of compilation by taking a mathematical expression (like "2 + 3 * 4") and transforming it through various stages to create an executable program.

The implementation follows a traditional compiler pipeline, converting high-level mathematical expressions into machine code through several distinct phases. This project serves as an introduction to compiler construction, demonstrating core concepts like lexical analysis, parsing, and code generation.

## Compilation Phases

### 1. Lexical Analysis (Lexer)

The lexer is the first phase of compilation, responsible for breaking down the input string into meaningful tokens.

**Implementation:**

- Reads input character by character
- Groups characters into tokens (numbers, operators)
- Each token has a type (NUMBER, PLUS, MINUS, etc.) and sometimes a value
- Handles whitespace and basic error detection

Example:

```
Input: "2 + 3 * 4"
Tokens: [NUMBER(2)] [PLUS] [NUMBER(3)] [MULTIPLY] [NUMBER(4)]
```

### 2. Parsing

The parser takes the stream of tokens and constructs an Abstract Syntax Tree (AST) that represents the mathematical expression while respecting operator precedence.

**Implementation:**

- Uses recursive descent parsing
- Builds a tree structure from tokens
- Handles operator precedence naturally through tree structure
- Each node in the tree represents an operation or value

Example AST for "2 + 3 * 4":

```
    +
   / \
  2   *
     / \
    3   4
```

### 3. Code Generation

The code generator traverses the AST and produces ARM64 assembly code that can be executed on the target machine.

**Implementation:**

- Converts AST nodes into assembly instructions
- Manages registers and stack operations
- Generates code that preserves operator precedence
- Produces human-readable assembly code

Example output:

```assembly
mov x0, #4    // Load 4
str x0, [sp, #-16]!
mov x0, #3    // Load 3
ldr x1, [sp], #16
mul x0, x0, x1    // 3 * 4
str x0, [sp, #-16]!
mov x0, #2    // Load 2
ldr x1, [sp], #16
add x0, x0, x1    // 2 + (3 * 4)
```

### 4. Assembly

The assembler converts the generated assembly code into machine code (binary instructions) that can be executed by the CPU.

**Implementation:**

- Converts assembly mnemonics to binary instructions
- Handles different instruction formats
- Manages instruction encoding for ARM64 architecture
- Produces binary output suitable for execution

Example:

```
mov x0, #2  →  11010010 10000000 00000000 00000010
```

### 5. Linking

The linker combines the generated machine code with necessary system libraries and creates an executable file.

**Implementation:**

- Resolves symbol references
- Handles relocations
- Creates proper Mach-O executable format (for macOS)
- Links with system libraries as needed

## Usage

```bash
# Build the project
mkdir build
cd build
cmake ..
make

# Run the calculator
./calc_compiler
```

Enter arithmetic expressions when prompted, for example:

```
> 2 + 3 * 4
14
```

## Limitations

- Only handles basic arithmetic operations (+, -, *, /)
- No support for variables or functions
- Limited error handling
- No optimization passes
