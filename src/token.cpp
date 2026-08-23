#include <string>
#include <unordered_map>

#include "token.h"

std::unordered_map<std::string,TokenType> symbolMap;
std::unordered_map<TokenType,std::string> strrepMap;
std::vector<Token> token;

void fill_symbolMap(std::unordered_map<std::string,TokenType>& symbolMap){
    symbolMap["<"] = TokenType::L_AB;
    symbolMap[">"] = TokenType::R_AB;
    symbolMap["("] = TokenType::PRN_LROUND;
    symbolMap[")"] = TokenType::PRN_RROUND;
    symbolMap[","] = TokenType::COMMA;
    symbolMap[":"] = TokenType::COLON;
    symbolMap["+"] = TokenType::PLUS;
    symbolMap["*"] = TokenType::MUL;
    symbolMap["if"] = TokenType::KW_IF;
    symbolMap["else"] = TokenType::KW_ELSE;
    symbolMap["loop"] = TokenType::KW_LOOP;
    symbolMap["="] = TokenType::EQ;
    symbolMap[">="] = TokenType::EQ_G;
    symbolMap["<="] = TokenType::EQ_L;
    symbolMap["du"] = TokenType::KW_DU;
    symbolMap["onsc"] = TokenType::KW_PRINT;
    symbolMap["\""] = TokenType::D_QUOTE;
    symbolMap["data"] = TokenType::KW_DATA;
    symbolMap["instructions"] = TokenType::KW_TEXT;
    symbolMap["exc"] = TokenType::BEGIN;
    symbolMap["end"] = TokenType::END;
}

void fill_strrepMap(std::unordered_map<TokenType,std::string> &strrepMap){
    strrepMap[TokenType::L_AB]           = "L_AB";
    strrepMap[TokenType::R_AB]           = "R_AB";
    strrepMap[TokenType::PRN_LROUND]     = "PRN_LROUND";
    strrepMap[TokenType::PRN_RROUND]     = "PRN_RROUND";
    strrepMap[TokenType::COMMA]          = "COMMA";
    strrepMap[TokenType::COLON]          = "COLON";
    strrepMap[TokenType::PLUS]           = "PLUS";
    strrepMap[TokenType::MUL]            = "MUL";
    strrepMap[TokenType::KW_IF]          = "KW_IF";
    strrepMap[TokenType::KW_ELSE]        = "KW_ELSE";
    strrepMap[TokenType::KW_LOOP]        = "KW_LOOP";
    strrepMap[TokenType::EQ]             = "EQ";
    strrepMap[TokenType::EQ_G]           = "EQ_G";
    strrepMap[TokenType::EQ_L]           = "EQ_L";
    strrepMap[TokenType::KW_DU]          = "KW_DU";
    strrepMap[TokenType::IDENTIFIER]     = "IDENTIFIER";
    strrepMap[TokenType::INTEGER]        = "INTEGER";
    strrepMap[TokenType::STRING]         = "STRING";
    strrepMap[TokenType::KW_PRINT]       = "KW_PRINT";
    strrepMap[TokenType::D_QUOTE]        = "D_QUOTE";
    strrepMap[TokenType::KW_DATA]        = "KW_DATA";
    strrepMap[TokenType::KW_TEXT]        = "KW_TEXT";
    strrepMap[TokenType::BEGIN]          = "BEGIN";
    strrepMap[TokenType::END]            = "END";
    strrepMap[TokenType::END_OF_FILE]    = "EOF";
 }
