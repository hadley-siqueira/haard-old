#include <iostream>
#include <cstring>
#include "scope/scope_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"
#include "parser/parser.h"

using namespace haard;

ScopeBuilder::ScopeBuilder() {
    logger = nullptr;
    current_scope = nullptr;
    current_class = nullptr;
    current_function = nullptr;
    current_source = nullptr;
    var_counter = 0;
    function_counter = 0;
    class_counter = 0;
    sbuilder_counter = 0;
}

void ScopeBuilder::build(Sources* sources) {
    define_sources(sources);
    build_sources(sources);
}

void ScopeBuilder::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void ScopeBuilder::build_source(Source* source) {
    enter_scope(source->get_scope());
    current_source = source;

    for (int i = 0; i < source->classes_count(); ++i) {
        build_class(source->get_class(i));
    }

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    current_source = nullptr;
    leave_scope();
}

void ScopeBuilder::build_class(Class* klass) {
    current_class = klass;

    enter_scope(klass->get_scope());
    build_class_methods(klass);
    klass->calculate_variables_offset();
    leave_scope();

    current_class = nullptr;
}

void ScopeBuilder::build_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        build_function(klass->get_method(i));
    }
}

void ScopeBuilder::build_function(Function* function) {
    enter_scope(function->get_scope());
    current_function = function;

    var_counter = 0;

    if (function->is_constructor()) {
        add_members_initialization(function);
        add_parent_constructors_call(function);
    } else if (function->is_destructor()) {
        add_members_destruction(function);
        add_parent_destructors_call(function);
    }

    build_compound_statement(function->get_statements());

    leave_scope(true);
}

void ScopeBuilder::build_statement(Statement* statement) {
    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        build_while_statement((WhileStatement*) statement);
        break;

    case STMT_FOR:
        build_for_statement((ForStatement*) statement);
        break;

    case STMT_FOREACH:
        build_foreach_statement((ForStatement*) statement);
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
    case STMT_ELIF:
    case STMT_ELSE:
        build_branch_statement((BranchStatement*) statement);
        break;

    case STMT_RETURN:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_GOTO:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_YIELD:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_CONTINUE:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_BREAK:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_VAR_DECL:
        build_variable_declaration((VarDeclaration*) statement);
        break;

    default:
        std::cout << "unknown statement\n";
        DBG;
        exit(0);
    }
}

void ScopeBuilder::build_compound_statement(CompoundStatement* stmts) {
    enter_scope(stmts->get_scope());

    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }

    leave_scope(true);
}

void ScopeBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeBuilder::build_jump_statement(JumpStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeBuilder::build_while_statement(WhileStatement* statement) {
    enter_scope(statement->get_scope());

    build_expression(statement->get_condition());
    build_compound_statement(statement->get_statements());

    leave_scope(true);
}

void ScopeBuilder::build_for_statement(ForStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_kind() == STMT_FOR) {
        build_expression(statement->get_initialization());
        build_expression(statement->get_condition());
        build_expression(statement->get_increment());
    }

    build_compound_statement(statement->get_statements());
    leave_scope(true);
}

void ScopeBuilder::build_foreach_statement(ForStatement* statement) {
    BinOp* expr;
    enter_scope(statement->get_scope());

    expr = (BinOp*) statement->get_condition();
    build_expression(expr->get_right());

    if (is_new_var_assign(expr)) {
        create_new_var(expr);
    }

    build_expression(expr->get_left());
    /*
    if (expr->get_left()->get_kind() == EXPR_ID) {
        build_expression(expr->get_right());
    }*/

    build_compound_statement(statement->get_statements());
    leave_scope(true);
}

void ScopeBuilder::build_branch_statement(BranchStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_condition()) {
        build_expression(statement->get_condition());
    }

    if (statement->get_true_statements()) {
        build_statement(statement->get_true_statements());
    }

    leave_scope(true);

    if (statement->get_false_statements()) {
        build_statement(statement->get_false_statements());
    }
}

void ScopeBuilder::build_variable_declaration(VarDeclaration *statement) {
    Variable* var = statement->get_variable();

    build_expression(statement->get_expression());

    if (var->get_type() == nullptr) {
        var->set_type(statement->get_expression()->get_type());
    } else {
        link_type(var->get_type());
    }

    var->set_uid(var_counter++);
    var->set_kind(VAR_LOCAL);
    current_scope->define(statement->get_variable());
    current_function->add_variable(var);
}

void ScopeBuilder::build_expression(Expression* expression) {
    if (expression == nullptr) {
        return;
    }

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_SCOPE:
        build_scope(bin);
        break;

    case EXPR_ID:
        build_identifier((Identifier*) expression);
        break;

    case EXPR_THIS:
        build_this((ThisExpression*) expression);
        break;

    case EXPR_NEW:
        build_new((NewExpression*) expression);
        break;

    case EXPR_DELETE:
    case EXPR_DELETE_ARRAY:
        build_delete(un);
        break;

    case EXPR_PRE_INC:
        build_pre_inc(un);
        break;

    case EXPR_PRE_DEC:
        build_pre_dec(un);
        break;

    case EXPR_POS_INC:
        build_pos_inc(un);
        break;

    case EXPR_POS_DEC:
        build_pos_dec(un);
        break;

    case EXPR_PARENTHESIS:
        build_parenthesis(un);
        break;

    case EXPR_INDEX:
        build_index_access(bin);
        break;

    case EXPR_CALL:
        build_call(bin);
        break;

    case EXPR_DOT:
        build_dot(bin);
        break;

    case EXPR_ASSIGN:
        build_assignment(bin);
        break;

    case EXPR_LOGICAL_OR:
    case EXPR_LOGICAL_OR_OPER:
        build_logical_or(bin);
        break;

    case EXPR_LOGICAL_AND:
    case EXPR_LOGICAL_AND_OPER:
        build_logical_and(bin);
        break;

    case EXPR_EQ:
        build_equal(bin);
        break;

    case EXPR_NE:
        build_not_equal(bin);
        break;

    case EXPR_GT:
        build_greater_than(bin);
        break;

    case EXPR_LT:
        build_less_than(bin);
        break;

    case EXPR_GE:
        build_greater_or_equal_than(bin);
        break;

    case EXPR_LE:
        build_less_or_equal_than(bin);
        break;

    case EXPR_PLUS:
        build_plus(bin);
        break;

    case EXPR_MINUS:
        build_minus(bin);
        break;

    case EXPR_TIMES:
    case EXPR_DIVISION:
    case EXPR_INTEGER_DIVISION:
    case EXPR_MODULO:
    case EXPR_POWER:
        build_binop(bin);
        break;

    case EXPR_BITWISE_OR:
        build_bitwise_or(bin);
        break;

    case EXPR_BITWISE_XOR:
        build_bitwise_xor(bin);
        break;

    case EXPR_BITWISE_AND:
        build_bitwise_and(bin);
        break;

    case EXPR_SLL:
        build_sll(bin);
        break;

    case EXPR_SRL:
        build_sll(bin);
        break;

    case EXPR_SRA:
        build_sra(bin);
        break;

    case EXPR_ADDRESS_OF:
        build_address_of(un);
        break;

    case EXPR_DEREFERENCE:
        build_dereference(un);
        break;

    case EXPR_LITERAL_BOOL:
        build_literal(literal, TYPE_BOOL);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal(literal, TYPE_INT);
        break;

    case EXPR_LITERAL_FLOAT:
        build_literal(literal, TYPE_FLOAT);
        break;

    case EXPR_LITERAL_DOUBLE:
        build_literal(literal, TYPE_DOUBLE);
        break;

    case EXPR_LITERAL_CHAR:
        build_literal(literal, TYPE_CHAR);
        break;

    case EXPR_LITERAL_STRING:
        build_literal_string(literal);
        break;

    case EXPR_STRING_BUILDER:
        build_string_builder((StringBuilder*) expression);
        break;

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, TYPE_SYMBOL);
        break;

    case EXPR_IN:
        build_expression_in(bin);
        break;

    case EXPR_INCLUSIVE_RANGE:
        build_inclusive_range(bin);
        break;

    case EXPR_EXCLUSIVE_RANGE:
        break;

    case EXPR_ARGS:
    case EXPR_FOR_INIT:
    case EXPR_FOR_INC:
        build_expression_list(exprlist);
        break;

    case EXPR_CAST:
        build_cast_expression((CastExpression*) expression);
        break;

    case EXPR_SIZEOF:
        build_sizeof(un);
        break;
    }
}

void ScopeBuilder::build_identifier(Identifier* id) {
    Symbol* sym = current_scope->has(id->get_lexeme());

    if (!sym) {
        logger->error_and_exit(error_message_id_not_in_scope(current_source, id));
    }

    id->set_symbol(sym);
}

void ScopeBuilder::build_scope(BinOp* bin) {
    Identifier* alias = (Identifier*) bin->get_left();
    Identifier* id = (Identifier*) bin->get_right();

    Import* import = current_source->get_import_with_alias(alias->get_lexeme());

    if (import == nullptr) {
        logger->error_and_exit("There is no import with alias");
    }

    Scope* scope = import->get_source()->get_scope();
    Symbol* sym = scope->local_has(id->get_lexeme());

    if (!sym) {
        logger->error_and_exit(error_message_id_not_in_scope(current_source, id));
    }

    id->set_symbol(sym);
    bin->set_type(id->get_type());
}

void ScopeBuilder::build_this(ThisExpression* expr) {
    if (current_class == nullptr) {
        std::cout << "Error: using this outside class";
        DBG;
        exit(0);
    }

    expr->set_type(new IndirectionType(TYPE_POINTER, current_class->get_self_type()));
}

void ScopeBuilder::build_new(NewExpression* op) {
    Type* type;

    link_type(op->get_new_type());
    type = op->get_new_type();

    build_expression_list(op->get_arguments());

    if (type->get_kind() == TYPE_ARRAY) {
        ArrayListType* atype = (ArrayListType*) type;
        op->set_type(new IndirectionType(TYPE_POINTER, atype->get_subtype()));
    } else {
        op->set_type(new IndirectionType(TYPE_POINTER, op->get_new_type()));
    }
}

void ScopeBuilder::build_delete(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_assignment(BinOp* bin) {
    bool new_var = false;
    build_expression(bin->get_right());

    if (is_new_var_assign(bin)) {
        new_var = true;
        create_new_var(bin);
    }
 
    build_expression(bin->get_left());

    if (new_var && is_constructor_call(bin)) {
        bin->set_kind(EXPR_CONSTRUCTOR_ASSIGNMENT);
    }
}

void ScopeBuilder::build_pre_inc(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_pre_dec(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_pos_inc(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_pos_dec(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_parenthesis(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void ScopeBuilder::build_logical_or(BinOp* op) {
    build_relational(op);
}

void ScopeBuilder::build_logical_and(BinOp* op) {
    build_relational(op);
}

void ScopeBuilder::build_call(BinOp* bin) {
    Type* tl;
    Type* tr;
    FunctionType* ftype;
    TypeList* args;

    build_expression(bin->get_left());
    build_expression(bin->get_right());

    tl = bin->get_left()->get_type();
    tr = bin->get_right()->get_type();

    if (bin->get_left()->get_kind() == EXPR_SCOPE) {
        BinOp* scope = (BinOp*) bin->get_left();

        Identifier* id = (Identifier*) scope->get_right();
        Symbol* sym = id->get_symbol();

        if (tl->get_kind() == TYPE_FUNCTION) {
            int index = sym->get_overloaded((TypeList*) tr);

            if (index >= 0) {
                id->set_overloaded_index(index);
            } else {
                std::cout << "Error: function not overloaded with signature\n";
                DBG;
                exit(0);
            }

            Function* f = (Function*) id->get_symbol()->get_descriptor(id->get_overloaded_index());
            ftype = (FunctionType*) f->get_self_type();
            bin->set_type(ftype->get_return_type());
        }
    } else if (bin->get_left()->get_kind() == EXPR_ID) {
        Identifier* id = (Identifier*) bin->get_left();
        Symbol* sym = id->get_symbol();

        if (tl->get_kind() == TYPE_FUNCTION) {
            int index = sym->get_overloaded((TypeList*) tr);

            if (index >= 0) {
                id->set_overloaded_index(index);
            } else {
                std::cout << "Error: function not overloaded with signature\n";
                DBG;
                exit(0);
            }

            Function* f = (Function*) id->get_symbol()->get_descriptor(id->get_overloaded_index());
            ftype = (FunctionType*) f->get_self_type();
            bin->set_type(ftype->get_return_type());
        } else if (tl->get_kind() == TYPE_NAMED && sym->get_kind() == SYM_CLASS) {
            Class* klass = (Class*) sym->get_descriptor();
            args = (TypeList*) tr;

            Symbol* sym2 = klass->get_scope()->local_has("init");
            int index = -1;

            if (sym2) {
                index = sym2->get_overloaded(args);
            }

            if (index >= 0) {
                bin->set_constructor_call(sym2);
                bin->set_overloaded_index(index);
            } else {
                std::cout << "Error: constructor not overloaded with signature\n";
                DBG;
                exit(0);
            }

            bin->set_type(sym->get_type());
        } else {
            // FIXME
            std::cout << "Error: not a function to be called\n";
            DBG;
            exit(0);
        }
    } else if (bin->get_left()->get_kind() == EXPR_DOT) {
        BinOp* dot = (BinOp*) bin->get_left();
        Identifier* id = (Identifier*) dot->get_right();
        Symbol* sym = id->get_symbol();

        if (tl->get_kind() == TYPE_FUNCTION) {
            int index = sym->get_overloaded((TypeList*) tr);

            if (index >= 0) {
                id->set_overloaded_index(index);
            } else {
                std::cout << "Error: function not overloaded with signature\n";
                DBG;
                exit(0);
            }

            Function* f = (Function*) id->get_symbol()->get_descriptor(id->get_overloaded_index());
            ftype = (FunctionType*) f->get_self_type();
            bin->set_type(ftype->get_return_type());
        }
    } else if (bin->get_left()->get_kind() == EXPR_TEMPLATE) {
        // FIXME assuming that is a function type
        TemplateExpression* expr = (TemplateExpression*) bin->get_left();

        std::cout << expr->get_types()->to_str() << std::endl;
        exit(0);
    }
}

void ScopeBuilder::build_dot(BinOp* bin) {
    Scope* scope;
    Symbol* symbol;
    Identifier* field;
    Type* tl;

    build_expression(bin->get_left());

    if (bin->get_left()->get_type()->get_kind() == TYPE_POINTER) {
        IndirectionType* ptype = (IndirectionType*) bin->get_left()->get_type();
        tl = ptype->get_subtype();
    } else {
        tl = bin->get_left()->get_type();
    }

    //tl = bin->get_left()->get_type();
    scope = tl->get_scope();
    field = (Identifier*) bin->get_right();
    symbol = scope->has_field(field->get_lexeme());

    // FIXME
    /*std::cout << __FILE__ << ' ' << __LINE__ << std::endl;
    std::cout << "fix me because the overloaded is not good, the type scope is not good and maybe some code"
                 "here will go to the call. Call must set the last overloaded index. Refactor call generation"
                 "handle method types\n";*/

    if (symbol) {
        field->set_type(symbol->get_type());
        field->set_symbol(symbol);
        bin->set_type(symbol->get_type());
    } else {
        std::cout << "debbuging...\n";
        std::cout << tl->to_cpp() << std::endl;
        scope->debug();
        std::cout << field->get_lexeme() << ' ' << field->get_line() << std::endl;
        DBG;
        exit(0);
    }
}

void ScopeBuilder::build_index_access(BinOp* bin) {
    Type* tleft;
    IndirectionType* ptype;
    ArrayListType* atype;

    build_expression(bin->get_left());
    build_expression(bin->get_right());

    tleft = bin->get_left()->get_type();

    if (tleft->get_kind() == TYPE_POINTER) {
        ptype = (IndirectionType*) tleft;
        bin->set_type(ptype->get_subtype());
    } else if (tleft->get_kind() == TYPE_ARRAY) {
        atype = (ArrayListType*) tleft;
        bin->set_type(atype->get_subtype());
    }
}

void ScopeBuilder::build_expression_in(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());
    bin->set_type(new Type(TYPE_BOOL));
}

void ScopeBuilder::build_inclusive_range(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());
    //FIXME should set a range type?
    bin->set_type(bin->get_left()->get_type());
}

void ScopeBuilder::build_cast_expression(CastExpression* expr) {
    build_expression(expr->get_expression());
    link_type(expr->get_cast_type());
    expr->set_type(expr->get_cast_type());
}

void ScopeBuilder::build_plus(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_minus(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_bitwise_or(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_bitwise_xor(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_bitwise_and(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_sll(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_srl(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_sra(BinOp* bin) {
    build_binop(bin);
}

void ScopeBuilder::build_greater_than(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_less_than(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_greater_or_equal_than(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_less_or_equal_than(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_equal(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_not_equal(BinOp* bin) {
    build_relational(bin);
}

void ScopeBuilder::build_address_of(UnOp* op) {
    Type* type;

    build_expression(op->get_expression());

    type = op->get_expression()->get_type();
    type = new IndirectionType(TYPE_POINTER, type);

    op->set_type(type);
}

void ScopeBuilder::build_dereference(UnOp* op) {
    IndirectionType* ptype;

    build_expression(op->get_expression());
    ptype = (IndirectionType*) op->get_expression()->get_type();
    op->set_type(ptype->get_subtype());
}

void ScopeBuilder::build_binop(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());

    Type* tleft = bin->get_left()->get_type();
    Type* tright = bin->get_right()->get_type();

    tleft = tleft->promote(tright);

    // FIXME
    bin->set_type(tleft);
}

void ScopeBuilder::build_relational(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());

    // FIXME
    bin->set_type(new Type(TYPE_BOOL));
}

void ScopeBuilder::build_literal(Literal* literal, int kind) {
    literal->set_type(new Type(kind));
}

void ScopeBuilder::build_literal_string(Literal* literal) {
    literal->set_type(new IndirectionType(TYPE_POINTER, new Type(TYPE_CHAR)));
    //build_literal(literal, TYPE_STR);
}

void ScopeBuilder::build_string_builder(StringBuilder* sb) {
    std::stringstream ss;
    NamedType* type;
    Variable* var = sb->get_variable();

    ss << "sbuilder" << sbuilder_counter++;
    std::string tmp_name = ss.str();

    var->set_name(tmp_name);
    type = new NamedType();

    type->set_name("String");
    var->set_type(type);
    link_type(type);

    var->set_uid(var_counter++);
    var->set_kind(VAR_LOCAL);
    current_scope->define(sb->get_variable());
    current_function->add_variable(var);

    {
        Identifier* name = new Identifier(tmp_name);
        Identifier* method = new Identifier("init");
        BinOp* dot = new BinOp(EXPR_DOT, name, method);
        ExpressionList* args = new ExpressionList(EXPR_ARGS);
        BinOp* call = new BinOp(EXPR_CALL, dot, args);
        sb->add_call(call);
        build_expression(call);
    }

    for (int i = 0; i < sb->expressions_count(); ++i) {
        Identifier* name = new Identifier(tmp_name);
        Identifier* method = new Identifier("add");
        BinOp* dot = new BinOp(EXPR_DOT, name, method);
        ExpressionList* args = new ExpressionList(EXPR_ARGS);

        args->add_expression(sb->get_expression(i));
        BinOp* call = new BinOp(EXPR_CALL, dot, args);
        sb->add_call(call);
        build_expression(call);
    }

    {
        Identifier* name = new Identifier(tmp_name);
        Identifier* method = new Identifier("c_str");
        BinOp* dot = new BinOp(EXPR_DOT, name, method);
        ExpressionList* args = new ExpressionList(EXPR_ARGS);
        BinOp* call = new BinOp(EXPR_CALL, dot, args);
        sb->add_call(call);
        build_expression(call);
    }

    sb->set_type(new IndirectionType(TYPE_POINTER, new Type(TYPE_CHAR)));
}

void ScopeBuilder::build_sizeof(UnOp* un) {
    build_expression(un->get_expression());
    un->set_type(new Type(TYPE_U64));
}

void ScopeBuilder::build_expression_list(ExpressionList* exprlist) {
    if (exprlist == nullptr) return;

    TypeList* types = new TypeList(TYPE_TUPLE);

    if (exprlist->expressions_count() > 0) {
        for (int i = 0; i < exprlist->expressions_count(); ++i) {
            build_expression(exprlist->get_expression(i));
            types->add_type(exprlist->get_expression(i)->get_type());
        }
    } else {
        types->add_type(new Type(TYPE_VOID));
    }

    exprlist->set_type(types);
}

bool ScopeBuilder::is_new_var_assign(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = current_scope->has(id->get_lexeme());

    return sym == nullptr;
}

void ScopeBuilder::create_new_var(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();

    Variable* var = new Variable(id);
    var->set_type(bin->get_right()->get_type());
    var->set_uid(var_counter++);
    var->set_kind(VAR_LOCAL);
    bin->set_initial_assign(true);
    current_scope->define(var);
    current_function->add_variable(var);
}

bool ScopeBuilder::is_constructor_call(BinOp* bin) {
    BinOp* call;
    Identifier* id;
    Symbol* sym;
    Type* type;

    if (bin->get_right()->get_kind() == EXPR_CALL) {
        call = (BinOp*) bin->get_right();

        if (call->get_left()->get_kind() == EXPR_ID) {
            id = (Identifier*) call->get_left();
            type = id->get_type();
            sym = id->get_symbol();

            return type->get_kind() == TYPE_NAMED && sym->get_kind() == SYM_CLASS;
        } else if (call->get_left()->get_kind() == EXPR_SCOPE) {

        }
    }

    return false;
}

// define methods
void ScopeBuilder::define_sources(Sources* sources) {
    connect_sibling_scopes(sources);

    define_sources_classes(sources);
    define_sources_functions(sources);
}

void ScopeBuilder::define_sources_classes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_classes(sources->get_source(i));
    }
}

void ScopeBuilder::define_sources_functions(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_functions(sources->get_source(i));
    }
}

void ScopeBuilder::define_source_classes(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }

    leave_scope();
}

void ScopeBuilder::define_source_functions(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->function_count(); ++i) {
        define_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeBuilder::define_class(Class* klass) {
    Symbol* sym;
    NamedType* self_type = new NamedType();

    current_class = klass;
    sym = current_scope->local_has(klass->get_name());

    if (sym != nullptr) {
        logger->error_and_exit(error_message_cant_define_class(klass, sym));
    }

    current_scope->define(klass);
    klass->set_uid(class_counter++);
    logger->info(info_message_defining_class(klass));

    self_type->set_name(klass->get_name());
    link_type(self_type);
    klass->set_self_type(self_type);

    enter_scope(klass->get_scope());
    define_class_template_header(klass);
    define_class_super(klass);
    define_class_variables(klass);
    define_class_methods(klass);

    leave_scope();
    current_class = nullptr;
}

void ScopeBuilder::define_class_variables(Class* klass) {
    for (int i = 0; i < klass->variables_count(); ++i) {
        define_class_variable(klass->get_variable(i));
        klass->get_variable(i)->set_uid(i); 
    }
}

void ScopeBuilder::define_class_variable(Variable* var) {
    Symbol* sym;
    Symbol* lsym;

    sym = current_scope->has_field(var->get_name());
    lsym = current_scope->local_has(var->get_name());

    if (!sym) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else if (sym->get_kind() != SYM_CLASS_VARIABLE) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else {
        if (lsym == nullptr) {
            logger->error_and_exit("<red>error: </red>class variable already defined in super class");
        } else {
            logger->error_and_exit("<red>error: </red>class variable already defined");
        }
    }
}

void ScopeBuilder::add_default_constructor(Class* klass) {
    if (klass->constructors_count() == 0) {
        std::string c = "def init : void\n    pass\n";
        Parser parser;
        Function* m = parser.read_function_from_string(c);
        klass->add_method(m);
        define_class_method(m);
    }
}

void ScopeBuilder::add_default_destructor(Class* klass) {
    if (klass->get_destructor() == nullptr) {
        std::string c = "def destroy : void\n    pass\n";
        Parser parser;
        Function* m = parser.read_function_from_string(c);
        klass->add_method(m);
        define_class_method(m);
    }
}

void ScopeBuilder::define_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        define_class_method(klass->get_method(i));
    }

    add_default_constructor(klass);
    add_default_destructor(klass);
}

void ScopeBuilder::define_class_method(Function* method) {
    Symbol* sym = current_scope->local_has(method->get_name());
    Symbol* sym2 = current_scope->has_field(method->get_name());

    define_method_signature(method);
    method->set_uid(function_counter++);

    if (!sym) {
        logger->info(info_message_defining_method(method));
        current_scope->define(method);
    } else if (sym->get_kind() == SYM_METHOD) {
        logger->info(info_message_defining_method(method));
        define_overloaded_function(sym, method);
    } else {
        logger->error_and_exit("can't define method");
    }

    if (sym2) {
        if (sym2->get_kind() == SYM_METHOD) {
            Function* m = (Function*) sym2->get_descriptor();

            if (m->is_virtual()) {
                method->set_parent_method(m);
                method->set_virtual(true);
            }
        }
    }

    if (method->is_virtual()) {
        current_class->set_virtual(true);
    }
}

void ScopeBuilder::define_method_signature(Function* method) {
    define_function_signature(method);
}

void ScopeBuilder::define_class_template_header(Class* klass) {
    TypeList* header = klass->get_template_header();

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            TemplateType* t = (TemplateType*) header->get_type(i);

            if (current_scope->local_has(t->get_name())) {
                logger->error_and_exit("error: already used type on template header");
            } else {
                current_scope->define(t);
            }
        }
    }
}

void ScopeBuilder::define_class_super(Class* klass) {
    // FIXME handle super with templates
    if (klass->has_super_class()) {
        link_type(klass->get_super_class());
        NamedType* named = (NamedType*) klass->get_super_class();
        Symbol* sym = named->get_symbol();
        Class* super = (Class*) sym->get_descriptor();
        current_scope->set_super(super->get_scope());
    }
}

void ScopeBuilder::connect_sibling_scopes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        Source* src = sources->get_source(i);
        Scope* scope = src->get_scope();

        for (int j = 0; j < src->import_count(); ++j) {
            scope->add_sibling(src->get_import(j)->get_source()->get_scope());
        }
    }
}

void ScopeBuilder::generate_deletables() {
    std::vector<Variable*> vars = current_scope->get_variables_to_be_deleted();

    for (int i = vars.size() - 1; i >= 0; --i) {
        Identifier* name = new Identifier(vars[i]->get_name());
        Identifier* method = new Identifier("destroy");
        BinOp* dot = new BinOp(EXPR_DOT, name, method);
        ExpressionList* args = new ExpressionList(EXPR_ARGS);
        BinOp* call = new BinOp(EXPR_CALL, dot, args);
        build_expression(call);

        current_scope->add_deletable(call);
    }
}

void ScopeBuilder::add_parent_constructors_call(Function* function) {
    Class* klass = function->get_class();
    Class* super;
    CompoundStatement* stmts;

    if (klass->has_super_class()) {
        super = klass->get_super_class_descriptor();
        stmts = function->get_statements();
        std::string name = super->get_name();

        Parser p;
        std::string cmd = "(this as " + name + "*).init()";
        Expression* expr = p.read_expression_from_string(cmd);
        ExpressionStatement* es = new ExpressionStatement(expr);
        build_statement(es);
        stmts->add_front(es);
    }
}

void ScopeBuilder::add_parent_destructors_call(Function* function) {
    Class* klass = function->get_class();
    Class* super;
    CompoundStatement* stmts;

    if (klass->has_super_class()) {
        super = klass->get_super_class_descriptor();
        stmts = function->get_statements();
        std::string name = super->get_name();

        Parser p;
        std::string cmd = "(this as " + name + "*).destroy()";
        Expression* expr = p.read_expression_from_string(cmd);
        ExpressionStatement* es = new ExpressionStatement(expr);
        build_statement(es);
        stmts->add_statement(es);
    }
}

void ScopeBuilder::add_members_initialization(Function* function) {
    Class* klass = function->get_class();
    CompoundStatement* stmts;
    Variable* var;
    Type* type;

    stmts = function->get_statements();

    for (int i = 0; i < klass->variables_count(); ++i) {
        var = klass->get_variable(i);
        type = var->get_type();

        if (type->is_class()) {
            std::string name = var->get_name();

            Parser p;
            std::string cmd = name + ".init()";
            Expression* expr = p.read_expression_from_string(cmd);
            ExpressionStatement* es = new ExpressionStatement(expr);
            build_statement(es);
            stmts->add_front(es);
        }
    }
}

void ScopeBuilder::add_members_destruction(Function* function) {
    Class* klass = function->get_class();
    CompoundStatement* stmts;
    Variable* var;
    Type* type;

    stmts = function->get_statements();

    for (int i = 0; i < klass->variables_count(); ++i) {
        var = klass->get_variable(i);
        type = var->get_type();

        if (type->is_class()) {
            std::string name = var->get_name();

            Parser p;
            std::string cmd = name + ".destroy()";
            Expression* expr = p.read_expression_from_string(cmd);
            ExpressionStatement* es = new ExpressionStatement(expr);
            build_statement(es);
            stmts->add_statement(es);
        }
    }
}

void ScopeBuilder::define_function(Function* function) {
    Symbol* sym = current_scope->local_has(function->get_name());

    define_function_signature(function);
    function->set_uid(function_counter++);

    if (!sym) {
        logger->info(info_message_defining_function(function));
        current_scope->define(function);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        logger->info(info_message_defining_function(function));
        define_overloaded_function(sym, function);
    } else {
        logger->error_and_exit("can't define function");
    }
}

void ScopeBuilder::define_function_signature(Function* function) {
    enter_scope(function->get_scope());

    define_function_template_header(function);
    define_function_parameters(function);
    link_type(function->get_return_type());
    define_function_self_type(function);

    leave_scope();
}

void ScopeBuilder::define_function_template_header(Function* function) {
    Symbol* sym;
    TypeList* header = function->get_template_header();

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            TemplateType* type = (TemplateType*) header->get_type(i);
            sym = current_scope->local_has(type->get_name());

            if (!sym) {
                current_scope->define(type);
                link_type(type->get_bind_type());
            } else {
                logger->error_and_exit("<red>error: </red>template name already defined");
            }
        }
    }
}

void ScopeBuilder::define_function_parameters(Function* function) {
    Variable* param;
    Symbol* sym;

    for (int i = 0; i < function->parameters_count(); ++i) {
        param = function->get_parameter(i);
        param->set_uid(i);
        link_type(param->get_type());

        sym = current_scope->has(param->get_name());

        if (!sym) {
            current_scope->define(SYM_PARAMETER, param);
        } else if (sym->get_kind() != SYM_PARAMETER) {
            current_scope->define(SYM_PARAMETER, param);
        } else {
            logger->error_and_exit("<red>error: </red>parameter already defined");
        }
    }
}

void ScopeBuilder::define_function_self_type(Function* function) {
    FunctionType* ftype = new FunctionType();

    if (function->get_template_header()) {
        TypeList* header = function->get_template_header();

        for (int i = 0; i < header->types_count(); ++i) {
            ftype->add_template(header->get_type(i));
        }
    }

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            ftype->add_param_type(function->get_parameter(i)->get_type());
        }
    } else {
        ftype->add_param_type(new Type(TYPE_VOID));
    }

    ftype->set_return_type(function->get_return_type());
    function->set_self_type(ftype);
    link_type(ftype);
}

void ScopeBuilder::define_overloaded_function(Symbol* symbol, Function* function) {
    for (int i = 0; i < symbol->overloaded_count(); ++i) {
        Function* other = (Function*) symbol->get_descriptor(i);

        if (function->same_signature(other)) {
            std::string msg("<red>error: </red>function with same signature: ");
            msg += function->get_name();
            logger->error_and_exit(msg);
        }
    }

    symbol->add_descriptor(function);
    function->set_overloaded_index(symbol->overloaded_count() - 1);
}

void ScopeBuilder::link_type(Type* type) {
    IndirectionType* it = (IndirectionType*) type;

    if (type == nullptr) {
        return;
    }

    switch (type->get_kind()) {
    case TYPE_NAMED:
        link_named_type((NamedType*) type);
        break;

    case TYPE_POINTER:
    case TYPE_REFERENCE:
        link_type(it->get_subtype());
        break;

    case TYPE_FUNCTION:
        link_function_type((FunctionType*) type);
        break;

    case TYPE_ARRAY:
        link_array_list_type((ArrayListType*) type);
        break;
    }
}

void ScopeBuilder::link_named_type(NamedType* type) {
    TypeList* header = type->get_template_header();
    Symbol* sym = current_scope->has(type->get_name());
    std::string name = type->get_name();

    if (!sym) {
        logger->error_and_exit("<red>error: </red> type <white>'" + name + "'</white> not in scope");
    }

    int kind = sym->get_kind();

    if (kind == SYM_CLASS) {
        type->set_symbol(sym);
    } else if (kind == SYM_TEMPLATE) {
        type->set_symbol(sym);
    } else {
        logger->error_and_exit("error: named type not in scope but is another entity");
    }

    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            link_type(header->get_type(i));
        }
    } 
}

void ScopeBuilder::link_function_type(FunctionType* type) {
    link_template_header(type->get_template_header());

    for (int i = 0; i < type->params_count(); ++i) {
        link_type(type->get_param_type(i));
    }

    link_type(type->get_return_type());
}

void ScopeBuilder::link_array_list_type(ArrayListType* type) {
    link_type(type->get_subtype());
    build_expression(type->get_expression());
}

void ScopeBuilder::link_template_header(TypeList* header) {
    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            link_type(header->get_type(i));
        }
    }
}

void ScopeBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

// PRIVATE
void ScopeBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void ScopeBuilder::leave_scope(bool has_deletables) {
    if (has_deletables) {
        generate_deletables();
    }

    current_scope = scopes.top();
    scopes.pop();
}
