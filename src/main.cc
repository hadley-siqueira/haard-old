#include <iostream>
#include <vector>
#include "token/token.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "defs.h"
#include "ast/ast.h"
#include "printer/printer.h"
#include "driver/driver.h"
#include "log/logger.h"

using namespace haard;

void test_scanner(std::string path) {
    Scanner s;

    std::vector<Token> tokens = s.read(path);

    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << tokens[i].to_str() << std::endl;
    }
}

void test_scanner2(std::string str) {
    Scanner s;

    std::vector<Token> tokens = s.read_from_string(str);

    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << tokens[i].to_str() << std::endl;
    }
}

void test_parser(std::string path) {
    Parser parser;
    Printer printer;
    Source* src = parser.read(path, "");
    printer.print_source(src);

    std::cout << printer.to_str();

    delete src;
}

void test_driver(int argc, char* argv[]) {
    Driver driver;

    driver.set_flags(argc, argv);
    driver.run();
}

int main(int argc, char* argv[]) {
    //test_scanner(argv[1]);
    test_driver(argc, argv);
    //test_scanner2("name.foo = 2");

    return 0;
}
