#include "lexer.hpp"
#include <cctype>

char Lexer::peek() const {
    if (m_position >= m_input.length()) {
        return '\0';
    }
    return m_input[m_position];
}

char Lexer::advance() {
    return m_input[m_position++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) {
        advance();
    }
}

Token Lexer::number() {
    std::string result;
    while (std::isdigit(peek())) {
        result += advance();
    }
    return Token(TokenType::NUMBER, result);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (m_position < m_input.length()) {
        skipWhitespace();
        char current = peek();
        
        if (std::isdigit(current)) {
            tokens.push_back(number());
        } else {
            advance();
            switch (current) {
                case '+': tokens.push_back(Token(TokenType::PLUS)); break;
                case '-': tokens.push_back(Token(TokenType::MINUS)); break;
                case '*': tokens.push_back(Token(TokenType::MULTIPLY)); break;
                case '/': tokens.push_back(Token(TokenType::DIVIDE)); break;
                case '(': tokens.push_back(Token(TokenType::LPAREN)); break;
                case ')': tokens.push_back(Token(TokenType::RPAREN)); break;
                default: tokens.push_back(Token(TokenType::INVALID));
            }
        }
    }
    
    tokens.push_back(Token(TokenType::EOL));
    return tokens;
}