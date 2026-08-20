#include <iostream>
#include <vector>
#include <string>
#include "token.h"
#include "parser.h"

std::vector<Sequence> node_sequence_vector;
std::vector<VarInitNode> var_node_vector;

void eat(int &i,int &size){
    i++;
    if(!(i < size)){
        std::cout << "EOF\n";
        std::exit(1);
    }
}

void variable_init(std::vector<Token> &token, int &i, int &size){
    TokenType curr_tp = token[i].type;

    if(curr_tp != TokenType::INTEGER){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    int value = std::stoi(token[i].lexeme);

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp != TokenType::COMMA){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp != TokenType::IDENTIFIER){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    std::string name = token[i].lexeme;

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp != TokenType::PRN_LROUND){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp != TokenType::INTEGER){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    int bytes = std::stoi(token[i].lexeme);

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp != TokenType::PRN_RROUND){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    var_node_vector.push_back({name,bytes,value});
    node_sequence_vector.push_back(Sequence::VAR_NODE);

    eat(i,size);

    curr_tp = token[i].type;
    if(curr_tp == TokenType::KW_TEXT) return;

    variable_init(token,i,size);
    return;
}

void parse_kwdata(std::vector<Token> &token, int &i, int &size){
    TokenType curr_tp = token[i].type;

    if(curr_tp != TokenType::COLON){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    eat(i,size);

    variable_init(token,i,size);
}

void parser(std::vector<Token> &token){
    int size = token.size();
    int i = 0;

    TokenType curr_tp = token[i].type;

    if(curr_tp != TokenType::KW_DATA){
        std::cout << "Syntax Error\n";
        std::exit(1);
    }

    eat(i,size);

    parse_kwdata(token,i,size);
    for(auto x : var_node_vector){
        std::cout << x.name << " " << x.bytes << " " << x.value << std::endl;
    }
    //curr_tp = token[i].type;
    //parse_kwtext(token,i,size);
}

