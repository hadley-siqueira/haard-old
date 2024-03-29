#include <iostream>
#include <cstring>
#include <set>
#include <regex>
#include "scope/old_scope_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"
#include "parser/parser.h"
#include "printer/printer.h"

using namespace haard;

OldScopeBuilder::OldScopeBuilder() {
    logger = nullptr;
    current_scope = nullptr;
    current_class = nullptr;
    current_function = nullptr;
    current_module = nullptr;
    var_counter = 0;
    function_counter = 0;
    class_counter = 0;
    sbuilder_counter = 0;
}

void OldScopeBuilder::build(Modules* modules) {
    define_modules(modules);
    build_modules(modules);
}

void OldScopeBuilder::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void OldScopeBuilder::build_module(Module* module) {
    enter_scope(module->get_scope());
    current_module = module;

    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }

    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }

    leave_scope();
    current_module = nullptr;
}

void OldScopeBuilder::build_class(Class* klass) {
    if (klass->is_template()) {
        return;
    }

    current_class = klass;

    enter_scope(klass->get_scope());
    build_class_methods(klass);
    klass->calculate_variables_offset();
    leave_scope();

    current_class = nullptr;
}

void OldScopeBuilder::build_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        build_function(klass->get_method(i));
    }
}

void OldScopeBuilder::build_function(Function* function) {
    if (function->is_template()) {
        return;
    }

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
    current_function = nullptr;
}

void OldScopeBuilder::build_statement(Statement* statement) {
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

void OldScopeBuilder::build_compound_statement(CompoundStatement* stmts) {
    enter_scope(stmts->get_scope());

    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }

    leave_scope(true);
}

void OldScopeBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void OldScopeBuilder::build_jump_statement(JumpStatement* statement) {
    build_expression(statement->get_expression());
}

void OldScopeBuilder::build_while_statement(WhileStatement* statement) {
    enter_scope(statement->get_scope());

    build_expression(statement->get_condition());
    build_compound_statement(statement->get_statements());

    leave_scope(true);
}

void OldScopeBuilder::build_for_statement(ForStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_kind() == STMT_FOR) {
        build_expression(statement->get_initialization());
        build_expression(statement->get_condition());
        build_expression(statement->get_increment());
    }

    build_compound_statement(statement->get_statements());
    leave_scope(true);
}

void OldScopeBuilder::build_foreach_statement(ForStatement* statement) {
    BinOp* expr;
    enter_scope(statement->get_scope());

    expr = (BinOp*) statement->get_condition();
    build_expression(expr->get_right());

    if (is_new_var_assign(expr)) {
        create_new_var(expr);
    }

    build_expression(expr->get_left());
    build_compound_statement(statement->get_statements());
    leave_scope(true);
}

void OldScopeBuilder::build_branch_statement(BranchStatement* statement) {
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

void OldScopeBuilder::build_variable_declaration(VarDeclaration *statement) {
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

void OldScopeBuilder::build_expression(Expression* expression) {
    if (expression == nullptr) {
        return;
    }

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        build_identifier((Identifier*) expression);
        break;

    case EXPR_THIS:
        build_this((This*) expression);
        break;

    case EXPR_NEW:
        build_new((New*) expression);
        break;

    case EXPR_DELETE:
    case EXPR_DELETE_ARRAY:
        build_delete(un);
        break;

    case EXPR_PRE_INCREMENT:
        build_pre_inc(un);
        break;

    case EXPR_PRE_DECREMENT:
        build_pre_dec(un);
        break;

    case EXPR_POS_INCREMENT:
        build_pos_inc(un);
        break;

    case EXPR_POS_DECREMENT:
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

    case EXPR_ASSIGNMENT:
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

    case EXPR_EQUAL:
        build_equal(bin);
        break;

    case EXPR_NOT_EQUAL:
        build_not_equal(bin);
        break;

    case EXPR_GREATER_THAN:
        build_greater_than(bin);
        break;

    case EXPR_LESS_THAN:
        build_less_than(bin);
        break;

    case EXPR_GREATER_THAN_OR_EQUAL:
        build_greater_or_equal_than(bin);
        break;

    case EXPR_LESS_THAN_OR_EQUAL:
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

    case EXPR_SHIFT_LEFT_LOGICAL:
        build_sll(bin);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL:
        build_sll(bin);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC:
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
        build_cast_expression((Cast*) expression);
        break;

    case EXPR_SIZEOF:
        build_sizeof(un);
        break;
    }
}

void OldScopeBuilder::build_identifier(Identifier* id) {
    Symbol* sym = nullptr;

    if (id->has_scope()) {
        Import* import = current_module->get_import_with_alias(id->get_alias());

        if (import == nullptr) {
            logger->error_and_exit("There is no import with alias");
        }

        Scope* scope = import->get_module()->get_scope();
        sym = scope->local_has(id->get_name());
    } else {
        sym = current_scope->has(id->get_name());
    }

    if (!sym) {
        logger->error_and_exit(error_message_id_not_in_scope(current_module, id));
    }

    id->set_symbol(sym);

    if (id->has_template()) {
        link_template_header(id->get_template_header());

        if (!sym->has_template(id->get_template_header())) {
            generate_templates(id);
            sym->add_template(id->get_template_header()->get_types());
        }
    }
}

void OldScopeBuilder::build_this(This* expr) {
    if (current_class == nullptr) {
        logger->error_and_exit("<red>error: </red>: using this outside class");
    }

    expr->set_type(new IndirectionType(TYPE_POINTER, current_class->get_self_type()));
}

void OldScopeBuilder::build_new(New* op) {
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

void OldScopeBuilder::build_delete(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_assignment(BinOp* bin) {
    bool new_var = false;
    build_expression(bin->get_right());

    if (is_new_var_assign(bin)) {
        new_var = true;
        create_new_var(bin);
    }
 
    build_expression(bin->get_left());

    if (new_var && is_constructor_call_assignment(bin)) {
        bin->set_kind(EXPR_CONSTRUCTOR_ASSIGNMENT);
    }
}

void OldScopeBuilder::build_pre_inc(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_pre_dec(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_pos_inc(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_pos_dec(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_parenthesis(UnOp* op) {
    build_expression(op->get_expression());
    op->set_type(op->get_expression()->get_type());
}

void OldScopeBuilder::build_logical_or(BinOp* op) {
    build_relational(op);
}

void OldScopeBuilder::build_logical_and(BinOp* op) {
    build_relational(op);
}

void OldScopeBuilder::build_call(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());

    if (is_function_call(bin)) {
        build_function_call(bin);
    } else if (is_constructor_call(bin)) {
        build_constructor_call(bin);
    } else if (is_member_call(bin)) {
        build_member_call(bin);
    } else {
        logger->error_and_exit("<red>error:</red> can't make a call");
    }
}

void OldScopeBuilder::build_function_call(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = id->get_symbol();

    int index = sym->get_overloaded((TypeList*) bin->get_right()->get_type());

    if (index >= 0) {
        id->set_overloaded_index(index);
    } else {
        std::cout << "Error: function not overloaded with signature\n";
        DBG;
        exit(0);
    }

    // Function* f = (Function*) id->get_symbol()->get_descriptor(id->get_overloaded_index());
    Function* f = (Function*) id->get_descriptor();
    FunctionType* ftype = (FunctionType*) f->get_self_type();
    bin->set_type(ftype->get_return_type());
}

void OldScopeBuilder::build_constructor_call(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = id->get_symbol();
    Class* klass = (Class*) sym->get_descriptor();
    TypeList* args = (TypeList*) bin->get_right()->get_type();

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
}

void OldScopeBuilder::build_member_call(BinOp* bin) {
    BinOp* dot = (BinOp*) bin->get_left();
    Identifier* id = (Identifier*) dot->get_right();
    Symbol* sym = id->get_symbol();

    Type* tl = bin->get_left()->get_type();
    Type* tr = bin->get_right()->get_type();

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
        FunctionType* ftype = (FunctionType*) f->get_self_type();
        bin->set_type(ftype->get_return_type());
    }
}

void OldScopeBuilder::build_dot(BinOp* bin) {
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
    symbol = scope->has_field(field->get_name());

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
        scope->debug();
        std::cout << field->get_name() << ' ' << field->get_line() << std::endl;
        DBG;
        exit(0);
    }
}

void OldScopeBuilder::build_index_access(BinOp* bin) {
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

void OldScopeBuilder::build_expression_in(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());
    bin->set_type(new Type(TYPE_BOOL));
}

void OldScopeBuilder::build_inclusive_range(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());
    //FIXME should set a range type?
    bin->set_type(bin->get_left()->get_type());
}

void OldScopeBuilder::build_cast_expression(Cast* expr) {
    build_expression(expr->get_expression());
    link_type(expr->get_cast_type());
    expr->set_type(expr->get_cast_type());
}

void OldScopeBuilder::build_plus(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_minus(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_bitwise_or(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_bitwise_xor(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_bitwise_and(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_sll(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_srl(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_sra(BinOp* bin) {
    build_binop(bin);
}

void OldScopeBuilder::build_greater_than(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_less_than(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_greater_or_equal_than(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_less_or_equal_than(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_equal(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_not_equal(BinOp* bin) {
    build_relational(bin);
}

void OldScopeBuilder::build_address_of(UnOp* op) {
    Type* type;

    build_expression(op->get_expression());

    type = op->get_expression()->get_type();
    type = new IndirectionType(TYPE_POINTER, type);

    op->set_type(type);
}

void OldScopeBuilder::build_dereference(UnOp* op) {
    IndirectionType* ptype;

    build_expression(op->get_expression());
    ptype = (IndirectionType*) op->get_expression()->get_type();
    op->set_type(ptype->get_subtype());
}

void OldScopeBuilder::build_binop(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());

    Type* tleft = bin->get_left()->get_type();
    Type* tright = bin->get_right()->get_type();

    tleft = tleft->promote(tright);

    // FIXME
    bin->set_type(tleft);
}

void OldScopeBuilder::build_relational(BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());

    // FIXME
    bin->set_type(new Type(TYPE_BOOL));
}

void OldScopeBuilder::build_literal(Literal* literal, int kind) {
    literal->set_type(new Type(kind));
}

void OldScopeBuilder::build_literal_string(Literal* literal) {
    literal->set_type(new IndirectionType(TYPE_POINTER, new Type(TYPE_CHAR)));
    //build_literal(literal, TYPE_STR);
}

void OldScopeBuilder::build_string_builder(StringBuilder* sb) {
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

void OldScopeBuilder::build_sizeof(UnOp* un) {
    build_expression(un->get_expression());
    un->set_type(new Type(TYPE_U64));
}

void OldScopeBuilder::build_expression_list(ExpressionList* exprlist) {
    if (exprlist == nullptr) return;

    TupleType* types = new TupleType();

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

bool OldScopeBuilder::is_new_var_assign(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = current_scope->has(id->get_name());

    return sym == nullptr;
}

void OldScopeBuilder::create_new_var(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();

    Variable* var = new Variable(id);
    var->set_type(bin->get_right()->get_type());
    var->set_uid(var_counter++);
    var->set_kind(VAR_LOCAL);
    bin->set_initial_assign(true);
    current_scope->define(var);
    current_function->add_variable(var);
}

bool OldScopeBuilder::is_constructor_call_assignment(BinOp* bin) {
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
        }
    }

    return false;
}

bool OldScopeBuilder::is_function_call(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    return bin->get_left()->get_type()->get_kind() == TYPE_FUNCTION;
}

bool OldScopeBuilder::is_constructor_call(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) bin->get_left();

    return id->get_type()->get_kind() == TYPE_NAMED &&
            id->get_symbol()->get_kind() == SYM_CLASS;
}

bool OldScopeBuilder::is_member_call(BinOp* bin) {
    return bin->get_left()->get_kind() == EXPR_DOT;
}

void OldScopeBuilder::generate_templates(Identifier* id) {
    Symbol* sym = id->get_symbol();
    std::vector<void*> descs = sym->get_descriptors(id->get_template_header()->get_types());

    if (sym->get_kind() == SYM_FUNCTION) {
        for (int i = 0; i < descs.size(); ++i) {
            Function* f = (Function*) descs[i];

            Function* res = generate_function_template(f, id->get_template_header()->get_types());

            Module* module = f->get_module();

            auto old_module = current_module;
            auto old_function = current_function;
            auto old_class = current_class;
            auto old_scope = current_scope;

            current_module = f->get_module();
            current_function = res;
            current_scope = module->get_scope();

            module->add_function(res);
            define_function(res);
            build_function(res);

            current_module = old_module;
            current_function = old_function;
            current_class = old_class;
            current_scope = old_scope;
        }
    } else if (sym->get_kind() == SYM_CLASS) {
        for (int i = 0; i < descs.size(); ++i) {
            Class* f = (Class*) descs[i];

            Class* res = generate_class_template(f, id->get_template_header()->get_types());

            Module* module = f->get_module();

            auto old_module = current_module;
            auto old_function = current_function;
            auto old_class = current_class;
            auto old_scope = current_scope;

            current_module = f->get_module();
            current_class = res;
            current_scope = module->get_scope();

            module->add_class(res);
            define_class(res);
            build_class(res);

            current_module = old_module;
            current_function = old_function;
            current_class = old_class;
            current_scope = old_scope;
        }
    }
}

Function* OldScopeBuilder::generate_function_template(Function* function, TypeList* types) {
    Function* output = nullptr;
    std::string body = function->get_original();
    TypeList* templates = function->get_template_header()->get_types();

    for (int i = 0; i < templates->types_count(); ++i) {
        std::string from = templates->get_type(i)->to_str();
        std::string to = types->get_type(i)->to_str();
        std::regex pattern("\\b" + from + "\\b");
        body = std::regex_replace(body, pattern, to);
    }

    Parser p(logger);
    p.set_path(function->get_path());
    output = p.read_function_from_string(body);
    output->set_template(false);

    return output;
}

Class* OldScopeBuilder::generate_class_template(Class* klass, TypeList* types) {
    Class* output = nullptr;
    std::string body = klass->get_original();
    TypeList* templates = klass->get_template_header()->get_types();

    for (int i = 0; i < templates->types_count(); ++i) {
        std::string from = templates->get_type(i)->to_str();
        std::string to = types->get_type(i)->to_str();
        std::regex pattern("\\b" + from + "\\b");
        body = std::regex_replace(body, pattern, to);
    }

    std::cout << body << '\n';
    Parser p(logger);
    p.set_path(klass->get_path());
    output = p.read_class_from_string(body);
    output->set_template(false);

    return output;
}

// define methods
void OldScopeBuilder::define_modules(Modules* modules) {
    connect_sibling_scopes(modules);

    define_modules_classes(modules);
    define_modules_functions(modules);
}

void OldScopeBuilder::define_modules_classes(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        define_module_classes(modules->get_module(i));
    }
}

void OldScopeBuilder::define_modules_functions(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        define_module_functions(modules->get_module(i));
    }
}

void OldScopeBuilder::define_module_classes(Module* module) {
    enter_scope(module->get_scope());

    for (int i = 0; i < module->classes_count(); ++i) {
        define_class(module->get_class(i));
    }

    leave_scope();
}

void OldScopeBuilder::define_module_functions(Module* module) {
    enter_scope(module->get_scope());

    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }

    leave_scope();
}

void OldScopeBuilder::define_class(Class* klass) {
    Symbol* sym;
    NamedType* self_type = new NamedType();
    Class* other;

    current_class = klass;
    sym = current_scope->local_has(klass->get_name());

    if (sym != nullptr) {
        other = (Class*) sym->get_descriptor();

        if (!other->is_template()) {
            logger->error_and_exit(error_message_cant_define_class(klass, sym));
        } else {
            sym->add_descriptor(klass);
        }
    }

    current_scope->define(klass);
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

void OldScopeBuilder::define_class_variables(Class* klass) {
    for (int i = 0; i < klass->variables_count(); ++i) {
        define_class_variable(klass->get_variable(i));
        klass->get_variable(i)->set_uid(i);
    }
}

void OldScopeBuilder::define_class_variable(Variable* var) {
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

void OldScopeBuilder::add_default_constructor(Class* klass) {
    if (klass->constructors_count() == 0) {
        std::string c = "def init : void\n    pass\n";
        Parser parser;
        Function* m = parser.read_function_from_string(c);
        klass->add_method(m);
        define_class_method(m);
    }
}

void OldScopeBuilder::add_default_destructor(Class* klass) {
    if (klass->get_destructor() == nullptr) {
        std::string c = "def destroy : void\n    pass\n";
        Parser parser;
        Function* m = parser.read_function_from_string(c);
        klass->add_method(m);
        define_class_method(m);
    }
}

void OldScopeBuilder::define_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        define_class_method(klass->get_method(i));
    }

    add_default_constructor(klass);
    add_default_destructor(klass);
}

void OldScopeBuilder::define_class_method(Function* method) {
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

void OldScopeBuilder::define_method_signature(Function* method) {
    define_function_signature(method);
}

void OldScopeBuilder::define_class_template_header(Class* klass) {
    if (klass->is_template()) {
        define_template_header(klass->get_template_header()->get_types());
    }
}

void OldScopeBuilder::define_class_super(Class* klass) {
    // FIXME handle super with templates
    if (klass->get_super_type()) {
        link_type(klass->get_super_type());
        NamedType* named = (NamedType*) klass->get_super_type();
        Symbol* sym = named->get_symbol();
        Class* super = (Class*) sym->get_descriptor();
        current_scope->set_super(super->get_scope());
    }
}

void OldScopeBuilder::connect_sibling_scopes(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        Module* src = modules->get_module(i);
        Scope* scope = src->get_scope();

        for (int j = 0; j < src->import_count(); ++j) {
            scope->add_sibling(src->get_import(j)->get_module()->get_scope());
        }
    }
}

void OldScopeBuilder::generate_deletables() {
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

void OldScopeBuilder::add_parent_constructors_call(Function* function) {
    Class* klass = (Class*) function->get_compound();
    Class* super;
    CompoundStatement* stmts;

    if (klass->get_super_type()) {
        super = (Class*) klass->get_super_descriptor();
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

void OldScopeBuilder::add_parent_destructors_call(Function* function) {
    Class* klass = (Class*) function->get_compound();
    Class* super;
    CompoundStatement* stmts;

    if (klass->get_super_type()) {
        super = (Class*) klass->get_super_descriptor();
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

void OldScopeBuilder::add_members_initialization(Function* function) {
    Class* klass = (Class*) function->get_compound();
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

void OldScopeBuilder::add_members_destruction(Function* function) {
    Class* klass = (Class*) function->get_compound();
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

void OldScopeBuilder::define_function(Function* function) {
    Symbol* sym = current_scope->local_has(function->get_name());

    define_function_signature(function);
    function->set_uid(function_counter++);

    if (!sym) {
        logger->info(info_message_define_function(function));
        current_scope->define(function);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        logger->info(info_message_define_function(function));
        define_overloaded_function(sym, function);
    } else {
        logger->error_and_exit("can't define function");
    }
}

void OldScopeBuilder::define_function_signature(Function* function) {
    enter_scope(function->get_scope());

    define_function_template_header(function);
    define_function_parameters(function);
    link_type(function->get_return_type());
    define_function_self_type(function);

    leave_scope();
}

void OldScopeBuilder::define_function_template_header(Function* function) {
    if (function->is_template()) {
        define_template_header(function->get_template_header()->get_types());
    }
}

void OldScopeBuilder::define_function_parameters(Function* function) {
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
            std::string name = param->get_name();
            std::string msg = "<red>error: </red>parameter '" + name + "' already defined";
            logger->error_and_exit(msg);
        }
    }
}

void OldScopeBuilder::define_function_self_type(Function* function) {
    FunctionType* ftype = new FunctionType();

    if (function->get_template_header()) {
        TypeList* header = function->get_template_header()->get_types();

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

void OldScopeBuilder::define_overloaded_function(Symbol* symbol, Function* function) {
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

void OldScopeBuilder::define_template_header(TypeList* types) {
    if (types) {
        for (int i = 0; i < types->types_count(); ++i) {
            NamedType* t = (NamedType*) types->get_type(i);

            if (current_scope->local_has(t->get_name())) {
                logger->error_and_exit("error: already used type on template header");
            } else {
                current_scope->define_template(t->get_name());
            }
        }
    }
}

void OldScopeBuilder::link_type(Type* type) {
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

void OldScopeBuilder::link_named_type(NamedType* type) {
    Symbol* sym = current_scope->has(type->get_name());
    std::string name = type->get_name();

    if (!sym) {
        logger->error_and_exit("<red>error: </red> type <white>'" + name + "'</white> not in scope");
    }

    int kind = sym->get_kind();

    if (kind == SYM_CLASS) {
        type->set_symbol(sym);
    } else if (kind == SYM_TEMPLATE) {

    } else {
        logger->error_and_exit("error: named type not in scope but is another entity");
    }
}

void OldScopeBuilder::link_function_type(FunctionType* type) {
    for (int i = 0; i < type->params_count(); ++i) {
        link_type(type->get_param_type(i));
    }

    link_type(type->get_return_type());
}

void OldScopeBuilder::link_array_list_type(ArrayListType* type) {
    link_type(type->get_subtype());
    build_expression(type->get_expression());
}

void OldScopeBuilder::link_template_header(TemplateHeader* header) {
    if (header) {
        for (int i = 0; i < header->types_count(); ++i) {
            link_type(header->get_type(i));
        }
    }
}

void OldScopeBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

void OldScopeBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void OldScopeBuilder::leave_scope(bool has_deletables) {
    if (has_deletables) {
        generate_deletables();
    }

    current_scope = scopes.top();
    scopes.pop();
}
