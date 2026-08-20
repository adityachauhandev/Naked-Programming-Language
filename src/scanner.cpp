#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <vector>

#include "token.h"
#include "scanner.h"

void handle_LAB(const char* base, int &i){
    std::string lexeme = "<";
    if(*(++base) == '='){
        lexeme = "<=";
        token.push_back({symbolMap[lexeme],lexeme});
        i++;
    }
    else token.push_back({symbolMap[lexeme],lexeme});
}

void handle_RAB(const char* base, int &i){
    std::string lexeme = ">";
    if(*(++base) == '='){
        lexeme = ">=";
        token.push_back({symbolMap[lexeme],lexeme});
        i++;
    }
    else token.push_back({symbolMap[lexeme],lexeme});
}

void handle_str(const char* base, int &i){
    std::string lexeme = "\"";
    base++;
    while(*(base) != '"' && *(base) != '\0'){
        lexeme.push_back(*(base));
        base++;
        i++;
    }
    lexeme.push_back(*(base));
    i++;
    token.push_back({TokenType::STRING,lexeme});
}

void handle_special(const char* base){
    std::string lexeme = "";
    lexeme.push_back(*(base));
    if(symbolMap.count(lexeme) > 0){
        token.push_back({symbolMap[lexeme],lexeme});
    }
    else{
        token.push_back({TokenType::UNKNOWN,lexeme});
    }
}

void handle_digit(const char* base, int &i){
    std::string lexeme = "";
    while(isdigit(*base)){
        lexeme.push_back(*base);
        base++;
        i++;
    }
    i--;
    token.push_back({TokenType::INTEGER,lexeme});
}

void handle_alphnum(const char* base, int &i){
    std::string lexeme = "";
    while(isalnum(*base)){
        lexeme.push_back(*base);
        base++;
        i++;
    }
    i--;
    if(symbolMap.count(lexeme) > 0){
        token.push_back({symbolMap[lexeme],lexeme});
    }
    else {
        token.push_back({TokenType::IDENTIFIER,lexeme});
    }

}

void scan(std::vector<char>& code_dump){
    int code_len = code_dump.size();

    for(int i = 0; i < code_len; i++){
        std::string lexeme = "";

        if(std::isspace(code_dump[i])) continue;

        if(code_dump[i] == '<') handle_LAB(&code_dump[i],i);

        else if(code_dump[i] == '>') handle_RAB(&code_dump[i],i);

        else if(code_dump[i] == '"') handle_str(&code_dump[i],i);

        else if(!isalnum(code_dump[i])) handle_special(&code_dump[i]);
        else if(isdigit(code_dump[i])) handle_digit(&code_dump[i],i);

        else handle_alphnum(&code_dump[i],i);
    }
}

















