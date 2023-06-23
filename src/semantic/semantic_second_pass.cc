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
    case EXPR_ID:
        build_identifier((Identifier*) expr);
        break;

    case EXPR_ASSIGNMENT:
        build_assignment((Assignment*) expr);
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

void SemanticSecondPass::build_identifier(Identifier* expr) {
    bool found = false;
    Symbol* sym = nullptr;

    sym = get_scope()->resolve(expr->get_name());

    if (sym == nullptr) {
        if (expr->has_template()) {
            int i;
            std::stringstream ss;

            ss << expr->get_name() << '<';

            for (i = 0; i < expr->get_template_header()->types_count() - 1; ++i) {
                ss << "%" << i << ", ";
            }

            ss << "%" << i << '>';
            std::cout << ss.str() << '\n'; exit(0);

            sym = get_scope()->resolve(ss.str());

            if (sym != nullptr) {
                found = true;
            }
        }
    } else {
        found = true;
    }

    if (!found) {
        log_error_and_exit("not in scope: " + expr->get_name());
    }

    std::cout << "id: " << expr->get_name() << '\n'
              << sym->get_qualified_name() << '\n'
              << get_scope()->debug();

    expr->set_symbol(sym);
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

void SemanticSecondPass::create_variable(Assignment* expr) {
    Identifier* id = (Identifier*) expr->get_left();

    Variable* var = new Variable(id);
    var->set_type(expr->get_right()->get_type());
    var->set_kind(VAR_LOCAL);
    var->set_uid(next_local_var_counter());
    get_scope()->define_local_variable(var);
    get_function()->add_variable(var);
}
