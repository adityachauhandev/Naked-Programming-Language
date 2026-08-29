#include <string_view>
#include <cctype>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "map.h"

void Scanner::advance(){
    curr_index++;
}

char Scanner::get_inputChar() const {
    return code[curr_index];
}

bool Scanner::is_at_end() const {
    return curr_index >= code.size();
}

char Scanner::look_ahead() const {
    if(curr_index + 1 >= code.size()) return '\0';
    else return code[curr_index + 1];
}

void Scanner::handle_angleBrckt() {
    switch (get_inputChar()) {
        case '<':
            if (look_ahead() == '=') {
                token.push_back({TokenType::EQ_L, "<="});
                advance();
            } else {
                token.push_back({TokenType::L_AB, "<"});
            }
            break;
        case '>':
            if (look_ahead() == '=') {
                token.push_back({TokenType::EQ_G, ">="});
                advance();
            } else {
                token.push_back({TokenType::R_AB, ">"});
            }
            break;
    }
    advance();
}

void Scanner::handle_str() {
    token.push_back({TokenType::D_QUOTE, "\""});
    advance();

    int start_index = curr_index;

    while (!is_at_end() && get_inputChar() != '"') {
        advance();
    }

    int length = curr_index - start_index;
    std::string_view lexeme = code.substr(start_index, length);
    token.push_back({TokenType::STRING, lexeme});

    if (!is_at_end() && get_inputChar() == '"') {
        token.push_back({TokenType::D_QUOTE, "\""});
        advance();
    } else {
        token.push_back({TokenType::END_OF_FILE, ""});
    }
}

void Scanner::handle_special(){
    std::string_view lexeme(&code[curr_index], 1);
    token.push_back({map[lexeme],lexeme});
    advance();
}

void Scanner::handle_digit(){
    int start_index = curr_index;
    advance();
    while(!is_at_end() && isdigit(get_inputChar())) advance();
    int length = curr_index - start_index;
    std::string_view lexeme(&code[start_index], length);
    token.push_back({TokenType::INTEGER,lexeme});
}

void Scanner::handle_alphnum(){
    int start_index = curr_index;
    advance();
    while(!is_at_end() && (isalnum(get_inputChar()) || get_inputChar() == '_')){
        advance();
    }

    int length = curr_index - start_index;
    std::string_view lexeme(&code[start_index], length);

    TokenType type = map[lexeme];
    if(type == TokenType::UNKNOWN) token.push_back({TokenType::IDENTIFIER,lexeme});
    else token.push_back({type,lexeme});
}

std::vector<Token>& Scanner::scan(){
    while(!is_at_end()){
        char inputChar = get_inputChar();

        if (std::isspace(inputChar) || inputChar == '\0') advance();
        else if(inputChar == '<' || inputChar == '>') handle_angleBrckt();
        else if(inputChar == '"') handle_str();
        else if(!isalnum(inputChar)) handle_special();
        else if(isdigit(inputChar)) handle_digit();
        else handle_alphnum();
    }
    token.push_back({TokenType::END_OF_FILE,""});
    return token;
}
