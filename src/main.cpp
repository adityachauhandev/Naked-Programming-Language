#include <iostream>
#include <fstream>
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string_view>

#include "scanner.h"
#include "map.h"
#include "token.h"
#include "parser.h"
#include "interpreter.h"

int main(){
    int fd = open("tests/code.npl", O_RDONLY);
    if (fd == -1) {
        std::cout << "Couldn't open the file\n";
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cout << "Couldn't get the metadata\n";
        close(fd);
        return 1;
    }
    size_t file_size = sb.st_size;

    const char* file_data = static_cast<const char*>(mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_data == MAP_FAILED) {
        std::cout << "Memory mapping failed\n";
        close(fd);
        return 1;
    }

    madvise(const_cast<char*>(file_data), file_size, MADV_SEQUENTIAL);
    madvise(const_cast<char*>(file_data), file_size, MADV_WILLNEED);
    std::string_view code_dump(file_data, file_size);

    Scanner s(code_dump);
    const std::vector<Token>& token = s.scan();

    Parser p(token);
    ParserOutput p_output = p.parse();

    Interpreter e(p_output);
    e.interpret();

    if (munmap(const_cast<char*>(file_data), file_size) == -1) {
        std::cout << "munmap failed\n";
        return 1;
    }
    close(fd);

    return 0;
}
