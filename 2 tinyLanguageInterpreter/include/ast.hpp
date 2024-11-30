#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Forward declaration
class Environment;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual int execute(Environment& env) = 0;
};

// Environment class to store variables
class Environment {
public:
    void define(const std::string& name, int value);
    int get(const std::string& name);
    void assign(const std::string& name, int value);

private:
    std::unordered_map<std::string, int> values;
};

// Specific node types
class NumberNode : public ASTNode {
public:
    explicit NumberNode(int value) : value(value) {}
    int execute(Environment& env) override;

private:
    int value;
};

class VariableNode : public ASTNode {
public:
    explicit VariableNode(std::string name) : name(std::move(name)) {}
    int execute(Environment& env) override;

private:
    std::string name;
};

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(std::string name, std::unique_ptr<ASTNode> value)
        : name(std::move(name)), value(std::move(value)) {}
    int execute(Environment& env) override;

private:
    std::string name;
    std::unique_ptr<ASTNode> value;
};

class PrintNode : public ASTNode {
public:
    explicit PrintNode(std::unique_ptr<ASTNode> expression)
        : expression(std::move(expression)) {}
    int execute(Environment& env) override;

private:
    std::unique_ptr<ASTNode> expression;
};

class IfNode : public ASTNode {
public:
    IfNode(std::unique_ptr<ASTNode> condition,
           std::vector<std::unique_ptr<ASTNode>> body)
        : condition(std::move(condition))
        , body(std::move(body)) {}
    int execute(Environment& env) override;

private:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;
};

class WhileNode : public ASTNode {
public:
    WhileNode(std::unique_ptr<ASTNode> condition,
              std::vector<std::unique_ptr<ASTNode>> body)
        : condition(std::move(condition))
        , body(std::move(body)) {}
    int execute(Environment& env) override;

private:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;
};

class ComparisonNode : public ASTNode {
public:
    enum class Op { Greater, Less };
    
    ComparisonNode(std::unique_ptr<ASTNode> left,
                   Op op,
                   std::unique_ptr<ASTNode> right)
        : left(std::move(left))
        , op(op)
        , right(std::move(right)) {}
    int execute(Environment& env) override;

private:
    std::unique_ptr<ASTNode> left;
    Op op;
    std::unique_ptr<ASTNode> right;
};