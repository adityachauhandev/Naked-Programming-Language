#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
    PLUS, MUL,
    EQ, EQ_G, EQ_L,
    KW_IF, KW_ELSE, KW_LOOP,
    KW_DDI, KW_DU,
    IDENTIFIER, INTEGER, STRING,
    L_AB, R_AB, PRN_LROUND, PRN_RROUND,
    COMMA, COLON,
    KW_PRINT, D_QUOTE,
    DATA, TEXT,
    BEGIN, END,
    UNKNOWN
};

struct Token{
    TokenType type;
    std::string lexeme;
};

void fill_symbolMap(std::unordered_map<std::string,TokenType>& symbolMap);
void fill_strrepMap(std::unordered_map<TokenType,std::string> &strrepMap);

extern std::unordered_map<TokenType,std::string> strrepMap;
extern std::unordered_map<std::string,TokenType> symbolMap;
extern std::vector<Token> token;
