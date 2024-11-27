#include "parser.hpp"

// Look at current token without consuming it
Token Parser::peek() const {
    if (isAtEnd()) return Token(TokenType::EOL);
    return m_tokens[m_current];
}

// Move to and return next token
Token Parser::advance() {
    if (!isAtEnd()) m_current++;
    return m_tokens[m_current - 1];
}

// Check if we've processed all tokens
bool Parser::isAtEnd() const {
    return m_current >= m_tokens.size() || 
           m_tokens[m_current].type == TokenType::EOL;
}

// Try to match and consume a token of given type
bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

// Check if current token is of given type
bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

// Main parsing entry point
ExprPtr Parser::parse() {
    return expression();
}

// Handle addition and subtraction
// This implements the grammar rule: expression → term (('+' | '-') term)*
ExprPtr Parser::expression() {
    // First get the left-hand term
    ExprPtr expr = term();
    
    // Keep processing while we see + or -
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        TokenType op = m_tokens[m_current - 1].type;
        ExprPtr right = term();  // Get the right-hand term
        // Create a new binary expression with the left, operator, and right
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

// Handle multiplication and division
// This implements the grammar rule: term → factor (('*' | '/') factor)*
ExprPtr Parser::term() {
    ExprPtr expr = factor();
    
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE)) {
        TokenType op = m_tokens[m_current - 1].type;
        ExprPtr right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

// Handle numbers (and eventually parentheses)
// This implements the grammar rule: factor → NUMBER
ExprPtr Parser::factor() {
    if (match(TokenType::NUMBER)) {
        // Create a number expression node
        // std::stod converts string to double
        return std::make_shared<NumberExpr>(
            std::stod(m_tokens[m_current - 1].value)
        );
    }
    
    throw std::runtime_error("Unexpected token");
}