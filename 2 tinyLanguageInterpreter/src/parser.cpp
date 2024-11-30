#include "parser.hpp"
#include <stdexcept>

/*
Parser Overview:
---------------
The parser implements a recursive descent parsing strategy for the Tiny Language.
It converts a flat sequence of tokens into a hierarchical Abstract Syntax Tree (AST).

Main Components and Flow:
1. parse() - Entry point that processes the entire program
2. statement() - Dispatcher that determines the type of statement and calls appropriate handler
3. Specialized parsers for each construct:
   - assignment() - Handles variable assignments (x = value)
   - ifStatement() - Processes if conditions and their blocks
   - whileStatement() - Handles while loops and their blocks
   - printStatement() - Manages print statements
   - expression() - Processes basic expressions (numbers and variables)
   - comparison() - Handles comparison operations (>, <)
   - block() - Processes indented blocks of code

Grammar Rules (in EBNF-like notation):
program    → statement* END
statement  → assignment | ifStatement | whileStatement | printStatement
assignment → IDENTIFIER "=" expression EOL
ifStatement → "if" comparison EOL block
whileStatement → "while" comparison EOL block
printStatement → "print" expression EOL
expression → NUMBER | IDENTIFIER
comparison → expression (">" | "<") expression
block      → statement+
*/

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    // Process statements until we reach the end of the file
    while (!isAtEnd() && peek().type != TokenType::END) {
        try {
            statements.push_back(statement());
            // Each statement should end with a newline
            match(TokenType::EOL);
        } catch (const std::runtime_error& e) {
            // Error recovery would go here in a production interpreter
            throw;
        }
    }
    
    return statements;
}

std::unique_ptr<ASTNode> Parser::statement() {
    // Dispatcher function that determines the type of statement
    // and calls the appropriate specialized parser
    if (match(TokenType::IF)) {
        return ifStatement();
    }
    if (match(TokenType::WHILE)) {
        return whileStatement();
    }
    if (match(TokenType::PRINT)) {
        return printStatement();
    }
    // If no keyword is matched, assume it's an assignment
    return assignment();
}

std::unique_ptr<ASTNode> Parser::assignment() {
    // Parse assignment statement: IDENTIFIER = expression
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
    consume(TokenType::EQUALS, "Expected '=' after variable name.");
    
    // Parse the value being assigned
    auto value = expression();
    return std::make_unique<AssignmentNode>(name.value, std::move(value));
}

std::unique_ptr<ASTNode> Parser::ifStatement() {
    // Parse if statement: "if" comparison EOL block
    
    // First parse the condition
    auto condition = comparison();
    consume(TokenType::EOL, "Expected newline after condition.");
    
    // Then parse the indented block of code
    auto body = block();
    
    return std::make_unique<IfNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::whileStatement() {
    // Parse while statement: "while" comparison EOL block
    
    // Similar to if statement, first parse condition
    auto condition = comparison();
    consume(TokenType::EOL, "Expected newline after condition.");
    
    // Then parse the indented block of code
    auto body = block();
    
    return std::make_unique<WhileNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::printStatement() {
    // Parse print statement: "print" expression
    auto value = expression();
    return std::make_unique<PrintNode>(std::move(value));
}

std::unique_ptr<ASTNode> Parser::expression() {
    // Currently handles only simple expressions:
    // - Number literals
    // - Variable references
    
    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberNode>(std::stoi(tokens[current - 1].value));
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<VariableNode>(tokens[current - 1].value);
    }
    
    throw std::runtime_error("Expected expression.");
}

std::unique_ptr<ASTNode> Parser::comparison() {
    // Parse comparison: expression ('>' | '<') expression
    
    // Get left side of comparison
    auto left = expression();
    
    // Look for comparison operator and build appropriate node
    if (match(TokenType::GREATER)) {
        auto right = expression();
        return std::make_unique<ComparisonNode>(
            std::move(left),
            ComparisonNode::Op::Greater,
            std::move(right)
        );
    }
    
    if (match(TokenType::LESS)) {
        auto right = expression();
        return std::make_unique<ComparisonNode>(
            std::move(left),
            ComparisonNode::Op::Less,
            std::move(right)
        );
    }
    
    throw std::runtime_error("Expected comparison operator.");
}

std::vector<std::unique_ptr<ASTNode>> Parser::block() {
    // Parse a block of statements
    // In our language, a block is a sequence of indented statements
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    while (!isAtEnd() && peek().type != TokenType::END) {
        // This is a simplified indentation check
        // A real interpreter would need more robust indentation handling
        if (peek().type != TokenType::IDENTIFIER && 
            peek().type != TokenType::PRINT) {
            break;
        }
        
        statements.push_back(statement());
        match(TokenType::EOL);  // Handle end of line
    }
    
    if (statements.empty()) {
        throw std::runtime_error("Expected at least one statement in block.");
    }
    
    return statements;
}

// Helper methods for token management

Token Parser::peek() {
    // Look at current token without consuming it
    if (isAtEnd()) return tokens.back();
    return tokens[current];
}

Token Parser::advance() {
    // Move to next token and return the current one
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    // Check if current token matches expected type
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    // Check if current token matches and consume it if so
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    // Consume token of expected type or throw error
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

bool Parser::isAtEnd() {
    // Check if we've reached the end of the token stream
    return current >= tokens.size();
}