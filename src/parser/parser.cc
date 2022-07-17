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
    indent();
    function->set_return_type(parse_type());

    if (has_parameters()) {
        parse_parameters(function);
    }

    function->set_statements(parse_compound_statement());
    dedent();

    return function;
}

void Parser::parse_parameters(Function* function) {
    Variable* param = nullptr;

    while (has_parameters()) {
        expect(TK_AT);
        expect(TK_ID);
        param = new Variable(matched);
        expect(TK_COLON);
        param->set_type(parse_type());
        function->add_parameter(param);
    }
}

Type* Parser::parse_type() {
    Type* type = nullptr;

    if (match(TK_INT)) {
        type = new Type(TYPE_INT, matched);
    } else if (match(TK_UINT)) {
        type = new Type(TYPE_UINT, matched);
    } else if (match(TK_FLOAT)) {
        type = new Type(TYPE_FLOAT, matched);
    } else if (match(TK_DOUBLE)) {
        type = new Type(TYPE_DOUBLE, matched);
    } else if (match(TK_SHORT)) {
        type = new Type(TYPE_SHORT, matched);
    } else if (match(TK_USHORT)) {
        type = new Type(TYPE_USHORT, matched);
    } else if (match(TK_LONG)) {
        type = new Type(TYPE_LONG, matched);
    } else if (match(TK_ULONG)) {
        type = new Type(TYPE_ULONG, matched);
    } else if (match(TK_CHAR)) {
        type = new Type(TYPE_CHAR, matched);
    } else if (match(TK_UCHAR)) {
        type = new Type(TYPE_UCHAR, matched);
    } else if (match(TK_SYMBOL)) {
        type = new Type(TYPE_SYMBOL, matched);
    } else if (match(TK_VOID)) {
        type = new Type(TYPE_VOID, matched);
    } else if (match(TK_BOOL)) {
        type = new Type(TYPE_BOOL, matched);
    } else if (match(TK_STR)) {
        type = new Type(TYPE_STR, matched);
    } else if (match(TK_I8)) {
        type = new Type(TYPE_I8, matched);
    } else if (match(TK_I16)) {
        type = new Type(TYPE_I16, matched);
    } else if (match(TK_I32)) {
        type = new Type(TYPE_I32, matched);
    } else if (match(TK_I64)) {
        type = new Type(TYPE_I64, matched);
    } else if (match(TK_U8)) {
        type = new Type(TYPE_U8, matched);
    } else if (match(TK_U16)) {
        type = new Type(TYPE_U16, matched);
    } else if (match(TK_U32)) {
        type = new Type(TYPE_U32, matched);
    } else if (match(TK_U64)) {
        type = new Type(TYPE_U64, matched);
    }

    return type;
}

Statement* Parser::parse_statement() {
    Statement* stmt = nullptr;

    if (lookahead(TK_WHILE)) {

    } else {
        stmt = new ExpressionStatement(parse_expression());
    }

    return stmt;
}

CompoundStatement* Parser::parse_compound_statement() {
    CompoundStatement* statements = new CompoundStatement();

    while (is_indentend()) {
        statements->add_statement(parse_statement());
    }

    return statements;
}

Expression* Parser::parse_expression() {
    Expression* expr = nullptr;

    expr = parse_assignment_expression();

    return expr;
}

Expression* Parser::parse_assignment_expression() {
    Expression* expr = parse_arith_expression();

    while (true) {
        if (match(TK_ASSIGNMENT)) {
            expr = new BinOp(EXPR_ASSIGN, expr, parse_arith_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_arith_expression() {
    Expression* expr = parse_identifier_expression();

    while (true) {
        if (match(TK_PLUS)) {
            expr = new BinOp(EXPR_PLUS, expr, parse_identifier_expression());
        } else if (match(TK_MINUS)) {
            expr = new BinOp(EXPR_MINUS, expr, parse_identifier_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_identifier_expression() {
    Expression* expr;

    expect(TK_ID);
    expr = new Identifier(matched);

    return expr;
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

bool Parser::is_indentend() {
    return tokens[idx].get_whitespace() > indent_stack.top();
}

bool Parser::has_parameters() {
    return lookahead(TK_AT) && lookahead(TK_ID, 1) && lookahead(TK_COLON, 2);
}

void Parser::indent() {
    indent_stack.push(matched.get_whitespace());
}

void Parser::dedent() {
    indent_stack.pop();
}
