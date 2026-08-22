#include <iostream>
#include <vector>
#include <string>
#include "token.h"
#include "parser.h"

class Parser {
private:
    std::vector<Token> token_vector;
    int curr_index;
    int size_token_vec;

    std::vector<Sequence> node_sequence_vector;
    std::vector<InitNode> init_node_vector;
    std::vector<BinaryNode> binary_node_vector;
    std::vector<UnaryNode> unary_node_vector;
    std::vector<UnaryIFNode> unaryif_node_vector;
    std::vector<BinaryIFNode> binaryif_node_vector;

    void expect(TokenType expected_tp);
    void expect(TokenType expected_tp1,TokenType expected_tp2);
    TokenType get_tp();
    std::string get_lxm();
    void advance();
    void EOFcheck();
    void syntaxErrStr(TokenType expected_tp, TokenType curr_tp);
    void syntaxErrStr(TokenType expected_tp1,TokenType expected_tp1,TokenType curr_tp);
    void parse_kwdata();
    void parse_init();
    void parse_instructions();
    void parse_update();
    bool is_binop(TokenType tp);

public:
    Parser(std::vector<Token> token){
        token_vector = token;
        curr_index = 0;
        size_token_vec = token_vector.size();
    }

    void parse();
};

void Parser::syntaxErrStr(TokenType expected_tp, TokenType curr_tp){
    std::cout << "There was a syntax error.\n";
    std::cout << "Expected token type was : " << expected_tp <<"\n";
    std::cout << "Got : " << curr_tp <<"\n";
    std::exit(1);
}

void Parser::syntaxErrStr(TokenType expected_tp1,TokenType expected_tp1,TokenType curr_tp){
    std::cout << "There was a syntax error.\n";
    std::cout << "Expected token type was : " << expected_tp1 <<" "<<"or"<< " "<<expected_tp2<<"\n";
    std::cout << "Got : " << curr_tp <<"\n";
    std::exit(1);
}

void Parser::EOFcheck(){
    if(curr_index == size){
        switch(data_init_begin || main_block_begin){
            case 0:
                std::cout << "Code parsed successfully.\n";
                break;
            case 1:
                std::cout << "Unexpected EOF.\n";
                std::cout << "Aborting parsing\n";
                std::exit(1);
                break;
        }
    }
}

void Parser::advance(){
    curr_index++;
    EOFcheck();
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

    init_node_vec.push_back({bytes,value,name});
    node_sequence_vector.push_back(Sequence::INIT_NODE);
}

void Parser::parse_kwdata(){
    expect(TokenType::COLON);
    advance();

    while(get_tp() != TokenType::KW_TEXT){
        parse_init();
    }
}

bool is_binop(TokenType tp){
    return tp == TokenType::PLUS || tp == TokenType::MUL;
}

void Parser::parse_update(){
    std::string id1;
    TokenType id1type;
    TokenType optype;
    std::string id2;
    TokenType id2type;
    std::string target;

    expect(TokenType::COLON);
    advance();

    expect(TokenType::INTEGER, TokenType::IDENTIFIER);
    id1 = get_lxm();
    id1type = get_tp();
    advance();

    optype = get_tp();
    if(is_binop(optype)){
        advance();
        expect(TokenType::INTEGER, TokenType::IDENTIFIER);
        id2 = get_lxm();
        id2type = get_tp();
        advance();
    }

    expect(TokenType::COMMA);
    advance();
    expect(TokenType::IDENTIFIER);
    target = get_lxm();
    advance();

    if(is_binop(optype)){
        binary_node_vector.push_back({id1,id1type,optype,id2,id2type,target});
        node_sequence_vector.push_back(Sequence::BINARY_NODE);
    }
    else {
        unary_node_vector.push_back({id1,id1type,target});
        node_sequence_vector.push_back(Sequence::UNARY_NODE);
    }
}

bool is_bincmp(TokenType tp){
    return tp == TokenType::EQ || tp == TokenType::EQ_G || tp == TokenType::EQ_L;
}

void parse_if(){
    std::string id1;
    TokenType id1type;
    TokenType cmptype;
    std::string id2;
    TokenType id2type;

    expect(TokenType::COLON);
    advance();
    expect(TokenType::PRN_LROUND);
    advance();

    expect(TokenType::INTEGER,TokenType::IDENTIFIER);
    id1 = get_lxm();
    id1type = get_tp();
    advance();

    cmptype = get_tp();
    if(is_bincmp(cmptype)){
        advance();
        expect(TokenType::INTEGER, TokenType::IDENTIFIER);
        id2 = get_lxm();
        id2type = get_tp();
        advance();
    }
    expect(TokenType::PRN_RROUND);
    advance();

    if(is_bincmp(cmptype)){
        binaryif_node_vector.push_back({id1,id1type,cmptype,id2,id2type});
        node_sequence_vector.push_back(TokenType::BINARY_IF);
    }
    else{
        unaryif_node_vector.push_back({id1,id1type});
        node_sequence_vector.push_back(TokenType::UNARY_IF);
    }

    parse_instructions();

    if(get_tp() == TokenType::KW_ELSE){
        advance();
        node_sequence_vector.push_back(TokenType::ELSE_NODE);
        parse_instructions();
    }
}

void Parser::parse_instructions(){
    expect(TokenType::BEGIN);
    advance();
    expect(TokenType::COLON);
    advance();

    TokenType curr_tp = get_tp();
    while(curr_tp != TokenType::EOF){
        switch(curr_tp){
            case TokenType::KW_DU:
                advance();
                parse_update();
                break;
            case TokenType::KW_IF:
                advance();
                parse_if();
                break;
            case TokenType::KW_LOOP:
                parse_loop();
                break;
            case TokenType::END:
                advance();
                return;
            default:
                std::cout << "Unexpected Token" << std::endl;
                std::exit(1);
        }
        curr_tp = get_tp();
    }
}

void Parser::parse_kwtext(){
    expect(TokenType::COLON);
    advance();
    parse_instructions();
}

//TODO: Handle the case where code is empty
//Need to handle EOF in that case
//First fix scanner before parser
void Parser::parse(){
    expect(TokenType::KW_DATA);
    advance();
    parse_kwdata();
    advance();
    parse_kwtext();
}

