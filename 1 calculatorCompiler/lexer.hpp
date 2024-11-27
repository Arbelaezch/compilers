#pragma once
#include <string>
#include <vector>

enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    EOL,
    INVALID
};

// Token Structure. Each token has a type and a value.
struct Token {
    TokenType type;
    std::string value;
    
    Token(TokenType t, std::string v = "") : type(t), value(v) {}
};

// Lexer class. It reads the input and tokenizes it into tokens.
class Lexer {
public:
    explicit Lexer(std::string input) : m_input(std::move(input)), m_position(0) {}
    std::vector<Token> tokenize();

private:
    std::string m_input; // Stores the input string
    size_t m_position;   // Current position in the input string
    
    char peek() const;   // Returns the next character without advancing the position
    char advance();      // Advances the position and returns the next character
    void skipWhitespace();  // Skips whitespace characters
    Token number();      // Read/tokenize a complete number
};