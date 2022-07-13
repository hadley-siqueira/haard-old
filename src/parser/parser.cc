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

    while (true) {
        if (lookahead(TK_IMPORT)) {
            source->add_import(parse_import());
        } else if (lookahead(TK_DEF)) {
            source->add_function(parse_function());
        } else {
            break;
        }
    }

    return source;
}

Import* Parser::parse_import() {
    Import* import = new Import();

    expect(TK_IMPORT);
    import->set_from_token(matched);

    expect(TK_ID);
    import->add_to_path(matched.get_lexeme());

    while (match(TK_DOT)) {
        if (match(TK_TIMES)) {
            import->add_to_path(matched.get_lexeme());
            break;
        }

        expect(TK_ID);
        import->add_to_path(matched.get_lexeme());
    }

    if (match(TK_AS)) {
        expect(TK_ID);
        import->set_alias(matched.get_lexeme());
    }

    return import;
}
            
Function* Parser::parse_function() {
    Function* function = new Function();

    expect(TK_DEF);
    expect(TK_ID);
    function->set_from_token(matched);

    expect(TK_COLON);
    expect(TK_ID);

    if (has_parameters()) {
        parse_parameters(function);
    }

    return function;
}

void Parser::parse_parameters(Function* function) {
    Variable* param = nullptr;

    while (has_parameters()) {
        expect(TK_AT);
        expect(TK_ID);
        param = new Variable(matched);
        expect(TK_COLON);
        expect(TK_INT);
        // param->set_type(parse_type());
        function->add_parameter(param);
    }
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

bool Parser::lookahead(int kind, int offset) {
    if (idx + offset < tokens.size()) {
        return tokens[idx + offset].get_kind() == kind;
    }

    return false;
}

bool Parser::has_parameters() {
    return lookahead(TK_AT) && lookahead(TK_ID, 1) && lookahead(TK_COLON, 2);
}
