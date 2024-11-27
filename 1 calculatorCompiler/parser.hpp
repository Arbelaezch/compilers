#pragma once  // This prevents multiple inclusions of this header file
#include "lexer.hpp"
#include "codegen.hpp"
#include <memory>  // Needed for smart pointers (std::shared_ptr)

// Forward declarations - Tell compiler these classes will exist
// without fully defining them yet
class Expression;
// Create an alias 'ExprPtr' for shared_ptr<Expression>
// shared_ptr is C++'s smart pointer that automatically manages memory
using ExprPtr = std::shared_ptr<Expression>;

// Abstract base class for all expression types
class Expression {
public:
    // virtual destructor allows proper cleanup of derived classes
    virtual ~Expression() = default;
    // Pure virtual function (like an abstract method in other languages)
    // = 0 means derived classes MUST implement this function
    virtual double evaluate() const = 0;
    virtual void generateCode(CodeGenerator& gen) = 0;
};

// Concrete class for number literals (like "5" in an expression)
class NumberExpr : public Expression {  // 'public Expression' means inheritance
    double value;
public:
    // explicit prevents implicit conversions
    // Constructor takes a double and stores it
    explicit NumberExpr(double v) : value(v) {}
    // Override the pure virtual function from base class
    // 'override' keyword ensures we're actually overriding a base class method
    double evaluate() const override { return value; }
    
    void generateCode(CodeGenerator& gen) override {
        // Load immediate value into x0
        gen.emit("mov x0, #" + std::to_string(static_cast<int>(value)));
    }
};

// Concrete class for binary operations (like 2 + 3)
class BinaryExpr : public Expression {
    ExprPtr left;    // Left side of operation (e.g., '2' in '2 + 3')
    ExprPtr right;   // Right side of operation (e.g., '3' in '2 + 3')
    TokenType op;    // The operator (e.g., PLUS for '+')
public:
    // Constructor: note use of std::move to transfer ownership of smart pointers
    BinaryExpr(ExprPtr l, TokenType o, ExprPtr r)
        : left(std::move(l)), right(std::move(r)), op(o) {}
    
    double evaluate() const override {
        // Recursively evaluate left and right expressions
        double l = left->evaluate();
        double r = right->evaluate();
        // Perform the actual operation
        switch (op) {
            case TokenType::PLUS: return l + r;
            case TokenType::MINUS: return l - r;
            case TokenType::MULTIPLY: return l * r;
            case TokenType::DIVIDE: return l / r;
            default: throw std::runtime_error("Unknown operator");
        }
    }

    void generateCode(CodeGenerator& gen) override {
        // Generate code for right side first
        right->generateCode(gen);
        // Save right result to stack
        gen.emit("str x0, [sp, #-16]!");  // Pre-decrement sp by 16
        
        // Generate code for left side
        left->generateCode(gen);
        
        // Load right result back
        gen.emit("ldr x1, [sp], #16");    // Post-increment sp by 16
        
        // Perform operation
        switch (op) {
            case TokenType::PLUS:
                gen.emit("add x0, x0, x1");
                break;
            case TokenType::MINUS:
                gen.emit("sub x0, x0, x1");
                break;
            case TokenType::MULTIPLY:
                gen.emit("mul x0, x0, x1");
                break;
            case TokenType::DIVIDE:
                gen.emit("sdiv x0, x0, x1");
                break;
            default:
                throw std::runtime_error("Unknown operator");
        }
    }
};

// Parser class that builds the expression tree
class Parser {
public:
    // Constructor takes vector of tokens by const reference (efficient, can't modify)
    explicit Parser(const std::vector<Token>& tokens)
        : m_tokens(tokens), m_current(0) {}
    
    ExprPtr parse();  // Main entry point for parsing

private:
    const std::vector<Token>& m_tokens;  // Store reference to tokens
    size_t m_current;                    // Current position in token stream

    // Helper functions declarations
    Token peek() const;    // Look at current token
    Token advance();       // Move to next token
    bool isAtEnd() const;  // Check if we're done
    
    // Recursive descent parser functions
    ExprPtr expression();  // Handle + and -
    ExprPtr term();       // Handle * and /
    ExprPtr factor();     // Handle numbers
    
    // Token matching helpers
    bool match(TokenType type);
    bool check(TokenType type) const;
};