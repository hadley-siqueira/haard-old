#include <iostream>
#include <vector>
#include "token/token.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "defs.h"
#include "ast/ast.h"

using namespace haard;

void test_scanner(std::string path) {
    Scanner s;

    std::vector<Token> tokens = s.read(path);

    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << tokens[i].to_str() << std::endl;
    }
}

void test_parser(std::string path) {
    Parser parser;
    Source* src = parser.read(path);

    std::cout << "#imports = " << src->import_count() << std::endl;
    for (int i = 0; i < src->import_count(); ++i) {
        std::cout << src->get_import(i)->to_str() << std::endl;
    }

    delete src;
}

int main(int argc, char* argv[]) {
    // test_scanner(argv[1]);
    test_parser(argv[1]);   

    return 0;
}
