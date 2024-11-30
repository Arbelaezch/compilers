#pragma once
#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    // Parsing methods for different constructs
    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<ASTNode> assignment();
    std::unique_ptr<ASTNode> ifStatement();
    std::unique_ptr<ASTNode> whileStatement();
    std::unique_ptr<ASTNode> printStatement();
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> comparison();
    std::vector<std::unique_ptr<ASTNode>> block();

    // Helper methods
    Token peek();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    bool isAtEnd();
};