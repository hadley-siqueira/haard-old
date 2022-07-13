#include <iostream>
#include "parser/parser.h"

using namespace haard;

Parser::Parser() {
    idx = 0;
}

Source* Parser::read(std::string path) {
    Scanner s;
    Source* source = nullptr;

    idx = 0;
    tokens = s.read(path);
    source = parse_source();
    source->set_path(path);

    return source;
}

Source* Parser::parse_source() {
    Source* source = new Source();

    return source;
}

void Parser::advance() {
    if (idx < tokens.size()) {
        ++idx;
    }
}

void Parser::expect(int kind) {
    if (match(kind)) {
        return;
    }

    std::cout << "parser error\n";
    exit(0);
}

bool Parser::match(int kind) {
    if (lookahead(kind)) {
        matched = tokens[idx];
        advance();
        return true;
    }

    return false;
}

bool Parser::lookahead(int kind) {
    return tokens[idx].get_kind() == kind;
}
