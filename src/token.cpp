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
    symbolMap["ddi"] = TokenType::KW_DDI;
    symbolMap["du"] = TokenType::KW_DU;
    symbolMap["onsc"] = TokenType::KW_PRINT;
    symbolMap["\""] = TokenType::D_QUOTE;
    symbolMap["data"] = TokenType::DATA;
    symbolMap["instructions"] = TokenType::TEXT;
    symbolMap["exc"] = TokenType::BEGIN;
    symbolMap["end"] = TokenType::END;
}

void fill_strrepMap(std::unordered_map<TokenType,std::string> &strrepMap){
    strrepMap[TokenType::L_AB]           = "Less Than";
    strrepMap[TokenType::R_AB]           = "Greater Than";
    strrepMap[TokenType::PRN_LROUND]     = "Left Round Brck";
    strrepMap[TokenType::PRN_RROUND]     = "Right Round Brck";
    strrepMap[TokenType::COMMA]          = "Comma";
    strrepMap[TokenType::COLON]          = "Colon";
    strrepMap[TokenType::PLUS]           = "Plus";
    strrepMap[TokenType::MUL]            = "Multiply";
    strrepMap[TokenType::KW_IF]          = "If";
    strrepMap[TokenType::KW_ELSE]        = "Else";
    strrepMap[TokenType::KW_LOOP]        = "Loop";
    strrepMap[TokenType::EQ]             = "Equality";
    strrepMap[TokenType::EQ_G]           = "Greater than or Equal to";
    strrepMap[TokenType::EQ_L]           = "Less than or Equal to";
    strrepMap[TokenType::KW_DDI]         = "Data declaration and Init";
    strrepMap[TokenType::KW_DU]          = "Data Update";
    strrepMap[TokenType::IDENTIFIER]     = "Identifier";
    strrepMap[TokenType::INTEGER]        = "Integer";
    strrepMap[TokenType::STRING]         = "String Literal";
    strrepMap[TokenType::KW_PRINT]       = "Print";
    strrepMap[TokenType::D_QUOTE]        = "Double Quoation";
    strrepMap[TokenType::DATA]           = "Data Section";
    strrepMap[TokenType::TEXT]           = "Text Section";
    strrepMap[TokenType::BEGIN]          = "Begin Block";
    strrepMap[TokenType::END]            = "End Block";
 }
