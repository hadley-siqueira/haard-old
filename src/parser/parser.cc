#include <iostream>
#include <sstream>
#include "utils/string_pool.h"
#include "parser/parser.h"
#include "log/messages.h"

using namespace haard;

Parser::Parser() {
    idx = 0;
    this->logger = nullptr;
}

Parser::Parser(Logger* logger) {
    idx = 0;
    this->logger = logger;
}

Source* Parser::read(std::string path) {
    Scanner s;
    Source* source = nullptr;

    idx = 0;
    this->path = path;
    tokens = s.read(path);
    source = parse_source();
    source->set_path(StringPool::get(path));

    return source;
}

Source* Parser::parse_source() {
    Source* source = new Source();

    while (true) {
        if (lookahead(TK_IMPORT)) {
            source->add_import(parse_import());
        } else if (lookahead(TK_DEF)) {
            source->add_function(parse_function());
        } else if (lookahead(TK_CLASS)) {
            source->add_class(parse_class());
        } else if (match(TK_EOF)) {
            break;
        } else {
            logger->error(error_message_unexpected_token(path, tokens[idx]));
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

Class* Parser::parse_class() {
    Token name;
    Class* klass = new Class();

    expect(TK_CLASS);
    expect(TK_ID);
    klass->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        klass->set_template_header(parse_template_header());
    }

    if (match(TK_LEFT_PARENTHESIS)) {
        klass->set_super_class(parse_type());
        expect(TK_RIGHT_PARENTHESIS);
    }

    expect(TK_COLON);
    indent();

    while (is_indentend()) {
        if (lookahead(TK_DEF)) {
            klass->add_method(parse_function());
        } else if (lookahead(TK_ID)){
            klass->add_variable(parse_class_variable());
        } else {
            break;
        }
    }

    dedent();

    return klass;

}

TemplateHeader* Parser::parse_template_header() {
    TemplateHeader* header = new TemplateHeader();

    expect(TK_BEGIN_TEMPLATE);
    expect(TK_ID);
    header->add_type(new TemplateType(matched));

    while (match(TK_COMMA)) {
        expect(TK_ID);
        header->add_type(new TemplateType(matched));
    }

    expect(TK_END_TEMPLATE);
    return header;
}

Variable* Parser::parse_class_variable() {
    Variable* var = new Variable();

    expect(TK_ID);
    var->set_from_token(matched);

    expect(TK_COLON);
    var->set_type(parse_type());

    return var;
}

Function* Parser::parse_function() {
    Type* type;
    Function* function = new Function();

    expect(TK_DEF);
    expect(TK_ID);
    function->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        function->set_template_header(parse_template_header());
    }

    expect(TK_COLON);
    indent();
    type = parse_type();

    if (type == nullptr) {
        logger->error(error_message_no_return_type(path, tokens[idx - 1]));
    } 

    function->set_return_type(type);

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
    return parse_function_type();
}

Type* Parser::parse_function_type() {
    Token token;
    TypeList* ftype = nullptr;
    Type* type = parse_tuple_type();

    if (match(TK_ARROW)) {
        token = matched;
        ftype = new TypeList(TYPE_FUNCTION, token, type, parse_tuple_type());

        while (match(TK_ARROW)) {
            ftype->add_type(parse_tuple_type());
        }

        type = ftype;
    }

    return type;
}

TypeList* Parser::parse_template_list_header() {
    Type* type;
    TypeList* types = new TypeList();

    expect(TK_BEGIN_TEMPLATE);
    expect(TK_ID);
    types->add_type(new TemplateType(matched));

    while (match(TK_COMMA)) {
        expect(TK_ID);
        types->add_type(new TemplateType(matched));
    }

    expect(TK_END_TEMPLATE);
    return types;
}

TypeList* Parser::parse_template_list() {
    Type* type;
    TypeList* types = new TypeList();

    expect(TK_BEGIN_TEMPLATE);
    types->add_type(parse_type());

    while (match(TK_COMMA)) {
        types->add_type(parse_type());
    }

    expect(TK_END_TEMPLATE);
    return types;
}

Type* Parser::parse_tuple_type() {
    Token token;
    TypeList* ftype = nullptr;
    Type* type = parse_union_type();

    if (match(TK_COMMA)) {
        token = matched;
        ftype = new TypeList(TYPE_TUPLE, token, type, parse_union_type());

        while (match(TK_COMMA)) {
            ftype->add_type(parse_union_type());
        }

        type = ftype;
    }

    return type;
}

Type* Parser::parse_union_type() {
    Token token;
    TypeList* ftype = nullptr;
    Type* type = parse_primary_type();

    if (match(TK_BITWISE_OR)) {
        token = matched;
        ftype = new TypeList(TYPE_UNION, token, type, parse_primary_type());

        while (match(TK_BITWISE_OR)) {
            ftype->add_type(parse_primary_type());
        }

        type = ftype;
    }

    return type;
}

Type* Parser::parse_primary_type() {
    Token token;
    Expression* expr = nullptr;
    Identifier* id = nullptr;
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
    } else if (match(TK_ID)) {
        NamedType* named = new NamedType();
        named->set_name(matched.get_lexeme());

        if (match(TK_SCOPE)) {
            named->set_alias(named->get_name());
            expect(TK_ID);
            named->set_name(matched.get_lexeme());
        } 

        if (lookahead(TK_BEGIN_TEMPLATE)) {
            named->set_template_header(parse_template_header());
        }

        type = named;
    } else if (match(TK_LEFT_SQUARE_BRACKET)) {
        token = matched;

        type = parse_type();
        expect(TK_RIGHT_SQUARE_BRACKET);
        type = new ArrayListType(TYPE_LIST, matched, type);
    } else if (match(TK_LEFT_CURLY_BRACKET)) {
        token = matched;

        type = parse_type();
        expect(TK_COLON);
        type = new HashType(matched, type, parse_type());
        expect(TK_RIGHT_CURLY_BRACKET);
    } else if (match(TK_LEFT_PARENTHESIS)) {
        type = new IndirectionType(TYPE_PARENTHESIS, matched, parse_type());
        expect(TK_RIGHT_PARENTHESIS);
    }

    while (type != nullptr) {
        if (match(TK_TIMES)) {
            type = new IndirectionType(TYPE_POINTER, matched, type);
        } else if (match(TK_POWER)) {
            type = new IndirectionType(TYPE_POINTER, matched, type);
            type = new IndirectionType(TYPE_POINTER, matched, type);
        } else if (match(TK_BITWISE_AND)) {
            type = new IndirectionType(TYPE_REFERENCE, matched, type);
        } else if (match(TK_LEFT_SQUARE_BRACKET)) {
            token = matched;

            if (!lookahead(TK_RIGHT_SQUARE_BRACKET)) {
                expr = parse_expression();
            }

            expect(TK_RIGHT_SQUARE_BRACKET);
            type = new ArrayListType(TYPE_ARRAY, matched, type, expr);
        } else {
            break;
        }
    }

    return type;
}

Statement* Parser::parse_statement() {
    Statement* stmt = nullptr;

    if (lookahead(TK_WHILE)) {
        stmt = parse_while_statement();
    } else if (lookahead(TK_FOR)) {
        stmt = parse_for_statement();
    } else if (lookahead(TK_IF)) {
        stmt = parse_if_statement();
    } else if (lookahead(TK_RETURN)) {
        stmt = parse_jump_statement(TK_RETURN, STMT_RETURN);
    } else if (lookahead(TK_YIELD)) {
        stmt = parse_jump_statement(TK_YIELD, STMT_YIELD);
    } else if (lookahead(TK_CONTINUE)) {
        stmt = parse_jump_statement(TK_CONTINUE, STMT_CONTINUE);
    } else if (lookahead(TK_GOTO)) {
        stmt = parse_jump_statement(TK_GOTO, STMT_GOTO);
    } else if (lookahead(TK_BREAK)) {
        stmt = parse_jump_statement(TK_BREAK, STMT_BREAK);
    } else if (lookahead(TK_VAR)) {
        stmt = parse_variable_declaration();
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

ForStatement* Parser::parse_for_statement() {
    ForStatement* stmt = new ForStatement();
    Expression* expr1;
    Expression* expr2;
    Expression* expr3;

    expect(TK_FOR);
    parse_for_statement_init(stmt);
    parse_for_statement_condition(stmt);
    parse_for_statement_increment(stmt);

    expect(TK_COLON);
    indent();
    stmt->set_statements(parse_compound_statement());
    dedent();

    return stmt;
}

void Parser::parse_for_statement_init(ForStatement* stmt) {
    int kind;
    Expression* expr;
    BinOp* bin;
    bool flag;
    std::vector<Expression*> exprs;

    if (lookahead(TK_SEMICOLON)) return;

    expr = parse_expression();
    kind = expr->get_kind();

    flag = kind == EXPR_IN || kind == EXPR_NOT_IN;

    // for i in ... ??
    if (flag) {
        stmt->set_kind(STMT_FOREACH);
        stmt->set_condition(expr);
    } else { // for expr ; cond ; expr
        exprs.push_back(expr);

        while (match(TK_COMMA)) {
            exprs.push_back(parse_expression());
        }

        kind = exprs[exprs.size() - 1]->get_kind();
        flag = kind == EXPR_IN || kind == EXPR_NOT_IN;

        if (flag) { // for expr, expr, ..., expr in ...
            stmt->set_kind(STMT_FOREACH);

            bin = (BinOp*) exprs[exprs.size() - 1];
            exprs.pop_back();
            exprs.push_back(bin->get_left());
            bin->set_left(new ExpressionList(EXPR_TUPLE, exprs));
            stmt->set_condition(bin);
        } else { // for expr ; expr; expr
            for (int i = 0; i < exprs.size(); ++i) {
                stmt->add_initialization(exprs[i]);
            }
        }
    }
}

void Parser::parse_for_statement_condition(ForStatement* stmt) {
    if (match(TK_SEMICOLON)) {
        if (lookahead(TK_SEMICOLON)) return;

        stmt->set_condition(parse_expression());
    }
}
void Parser::parse_for_statement_increment(ForStatement* stmt) {
    if (match(TK_SEMICOLON)) {
        if (lookahead(TK_SEMICOLON)) return;

        stmt->add_increment(parse_expression());

        while (match(TK_COMMA)) {
            stmt->add_increment(parse_expression());
        }
    }
}

BranchStatement* Parser::parse_if_statement() {
    Token token;
    Expression* expression;
    CompoundStatement* statements;
    BranchStatement* stmt = new BranchStatement(STMT_IF);

    expect(TK_IF);
    stmt->set_condition(parse_expression());

    expect(TK_COLON);
    indent();
    stmt->set_true_statements(parse_compound_statement());
    dedent();

    if (lookahead(TK_ELIF) && is_indentend()) {
        stmt->set_false_statements(parse_elif_statement());
    } else if (lookahead(TK_ELSE) && is_indentend()) {
        stmt->set_false_statements(parse_else_statement());
    }

    return stmt;
}

BranchStatement* Parser::parse_elif_statement() {
    Token token;
    Expression* expression;
    CompoundStatement* statements;
    BranchStatement* stmt = new BranchStatement(STMT_ELIF);

    expect(TK_ELIF);
    token = matched;
    stmt->set_condition(parse_expression());

    expect(TK_COLON);
    indent();
    stmt->set_true_statements(parse_compound_statement());
    dedent();

    if (lookahead(TK_ELIF) && is_indentend()) {
        stmt->set_false_statements(parse_elif_statement());
    } else if (lookahead(TK_ELSE) && is_indentend()) {
        stmt->set_false_statements(parse_else_statement());
    }

    return stmt;
}

BranchStatement* Parser::parse_else_statement() {
    Token token;
    CompoundStatement* statements;
    BranchStatement* stmt = new BranchStatement(STMT_ELSE);

    expect(TK_ELSE);
    token = matched;

    expect(TK_COLON);
    indent();
    stmt->set_true_statements(parse_compound_statement());
    dedent();

    return stmt;
}

JumpStatement* Parser::parse_jump_statement(int tkind, int skind) {
    JumpStatement* stmt = new JumpStatement(skind);
    expect(tkind);

    if (next_token_same_line()) {
        stmt->set_expression(parse_expression());
    }

    return stmt;
}

CompoundStatement* Parser::parse_compound_statement() {
    CompoundStatement* statements = new CompoundStatement();

    if (match(TK_PASS)) {
        return statements;
    }

    while (is_indentend() && !lookahead(TK_RIGHT_CURLY_BRACKET)) {
        statements->add_statement(parse_statement());
    }

    return statements;
}

VarDeclaration* Parser::parse_variable_declaration() {
    VarDeclaration* decl;
    Variable* var;

    expect(TK_VAR);
    expect(TK_ID);
    var = new Variable(matched);
    decl = new VarDeclaration();
    decl->set_variable(var);

    if (match(TK_COLON)) {
        var->set_type(parse_type());
    }

    if (match(TK_ASSIGNMENT)) {
        decl->set_expression(parse_expression());
    }

    return decl;
}

Expression* Parser::parse_expression() {
    Expression* expr = nullptr;

    expr = parse_assignment_expression();

    return expr;
}

Expression* Parser::parse_assignment_expression() {
    Token oper;
    Expression* expr = parse_cast_expression();

    while (true) {
        if (match(TK_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_AND_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_AND_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_XOR_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_XOR_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_OR_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_OR_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_NOT_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_BITWISE_NOT_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_DIVISION_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_INTEGER_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_INTEGER_DIVISIO_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_MINUS_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_MINUS_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_MODULO_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_MODULO_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_PLUS_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_PLUS_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_TIMES_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_TIMES_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_SLL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SLL_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_SRA_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SRA_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_SRL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SRL_ASSIGN, oper, expr, parse_cast_expression());
        } else if (match(TK_SPECIAL_ASSIGNMENT)) {
            oper = matched;
            expr = new BinOp(EXPR_SPECIAL_ASSIGN, oper, expr, parse_cast_expression());
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
    Expression* expr = parse_range_expression();

    while (true) {
        if (match(TK_LT)) {
            oper = matched;
            expr = new BinOp(EXPR_LT, oper, expr, parse_range_expression());
        } else if (match(TK_GT)) {
            oper = matched;
            expr = new BinOp(EXPR_GT, oper, expr, parse_range_expression());
        } else if (match(TK_LE)) {
            oper = matched;
            expr = new BinOp(EXPR_LE, oper, expr, parse_range_expression());
        } else if (match(TK_GE)) {
            oper = matched;
            expr = new BinOp(EXPR_GE, oper, expr, parse_range_expression());
        } else if (match(TK_IN)) {
            oper = matched;
            expr = new BinOp(EXPR_IN, oper, expr, parse_range_expression());
        } else if (match(TK_NOT)) {
            oper = matched;
            expect(TK_IN);
            expr = new BinOp(EXPR_NOT_IN, oper, expr, parse_range_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_range_expression() {
    Token oper;
    Expression* expr = parse_arith_expression();

    while (true) {
        if (match(TK_INCLUSIVE_RANGE)) {
            oper = matched;
            expr = new BinOp(EXPR_INCLUSIVE_RANGE, oper, expr, parse_arith_expression());
        } else if (match(TK_EXCLUSIVE_RANGE)) {
            oper = matched;
            expr = new BinOp(EXPR_EXCLUSIVE_RANGE, oper, expr, parse_arith_expression());
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
        if (next_token_same_line() && lookahead(TK_TIMES)) {
            match(TK_TIMES);
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
    } else if (match(TK_DOUBLE_DOLAR)) {
        oper = matched;
        expr = new UnOp(EXPR_DOUBLE_DOLAR, oper, parse_unary_expression());
    } else if (lookahead(TK_NEW)) {
        expr = parse_new_expression();
    } else if (lookahead(TK_DELETE)) {
        expr = parse_delete_expression();
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
        } else if (next_token_same_line() && match(TK_LEFT_PARENTHESIS)) {
            oper = matched;
            expr = new BinOp(EXPR_CALL, oper, expr, parse_argument_list());
            expect(TK_RIGHT_PARENTHESIS);
        } else if (next_token_same_line() && match(TK_INC)) {
            oper = matched;
            expr = new UnOp(EXPR_POS_INC, oper, expr);
        } else if (next_token_same_line() && match(TK_DEC)) {
            oper = matched;
            expr = new UnOp(EXPR_POS_DEC, oper, expr);
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_anonymous_function() {
    Token name;
    Type* type;
    Variable* var;
    Function* func = new Function();

    expect(TK_BITWISE_OR);

    if (!lookahead(TK_BITWISE_OR)) {
        expect(TK_ID);
        var = new Variable(matched);

        if (match(TK_COLON)) {
            var->set_type(parse_type());
        }

        func->add_parameter(var);

        while (match(TK_COMMA)) {
            expect(TK_ID);
            var = new Variable(matched);

            if (match(TK_COLON)) {
                var->set_type(parse_type());
            }

            func->add_parameter(var);
        }
    }

    expect(TK_BITWISE_OR);

    if (match(TK_ARROW)) {
        func->set_return_type(parse_type());
    }

    expect(TK_LEFT_CURLY_BRACKET);
    func->set_statements(parse_compound_statement());
    expect(TK_RIGHT_CURLY_BRACKET);

    return new FunctionExpression(func);
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
    } else if (match(TK_THIS)) {
        expr = new ThisExpression(matched);
    } else if (lookahead(TK_ID)) {
        expr = parse_scope_expression();

        if (lookahead(TK_BEGIN_TEMPLATE)) {
            expr = new TemplateExpression(expr, parse_template_list());
        }
    } else if (match(TK_SCOPE)) {
        expr = new UnOp(EXPR_GLOBAL_SCOPE, parse_identifier_expression());
    } else if (lookahead(TK_BITWISE_OR)) {
        expr = parse_anonymous_function();
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

NewExpression* Parser::parse_new_expression() {
    NewExpression* expr = new NewExpression();

    expect(TK_NEW);
    expr->set_new_type(parse_type());

    if (match(TK_LEFT_PARENTHESIS)) {
        expr->set_arguments(parse_argument_list());
        expect(TK_RIGHT_PARENTHESIS);
    }

    return expr;
}

Expression* Parser::parse_delete_expression() {
    Token oper;
    int kind = EXPR_DELETE;
    UnOp* expr = nullptr;

    expect(TK_DELETE);
    oper = matched;

    if (match(TK_LEFT_SQUARE_BRACKET)) {
        kind = EXPR_DELETE_ARRAY;
        expect(TK_RIGHT_SQUARE_BRACKET);
    }

    expr = new UnOp(kind, oper, parse_expression());
    return expr;
}

Expression *Parser::parse_cast_expression() {
    Expression* expr = parse_logical_or_expression();

    if (match(TK_AS)) {
        expr = new CastExpression(expr, parse_type());
    }

    return expr;
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

    logger->error(error_message_expected_token(path, kind, tokens[idx]));
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

bool Parser::next_token_same_line() {
    return matched.get_line() == tokens[idx].get_line();
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
