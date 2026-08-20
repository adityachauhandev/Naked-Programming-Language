#pragma once
#include <vector>

void parser(std::vector<Token>& token);

enum class Sequence {
    VAR_NODE
};

struct VarInitNode{
    std::string name;
    int bytes;
    int value;
};
