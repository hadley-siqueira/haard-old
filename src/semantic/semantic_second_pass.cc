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

void SemanticSecondPass::build_statement(Statement* stmt) {
    int kind = stmt->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) stmt);
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

    case EXPR_CALL:
        build_call((Call*) expr);
        break;

    case EXPR_ID:
        build_identifier((Identifier*) expr);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal_integer((Literal*) expr);
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
}

void SemanticSecondPass::build_call(Call* expr) {
    build_expression_list(expr->get_arguments());

    if (is_function_call(expr)) {
        build_function_call(expr);
    } else if (is_constructor_call(expr)) {
        build_constructor_call(expr);
    }
}

void SemanticSecondPass::build_function_call(Call* expr) {
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
        log_error_and_exit("second pass: couldn't resolve function call " + id->get_name());
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

void SemanticSecondPass::build_literal_integer(Literal* expr) {
    expr->set_type(new Type(TYPE_INT));
}

void SemanticSecondPass::build_literal_float(Literal* expr) {
    expr->set_type(new Type(TYPE_FLOAT));
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

bool SemanticSecondPass::is_function_call(Call* expr) {
    Expression* obj = expr->get_object();
    int kind = obj->get_kind();

    if (kind == EXPR_ID) {
        Identifier* id = (Identifier*) expr->get_object();

        Symbol* sym = get_scope()->resolve(id->get_name());
        SymbolDescriptor* desc = sym->get_descriptor(0);
        return desc->get_kind() == SYM_FUNCTION;
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
    get_scope()->define_local_variable(var);
    get_function()->add_variable(var);

    std::stringstream ss;

    ss << "creating local variable <white>" + id->get_name() + ":";
    ss << var->get_type()->to_str() + "</white>";
    log_info(ss.str());
}
