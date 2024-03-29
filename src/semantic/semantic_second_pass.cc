#include <iostream>
#include "semantic/semantic_second_pass.h"
#include "log/actions.h"
#include "log/infos.h"
#include "parser/parser.h"
#include "semantic/semantic_first_pass.h"

using namespace haard;

void SemanticSecondPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticSecondPass::build_module(Module* module) {
    enter_scope(module->get_scope());
    set_compound(nullptr);

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
    set_compound(klass);

    if (klass->get_super_type()) {
        link_type(klass->get_super_type());
    }

    for (int i = 0; i < klass->methods_count(); ++i) {
        build_method(klass->get_method(i));
    }

    set_compound(nullptr);
    leave_scope();
}

void SemanticSecondPass::build_function(Function* function) {
    if (function->is_template()) {
        return;
    }

    set_function(function);
    reset_local_var_counter();
    enter_scope(function->get_scope());

    if (function->is_constructor()) {
        add_members_initialization(function);
        add_parent_constructor_call(function);
    }

    build_statement(function->get_statements());

    leave_scope();
    set_function(nullptr);
}

void SemanticSecondPass::build_method(Function* method) {
    build_function(method);
}

void SemanticSecondPass::build_statement(Statement* stmt) {
    if (stmt == nullptr) return;

    int kind = stmt->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) stmt);
        break;

    case STMT_WHILE:
        build_while_statement((WhileStatement*) stmt);
        break;

    case STMT_IF:
        build_if_statement((If*) stmt);
        break;

    case STMT_ELIF:
        build_elif_statement((Elif*) stmt);
        break;

    case STMT_ELSE:
        build_else_statement((Else*) stmt);
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) stmt);
        break;

    case STMT_RETURN:
        build_return_statement((JumpStatement*) stmt);
        break;

    case STMT_VAR_DECL:
        build_var_declaration_statement((VarDeclaration*) stmt);
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

void SemanticSecondPass::build_if_statement(If* stmt) {
    enter_scope(stmt->get_scope());

    build_expression(stmt->get_condition());
    build_statement(stmt->get_true_statements());

    leave_scope();

    build_statement(stmt->get_false_statements());
}

void SemanticSecondPass::build_elif_statement(Elif* stmt) {
    enter_scope(stmt->get_scope());

    build_expression(stmt->get_condition());
    build_statement(stmt->get_true_statements());

    leave_scope();

    build_statement(stmt->get_false_statements());
}

void SemanticSecondPass::build_else_statement(Else* stmt) {
    enter_scope(stmt->get_scope());

    build_statement(stmt->get_statements());

    leave_scope();
}

void SemanticSecondPass::build_return_statement(JumpStatement* stmt) {
    build_expression(stmt->get_expression());
}

void SemanticSecondPass::build_var_declaration_statement(VarDeclaration* stmt) {
    Expression* expr = stmt->get_expression();

    if (expr) {
        build_expression(expr);
    }

    Variable* var = stmt->get_variable();
    link_type(var->get_type());
    var->set_kind(VAR_LOCAL);
    var->set_uid(next_local_var_counter());
    get_scope()->define_local_variable(var);
    get_function()->add_variable(var);

    std::stringstream ss;

    ss << "creating local variable <white>" + var->get_name() + ":";
    ss << var->get_type()->to_str() + "</white>";
    log_info(ss.str());
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

    case EXPR_EQUAL:
        build_equal((Equal*) expr);
        break;

    case EXPR_NOT_EQUAL:
        build_not_equal((NotEqual*) expr);
        break;

    case EXPR_LESS_THAN:
        build_less_than((LessThan*) expr);
        break;

    case EXPR_PLUS:
        build_plus((Plus*) expr);
        break;

    case EXPR_MINUS:
        build_minus((Minus*) expr);
        break;

    case EXPR_TIMES:
        build_times((Times*) expr);
        break;

    case EXPR_DIVISION:
        build_division((Division*) expr);
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

    case EXPR_INDEX:
        build_index((Index*) expr);
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

    case EXPR_LITERAL_STRING:
        build_literal_string((Literal*) expr);
        break;

    case EXPR_STRING_BUILDER:
        build_string_builder((StringBuilder*) expr);
        break;

    case EXPR_CAST:
        build_cast((Cast*) expr);
        break;

    case EXPR_PRE_INCREMENT:
        build_pre_increment((PreIncrement*) expr);
        break;

    case EXPR_PRE_DECREMENT:
        build_pre_decrement((PreDecrement*) expr);
        break;

    case EXPR_POS_INCREMENT:
        build_pos_increment((PosIncrement*) expr);
        break;

    case EXPR_POS_DECREMENT:
        build_pos_decrement((PosDecrement*) expr);
        break;

    case EXPR_PARENTHESIS:
        build_parenthesis((Parenthesis*) expr);
        break;

    case EXPR_THIS:
        build_this((This*) expr);
        break;

    default:
        break;
    }
}

void SemanticSecondPass::build_assignment(Assignment* expr) {
    build_expression(expr->get_right());

    if (is_new_variable(expr)) {
        create_variable(expr);

        if (expr->get_right()->get_type()->is_user_type()) {
            expr->is_constructor_call(true);
        }
    }

    build_expression(expr->get_left());
    expr->set_type(expr->get_left()->get_type());
}

void SemanticSecondPass::build_equal(Equal* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    expr->set_type(new Type(TYPE_BOOL));
}

void SemanticSecondPass::build_not_equal(NotEqual* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    expr->set_type(new Type(TYPE_BOOL));
}

void SemanticSecondPass::build_less_than(LessThan* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    expr->set_type(new Type(TYPE_BOOL));
}

void SemanticSecondPass::build_plus(Plus* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_minus(Minus* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_times(Times* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_division(Division* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_bitwise_or(BitwiseOr* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_bitwise_xor(BitwiseXor* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::build_bitwise_and(BitwiseAnd* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    Type* tright = expr->get_right()->get_type();
    Type* etype = tleft->promote(tright);

    expr->set_type(etype);
}

void SemanticSecondPass::print_shift_left_logical(ShiftLeftLogical* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    expr->set_type(tleft);
}

void SemanticSecondPass::print_shift_right_logical(ShiftRightLogical* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    expr->set_type(tleft);
}

void SemanticSecondPass::print_shift_right_arithmetic(ShiftRightArithmetic* expr) {
    build_expression(expr->get_left());
    build_expression(expr->get_right());

    Type* tleft = expr->get_left()->get_type();
    expr->set_type(tleft);
}

void SemanticSecondPass::build_call(Call* expr) {
    build_expression_list(expr->get_arguments());

    if (is_simple_call(expr)) {
        build_simple_call(expr);
    } else if (is_member_call(expr)) {
        build_member_call(expr);
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

void SemanticSecondPass::build_index(Index* expr) {
    build_expression(expr->get_object());
    build_expression(expr->get_index());

    Type* type = expr->get_object()->get_type();

    if (type->get_kind() == TYPE_POINTER) {
        auto ptype = (IndirectionType*) type;
        expr->set_type(ptype->get_subtype());
    } else if (type->get_kind() == TYPE_ARRAY) {
        auto atype = (ArrayListType*) type;
        expr->set_type(atype->get_subtype());
    }
}

void SemanticSecondPass::build_dot(Dot* expr) {
    build_expression(expr->get_left());

    Identifier* id = (Identifier*) expr->get_right();
    Type* type = expr->get_left()->get_type();

    if (type->get_kind() == TYPE_POINTER) {
        type = ((IndirectionType*) type)->get_subtype();
    }

    if (type->get_kind() == TYPE_NAMED) {
        Scope* scope = type->get_scope();

        if (scope) {
            Symbol* sym = scope->resolve_field(id->get_name());

            if (sym) {
                if (sym->get_type()) {
                    expr->set_type(sym->get_type());
                    id->set_symbol_descriptor(sym->get_descriptor());
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

void SemanticSecondPass::build_cast(Cast* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_cast_type());
    link_type(expr->get_type());
}

void SemanticSecondPass::build_pre_increment(PreIncrement* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_expression()->get_type());
    link_type(expr->get_type());
}

void SemanticSecondPass::build_pre_decrement(PreDecrement* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_expression()->get_type());
    link_type(expr->get_type());
}

void SemanticSecondPass::build_pos_increment(PosIncrement* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_expression()->get_type());
    link_type(expr->get_type());
}

void SemanticSecondPass::build_pos_decrement(PosDecrement* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_expression()->get_type());
    link_type(expr->get_type());
}

void SemanticSecondPass::build_parenthesis(Parenthesis* expr) {
    build_expression(expr->get_expression());
    expr->set_type(expr->get_expression()->get_type());
    link_type(expr->get_type());
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

void SemanticSecondPass::build_template_header(TemplateHeader* theader) {
    if (theader == nullptr) return;

    for (int i = 0; i < theader->types_count(); ++i) {
        link_type(theader->get_type(i));
    }
}

void SemanticSecondPass::build_this(This* expr) {
    if (get_compound() == nullptr) {
        log_error_and_exit("using this outside user type");
    }

    expr->set_type(new IndirectionType(TYPE_POINTER, get_compound()->get_self_type()));
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

void SemanticSecondPass::build_literal_string(Literal* expr) {
    Type* t = new Type(TYPE_CHAR);
    t = new IndirectionType(TYPE_POINTER, t);
    expr->set_type(t);
}

void SemanticSecondPass::build_string_builder(StringBuilder* expr) {

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

bool SemanticSecondPass::is_member_call(Call* expr) {
    int kind = expr->get_object()->get_kind();

    return kind == EXPR_DOT || kind == EXPR_ARROW;
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
    build_template_header(id->get_template_header());

    if (sym == nullptr) {
        log_error_and_exit("second pass: not in scope " + id->get_name());
    }

    SymbolDescriptor* idx = find_best_match(sym, args, id->get_template_header());

    if (idx == nullptr) {
        log_error_and_exit("second pass: no match signature");
    }

    set_call_type(expr, idx);
    id->set_symbol_descriptor(idx);
}

void SemanticSecondPass::build_member_call(Call* expr) {
    Identifier* field = nullptr;
    Expression* obj = nullptr;
    Symbol* sym = nullptr;
    ExpressionList* args = expr->get_arguments();
    int kind = expr->get_object()->get_kind();

    if (kind == EXPR_DOT) {
        Dot* dot = (Dot*) expr->get_object();
        obj = dot->get_object();
        field = (Identifier*) dot->get_field();
    } else if (kind == EXPR_ARROW) {
        Arrow* arrow = (Arrow*) expr->get_object();
        obj = arrow->get_object();
        field = (Identifier*) arrow->get_field();
    }

    build_expression(obj);
    Type* type = obj->get_type();

    if (type->get_kind() == TYPE_POINTER) {
        type = ((IndirectionType*) type)->get_subtype();
    }

    if (type->is_user_type()) {
        Scope* scope = type->get_scope();

        if (scope) {
            sym = scope->resolve_field(field->get_name());

            if (sym) {

            } else {
                log_error_and_exit("semantic_second_pass: invalid member access 00");
            }
        } else {
            log_error_and_exit("semantic_second_pass: invalid member access 11");
        }
    } else {
        log_error_and_exit("Invalid member access: not a named type");
    }

    if (sym == nullptr) {
        log_error_and_exit("second pass: not in scope " + field->get_name());
    }

    SymbolDescriptor* idx = find_best_match(sym, args);

    if (idx == nullptr) {
        log_error_and_exit("second pass: no match signature");
    }

    set_call_type(expr, idx);
    field->set_symbol_descriptor(idx);
}

void SemanticSecondPass::set_call_type(Call* expr, SymbolDescriptor* desc) {
    Function* function = (Function*) desc->get_descriptor();
    expr->set_function(function);

    if (function->is_constructor()) {
        CompoundTypeDescriptor* decl = function->get_compound();
        expr->set_type(decl->get_self_type());
    } else {
        expr->set_type(function->get_return_type());
    }
}

void SemanticSecondPass::add_members_initialization(Function* function) {
    Class* klass = (Class*) function->get_compound();
    CompoundStatement* stmts;
    Variable* var;
    Type* type;

    stmts = function->get_statements();

    for (int i = 0; i < klass->fields_count(); ++i) {
        var = klass->get_field(i);
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

void SemanticSecondPass::add_parent_constructor_call(Function* function) {
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

SymbolDescriptor* SemanticSecondPass::find_best_match(Symbol* sym, ExpressionList* args, TemplateHeader* templates) {
    Function* function;

    for (int i = 0; i < sym->descriptors_count(); ++i) {
        SymbolDescriptor* desc = sym->get_descriptor(i);
        int kind = desc->get_kind();

        if (kind == SYM_METHOD || kind == SYM_FUNCTION) {
            function = (Function*) desc->get_descriptor();

            if (compare_match(function, args)) {
                return desc;
            }
        } else if (kind == SYM_CLASS || kind == SYM_STRUCT || kind == SYM_ENUM || kind == SYM_UNION) {
            CompoundTypeDescriptor* klass = (CompoundTypeDescriptor*) desc->get_descriptor();

            if (klass->is_template()) {
                /*if (templates->types_count() == klass->get_template_header()->types_count()) {
                    Declaration* decl = klass->get_templates(templates);

                    if (decl == nullptr) {
                        std::string nk = klass->get_with_templates(templates->get_types());

                        Parser p;
                        p.set_path(klass->get_path());
                        auto nklass = p.read_class_from_string(nk);
                        nklass->get_template_header()->set_template_flag(false);
                        klass->add_instance(nklass);
                        auto ns = nklass->get_scope();
                        ns->set_parent(klass->get_scope()->get_parent());
                        nklass->set_module(klass->get_module());

                        SemanticFirstPass fp;
                        fp.build_class(nklass);
                        //build_class(nklass);
                        DBG; exit(0);
                    }
                } else {
                    std::cout << "found\n"; DBG; exit(0);
                }*/
            }

            Symbol* ss = klass->get_scope()->resolve_field("init");

            for (int j = 0; j < ss->descriptors_count(); ++j) {
                desc = ss->get_descriptor(j);

                function = (Function*) desc->get_descriptor();

                if (compare_match(function, args)) {
                    return desc;
                }
            }
        }
    }

    return nullptr;
}

bool SemanticSecondPass::compare_match(Function* function, ExpressionList* args) {
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
