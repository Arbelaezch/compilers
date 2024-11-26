# Compiler Learning Project

## Overview

This repository contains a series of increasingly complex compiler implementations, built as a learning exercise to understand compiler design and implementation. Each subdirectory represents a different compiler project, starting from basic concepts and progressively adding more advanced features.

## Project Structure & Learning Path

1. **Calculator Compiler**
   - Basic arithmetic expression evaluation
   - Introduction to lexical analysis (tokenization)
   - Simple parsing concepts
   - Understanding compiler pipelines
   - Building an Abstract Syntax Tree (AST)

2. **Tiny Language Interpreter**
   - Variables and assignment operations
   - Control flow (if statements, while loops)
   - Symbol table implementation
   - Scope management
   - Basic interpreter patterns

3. **Stack-Based Compiler**
   - Stack machine architecture
   - Bytecode generation
   - Basic arithmetic and variable operations
   - Understanding stack-based computation
   - Introduction to intermediate representations

4. **Basic Procedural Language**
   - Function definitions and calls
   - Scope rules and variable binding
   - Basic type checking
   - Control flow analysis
   - Introduction to LLVM IR

5. **Full Language Compiler**
   - Complex data structures (structs/objects)
   - Type system implementation
   - Basic optimizations
   - Real assembly generation (x86/ARM)
   - Advanced compiler techniques

## Getting Started

### Prerequisites

- C++ compiler (g++ or clang++)
- CMake (version 3.15 or higher)

### Building the Projects

Each project contains its own build instructions in its respective directory. Generally, you can build any project using:

```bash
cd [project-directory]
mkdir build
cd build
cmake ..
make
```

## Learning Objectives

This project aims to understand:

1. How compilers break down source code into tokens
2. How to parse these tokens into meaningful structures
3. How to generate executable code from these structures
4. Different optimization techniques
5. Various compiler architectures and designs
