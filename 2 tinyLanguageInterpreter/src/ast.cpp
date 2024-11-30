#include "ast.hpp"
#include <iostream>
#include <stdexcept>

// Environment methods
void Environment::define(const std::string& name, int value) {
    values[name] = value;
}

int Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it == values.end()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return it->second;
}

void Environment::assign(const std::string& name, int value) {
    if (values.find(name) == values.end()) {
        throw std::runtime_error("Cannot assign to undefined variable: " + name);
    }
    values[name] = value;
}

// Node execution methods
int NumberNode::execute(Environment& env) {
    return value;
}

int VariableNode::execute(Environment& env) {
    return env.get(name);
}

int AssignmentNode::execute(Environment& env) {
    int val = value->execute(env);
    env.define(name, val);
    return val;
}

int PrintNode::execute(Environment& env) {
    int val = expression->execute(env);
    std::cout << val << std::endl;
    return val;
}

int IfNode::execute(Environment& env) {
    if (condition->execute(env)) {
        for (const auto& stmt : body) {
            stmt->execute(env);
        }
    }
    return 0;  // Return value not used for control flow
}

int WhileNode::execute(Environment& env) {
    while (condition->execute(env)) {
        for (const auto& stmt : body) {
            stmt->execute(env);
        }
    }
    return 0;  // Return value not used for control flow
}

int ComparisonNode::execute(Environment& env) {
    int l = left->execute(env);
    int r = right->execute(env);
    
    switch (op) {
        case Op::Greater:
            return l > r;
        case Op::Less:
            return l < r;
    }
    return 0;  // Shouldn't reach here
}