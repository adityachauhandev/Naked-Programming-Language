#include <iostream>
#include <vector>
#include <string>
#include "token.h"
#include "parser.h"

void Parser::syntaxErrStr(TokenType expected_tp, TokenType curr_tp){
    std::cout << "There was a syntax error.\n";
    std::cout << "Expected token type was : " << strrepMap[expected_tp] <<"\n";
    std::cout << "Got : " << strrepMap[curr_tp] <<"\n";
    std::exit(1);
}

void Parser::syntaxErrStr(TokenType expected_tp1,TokenType expected_tp2,TokenType curr_tp){
    std::cout << "There was a syntax error.\n";
    std::cout << "Expected token type was : " << strrepMap[expected_tp1] <<" "<<"or"<< " "<<strrepMap[expected_tp2]<<"\n";
    std::cout << "Got : " << strrepMap[curr_tp] <<"\n";
    std::exit(1);
}

void Parser::advance(){
    //std::cout << "Consumed : " << strrepMap[get_tp()] <<"\n";
    curr_index++;
}

TokenType Parser::get_tp(){
    TokenType curr_tp = token_vector[curr_index].type;
    return curr_tp;
}

std::string Parser::get_lxm(){
    std::string curr_lxm = token_vector[curr_index].lexeme;
    return curr_lxm;
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
    std::string value = get_lxm();
    advance();
    expect(TokenType::COMMA);
    advance();
    expect(TokenType::IDENTIFIER);
    std::string name = get_lxm();
    advance();
    expect(TokenType::PRN_LROUND);
    advance();
    expect(TokenType::INTEGER);
    std::string bytes = get_lxm();
    advance();
    expect(TokenType::PRN_RROUND);
    advance();

    init_node_vector.push_back({bytes,value,name});
    node_sequence_vector.push_back(Sequence::INIT_NODE);
    node_combined_index.push_back(init_node_vector.size() - 1);
}

void Parser::parse_kwdata(){
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

Expression Parser::parse_expression(){
    Expression expr;
    expr.optype = TokenType::EMPTY;
    expr.id2 = "";
    expr.id2type = TokenType::EMPTY;

    expect(TokenType::INTEGER,TokenType::IDENTIFIER);
    expr.id1 = get_lxm();
    expr.id1type = get_tp();
    advance();

    if(is_bincmp(get_tp()) || is_binop(get_tp())){
        expr.optype = get_tp();
        advance();

        expect(TokenType::INTEGER,TokenType::IDENTIFIER);
        expr.id2 = get_lxm();
        expr.id2type = get_tp();
        advance();
    }
    return expr;
}

void Parser::parse_update(){
    Expression expr = parse_expression();
    expect(TokenType::COMMA);
    advance();
    expect(TokenType::IDENTIFIER);
    std::string target = get_lxm();
    advance();

    update_node_vector.push_back({expr,target});
    node_sequence_vector.push_back(Sequence::UPDATE_NODE);
    node_combined_index.push_back(update_node_vector.size() - 1);
}

void Parser::parse_controlFlow(TokenType tp){
    Expression expr = parse_expression();

    flow_node_vector.push_back({expr,tp});
    node_sequence_vector.push_back(Sequence::FLOW_NODE);
    node_combined_index.push_back(flow_node_vector.size() - 1);

    expect(TokenType::BEGIN);

    if(tp == TokenType::KW_IF) {
        node_sequence_vector.push_back(Sequence::BEGIN_IF);
        node_combined_index.push_back(-1);
    }
    else {
        node_sequence_vector.push_back(Sequence::BEGIN_LOOP);
        node_combined_index.push_back(-1);
    }

    advance();
    expect(TokenType::COLON);
    advance();

    parse_instructions();

    expect(TokenType::END);

    if(tp == TokenType::KW_IF) {
        node_sequence_vector.push_back(Sequence::END_IF);
        node_combined_index.push_back(-1);
    }
    else {
        node_sequence_vector.push_back(Sequence::END_LOOP);
        node_combined_index.push_back(-1);

    }

    advance();

    if(tp == TokenType::KW_IF && get_tp() == TokenType::KW_ELSE){
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

void Parser::parse_onsc(){
    std::string str = "";
    TokenType arg_tp = TokenType::EMPTY;
    std::string arg_vl = "";
    std::vector<onscArg> arg_vector;

    expect(TokenType::D_QUOTE);
    advance();
    expect(TokenType::STRING);
    str += get_lxm();
    advance();
    expect(TokenType::D_QUOTE);
    advance();
    while(get_tp() == TokenType::COMMA){
        advance();
        expect(TokenType::INTEGER,TokenType::IDENTIFIER);
        arg_tp = get_tp();
        arg_vl = get_lxm();
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
                advance();
                expect(TokenType::COLON);
                advance();
                parse_update();
                break;
            case TokenType::KW_IF:
                advance();
                expect(TokenType::COLON);
                advance();
                parse_controlFlow(TokenType::KW_IF);
                break;
            case TokenType::KW_LOOP:
                advance();
                expect(TokenType::COLON);
                advance();
                parse_controlFlow(TokenType::KW_LOOP);
                break;
            case TokenType::KW_PRINT:
                advance();
                expect(TokenType::COLON);
                advance();
                parse_onsc();
                break;
            default:
                std::cout << "Unexpected Token" << std::endl;
                std::cout << strrepMap[get_tp()] << std::endl;
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
    expect(TokenType::KW_DATA);
    advance();
    parse_kwdata();
    parse_kwtext();

    return {node_sequence_vector,
            node_combined_index,
            init_node_vector,
            update_node_vector,
            flow_node_vector,
            onsc_node_vector};
}


