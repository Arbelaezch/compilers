#pragma once
#include <string>

enum class TokenType {
    NUMBER,     // Integer literals
    IDENTIFIER, // Variable names
    EQUALS,     // =
    GREATER,    // >
    LESS,       // <
    MINUS,      // -
    IF,         // if keyword
    WHILE,      // while keyword
    PRINT,      // print keyword
    EOL,        // End of line
    END         // End of file
};

class Token {
public:
    Token(TokenType type, std::string value = "", int line = 0);
    
    TokenType type;
    std::string value;
    int line;
};