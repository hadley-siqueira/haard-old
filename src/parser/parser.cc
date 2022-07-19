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
        stmt = parse_while_statement();
    } else {
        stmt = new ExpressionStatement(parse_expression());
    }

    return stmt;
}

WhileStatement* Parser::parse_while_statement() {
    WhileStatement* stmt = new WhileStatement();

    expect(TK_WHILE);
    stmt->set_condition(parse_expression());
    expect(TK_COLON);
    indent();
    stmt->set_statements(parse_compound_statement());
    dedent();

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
    Token oper;
    Expression* expr = parse_range_expression();

    while (true) {
        if (match(TK_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_BITWISE_AND_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_AND_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_BITWISE_XOR_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_XOR_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_BITWISE_OR_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_OR_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_BITWISE_NOT_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_NOT_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_DIVISION_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_INTEGER_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_INTEGER_DIVISIO_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_MINUS_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_MINUS_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_MODULO_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_MODULO_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_PLUS_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_PLUS_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_TIMES_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_TIMES_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_SLL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SLL_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_SRA_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SRA_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_SRL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SRL_ASSIGN, oper, expr, parse_range_expression());
        } else if (match(TK_SPECIAL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SPECIAL_ASSIGN, oper, expr, parse_range_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_range_expression() {
    Token oper;
    Expression* expr = parse_logical_or_expression();

    while (true) {
        if (match(TK_INCLUSIVE_RANGE)) {
            oper = matched;
            expr = new BinOp(EXPR_INCLUSIVE_RANGE, oper, expr, parse_logical_or_expression());
        } else if (match(TK_EXCLUSIVE_RANGE)) {
            oper = matched;
            expr = new BinOp(EXPR_EXCLUSIVE_RANGE, oper, expr, parse_logical_or_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_logical_or_expression() {
    Token oper;
    Expression* expr = parse_logical_and_expression();

    while (true) {
        if (match(TK_OR)) {
            oper = matched;
            expr = new BinOp(EXPR_LOGICAL_OR, oper, expr, parse_logical_and_expression());
        } else if (match(TK_LOGICAL_OR)) {
            oper = matched;
            expr = new BinOp(EXPR_LOGICAL_OR_OPER, oper, expr, parse_logical_and_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_logical_and_expression() {
    Token oper;
    Expression* expr = parse_equality_expression();

    while (true) {
        if (match(TK_AND)) {
            oper = matched;
            expr = new BinOp(EXPR_LOGICAL_AND, oper, expr, parse_equality_expression());
        } else if (match(TK_LOGICAL_AND)) {
            oper = matched;
            expr = new BinOp(EXPR_LOGICAL_AND_OPER, oper, expr, parse_equality_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_equality_expression() {
    Token oper;
    Expression* expr = parse_relational_expression();

    while (true) {
        if (match(TK_EQ)) {
            oper = matched;
            expr = new BinOp(EXPR_EQ, oper, expr, parse_relational_expression());
        } else if (match(TK_NE)) {
            oper = matched;
            expr = new BinOp(EXPR_NE, oper, expr, parse_relational_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_relational_expression() {
    Token oper;
    Expression* expr = parse_arith_expression();

    while (true) {
        if (match(TK_LT)) {
            oper = matched;
            expr = new BinOp(EXPR_LT, oper, expr, parse_arith_expression());
        } else if (match(TK_GT)) {
            oper = matched;
            expr = new BinOp(EXPR_GT, oper, expr, parse_arith_expression());
        } else if (match(TK_LE)) {
            oper = matched;
            expr = new BinOp(EXPR_LE, oper, expr, parse_arith_expression());
        } else if (match(TK_GE)) {
            oper = matched;
            expr = new BinOp(EXPR_GE, oper, expr, parse_arith_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_arith_expression() {
    Token oper;
    Expression* expr = parse_term_expression();

    while (true) {
        if (match(TK_PLUS)) {
            oper = matched;
            expr = new BinOp(EXPR_PLUS, oper, expr, parse_term_expression());
        } else if (match(TK_MINUS)) {
            oper = matched;
            expr = new BinOp(EXPR_MINUS, oper, expr, parse_term_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_term_expression() {
    Token oper;
    Expression* expr = parse_bitwise_or_expression();

    while (true) {
        if (match(TK_TIMES)) {
            oper = matched;
            expr = new BinOp(EXPR_TIMES, oper, expr, parse_bitwise_or_expression());
        } else if (match(TK_DIVISION)) {
            oper = matched;
            expr = new BinOp(EXPR_DIVISION, oper, expr, parse_bitwise_or_expression());
        } else if (match(TK_INTEGER_DIVISION)) {
            oper = matched;
            expr = new BinOp(EXPR_INTEGER_DIVISION, oper, expr, parse_bitwise_or_expression());
        } else if (match(TK_MODULO)) {
            oper = matched;
            expr = new BinOp(EXPR_MODULO, oper, expr, parse_bitwise_or_expression());
        } else if (match(TK_POWER)) {
            oper = matched;
            expr = new BinOp(EXPR_MODULO, oper, expr, parse_bitwise_or_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_bitwise_or_expression() {
    Token oper;
    Expression* expr = parse_bitwise_xor_expression();

    while (true) {
        if (match(TK_BITWISE_OR)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_OR, oper, expr, parse_bitwise_xor_expression()); 
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_bitwise_xor_expression() {
    Token oper;
    Expression* expr = parse_bitwise_and_expression();

    while (true) {
        if (match(TK_BITWISE_XOR)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_XOR, oper, expr, parse_bitwise_and_expression()); 
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_bitwise_and_expression() {
    Token oper;
    Expression* expr = parse_shift_expression();

    while (true) {
        if (match(TK_BITWISE_AND)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_AND, oper, expr, parse_shift_expression()); 
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_shift_expression() {
    Token oper;
    Expression* expr = parse_unary_expression();

    while (true) {
        if (match(TK_SLL)) {
            oper = matched;
            expr = new BinOp(EXPR_SLL, oper, expr, parse_unary_expression()); 
        } else if (match(TK_SRL)) {
            oper = matched;
            expr = new BinOp(EXPR_SRL, oper, expr, parse_unary_expression()); 
        } else if (match(TK_SRA)) {
            oper = matched;
            expr = new BinOp(EXPR_SRA, oper, expr, parse_unary_expression()); 
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_unary_expression() {
    Token oper;
    Expression* expr = nullptr;

    if (match(TK_LOGICAL_NOT)) {
        oper = matched;
        expr = new UnOp(EXPR_LOGICAL_NOT_OPER, oper, parse_unary_expression());
    } else if (match(TK_NOT)) {
        oper = matched;
        expr = new UnOp(EXPR_LOGICAL_NOT, oper, parse_unary_expression());
    } else if (match(TK_BITWISE_AND)) {
        oper = matched;
        expr = new UnOp(EXPR_ADDRESS_OF, oper, parse_unary_expression());
    } else if (match(TK_TIMES)) {
        oper = matched;
        expr = new UnOp(EXPR_DEREFERENCE, oper, parse_unary_expression());
    } else if (match(TK_POWER)) {
        oper = matched;
        expr = new UnOp(EXPR_DEREFERENCE, oper, parse_unary_expression());
        expr = new UnOp(EXPR_DEREFERENCE, oper, expr);
    } else if (match(TK_BITWISE_NOT)) {
        oper = matched;
        expr = new UnOp(EXPR_BITWISE_NOT, oper, parse_unary_expression());
    } else if (match(TK_MINUS)) {
        oper = matched;
        expr = new UnOp(EXPR_UNARY_MINUS, oper, parse_unary_expression());
    } else if (match(TK_PLUS)) {
        oper = matched;
        expr = new UnOp(EXPR_UNARY_PLUS, oper, parse_unary_expression());
    } else if (match(TK_INC)) {
        oper = matched;
        expr = new UnOp(EXPR_PRE_INC, oper, parse_unary_expression());
    } else if (match(TK_DEC)) {
        oper = matched;
        expr = new UnOp(EXPR_PRE_DEC, oper, parse_unary_expression());
    } else if (match(TK_SIZEOF)) {
        oper = matched;
        expect(TK_LEFT_PARENTHESIS);
        expr = new UnOp(EXPR_SIZEOF, oper, parse_unary_expression());
        expect(TK_RIGHT_PARENTHESIS);
    } else {
        expr = parse_postfix_expression();
    }

    return expr;
}

Expression* Parser::parse_postfix_expression() {
    Token oper;
    Expression* expr = parse_primary_expression();

    while (true) {
        if (match(TK_DOT)) {
            oper = matched;
            expr = new BinOp(EXPR_DOT, oper, expr, parse_identifier_expression());
        } else if (match(TK_ARROW)) {
            oper = matched;
            expr = new BinOp(EXPR_ARROW, oper, expr, parse_identifier_expression());
        } else if (match(TK_LEFT_SQUARE_BRACKET)) {
            oper = matched;
            expr = new BinOp(EXPR_INDEX, oper, expr, parse_expression());
            expect(TK_RIGHT_SQUARE_BRACKET);
        } else if (match(TK_LEFT_PARENTHESIS)) {
            oper = matched;
            expr = new BinOp(EXPR_CALL, oper, expr, parse_argument_list());
            expect(TK_RIGHT_PARENTHESIS);
        } else {
            break;
        }
    }

    return expr;
}

ExpressionList* Parser::parse_argument_list() {
    ExpressionList* arguments = new ExpressionList(EXPR_ARGS);

    if (!lookahead(TK_RIGHT_PARENTHESIS)) {
        arguments->add_expression(parse_expression());

        while (match(TK_COMMA)) {
            arguments->add_expression(parse_expression());
        }
    }

    return arguments;
}

Expression* Parser::parse_primary_expression() {
    Expression* expr = nullptr;

    if (lookahead(TK_LEFT_PARENTHESIS)) {
        expr = parse_parenthesis_or_tuple();
    } else if (lookahead(TK_LEFT_SQUARE_BRACKET)) {
        expr = parse_list_expression();
    } else if (lookahead(TK_LEFT_CURLY_BRACKET)) {
        expr = parse_array_or_hash();
    } else if (match(TK_LITERAL_INTEGER)) {
        expr = new Literal(EXPR_LITERAL_INTEGER, matched);
    } else if (match(TK_LITERAL_FLOAT)) {
        expr = new Literal(EXPR_LITERAL_FLOAT, matched);
    } else if (match(TK_LITERAL_DOUBLE)) {
        expr = new Literal(EXPR_LITERAL_DOUBLE, matched);
    } else if (match(TK_LITERAL_CHAR)) {
        expr = new Literal(EXPR_LITERAL_CHAR, matched);
    } else if (match(TK_LITERAL_STRING)) {
        expr = new Literal(EXPR_LITERAL_STRING, matched);
    } else if (match(TK_LITERAL_SYMBOL)) {
        expr = new Literal(EXPR_LITERAL_SYMBOL, matched);
    } else if (match(TK_TRUE)) {
        expr = new Literal(EXPR_LITERAL_BOOL, matched);
    } else if (match(TK_FALSE)) {
        expr = new Literal(EXPR_LITERAL_BOOL, matched);
    } else if (match(TK_NULL)) {
        expr = new Literal(EXPR_LITERAL_NULL, matched);
    } else if (lookahead(TK_ID)) {
        expr = parse_scope_expression();
    } else if (match(TK_SCOPE)) {
        expr = new UnOp(EXPR_GLOBAL_SCOPE, parse_identifier_expression());
    }

    return expr;
}

Expression* Parser::parse_parenthesis_or_tuple() {
    Token oper;
    Expression* expr = nullptr;
    ExpressionList* list = nullptr;

    expect(TK_LEFT_PARENTHESIS);
    oper = matched;
    expr = parse_expression();

    if (lookahead(TK_COMMA)) {
        list = new ExpressionList(EXPR_TUPLE, expr);

        while (match(TK_COMMA)) {
            if (!lookahead(TK_RIGHT_PARENTHESIS)) {
                list->add_expression(parse_expression());
            }
        }

        expr = list;
    } else if (lookahead(TK_RIGHT_PARENTHESIS)) {
        expr = new UnOp(EXPR_PARENTHESIS, oper, expr);
    }

    expect(TK_RIGHT_PARENTHESIS);

    return expr;
}

Expression* Parser::parse_list_expression() {
    ExpressionList* list = new ExpressionList(EXPR_LIST);

    expect(TK_LEFT_SQUARE_BRACKET);

    if (!lookahead(TK_RIGHT_SQUARE_BRACKET)) {
        list->add_expression(parse_expression());

        while (match(TK_COMMA)) {
            if (!lookahead(TK_RIGHT_SQUARE_BRACKET)) {
                list->add_expression(parse_expression());
            }
        }
    }

    expect(TK_RIGHT_SQUARE_BRACKET);
    return list;
}

Expression* Parser::parse_array_or_hash() {
    Expression* expr = nullptr;
    ExpressionList* list = nullptr;

    expect(TK_LEFT_CURLY_BRACKET);

    if (!lookahead(TK_RIGHT_CURLY_BRACKET)) {
        expr = parse_expression();

        if (expr->get_kind() == EXPR_ID && lookahead(TK_COLON)) {
            list = parse_hash(expr);
            list->set_kind(EXPR_HASH);
        } else {
            list = new ExpressionList(EXPR_ARRAY, expr);

            while (match(TK_COMMA)) {
                if (!lookahead(TK_RIGHT_CURLY_BRACKET)) {
                    list->add_expression(parse_expression());
                }
            }
        }
    }

    expect(TK_RIGHT_CURLY_BRACKET);
    return list;
}

ExpressionList* Parser::parse_hash(Expression* key) {
    Expression* expr = nullptr;
    ExpressionList* hash = new ExpressionList(EXPR_HASH_RAW);

    expect(TK_COLON);
    expr = new BinOp(EXPR_HASH_PAIR, key, parse_expression());
    hash->add_expression(expr);

    while (match(TK_COMMA)) {
        key = parse_identifier_expression();
        expect(TK_COLON);
        expr = new BinOp(EXPR_HASH_PAIR, key, parse_expression());
        hash->add_expression(expr);
    }

    return hash;
}

Expression* Parser::parse_scope_expression() {
    Token oper;
    Expression* expr = parse_identifier_expression();

    if (match(TK_SCOPE)) {
        oper = matched;
        expr = new BinOp(EXPR_SCOPE, oper, expr, parse_identifier_expression());
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

    Token token;
    token.set_kind(kind);
    std::cout << "parser error\n";
    std::cout << token.to_str() << std::endl;
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
