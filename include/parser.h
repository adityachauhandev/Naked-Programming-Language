#pragma once
#include <vector>

void parser(std::vector<Token>& token);

enum class Sequence {
    INIT_NODE,
    BINARY_NODE,
    UNARY_NODE,
    UNARY_IF,
    BINARY_IF,
    ELSE_NODE
};

struct InitNode{
    std::string bytes;
    std::string value;
    std::string name;
};

struct BinaryNode{
    std::string id1;
    TokenType id1type;
    TokenType optype;
    std::string id2;
    TokenType id2type;
    std::string target;
};

struct UnaryNode{
    std::string id;
    TokenType idtype;
    std::string target;
};

struct UnaryIFNode{
    std::string id;
    TokenType idtype;
};

struct BinaryIFNode{
    std::string id1;
    TokenType id1type;
    TokenType cmptype;
    std::string id2;
    TokenType id2type;
};

