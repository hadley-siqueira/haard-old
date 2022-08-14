#include <iostream>
#include <cstring>
#include "printer/printer.h"
#include "scope/scope_builder.h"

using namespace haard;

ScopeBuilder::ScopeBuilder() {
    current_scope = nullptr;
    class_counter = 0;
    function_counter = 0;
}

void ScopeBuilder::build_sources(Sources* sources) {
    define_sources_elements(sources);

    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void ScopeBuilder::build_source(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        build_class(source->get_class(i));
    }

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeBuilder::build_class(Class* klass) {
    enter_scope(klass->get_scope());
    current_class = klass;

    build_class_variables(klass);
    build_class_methods(klass);

    leave_scope();
}

void ScopeBuilder::build_class_variables(Class* klass) {
    for (int i = 0; i < klass->variables_count(); ++i) {
        build_class_variable(klass->get_variable(i));
        klass->get_variable(i)->set_uid(i); 
    }
}

void ScopeBuilder::build_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        build_function(klass->get_method(i));
    }
}

void ScopeBuilder::build_function(Function* func) {
    if (func->is_template()) {
        if (strcmp(func->get_name(), "soma") == 0) {
            std::cout << "LETS GO\n";
            TypeList* b = new TypeList();
            b->add_type(new Type(TYPE_INT));
            Function* k = func->get_with_template_binding(b);
            Printer p;
            p.print_function(k);
            std::cout << p.to_str() << "\n\n";
        }

        return;
    } 

    var_counter = 0;

    enter_scope(func->get_scope());
    current_function = func;

    build_compound_statement(func->get_statements());
    leave_scope();
}

void ScopeBuilder::build_class_variable(Variable* var) {
    Symbol* sym;

    sym = current_scope->has_field(var->get_name());

    if (!sym) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        build_type(var->get_type());
    } else if (sym->get_kind() != SYM_CLASS_VARIABLE) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        build_type(var->get_type());
    } else {
        std::cout << "class variable already defined\n";
    }
}

void ScopeBuilder::build_type(Type* type) {
    if (type == nullptr) return;

    switch (type->get_kind()) {
    case TYPE_NAMED:
        build_named_type((NamedType*) type);
        break;

    case TYPE_POINTER:
    case TYPE_REFERENCE:
        build_indirection_type((IndirectionType*) type);
        break;

    case TYPE_ARRAY:
    case TYPE_LIST:
        build_array_list_type((ArrayListType*) type);
        break;

    case TYPE_HASH:
    case TYPE_FUNCTION:
    case TYPE_TUPLE:
    case TYPE_PARENTHESIS:
    case TYPE_UNION:
        break;
    }
}

void ScopeBuilder::build_named_type(NamedType* type) {
    Symbol* sym = current_scope->has(type->get_name()->get_lexeme());

    if (!sym) {
        std::cout << "Error: named type not in scope\n";
        exit(0);
    }

    int kind = sym->get_kind();

    if (kind == SYM_CLASS) {
        type->set_symbol(sym);
    } else {
        std::cout << "Error: named type not in scope but is another entity\n";
        exit(0);
    }
}

void ScopeBuilder::build_indirection_type(IndirectionType* type) {
    build_type(type->get_subtype());
}

void ScopeBuilder::build_array_list_type(ArrayListType* type) {
    build_type(type->get_subtype());
    build_expression(type->get_expression());
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
    case STMT_FOREACH:
        build_for_statement((ForStatement*) statement);
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
   //     build_variable_declaration((VarDeclaration*) statement);
        break;
    }
}

void ScopeBuilder::build_compound_statement(CompoundStatement* stmts) {
    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }
}

void ScopeBuilder::build_while_statement(WhileStatement* statement) {
    enter_scope(statement->get_scope());

    build_expression(statement->get_condition());
    build_compound_statement(statement->get_statements());

    leave_scope();
}

void ScopeBuilder::build_for_statement(ForStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_kind() == STMT_FOR) {
        build_expression(statement->get_initialization());
        build_expression(statement->get_condition());
        build_expression(statement->get_increment());
    }

    build_compound_statement(statement->get_statements());
    leave_scope();
}

void ScopeBuilder::build_branch_statement(BranchStatement* statement) {
    enter_scope(statement->get_scope());

    if (statement->get_condition()) {
        build_expression(statement->get_condition());
    }

    if (statement->get_true_statements()) {
        build_statement(statement->get_true_statements());
    }

    leave_scope();

    if (statement->get_false_statements()) {
        build_statement(statement->get_false_statements());
    }
}

void ScopeBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeBuilder::build_jump_statement(JumpStatement* statement) {
    build_expression(statement->get_expression());
}

void ScopeBuilder::build_expression(Expression* expression) {
    if (expression == nullptr) return;

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        build_identifier((Identifier*) expression);
        break;

    case EXPR_SCOPE:
        build_expression(bin->get_left());
        build_expression(bin->get_right());
        break;

    case EXPR_GLOBAL_SCOPE:
        build_expression(un->get_expression());
        break;

    case EXPR_ASSIGN:
        build_assignment(bin);
        break;

    case EXPR_SPECIAL_ASSIGN:
        build_binop(":=", bin);
        break;

    case EXPR_SRL_ASSIGN:
        build_binop(">>>=", bin);
        break;

    case EXPR_SRA_ASSIGN:
        build_binop(">>=", bin);
        break;

    case EXPR_SLL_ASSIGN:
        build_binop("<<=", bin);
        break;

    case EXPR_TIMES_ASSIGN:
        build_binop("*=", bin);
        break;

    case EXPR_PLUS_ASSIGN:
        build_binop("+=", bin);
        break;

    case EXPR_MODULO_ASSIGN:
        build_binop("%=", bin);
        break;

    case EXPR_MINUS_ASSIGN:
        build_binop("-=", bin);
        break;

    case EXPR_INTEGER_DIVISIO_ASSIGN:
        build_binop("//=", bin);
        break;

    case EXPR_DIVISION_ASSIGN:
        build_binop("/=", bin);
        break;

    case EXPR_BITWISE_NOT_ASSIGN:
        build_binop("~=", bin);
        break;

    case EXPR_BITWISE_OR_ASSIGN:
        build_binop("|=", bin);
        break;

    case EXPR_BITWISE_XOR_ASSIGN:
        build_binop("^=", bin);
        break;

    case EXPR_BITWISE_AND_ASSIGN:
        build_binop("&=", bin);
        break;

    case EXPR_PLUS:
        build_binop("+", bin);
        break;

    case EXPR_MINUS:
        build_binop("-", bin);
        break;

    case EXPR_TIMES:
        build_binop("*", bin);
        break;

    case EXPR_DIVISION:
        build_binop("/", bin);
        break;

    case EXPR_INTEGER_DIVISION:
        build_binop("//", bin);
        break;

    case EXPR_MODULO:
        build_binop("%", bin);
        break;

    case EXPR_POWER:
        build_binop("**", bin);
        break;

    case EXPR_BITWISE_OR:
        build_binop("|", bin);
        break;

    case EXPR_BITWISE_XOR:
        build_binop("^", bin);
        break;

    case EXPR_BITWISE_AND:
        build_binop("&", bin);
        break;

    case EXPR_SLL:
        build_binop("<<", bin);
        break;

    case EXPR_SRL:
        build_binop(">>>", bin);
        break;

    case EXPR_SRA:
        build_binop(">>", bin);
        break;

    case EXPR_INCLUSIVE_RANGE:
        build_binop("..", bin);
        break;

    case EXPR_EXCLUSIVE_RANGE:
        build_binop("...", bin);
        break;

    case EXPR_LOGICAL_OR:
        build_binop("or", bin);
        break;

    case EXPR_LOGICAL_OR_OPER:
        build_binop("||", bin);
        break;

    case EXPR_LOGICAL_AND:
        build_binop("and", bin);
        break;

    case EXPR_LOGICAL_AND_OPER:
        build_binop("&&", bin);
        break;

    case EXPR_EQ:
        build_binop("==", bin);
        break;

    case EXPR_NE:
        build_binop("!=", bin);
        break;

    case EXPR_LT:
        build_binop("<", bin);
        break;

    case EXPR_GT:
        build_binop(">", bin);
        break;

    case EXPR_LE:
        build_binop("<=", bin);
        break;

    case EXPR_GE:
        build_binop(">=", bin);
        break;

    case EXPR_IN:
        build_binop("in", bin);
        break;

    case EXPR_NOT_IN:
        build_binop("not in", bin);
        break;

    case EXPR_LOGICAL_NOT_OPER:
        build_unop("!", un);
        break;

    case EXPR_LOGICAL_NOT:
        build_unop("not ", un);
        break;

    case EXPR_ADDRESS_OF:
        build_address_of(un);
        break;

    case EXPR_DEREFERENCE:
        build_unop("*", un);
        break;

    case EXPR_BITWISE_NOT:
        build_unop("~", un);
        break;

    case EXPR_UNARY_MINUS:
        build_unop("-", un);
        break;

    case EXPR_UNARY_PLUS:
        build_unop("+", un);
        break;

    case EXPR_PRE_INC:
        build_unop("++", un);
        break;

    case EXPR_PRE_DEC:
        build_unop("--", un);
        break;

    case EXPR_POS_INC:
        build_unop("++", un, false);
        break;

    case EXPR_POS_DEC:
        build_unop("--", un, false);
        break;

    case EXPR_SIZEOF:
        build_unop("!", un);
        break;

    case EXPR_PARENTHESIS:
        build_expression(un->get_expression());
        break;

    case EXPR_CALL:
        build_call_expression(bin);
        break;

    case EXPR_INDEX:
        build_expression(bin->get_left());
        build_expression(bin->get_right());
        break;

    case EXPR_ARROW:
        build_expression(bin->get_left());
        build_expression(bin->get_right());
        break;

    case EXPR_DOT:
        build_dot(bin);
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
        build_literal(literal, TYPE_STR);
        break;

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, TYPE_SYMBOL);
        break;

    case EXPR_LITERAL_NULL:
        break;

    case EXPR_TUPLE:
        build_expression_list(exprlist);
        break;

    case EXPR_LIST:
        build_expression_list(exprlist);
        break;

    case EXPR_ARRAY:
        build_expression_list(exprlist);
        break;

    case EXPR_ARGS:
        build_expression_list(exprlist);
        break;

    case EXPR_FOR_INIT:
    case EXPR_FOR_INC:
        build_expression_list(exprlist);
        break;

    case EXPR_HASH:
    case EXPR_HASH_RAW:
        build_hash(exprlist);
        break;

    case EXPR_FUNCTION:
        build_function_expression((FunctionExpression*) expression);
        break;

    case EXPR_NEW:
        build_new_expression((NewExpression*) expression);
        break;
    }
}

void ScopeBuilder::build_assignment(BinOp* bin) {
    int lkind;
    Symbol* sym;
    Expression* left;
    Identifier* id;
    Variable* var;
    Type* rtype;

    build_expression(bin->get_right());

    rtype = bin->get_right()->get_type();
    left = bin->get_left();
    lkind = left->get_kind();
    bin->set_type(rtype);

    if (lkind == EXPR_ID) {
        id = (Identifier*) left;
        sym = current_scope->has(id->get_lexeme());

        if (!sym) {
            var = new Variable(id);
            var->set_type(rtype);
            var->set_uid(var_counter++);
            var->set_kind(VAR_LOCAL);
            bin->set_initial_assign(true);
            sym = current_scope->define(SYM_VARIABLE, var);
            current_function->add_variable(var);
        }
 
        // FIXME
        id->set_symbol(sym);
        id->set_type(rtype);
        std::cout << "=... "; current_scope->debug(); std::cout << '\n';
    } else {
        build_expression(left);
    }
}

void ScopeBuilder::build_address_of(UnOp* op) {
    Type* type;

    build_expression(op->get_expression());

    type = op->get_expression()->get_type();
    type = new IndirectionType(TYPE_POINTER, type);

    op->set_type(type);
}

void ScopeBuilder::build_dot(BinOp* bin) {
    int lkind;
    Type* tleft;
    NamedType* named;
    Symbol* sym;
    Scope* scope;

    build_expression(bin->get_left());

    tleft = bin->get_left()->get_type();
    lkind = tleft->get_kind();

    if (lkind == TYPE_NAMED && bin->get_right()->get_kind() == EXPR_ID) {
        Identifier* id = (Identifier*) bin->get_right();
        const char* name = id->get_lexeme();
        named = (NamedType*) tleft;

        sym = named->has_field(name);

        if (sym) {
            id->set_symbol(sym);
            id->set_type(sym->get_type());
            bin->set_type(sym->get_type());
        } else {
            std::cout << "NOT FOUND SYMBOL\n"; exit(0);
        }
    }
}

void ScopeBuilder::build_call_expression(BinOp* bin) {
    Type* tl;
    Type* tr;
    TypeList* ft;
    TypeList* args;

    build_expression(bin->get_left());
    build_expression(bin->get_right());

    tl = bin->get_left()->get_type();
    tr = bin->get_right()->get_type();

    if (bin->get_left()->get_kind() == EXPR_ID) {
        Identifier* id = (Identifier*) bin->get_left();
        Symbol* sym = id->get_symbol();

        if (tl->get_kind() == TYPE_FUNCTION) {
            args = (TypeList*) tr;
            int i = 0;
            bool found = false;

            for (i = 0; i < sym->overloaded_count(); ++i) {
                Function* f = (Function*) sym->get_descriptor(i);
                ft = (TypeList*) f->get_self_type();

                if (ft->check_arguments_type(args)) {
                    found = true;
                    break;
                }
            }

            if (found) {
                id->set_overloaded_index(i);
            } else {
                // FIXME
                std::cout << "Error: function not overloaded with signature\n";
                exit(0);
            }

            bin->set_type(ft->get_return_type());
        } else if (tl->get_kind() == TYPE_NAMED && sym->get_kind() == SYM_CLASS) {
            bool found = false;
            int i = 0;
            Class* klass = (Class*) sym->get_descriptor();
            args = (TypeList*) tr;

            for (i = 0; i < klass->constructors_count(); ++i) {
                Function* f = (Function*) klass->get_constructor(i);
                ft = (TypeList*) f->get_self_type();

                if (ft->check_arguments_type(args)) {
                    found = true;
                    break;
                }
            }

            if (found) {
                // id->set_overloaded_index(i);
            } else {
                // FIXME
                std::cout << "Error: constructor not overloaded with signature\n";
                exit(0);
            }

            bin->set_type(sym->get_type());
        } else {
            // FIXME
            std::cout << "Error: not a function to be called\n";
            exit(0);
        }
    } else if (bin->get_left()->get_kind() == EXPR_DOT) {
        BinOp* dot = (BinOp*) bin->get_left();
        Identifier* id = (Identifier*) dot->get_right();
        Symbol* sym = id->get_symbol();

        if (tl->get_kind() == TYPE_FUNCTION) {
            args = (TypeList*) tr;
            int i = 0;
            bool found = false;

            for (i = 0; i < sym->overloaded_count(); ++i) {
                Function* f = (Function*) sym->get_descriptor(i);
                ft = (TypeList*) f->get_self_type();

                if (ft->check_arguments_type(args)) {
                    found = true;
                    break;
                }
            }

            if (found) {
                id->set_overloaded_index(i);
            } else {
                // FIXME
                std::cout << "Error: function not overloaded with signature\n";
                exit(0);
            }

            bin->set_type(ft->get_return_type());
        }
    } else if (bin->get_left()->get_kind() == EXPR_TEMPLATE) {

    }
}

void ScopeBuilder::build_binop(std::string oper, BinOp* bin) {
    build_expression(bin->get_left());
    build_expression(bin->get_right());
    bin->set_type(bin->get_left()->get_type());
}

void ScopeBuilder::build_unop(std::string oper, UnOp* un, bool before) {
    build_expression(un->get_expression());
    un->set_type(un->get_expression()->get_type());
}

void ScopeBuilder::build_identifier(Identifier* id) {
    Symbol* sym = current_scope->has(id->get_lexeme());

    // FIXME
    std::cout << "id... "; current_scope->debug(); std::cout << '\n';

    if (!sym) {
        // FIXME
        std::cout << "Error: undefined id " << id->get_lexeme() << "\n";
        exit(0);
    }

    id->set_symbol(sym);
    id->set_type(sym->get_type());
}

void ScopeBuilder::build_literal(Literal* literal, int kind) {
    literal->set_type(new Type(kind));
}

void ScopeBuilder::build_expression_list(ExpressionList* tuple) {
    TypeList* types = new TypeList(TYPE_TUPLE);

    if (tuple->expressions_count() > 0) {
        for (int i = 0; i < tuple->expressions_count(); ++i) {
            build_expression(tuple->get_expression(i));
            types->add_type(tuple->get_expression(i)->get_type());
        }
    } else {
        types->add_type(new Type(TYPE_VOID));
    }

    tuple->set_type(types);
}

void ScopeBuilder::build_hash(ExpressionList* hash) {
/*
    BinOp* pair;
    int i;

    if (hash->get_kind() == EXPR_HASH) {
        out << "{";
    }

    for (i = 0; i < hash->expressions_count() - 1; ++i) {
        pair = (BinOp*) hash->get_expression(i);
        build_expression(pair->get_left());
        out << ": ";
        build_expression(pair->get_right());
        out << ", ";
    }

    pair = (BinOp*) hash->get_expression(i);
    build_expression(pair->get_left());
    out << ": ";
    build_expression(pair->get_right());

    if (hash->get_kind() == EXPR_HASH) {
        out << "}";
    }*/
}

void ScopeBuilder::build_function_expression(FunctionExpression* function) {
/*
    int i;
    Function* f = function->get_function();

    out << "|";

    if (f->parameters_count() > 0) {
        for (i = 0; i < f->parameters_count() - 1; ++i) {
            out << f->get_parameter(i)->get_name();
            out << ", ";
        }

        out << f->get_parameter(i)->get_name();
    }

    out << "| {\n";

    indent();
    build_compound_statement(f->get_statements());
    dedent();

    build_indentation();
    out << "}";*/
}

void ScopeBuilder::build_new_expression(NewExpression* expr) {
    build_type(expr->get_new_type());

    if (expr->has_arguments()) {
        build_expression_list(expr->get_arguments());
    }

    expr->set_type(new IndirectionType(TYPE_POINTER, expr->get_new_type()));
}

void ScopeBuilder::define_sources_elements(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_elements(sources->get_source(i));
    }
}

void ScopeBuilder::define_source_elements(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->import_count(); ++i) {
        current_scope->add_sibling(source->get_import(i)->get_source()->get_scope());
    }

    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }

    for (int i = 0; i < source->function_count(); ++i) {
        define_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeBuilder::define_class(Class* klass) {
    NamedType* type;
    Identifier* id;
    Symbol* sym = current_scope->has(klass->get_name());

    if (!sym) {
        sym = current_scope->define(klass);

        id = new Identifier();
        id->set_line(klass->get_line());
        id->set_column(klass->get_column());
        id->set_lexeme(klass->get_name());
        type = new NamedType();
        type->set_name(id);
        type->set_symbol(sym);
        klass->set_self_type(type);
        klass->set_uid(class_counter++);

        enter_scope(klass->get_scope());

        for (int i = 0; i < klass->methods_count(); ++i) {
            define_method(klass->get_method(i));
        }

        build_class_variables(klass);

        leave_scope();
    } else {
        std::cout << "Error: you tried to define a class named '" << klass->get_name() << "', but it is already defined. Other occurrence\n";
        exit(0);
    }
}

void ScopeBuilder::define_function_template_list(Function* func) {
    TypeList* types = func->get_template_list();

    if (types) {
        for (int i = 0; i < types->types_count(); ++i) {
            TemplateType* type = (TemplateType*) types->get_type(i);
            current_scope->define(type);
        }
    }
}

void ScopeBuilder::define_function(Function* func) {
    Symbol* sym = current_scope->has(func->get_name());

    enter_scope(func->get_scope());
    define_function_template_list(func);
    define_function_parameters(func);
    define_function_self_type(func);
    func->set_uid(function_counter++);
    leave_scope();

    if (!sym) {
        current_scope->define(func);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        define_overloaded_function(sym, func);
    } else {
        std::cout << "Error: you tried to define a function named '" << func->get_name() << "', but it is already defined. Other occurrence\n";
    }
}

void ScopeBuilder::define_method(Function* func) {
    Symbol* sym = current_scope->has(func->get_name());

    enter_scope(func->get_scope());
    define_function_parameters(func);
    define_function_self_type(func);
    func->set_uid(function_counter++);
    leave_scope();

    if (!sym) {
        current_scope->define(SYM_METHOD, func);
    } else if (sym->get_kind() == SYM_METHOD) {
        define_overloaded_function(sym, func);
    } else {
        std::cout << "Error: you tried to define a method named '" << func->get_name() << "', but it is already defined. Other occurrence\n";
    }
}

void ScopeBuilder::define_function_parameters(Function* func) {
    Variable* param;
    Symbol* sym;

    for (int i = 0; i < func->parameters_count(); ++i) {
        param = func->get_parameter(i);
        param->set_uid(i);
        build_type(param->get_type());

        sym = current_scope->has(param->get_name());

        if (!sym) {
            current_scope->define(SYM_PARAMETER, param);
        } else if (sym->get_kind() != SYM_PARAMETER) {
            current_scope->define(SYM_PARAMETER, param);
        } else {
            std::cout << "parameter already defined\n";
        }
    }
}

void ScopeBuilder::define_function_self_type(Function* func) {
    TypeList* types = new TypeList(TYPE_FUNCTION);

    build_type(func->get_return_type());

    if (func->parameters_count() > 0) {
        for (int i = 0; i < func->parameters_count(); ++i) {
            types->add_type(func->get_parameter(i)->get_type());
        }
    } else {
        types->add_type(new Type(TYPE_VOID));
    }

    types->add_type(func->get_return_type());
    func->set_self_type(types);
}

void ScopeBuilder::define_overloaded_function(Symbol* sym, Function* func) {
    for (int i = 0; i < sym->overloaded_count(); ++i) {
        Function* other = (Function*) sym->get_descriptor(i);

        if (func->get_self_type()->equal(other->get_self_type())) {
            // FIXME
            std::cout << "Error: " << func->get_name() << 
                " already defined function with signature\n";
            exit(0);
        } 
    }

    sym->add_descriptor(func);
}

void ScopeBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void ScopeBuilder::leave_scope() {
    current_scope = scopes.top();
    scopes.pop();
}

bool ScopeBuilder::is_function_call(Expression* expr) {
    if (expr->get_kind() != EXPR_ID) {
        return false;
    }

    if (expr->get_type()->get_kind() != TYPE_FUNCTION) {
        return false;
    }

    return true;
}

bool ScopeBuilder::is_constructor_call(Expression* expr) {
    if (expr->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) expr;
    Symbol* sym = id->get_symbol();

    if (expr->get_type()->get_kind() == TYPE_NAMED && sym->get_kind() == SYM_CLASS) {
        return true;
    }

    return false;
}
