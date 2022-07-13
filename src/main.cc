#include <iostream>
#include <vector>
#include "token/token.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "defs.h"

using namespace haard;

int main(int argc, char* argv[]) {
    Token token;
    token.set_line(2);
    Scanner s;
    std::vector<Token> tokens = s.read(argv[1]);

    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << tokens[i].to_str() << std::endl;
    }

    Parser parser;
    parser.read(argv[1]);

    return 0;
}
