#include "token.hpp"

Token::Token(TokenType type, std::string value, int line)
    : type(type), value(std::move(value)), line(line) {}