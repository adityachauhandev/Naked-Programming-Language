#pragma once
#include <vector>
#include <string>

#include "token.h"

enum class Sequence {
    INIT_NODE,
    UPDATE_NODE,
    BEGIN,END,
    BEGIN_MAIN,END_MAIN,BEGIN_IF,END_IF,BEGIN_ELSE,END_ELSE,BEGIN_LOOP,END_LOOP,
    ELSE_NODE,
    FLOW_NODE,
    PRINT_NODE
};

struct Expression{
    std::string id1;
    TokenType id1type;
    TokenType optype;
    std::string id2;
    TokenType id2type;
};

struct InitNode{
    std::string bytes;
    std::string value;
    std::string name;
};

struct UpdateNode{
    Expression expr;
    std::string target;
};

struct FlowNode{
    Expression expr;
    TokenType flow_tp;
};

struct onscArg{
    std::string value;
    TokenType tp;
};

struct onscNode{
    std::string str;
    std::vector<onscArg> arg_vec;
};

struct ParserOutput{
    std::vector<Sequence> node_sequence_vector;
    std::vector<int> node_combined_index;
    std::vector<InitNode> init_node_vector;
    std::vector<UpdateNode> update_node_vector;
    std::vector<FlowNode> flow_node_vector;
    std::vector<onscNode> onsc_node_vector;
};

class Parser {
private:
    std::vector<Token> token_vector;
    int curr_index;
    int size_token_vec;

    std::vector<Sequence> node_sequence_vector;
    std::vector<int> node_combined_index;
    std::vector<InitNode> init_node_vector;
    std::vector<UpdateNode> update_node_vector;
    std::vector<FlowNode> flow_node_vector;
    std::vector<onscNode> onsc_node_vector;

    void expect(TokenType expected_tp);
    void expect(TokenType expected_tp1,TokenType expected_tp2);
    TokenType get_tp();
    std::string get_lxm();
    void advance();
    void syntaxErrStr(TokenType expected_tp, TokenType curr_tp);
    void syntaxErrStr(TokenType expected_tp1,TokenType expected_tp2,TokenType curr_tp);
    void parse_kwdata();
    void parse_kwtext();
    void parse_init();
    void parse_instructions();
    void parse_update();
    bool is_binop(TokenType tp);
    bool is_bincmp(TokenType tp);
    Expression parse_expression();
    void parse_controlFlow(TokenType tp);
    void parse_onsc();

public:
    Parser(std::vector<Token> &token){
        token_vector = token;
        curr_index = 0;
        size_token_vec = token_vector.size();
    }
    ParserOutput parse();
};



