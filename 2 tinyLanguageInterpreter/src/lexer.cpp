#include "lexer.hpp"
#include <cctype>
#include <unordered_map>

// Define keywords that our language supports
// These will be recognized as special tokens rather than regular identifiers
static std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::IF},
    {"while", TokenType::WHILE},
    {"print", TokenType::PRINT}
};

// Constructor: Initialize lexer with source code
Lexer::Lexer(std::string source) : source(std::move(source)) {}

// Main tokenization method that processes the entire source code
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        // Skip any whitespace between tokens
        skipWhitespace();
        if (isAtEnd()) break;

        // Peek at the next character to determine token type
        char c = peek();
        
        // Handle numbers (integer literals)
        if (std::isdigit(c)) {
            tokens.push_back(number());
        }
        // Handle identifiers (variable names) and keywords
        else if (std::isalpha(c)) {
            tokens.push_back(identifier());
        }
        // Handle single-character tokens
        else {
            advance(); // Consume the character
            switch (c) {
                case '=': tokens.emplace_back(TokenType::EQUALS, "=", line); break;
                case '>': tokens.emplace_back(TokenType::GREATER, ">", line); break;
                case '<': tokens.emplace_back(TokenType::LESS, "<", line); break;
                case '-': tokens.emplace_back(TokenType::MINUS, "-", line); break;
                case '\n': 
                    // Track end of lines for proper indentation and scope management
                    tokens.emplace_back(TokenType::EOL, "\\n", line);
                    line++; // Increment line counter for error reporting
                    break;
                default:
                    // In a production lexer, you'd want to handle unexpected characters
                    // by throwing an error with line number and character information
                    break;
            }
        }
    }

    // Add final END token to signify end of input
    tokens.emplace_back(TokenType::END, "", line);
    return tokens;
}

// Advance to next character in source and return current character
char Lexer::advance() {
    return source[position++];
}

// Look at next character without consuming it
char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[position];
}

// Skip over spaces and tabs, but not newlines
// Newlines are important for our language's structure
void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek()) && peek() != '\n') {
        advance();
    }
}

// Process and return a complete number token
Token Lexer::number() {
    std::string num;
    // Collect all consecutive digits
    while (!isAtEnd() && std::isdigit(peek())) {
        num += advance();
    }
    return Token(TokenType::NUMBER, num, line);
}

// Process and return an identifier or keyword token
Token Lexer::identifier() {
    std::string id;
    // Collect characters that can be part of an identifier
    // We allow alphanumeric characters and underscore
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        id += advance();
    }
    
    // Check if the identifier is actually a keyword
    auto it = keywords.find(id);
    if (it != keywords.end()) {
        // If it's a keyword, return the corresponding keyword token
        return Token(it->second, id, line);
    }
    
    // If it's not a keyword, it's a regular identifier (variable name)
    return Token(TokenType::IDENTIFIER, id, line);
}

// Check if we've reached the end of the source code
bool Lexer::isAtEnd() {
    return position >= source.length();
}