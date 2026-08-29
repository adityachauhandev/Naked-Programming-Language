#pragma once
#include <vector>
#include "token.h"
#include "map.h"

class Scanner{
private:
    const TokenMap map;

    std::string_view code;
    std::vector<Token> token;

    int curr_index = 0;

    void advance();
    char get_inputChar() const;
    bool is_at_end() const;
    char look_ahead() const;
    void handle_angleBrckt();
    void handle_str();
    void handle_special();
    void handle_digit();
    void handle_alphnum();

public:
    Scanner(std::string_view code_dump) : code(code_dump) {}

    std::vector<Token>& scan();
};
