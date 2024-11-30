#pragma once
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position = 0;
    size_t line = 1;

    char advance();
    char peek();
    void skipWhitespace();
    Token number();
    Token identifier();
    bool isAtEnd();
};