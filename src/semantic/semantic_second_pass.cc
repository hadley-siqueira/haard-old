#include <iostream>
#include "semantic/semantic_second_pass.h"
#include "log/actions.h"
#include "log/infos.h"

using namespace haard;

void SemanticSecondPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticSecondPass::build_module(Module* module) {
    enter_scope(module->get_scope());

    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }

    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }

    leave_scope();
}

void SemanticSecondPass::build_class(Class* klass) {
    enter_scope(klass->get_scope());

    for (int i = 0; i < klass->methods_count(); ++i) {
        build_method(klass->get_method(i));
    }

    leave_scope();
}

void SemanticSecondPass::build_function(Function* function) {
    if (function->is_template()) {
        return;
    }

    set_function(function);
    reset_local_var_counter();
    enter_scope(function->get_scope());

    build_statement(function->get_statements());

    leave_scope();
    set_function(nullptr);
}

void SemanticSecondPass::build_method(Function* method) {
    build_function(method);
}

void SemanticSecondPass::build_statement(Statement* stmt) {
    int kind = stmt->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) stmt);
        break;

    case STMT_WHILE:
        build_while_statement((WhileStatement*) stmt);
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) stmt);
        break;

    default:
        break;
    }
}

void SemanticSecondPass::build_compound_statement(CompoundStatement* stmt) {
    enter_scope(stmt->get_scope());

    for (int i = 0; i < stmt->statements_count(); ++i) {
        build_statement(stmt->get_statement(i));
    }

    leave_scope();
}

void SemanticSecondPass::build_while_statement(WhileStatement* stmt) {
    enter_scope(stmt->get_scope());

    build_expression(stmt->get_condition());
    build_statement(stmt->get_statements());

    leave_scope();
}

void SemanticSecondPass::build_expression_statement(ExpressionStatement* stmt) {
    build_expression(stmt->get_expression());
}

void SemanticSecondPass::build_expression(Expression* expr) {
    int kind;

    if (expr == nullptr) {
        return;
    }

    kind = expr->get_kind();

    switch (kind) {
    case EXPR_ASSIGNMENT:
        build_assignment((Assignment*) expr);
        break;

    case EXPR_LESS_THAN:
        build_less_than((LessThan*) expr);
        break;

    case EXPR_PLUS:
        build_plus((Plus*) expr);
        break;

    case EXPR_CALL:
        build_call((Call*) expr);
        break;

    case EXPR_ADDRESS_OF:
        build_address_of((AddressOf*) expr);
        break;

    case EXPR_DEREFERENCE:
        build_dereference((Dereference*) expr);
        break;

    case EXPR_DOT:
        build_dot((Dot*) expr);
        break;

    case EXPR_ID:
        build_identifier((Identifier*) expr);
        break;

    case EXPR_LITERAL_BOOL:
        build_literal_bool((Literal*) expr);
        break;

    case EXPR_LITERAL_CHAR:
        build_literal_char((Literal*) expr);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal_integer((Literal*) expr);
        break;

    case EXPR_LITERAL_FLOAT:
        build_literal_float((Literal*) expr);
        break;

    case EXPR_LITERAL_DOUBLE:
        build_literal_double((Literal*) expr);
        break;

    default:
        break;
    }
}

void SemanticSecondPass::build_assignment(Assignment* expr) {
    build_expression(expr->get_right());

    if (is_new_variable(expr)) {
        create_variable(expr);
    }

    build_expression(expr->get_left());
    expr->set_type(expr->get_left()->get_type());
}

void SemanticSecondPass::build_less_than(LessThan* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    expr->set_type(new Type(TYPE_BOOL));
}

void SemanticSecondPass::build_plus(Plus* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    expr->set_type(expr->get_left()->get_type());
}

void SemanticSecondPass::build_call(Call* expr) {
    build_expression_list(expr->get_arguments());

    if (is_simple_call(expr)) {
        build_simple_call(expr);
    } else if (is_method_call(expr)) {
        build_method_call(expr);
    } else if (is_constructor_call(expr)) {
        build_constructor_call(expr);
    }
}

void SemanticSecondPass::build_method_call(Call* expr) {
    bool found = false;
    Function* best = nullptr;
    Identifier* id = (Identifier*) expr->get_object();
    Function* f = nullptr;

    Symbol* sym = get_scope()->resolve(id->get_name());

    for (int i = 0; i < sym->descriptors_count(); ++i) {
        SymbolDescriptor* desc = sym->get_descriptor(i);
        f = (Function*) desc->get_descriptor();

        if (f->parameters_count() == expr->get_arguments()->expressions_count()) {
            bool flag = true;

            for (int j = 0; j < expr->get_arguments()->expressions_count(); ++j) {
                Type* t1 = expr->get_arguments()->get_expression(j)->get_type();
                Type* t2 = f->get_parameter(j)->get_type();

                if (!t1->equal(t2)) {
                    flag = false;
                }
            }

            if (flag) {
                found = true;
                best = f;
            }
        }
    }

    if (found) {
        expr->set_function(best);
        expr->set_type(best->get_return_type());
        log_info("function: " + best->get_qualified_name());
    } else {
        log_error_and_exit("second pass: couldn't resolve method call " + id->get_name());
    }
}

void SemanticSecondPass::build_constructor_call(Call* expr) {
    bool found = false;
    Function* best = nullptr;
    Identifier* id = (Identifier*) expr->get_object();
    SymbolDescriptor* desc;
    CompoundTypeDescriptor* f;

    Symbol* sym = get_scope()->resolve(id->get_name());

    for (int i = 0; i < sym->descriptors_count(); ++i) {
        desc = sym->get_descriptor(i);
        f = (CompoundTypeDescriptor*) desc->get_descriptor();

        for (int j = 0; j < f->methods_count(); ++j) {
            Function* ff = (Function*) f->get_method(j);

            if (ff->get_name() == "init") {
                if (ff->parameters_count() == expr->get_arguments()->expressions_count()) {
                    bool flag = true;

                    for (int k = 0; k < expr->get_arguments()->expressions_count(); ++k) {
                        Type* t1 = expr->get_arguments()->get_expression(k)->get_type();
                        Type* t2 = ff->get_parameter(k)->get_type();

                        if (!t1->equal(t2)) {
                            flag = false;
                        }
                    }

                    if (flag) {
                        found = true;
                        best = ff;
                    }
                }
            }
        }
    }

    if (found) {
        expr->set_function(best);
        expr->set_type(f->get_self_type());
        log_info("constructor: " + best->get_qualified_name());
    } else {
        log_error_and_exit("second pass: couldn't resolve constructor call " + id->get_name());
    }
}

void SemanticSecondPass::build_expression_list(ExpressionList* list) {
    if (list) {
        for (int i = 0; i < list->expressions_count(); ++i) {
            build_expression(list->get_expression(i));
        }
    }
}

void SemanticSecondPass::build_address_of(AddressOf* expr) {
    build_expression(expr->get_expression());
    expr->set_type(new IndirectionType(TYPE_POINTER, expr->get_expression()->get_type()));
}

void SemanticSecondPass::build_dereference(Dereference* expr) {
    build_expression(expr->get_expression());

    Type* t = expr->get_expression()->get_type();

    if (t->get_kind() != TYPE_POINTER) {
        log_error_and_exit("semantic_second_pass: invalid dereference");
    }

    IndirectionType* pointer_type = (IndirectionType*) t;
    expr->set_type(pointer_type->get_subtype());
}

void SemanticSecondPass::build_dot(Dot* expr) {
    build_expression(expr->get_left());

    Identifier* id = (Identifier*) expr->get_right();
    Type* type = expr->get_left()->get_type();

    if (type->get_kind() == TYPE_NAMED) {
        Scope* scope = type->get_scope();

        if (scope) {
            Symbol* sym = scope->resolve_field(id->get_name());

            if (sym) {
                if (sym->get_type()) {
                    expr->set_type(sym->get_type());
                } else {
                    std::cout << "NOOO\n" << sym->get_name() << '\n';
                    exit(0);
                }
            } else {
                log_error_and_exit("semantic_second_pass: invalid member access 0");
            }
        } else {
            log_error_and_exit("semantic_second_pass: invalid member access 1");
        }
    } else {
        log_error_and_exit("semantic_second_pass: invalid member access 2");
    }
}

void SemanticSecondPass::build_identifier(Identifier* expr) {
    Symbol* sym = nullptr;

    sym = get_scope()->resolve(expr->get_name());

    if (sym == nullptr) {
        log_error_and_exit("not in scope: " + expr->get_name());
    }

    if (sym->descriptors_count() == 1) {
        expr->set_symbol_descriptor(sym->get_descriptor(0));
    } else {
        log_error_and_exit("first pass: can't resolve id " + expr->get_name() + ". Multiple definitions");
    }
}

void SemanticSecondPass::build_literal_bool(Literal* expr) {
    expr->set_type(new Type(TYPE_BOOL));
}

void SemanticSecondPass::build_literal_char(Literal* expr) {
    expr->set_type(new Type(TYPE_CHAR));
}

void SemanticSecondPass::build_literal_integer(Literal* expr) {
    expr->set_type(new Type(TYPE_INT));
}

void SemanticSecondPass::build_literal_float(Literal* expr) {
    expr->set_type(new Type(TYPE_FLOAT));
}

void SemanticSecondPass::build_literal_double(Literal* expr) {
    expr->set_type(new Type(TYPE_DOUBLE));
}

bool SemanticSecondPass::is_new_variable(Assignment* expr) {
    if (expr->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) expr->get_left();

    if (id->has_scope()) {
        return false;
    }

    Symbol* sym = get_scope()->resolve(id->get_name());

    return sym == nullptr;
}

bool SemanticSecondPass::is_simple_call(Call* expr) {
    return expr->get_object()->get_kind() == EXPR_ID;
}

bool SemanticSecondPass::is_method_call(Call* expr) {
    Expression* obj = expr->get_object();
    int kind = obj->get_kind();

    if (kind == EXPR_ID) {
        Identifier* id = (Identifier*) expr->get_object();

        Symbol* sym = get_scope()->resolve(id->get_name());

        if (sym == nullptr) {
            log_info(get_scope()->debug());
            log_error_and_exit(id->get_name() + " not in scope");
        }

        SymbolDescriptor* desc = sym->get_descriptor(0);
        return desc->get_kind() == SYM_METHOD;
    } else if (kind == EXPR_DOT) {
        Dot* member = (Dot*) obj;

        build_expression(member->get_left());

        Identifier* id = (Identifier*) member->get_right();
        Type* type = member->get_left()->get_type();
        Scope* scope = type->get_scope();

        Symbol* sym = scope->resolve_field(id->get_name());

        if (sym == nullptr) {
            log_info(get_scope()->debug());
            log_error_and_exit(id->get_name() + " not in scope");
        }

        SymbolDescriptor* desc = sym->get_descriptor(0);
        return desc->get_kind() == SYM_METHOD;
    }

    return false;
}


bool SemanticSecondPass::is_constructor_call(Call* expr) {
    Expression* obj = expr->get_object();
    int kind = obj->get_kind();

    if (kind == EXPR_ID) {
        Identifier* id = (Identifier*) expr->get_object();

        Symbol* sym = get_scope()->resolve(id->get_name());
        SymbolDescriptor* desc = sym->get_descriptor(0);
        kind = desc->get_kind();

        return kind == SYM_CLASS ||
               kind == SYM_ENUM  ||
               kind == SYM_UNION ||
               kind == SYM_STRUCT;
    }

    return false;
}

void SemanticSecondPass::create_variable(Assignment* expr) {
    Identifier* id = (Identifier*) expr->get_left();

    Variable* var = new Variable(id);
    var->set_type(expr->get_right()->get_type());
    var->set_kind(VAR_LOCAL);
    var->set_uid(next_local_var_counter());
    auto sym = get_scope()->define_local_variable(var);
    get_function()->add_variable(var);

    std::stringstream ss;

    ss << "creating local variable <white>" + id->get_name() + ":";
    ss << var->get_type()->to_str() + "</white>";
    log_info(ss.str());
    expr->set_initial_value(true);

    id->set_symbol_descriptor(sym);
}
// foo
// method() -> inside the class
// Class() -> call constructor

void SemanticSecondPass::build_simple_call(Call* expr) {
    Identifier* id = (Identifier*) expr->get_object();
    ExpressionList* args = expr->get_arguments();

    Symbol* sym = get_scope()->resolve(id->get_name());

    if (sym == nullptr) {
        log_error_and_exit("second pass: not in scope " + id->get_name());
    }

    if (sym->get_descriptor(0)->get_kind() == SYM_CLASS) {
        Scope* s = sym->get_descriptor(0)->get_descriptor_scope();
        sym = s->resolve_field("init");

        if (sym) {
            std::cout << "found\n";
        } else {
            std::cout << "not found\n";
        }
        //std::cout << s->debug() << '\n';
        //exit(0);
    }

    int idx = find_best_match(sym, args);

    if (idx < 0) {
        log_error_and_exit("second pass: no match signature");
    }

    set_call_type(expr, sym, idx);
    id->set_symbol_descriptor(sym->get_descriptor(idx));
}

void SemanticSecondPass::set_call_type(Call* expr, Symbol* sym, int idx) {
    Function* function = (Function*) sym->get_descriptor(idx)->get_descriptor();
    expr->set_function(function);

    if (function->is_constructor()) {
        CompoundTypeDescriptor* decl = function->get_compound();
        expr->set_type(decl->get_self_type());
    } else {
        expr->set_type(function->get_return_type());
    }
}

int SemanticSecondPass::find_best_match(Symbol* sym, ExpressionList* args) {
    for (int i = 0; i < sym->descriptors_count(); ++i) {
        if (compare_match(sym->get_descriptor(i), args)) {
            return i;
        }
    }

    return -1;
}

bool SemanticSecondPass::compare_match(SymbolDescriptor* desc, ExpressionList* args) {
    Function* function = (Function*) desc->get_descriptor();

    if (function->parameters_count() < args->expressions_count()) {
        return false;
    }

    for (int i = 0; i < args->expressions_count(); ++i) {
        Type* param_type = function->get_parameter(i)->get_type();
        Type* arg_type = args->get_expression(i)->get_type();

        if (!param_type->equal(arg_type)) {
            return false;
        }
    }

    if (function->parameters_count() > args->expressions_count()) {
        // TODO: need to handle default parameters?
    }

    return true;
}
