#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <chrono>

#include "scanner.h"
#include "token.h"
#include "parser.h"

int main(){
    std::ifstream source_code("tests/code.npl", std::ios::binary | std::ios::ate);

    if(!source_code) {
        std::cout<< "Couldn't open the file\n";
        return 1;
    }

    unsigned int file_size = source_code.tellg();
    std::vector<char> code_dump(file_size);

    source_code.seekg(0,std::ios::beg);
    source_code.read(code_dump.data(), file_size);

    fill_symbolMap(symbolMap);
    fill_strrepMap(strrepMap);

    scan(code_dump);
    parser(token);

    /*std::cout << std::left
     << std::setw(20) << "TYPE"
     << std::setw(30) << "LEXEME"
     << '\n';
     for(auto x : token){
         std::cout << std::string(50, '-') << '\n';

         std::cout << std::left
         << std::setw(20) << strrepMap[x.type]
         << std::setw(30) << x.lexeme
         << '\n';
     }*/

    return 0;
}
