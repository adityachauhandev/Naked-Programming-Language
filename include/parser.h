#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <variant>

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
    std::variant<int,std::string_view> id1;
    TokenType id1type = TokenType::EMPTY;
    TokenType optype = TokenType::EMPTY;
    std::variant<int,std::string_view> id2;
    TokenType id2type = TokenType::EMPTY;
};

struct InitNode{
    int value;
    std::string_view name;
};

struct UpdateNode{
    Expression expr;
    std::string_view target;
};

struct FlowNode{
    Expression expr;
    TokenType flow_tp;
};

struct onscArg{
    std::variant<int,std::string_view> value;
    TokenType tp;
};

struct onscNode{
    std::string_view str;
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
    const std::vector<Token>& token_vector;
    int curr_index = 0;
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
    std::string_view get_lxm();
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
    void resolve_id(std::variant<int, std::string_view>& id, TokenType& tp);
    bool resolve_op(TokenType& op_tp);
    Expression parse_expression();
    void parse_controlFlow(TokenType tp);
    void parse_onsc();

public:
    Parser(const std::vector<Token>& token): token_vector(token) {
        size_token_vec = token_vector.size();
    }
    ParserOutput parse();
};



