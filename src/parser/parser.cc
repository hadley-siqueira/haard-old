#include <iostream>
#include <sstream>
#include "parser/parser.h"
#include "log/actions.h"
#include "log/errors.h"

using namespace haard;

Parser::Parser() {
    idx = 0;
}

Module* Parser::read(std::string path, std::string relative_path) {
    Scanner s;
    Module* module = nullptr;

    idx = 0;
    set_path(path);
    tokens = s.read(path);
    module = parse_module();
    module->set_path(path);
    module->set_relative_path(relative_path);

    return module;
}

Expression* Parser::read_expression_from_string(std::string str) {
    Scanner s;
    Expression* expr;

    tokens = s.read_from_string(str);
    expr = parse_expression();

    return expr;
}

Function* Parser::read_function_from_string(std::string str) {
    Scanner s;

    tokens = s.read_from_string(str);
    return parse_function();
}

Class* Parser::read_class_from_string(std::string str) {
    Scanner s;

    tokens = s.read_from_string(str);
    return parse_class();
}

Module* Parser::parse_module() {
    Module* module = new Module();

    while (true) {
        if (lookahead(TK_IMPORT)) {
            module->add_import(parse_import());
        } else if (lookahead(TK_DEF)) {
            module->add_function(parse_function());
        } else if (lookahead(TK_CLASS)) {
            module->add_class(parse_class());
        } else if (lookahead(TK_STRUCT)) {
            module->add_struct(parse_struct());
        } else if (lookahead(TK_ENUM)) {
            module->add_enum(parse_enum());
        } else if (lookahead(TK_UNION)) {
            module->add_union(parse_union());
        } else if (lookahead(TK_AT)) {
            parse_annotation();
        } else if (match(TK_EOF)) {
            break;
        } else {
            log_error_and_exit(error_unexpected_token(path, tokens[idx]));
        }
    }

    return module;
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
    Class* klass = new Class();
    int begin;
    int end;

    if (annotations.size() > 0) {
        klass->set_annotations(annotations);
        annotations.clear();
    }

    expect(TK_CLASS);
    begin = matched.get_begin();

    expect(TK_ID);
    klass->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        klass->set_template_header(parse_template_header(), true);
    }

    if (match(TK_LEFT_PARENTHESIS)) {
        klass->set_super_type(parse_type());
        expect(TK_RIGHT_PARENTHESIS);
    }

    expect(TK_COLON);
    indent();

    while (is_indentend()) {
        if (lookahead(TK_DEF)) {
            klass->add_method(parse_function());
        } else if (lookahead(TK_ID)) {
            klass->add_field(parse_class_variable());
        } else if (lookahead(TK_AT)) {
            parse_annotation();
        } else if (match(TK_PASS)) {
            break;
        } else {
            break;
        }
    }

    end = matched.get_end();
    dedent();

    klass->set_begin(begin);
    klass->set_end(end);
    return klass;
}

Struct* Parser::parse_struct() {
    Struct* data = new Struct();
    int begin;
    int end;

    if (annotations.size() > 0) {
        data->set_annotations(annotations);
        annotations.clear();
    }

    expect(TK_STRUCT);
    begin = matched.get_begin();

    expect(TK_ID);
    data->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        data->set_template_header(parse_template_header(), true);
    }

    expect(TK_COLON);
    indent();

    while (is_indentend()) {
        if (lookahead(TK_DEF)) {
            data->add_method(parse_function());
        } else if (lookahead(TK_ID)) {
            data->add_field(parse_field());
        } else if (lookahead(TK_AT)) {
            parse_annotation();
        } else if (match(TK_PASS)) {
            break;
        } else {
            break;
        }
    }

    end = matched.get_end();
    dedent();

    data->set_begin(begin);
    data->set_end(end);

    return data;
}

Enum* Parser::parse_enum() {
    Enum* data = new Enum();
    int begin;
    int end;

    if (annotations.size() > 0) {
        data->set_annotations(annotations);
        annotations.clear();
    }

    expect(TK_ENUM);
    begin = matched.get_begin();

    expect(TK_ID);
    data->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        data->set_template_header(parse_template_header(), true);
    }

    if (match(TK_LEFT_PARENTHESIS)) {
        data->set_super_type(parse_type());
        expect(TK_RIGHT_PARENTHESIS);
    }

    expect(TK_COLON);
    indent();

    while (is_indentend()) {
        if (lookahead(TK_DEF)) {
            data->add_method(parse_function());
        } else if (lookahead(TK_ID)) {
            data->add_field(parse_enum_field());
        } else if (lookahead(TK_AT)) {
            parse_annotation();
        } else if (match(TK_PASS)) {
            break;
        } else {
            break;
        }
    }

    end = matched.get_end();
    dedent();

    data->set_begin(begin);
    data->set_end(end);

    return data;
}

Union* Parser::parse_union() {
    Union* data = new Union();
    int begin;
    int end;

    if (annotations.size() > 0) {
        data->set_annotations(annotations);
        annotations.clear();
    }

    expect(TK_UNION);
    begin = matched.get_begin();

    expect(TK_ID);
    data->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        data->set_template_header(parse_template_header(), true);
    }

    expect(TK_COLON);
    indent();

    while (is_indentend()) {
        if (lookahead(TK_DEF)) {
            data->add_method(parse_function());
        } else if (lookahead(TK_ID)) {
            data->add_field(parse_field());
        } else if (lookahead(TK_AT)) {
            parse_annotation();
        } else if (match(TK_PASS)) {
            break;
        } else {
            break;
        }
    }

    end = matched.get_end();
    dedent();

    data->set_begin(begin);
    data->set_end(end);

    return data;
}

Variable* Parser::parse_field() {
    Variable* field = new Variable();

    expect(TK_ID);
    field->set_from_token(matched);

    expect(TK_COLON);
    field->set_type(parse_type());

    if (match(TK_ASSIGNMENT)) {
        field->set_initial_value(parse_expression());
    }

    return field;
}

Variable* Parser::parse_enum_field() {
    Variable* field = new Variable();

    expect(TK_ID);
    field->set_from_token(matched);

    if (match(TK_COLON)) {
        field->set_type(parse_type());
    }

    if (match(TK_ASSIGNMENT)) {
        field->set_initial_value(parse_expression());
    }

    return field;
}

TemplateHeader* Parser::parse_template_header() {
    TemplateHeader* header = new TemplateHeader();

    expect(TK_BEGIN_TEMPLATE);
    header->add_type(parse_type());

    while (match(TK_COMMA)) {
        header->add_type(parse_type());
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
    int begin;
    int end;

    Type* type;
    Function* function = new Function();

    if (annotations.size() > 0) {
        function->set_annotations(annotations);
        annotations.clear();
    }

    expect(TK_DEF);
    begin = matched.get_begin();

    expect(TK_ID);
    function->set_from_token(matched);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        function->set_template_header(parse_template_header(), true);
    }

    expect(TK_COLON);
    indent();
    type = parse_type();

    if (type == nullptr) {
        //log_error(error_message_no_return_type(path, tokens[idx - 1]));
        DBG; exit(0);
    } 

    function->set_return_type(type);

    if (has_parameters()) {
        parse_parameters(function);
    }

    function->set_statements(parse_compound_statement());
    end = matched.get_end();
    dedent();

    function->set_begin(begin);
    function->set_end(end);

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

TypeList* Parser::parse_template_list_header() {
    TypeList* types = new TypeList();

    expect(TK_BEGIN_TEMPLATE);
    expect(TK_ID);
    types->add_type(new NamedType(matched));

    while (match(TK_COMMA)) {
        expect(TK_ID);
        types->add_type(new NamedType(matched));
    }

    expect(TK_END_TEMPLATE);
    return types;
}

TypeList* Parser::parse_template_list() {
    TypeList* types = new TypeList();

    expect(TK_BEGIN_TEMPLATE);
    types->add_type(parse_type());

    while (match(TK_COMMA)) {
        types->add_type(parse_type());
    }

    expect(TK_END_TEMPLATE);
    return types;
}

Type* Parser::parse_type() {
    return parse_function_type();
}

Type* Parser::parse_function_type() {
    FunctionType* ftype = nullptr;
    Type* type = parse_tuple_type();

    if (lookahead(TK_ARROW)) {
        ftype = new FunctionType();

        while (match(TK_ARROW)) {
            ftype->add_param_type(type);
            type = parse_tuple_type();
        }

        ftype->set_return_type(type);
        type = ftype;
    }

    return type;
}

Type* Parser::parse_tuple_type() {
    TupleType* tuple_type = nullptr;

    if (match(TK_LEFT_PARENTHESIS)) {
        tuple_type = new TupleType();

        tuple_type->add_type(parse_type());

        while (match(TK_COMMA)) {
            tuple_type->add_type(parse_type());
        }

        expect(TK_RIGHT_PARENTHESIS);
        return tuple_type;
    }

    return parse_primary_type();
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
        if (next_token_same_line() && match(TK_TIMES)) {
            type = new IndirectionType(TYPE_POINTER, matched, type);
        } else if (next_token_same_line() && match(TK_POWER)) {
            type = new IndirectionType(TYPE_POINTER, matched, type);
            type = new IndirectionType(TYPE_POINTER, matched, type);
        } else if (next_token_same_line() && match(TK_BITWISE_AND)) {
            type = new IndirectionType(TYPE_REFERENCE, matched, type);
        } else if (next_token_same_line() && match(TK_LEFT_SQUARE_BRACKET)) {
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
    Expression* condition;
    WhileStatement* stmt = new WhileStatement();

    expect(TK_WHILE);
    stmt->set_from_token(matched);
    condition = parse_expression();

    if (condition == nullptr) {
        log_error_and_exit(error_expected_while_expression(path, stmt));
    }

    stmt->set_condition(condition);
    expect(TK_COLON);
    indent();
    stmt->set_statements(parse_compound_statement());
    dedent();

    return stmt;
}

ForStatement* Parser::parse_for_statement() {
    ForStatement* stmt = new ForStatement();

    expect(TK_FOR);
    parse_for_statement_init(stmt);
    parse_for_statement_condition(stmt);
    parse_for_statement_increment(stmt);

    expect(TK_COLON);
    indent();

    if (next_token_same_line()) {
        DBG;
        log_error_and_exit("<red>error: </red>Unexpected tokens after colon. Expected them to be on a new line");
    }

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
            //bin->set_left(new ExpressionList(EXPR_TUPLE, exprs));
            DBG; exit(0);
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

Statement* Parser::parse_if_statement() {
    If* stmt = new If();

    expect(TK_IF);
    stmt->set_from_token(matched);
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

Statement* Parser::parse_elif_statement() {
    Expression* condition;
    Elif* stmt = new Elif();

    expect(TK_ELIF);
    stmt->set_from_token(matched);
    condition = parse_expression();

    if (condition == nullptr) {
        log_error_and_exit(error_expected_elif_expression(path, stmt));
    }

    stmt->set_condition(condition);

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

Statement* Parser::parse_else_statement() {
    Else* stmt = new Else();

    expect(TK_ELSE);
    stmt->set_from_token(matched);

    expect(TK_COLON);
    indent();
    stmt->set_statements(parse_compound_statement());
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

void Parser::parse_annotation() {
    expect(TK_AT);
    expect_on_same_line(TK_ID);
    Annotation* an = new Annotation();
    an->set_value(matched.get_lexeme());
    annotations.push_back(an);
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
            expr = new Assignment(oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_AND_ASSIGNMENT)) {
            oper = matched;
            expr = new BitwiseAndAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_XOR_ASSIGNMENT)) {
            oper = matched;
            expr = new BitwiseXorAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_OR_ASSIGNMENT)) {
            oper = matched;
            expr = new BitwiseOrAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_BITWISE_NOT_ASSIGNMENT)) {
            oper = matched;
            expr = new BitwiseNotAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new DivisionAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_INTEGER_DIVISION_ASSIGNMENT)) {
            oper = matched;
            expr = new IntegerDivisionAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_MINUS_ASSIGNMENT)) {
            oper = matched;
            expr = new MinusAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_MODULO_ASSIGNMENT)) {
            oper = matched;
            expr = new ModuloAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_PLUS_ASSIGNMENT)) {
            oper = matched;
            expr = new PlusAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_TIMES_ASSIGNMENT)) {
            oper = matched;
            expr = new TimesAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_SLL_ASSIGNMENT)) {
            oper = matched;
            expr = new ShiftLeftLogicalAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_SRA_ASSIGNMENT)) {
            oper = matched;
            expr = new ShiftRightArithmeticAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_SRL_ASSIGNMENT)) {
            oper = matched;
            expr = new ShiftRightLogicalAssignment(oper, expr, parse_cast_expression());
        } else if (match(TK_SPECIAL_ASSIGNMENT)) {
            oper = matched;
            expr = new SpecialAssignment(oper, expr, parse_cast_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_cast_expression() {
    Expression* expr = parse_logical_or_expression();

    if (match(TK_AS)) {
        expr = new Cast(expr, parse_type());
    }

    return expr;
}

Expression* Parser::parse_logical_or_expression() {
    Token oper;
    Expression* expr = parse_logical_and_expression();

    while (true) {
        if (match(TK_OR)) {
            oper = matched;
            expr = new LogicalOr(oper, expr, parse_logical_and_expression());
        } else if (match(TK_LOGICAL_OR)) {
            oper = matched;
            expr = new LogicalOrOper(oper, expr, parse_logical_and_expression());
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
            expr = new LogicalAnd(oper, expr, parse_equality_expression());
        } else if (match(TK_LOGICAL_AND)) {
            oper = matched;
            expr = new LogicalAndOper(oper, expr, parse_equality_expression());
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
            expr = new Equal(oper, expr, parse_relational_expression());
        } else if (match(TK_NE)) {
            oper = matched;
            expr = new NotEqual(oper, expr, parse_relational_expression());
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
            expr = new LessThan(oper, expr, parse_range_expression());
        } else if (match(TK_GT)) {
            oper = matched;
            expr = new GreaterThan(oper, expr, parse_range_expression());
        } else if (match(TK_LE)) {
            oper = matched;
            expr = new LessThanOrEqual(oper, expr, parse_range_expression());
        } else if (match(TK_GE)) {
            oper = matched;
            expr = new GreaterThanOrEqual(oper, expr, parse_range_expression());
        } else if (match(TK_IN)) {
            oper = matched;
            expr = new In(oper, expr, parse_range_expression());
        } else if (match(TK_NOT)) {
            oper = matched;
            expect(TK_IN);
            expr = new NotIn(oper, expr, parse_range_expression());
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
            expr = new InclusiveRange(oper, expr, parse_arith_expression());
        } else if (match(TK_EXCLUSIVE_RANGE)) {
            oper = matched;
            expr = new ExclusiveRange(oper, expr, parse_arith_expression());
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
            expr = new Plus(oper, expr, parse_term_expression());
        } else if (match(TK_MINUS)) {
            oper = matched;
            expr = new Minus(oper, expr, parse_term_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_term_expression() {
    Token oper;
    Expression* expr = parse_power_expression();

    while (true) {
        if (next_token_same_line() && lookahead(TK_TIMES)) {
            match(TK_TIMES);
            oper = matched;
            expr = new Times(oper, expr, parse_power_expression());
        } else if (match(TK_DIVISION)) {
            oper = matched;
            expr = new Division(oper, expr, parse_power_expression());
        } else if (match(TK_INTEGER_DIVISION)) {
            oper = matched;
            expr = new IntegerDivision(oper, expr, parse_power_expression());
        } else if (match(TK_MODULO)) {
            oper = matched;
            expr = new Modulo(oper, expr, parse_power_expression());
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::parse_power_expression() {
    Token oper;
    Expression* expr = parse_bitwise_or_expression();

    while (true) {
        if (match(TK_POWER)) {
            oper = matched;
            expr = new Power(oper, expr, parse_bitwise_or_expression());
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
            expr = new BitwiseOr(oper, expr, parse_bitwise_xor_expression());
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
            expr = new BitwiseXor(oper, expr, parse_bitwise_and_expression());
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
            expr = new BitwiseAnd(oper, expr, parse_shift_expression());
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
            expr = new ShiftLeftLogical(oper, expr, parse_unary_expression());
        } else if (match(TK_SRL)) {
            oper = matched;
            expr = new ShiftRightLogical(oper, expr, parse_unary_expression());
        } else if (match(TK_SRA)) {
            oper = matched;
            expr = new ShiftRightArithmetic(oper, expr, parse_unary_expression());
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
        expr = new LogicalNotOper(oper, parse_unary_expression());
    } else if (match(TK_NOT)) {
        oper = matched;
        expr = new LogicalNot(oper, parse_unary_expression());
    } else if (match(TK_BITWISE_AND)) {
        oper = matched;
        expr = new AddressOf(oper, parse_unary_expression());
    } else if (match(TK_TIMES)) {
        oper = matched;
        expr = new Dereference(oper, parse_unary_expression());
    } else if (match(TK_POWER)) {
        oper = matched;
        expr = new Dereference(oper, parse_unary_expression());
        expr = new Dereference(oper, expr);
    } else if (match(TK_BITWISE_NOT)) {
        oper = matched;
        expr = new BitwiseNot(oper, parse_unary_expression());
    } else if (match(TK_MINUS)) {
        oper = matched;
        expr = new UnaryMinus(oper, parse_unary_expression());
    } else if (match(TK_PLUS)) {
        oper = matched;
        expr = new UnaryPlus(oper, parse_unary_expression());
    } else if (match(TK_INC)) {
        oper = matched;
        expr = new PreIncrement(oper, parse_unary_expression());
    } else if (match(TK_DEC)) {
        oper = matched;
        expr = new PreDecrement(oper, parse_unary_expression());
    } else if (match(TK_SIZEOF)) {
        oper = matched;
        expect(TK_LEFT_PARENTHESIS);
        expr = new Sizeof(oper, parse_unary_expression());
        expect(TK_RIGHT_PARENTHESIS);
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
            expr = new Dot(oper, expr, parse_identifier_expression());
        } else if (match(TK_ARROW)) {
            oper = matched;
            expr = new Arrow(oper, expr, parse_identifier_expression());
        } else if (match(TK_LEFT_SQUARE_BRACKET)) {
            oper = matched;
            expr = new Index(oper, expr, parse_expression());
            expect(TK_RIGHT_SQUARE_BRACKET);
        } else if (next_token_same_line() && match(TK_LEFT_PARENTHESIS)) {
            oper = matched;
            expr = new Call(oper, expr, parse_argument_list());
            expect(TK_RIGHT_PARENTHESIS);
        } else if (next_token_same_line() && match(TK_INC)) {
            oper = matched;
            expr = new PosIncrement(oper, expr);
        } else if (next_token_same_line() && match(TK_DEC)) {
            oper = matched;
            expr = new PosDecrement(oper, expr);
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
    ExpressionList* arguments = new ExpressionList();

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
    } else if (lookahead(TK_LITERAL_STRING)) {
        expr = parse_string_literal();
    } else if (match(TK_LITERAL_SYMBOL)) {
        expr = new Literal(EXPR_LITERAL_SYMBOL, matched);
    } else if (match(TK_TRUE)) {
        expr = new Literal(EXPR_LITERAL_BOOL, matched);
    } else if (match(TK_FALSE)) {
        expr = new Literal(EXPR_LITERAL_BOOL, matched);
    } else if (match(TK_NULL)) {
        expr = new Literal(EXPR_LITERAL_NULL, matched);
    } else if (match(TK_THIS)) {
        expr = new This(matched);
    } else if (lookahead(TK_ID)) {
        expr = parse_identifier_expression();
    } else if (lookahead(TK_SCOPE)) {
        expr = parse_identifier_expression();
    } else if (lookahead(TK_BITWISE_OR)) {
        expr = parse_anonymous_function();
    }

    return expr;
}

Expression* Parser::parse_string_literal() {
    Expression* expr;
    StringBuilder* builder;
    std::vector<std::string> parts;

    expect(TK_LITERAL_STRING);
    std::string str(matched.get_lexeme());

    if (has_interpolation(str)) {
        builder = new StringBuilder();
        parts = split_interpolation(str);

        for (int i = 0; i < parts.size(); ++i) {
            //std::cout << '>' << parts[i] << "<\n";

            if (parts[i].size() > 0) {
                if (parts[i][0] == '$' || parts[i][0] == '{') {
                    parts[i][0] = ' ';

                    Parser p;
                    builder->add_expression(p.read_expression_from_string(parts[i]));
                } else {
                    builder->add_expression(new Literal(EXPR_LITERAL_STRING, parts[i]));
                }
            }
        }

        expr = builder;
    } else {
        expr = new Literal(EXPR_LITERAL_STRING, matched);
    }

    return expr;
}

Expression* Parser::parse_parenthesis_or_tuple() {
    Token oper;
    Expression* expr = nullptr;
    Tuple* tuple = nullptr;

    expect(TK_LEFT_PARENTHESIS);
    oper = matched;
    expr = parse_expression();

    if (lookahead(TK_COMMA)) {
        tuple = new Tuple();
        tuple->add_expression(expr);

        while (match(TK_COMMA)) {
            if (!lookahead(TK_RIGHT_PARENTHESIS)) {
                tuple->add_expression(parse_expression());
            }
        }

        expr = tuple;
    } else if (lookahead(TK_RIGHT_PARENTHESIS)) {
        expr = new Parenthesis(oper, expr);
    }

    expect(TK_RIGHT_PARENTHESIS);
    return expr;
}

Expression* Parser::parse_list_expression() {
    List* list = new List();

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
    Array* array;

    expect(TK_LEFT_CURLY_BRACKET);

    if (!lookahead(TK_RIGHT_CURLY_BRACKET)) {
        expr = parse_expression();

        if (expr->get_kind() == EXPR_ID && lookahead(TK_COLON)) {
            expr = parse_hash(expr);
        } else {
            array = new Array();
            array->add_expression(expr);

            while (match(TK_COMMA)) {
                if (!lookahead(TK_RIGHT_CURLY_BRACKET)) {
                    array->add_expression(parse_expression());
                }
            }

            expr = array;
        }
    }

    expect(TK_RIGHT_CURLY_BRACKET);
    return expr;
}

Hash* Parser::parse_hash(Expression* key) {
    Expression* expr = nullptr;
    Hash* hash = new Hash();

    expect(TK_COLON);
    expr = new HashPair(key, parse_expression());

    hash->add_expression(expr);

    while (match(TK_COMMA)) {
        key = parse_identifier_expression();
        expect(TK_COLON);
        expr = new HashPair(key, parse_expression());
        hash->add_expression(expr);
    }

    return hash;
}

New* Parser::parse_new_expression() {
    New* expr = new New();

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
    Expression* expr = nullptr;

    expect(TK_DELETE);
    oper = matched;

    if (match(TK_LEFT_SQUARE_BRACKET)) {
        expect(TK_RIGHT_SQUARE_BRACKET);
        expr = new DeleteArray(oper, parse_expression());
    } else {
        expr = new Delete(oper, parse_expression());
    }

    return expr;
}

Identifier* Parser::parse_identifier_expression() {
    Identifier* id = nullptr;
    Token name;
    Token scope;

    // global scope
    if (match(TK_SCOPE)) {
        scope = matched;

        expect(TK_ID);
        name = matched;
    } else {
        expect(TK_ID);
        name = matched;

        if (match(TK_SCOPE)) {
            scope = name;

            expect(TK_ID);
            name = matched;
        }
    }

    id = new Identifier(scope, name);

    if (lookahead(TK_BEGIN_TEMPLATE)) {
        id->set_template_header(parse_template_header());
    }

    return id;
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

    log_error_and_exit(error_unexpected_token(path, tokens[idx]));
}

void Parser::expect_on_same_line(int kind) {
    if (next_token_same_line() && match(kind)) {
        return;
    }

    //log_error(error_message_expected_token(path, kind, tokens[idx]));
    DBG; exit(0);
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

std::vector<std::string> Parser::split_interpolation(std::string str) {
    std::vector<std::string> res;
    std::string buffer;
    char sep = str[0]; // is it a " or '?

    for (int i = 1; i < str.size() - 1; ++i) {
        if (str[i] == '\\') {
            buffer += str[i];
            buffer += str[i + 1];
            i += 1;
        } else if (str[i] == '$' || str[i] == '{' || str[i] == '}') {
            res.push_back(buffer);
            buffer = "";

            if (str[i] == '$' || str[i] == '{') {
                buffer += str[i];
            }

            if (str[i] == '$') {
                ++i;

                while (i < str.size() - 1 && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || (str[i] == '_'))) {
                    buffer += str[i];
                    ++i;
                }

                res.push_back(buffer);
                buffer = "";

                if (i < str.size() && str[i] != sep) {
                    buffer += str[i];
                }
            }
        } else {
            buffer += str[i];
        }
    }

    res.push_back(buffer);

    for (int i = 0; i < res.size(); ++i) {
        char c = res[i][0];

        if (c != '$' && c != '{') {
            std::stringstream ss;
            ss << '"' << res[i] << '"';
            res[i] = ss.str();
        }
    }

    return res;
}

bool Parser::has_interpolation(std::string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\\') {
            i += 2;
        } else if (str[i] == '$' || str[i] == '{') {
            return true;
        }
    }

    return false;
}

std::string Parser::get_path() const {
    return path;
}

void Parser::set_path(const std::string& value) {
    path = value;
}
