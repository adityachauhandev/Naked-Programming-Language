#include <iostream>
#include <vector>
#include <string>
#include <charconv>
#include <variant>
#include <string_view>
#include "token.h"
#include "parser.h"

void Parser::syntaxErrStr(TokenType expected_tp, TokenType curr_tp){
    std::cerr << "There was a syntax error.\n";
    std::exit(1);
}

void Parser::syntaxErrStr(TokenType expected_tp1,TokenType expected_tp2,TokenType curr_tp){
    std::cerr << "There was a syntax error.\n";
    std::exit(1);
}

void Parser::advance(){
    curr_index++;
    //TODO:Check the bound
}

TokenType Parser::get_tp(){
    //TODO:Check the bound
    TokenType curr_tp = token_vector[curr_index].type;
    return curr_tp;
}

std::string_view Parser::get_lxm(){
    return token_vector[curr_index].lexeme;
}

void Parser::expect(TokenType expected_tp){
    TokenType curr_tp = get_tp();
    if(expected_tp != curr_tp){
        syntaxErrStr(expected_tp,curr_tp);
    }
}

void Parser::expect(TokenType expected_tp1,TokenType expected_tp2){
    TokenType curr_tp = get_tp();
    if(expected_tp1 != curr_tp && expected_tp2 != curr_tp){
        syntaxErrStr(expected_tp1,expected_tp2,curr_tp);
    }
}

void Parser::parse_init(){
    expect(TokenType::INTEGER);
    int value;
    std::string_view num_lxm = get_lxm();
    std::from_chars(num_lxm.data(), num_lxm.data() + num_lxm.size(), value);

    advance();

    expect(TokenType::COMMA);
    advance();

    expect(TokenType::IDENTIFIER);
    std::string_view name = get_lxm();
    advance();

    init_node_vector.push_back({value,name});
    node_sequence_vector.push_back(Sequence::INIT_NODE);
    node_combined_index.push_back(init_node_vector.size() - 1);
}

void Parser::parse_kwdata(){
    expect(TokenType::KW_DATA);
    advance();
    expect(TokenType::COLON);
    advance();

    while(get_tp() != TokenType::KW_TEXT){
        parse_init();
    }
}

bool Parser::is_bincmp(TokenType tp){
    return tp == TokenType::EQ || tp == TokenType::EQ_G ||
    tp == TokenType::EQ_L || tp == TokenType::L_AB || tp == TokenType::R_AB;
}

bool Parser::is_binop(TokenType tp){
    return tp == TokenType::PLUS || tp == TokenType::MUL;
}

void Parser::resolve_id(std::variant<int, std::string_view>& id)
{
    expect(TokenType::INTEGER, TokenType::IDENTIFIER);

    std::string_view lxm = get_lxm();
    if (get_tp() == TokenType::INTEGER) {
        int temp = 0;
        std::from_chars(lxm.data(), lxm.data() + lxm.size(), temp);
        id = temp;
    } else {
        id = lxm;
    }
    advance();
}

bool Parser::resolve_op(TokenType& op_tp) {
    if (is_bincmp(get_tp()) || is_binop(get_tp())) {
        op_tp = get_tp();
        advance();
        return true;
    }
    return false;
}

Expression Parser::parse_expression() {
    Expression expr;
    resolve_id(expr.id1);
    if (resolve_op(expr.optype)) resolve_id(expr.id2);
    return expr;
}

void Parser::parse_update(){
    advance();

    expect(TokenType::COLON);
    advance();

    Expression expr = parse_expression();

    expect(TokenType::COMMA);
    advance();

    expect(TokenType::IDENTIFIER);
    std::string_view target = get_lxm();
    advance();

    update_node_vector.push_back({expr,target});
    node_sequence_vector.push_back(Sequence::UPDATE_NODE);
    node_combined_index.push_back(update_node_vector.size() - 1);
}

void Parser::parse_if(){
    advance();
    expect(TokenType::COLON);
    advance();

    Expression expr = parse_expression();

    if_node_vector.push_back({expr});
    node_sequence_vector.push_back(Sequence::IF_NODE);
    node_combined_index.push_back(if_node_vector.size() - 1);


    expect(TokenType::BEGIN);

    node_sequence_vector.push_back(Sequence::BEGIN_IF);
    node_combined_index.push_back(-1);

    advance();
    expect(TokenType::COLON);
    advance();

    parse_instructions();

    expect(TokenType::END);

    node_sequence_vector.push_back(Sequence::END_IF);
    node_combined_index.push_back(-1);

    advance();

    if(get_tp() == TokenType::KW_ELSE){
        node_sequence_vector.push_back(Sequence::ELSE_NODE);
        node_combined_index.push_back(-1);

        advance();

        expect(TokenType::COLON);
        advance();

        expect(TokenType::BEGIN);

        node_sequence_vector.push_back(Sequence::BEGIN_ELSE);
        node_combined_index.push_back(-1);

        advance();

        expect(TokenType::COLON);
        advance();

        parse_instructions();

        expect(TokenType::END);
        node_sequence_vector.push_back(Sequence::END_ELSE);
        node_combined_index.push_back(-1);
        advance();
    }

}

void Parser::parse_loop(){
    advance();
    expect(TokenType::COLON);
    advance();

    Expression expr = parse_expression();

    loop_node_vector.push_back({expr});
    node_sequence_vector.push_back(Sequence::LOOP_NODE);
    node_combined_index.push_back(loop_node_vector.size() - 1);


    expect(TokenType::BEGIN);

    node_sequence_vector.push_back(Sequence::BEGIN_LOOP);
    node_combined_index.push_back(-1);

    advance();
    expect(TokenType::COLON);
    advance();

    parse_instructions();

    expect(TokenType::END);

    node_sequence_vector.push_back(Sequence::END_LOOP);
    node_combined_index.push_back(-1);

    advance();
}

void Parser::parse_onsc(){
    std::string_view str = "";
    TokenType arg_tp = TokenType::EMPTY;
    std::variant<int,std::string_view> arg_vl;
    std::vector<onscArg> arg_vector;

    expect(TokenType::D_QUOTE);
    advance();
    expect(TokenType::STRING);
    str = get_lxm();
    advance();
    expect(TokenType::D_QUOTE);
    advance();
    while(get_tp() == TokenType::COMMA){
        advance();
        expect(TokenType::INTEGER,TokenType::IDENTIFIER);
        arg_tp = get_tp();
        if(arg_tp == TokenType::INTEGER){
            int temp = 0;
            std::string_view num_lxm = get_lxm();
            std::from_chars(num_lxm.data(), num_lxm.data() + num_lxm.size(), temp);
            arg_vl = temp;
        }
        else arg_vl = get_lxm();

        arg_vector.push_back({arg_vl,arg_tp});
        advance();
    }

    onsc_node_vector.push_back({str,arg_vector});
    node_sequence_vector.push_back(Sequence::PRINT_NODE);
    node_combined_index.push_back(onsc_node_vector.size() - 1);
}

void Parser::parse_instructions(){
    TokenType curr_tp = get_tp();
    while(curr_tp != TokenType::END){
        switch(curr_tp){
            case TokenType::KW_DU:
                parse_update();
                break;
            case TokenType::KW_IF:
                parse_if();
                break;
            case TokenType::KW_LOOP:
                parse_loop();
                break;
            case TokenType::KW_PRINT:
                advance();
                expect(TokenType::COLON);
                advance();
                parse_onsc();
                break;
            default:
                std::cout << "Unexpected Token" << std::endl;
                std::exit(1);
        }
        curr_tp = get_tp();
    }
}

void Parser::parse_kwtext(){
    expect(TokenType::KW_TEXT);
    advance();

    expect(TokenType::COLON);
    advance();

    expect(TokenType::BEGIN);
    node_sequence_vector.push_back(Sequence::BEGIN_MAIN);
    node_combined_index.push_back(-1);
    advance();

    expect(TokenType::COLON);
    advance();

    parse_instructions();

    expect(TokenType::END);
    advance();
    node_sequence_vector.push_back(Sequence::END_MAIN);
    node_combined_index.push_back(-1);
    expect(TokenType::END_OF_FILE);
}

ParserOutput Parser::parse(){
    parse_kwdata();
    parse_kwtext();

    return {node_sequence_vector,
            node_combined_index,
            init_node_vector,
            update_node_vector,
            if_node_vector,
            loop_node_vector,
            onsc_node_vector};
}


