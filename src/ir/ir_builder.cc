#include <iostream>
#include <cstring>
#include "ir/ir_builder.h"
#include "ir/ir_call.h"
#include "ir/ir_label.h"
#include "printer/ir_printer.h"

using namespace haard;

IRBuilder::IRBuilder() {
    logger = nullptr;
    current_module = new IRModule();
    ctx = nullptr;
}

IRBuilder::~IRBuilder() {
    for (int i = 0; i < functions.size(); ++i) {
        delete functions[i];
    }
}

void IRBuilder::build(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void IRBuilder::build_source(Source* source) {
    IRModule* module = new IRModule();
    current_module = module;

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    current_module = nullptr;
    modules.push_back(module);
    IRPrinter printer;

    printer.print_module(module);
}

void IRBuilder::build_class(Class* klass) {

}

void IRBuilder::build_function(Function* function) {
    IRFunction* ir_func = new IRFunction();

    ir_func->set_name(function->get_qualified_name());

    functions.push_back(ir_func);
    ctx = ir_func->get_context();

    for (int i = 0; i < function->parameters_count(); ++i) {
        IRValue* p = ctx->new_temporary();

        ir_func->add_parameter(p);

        IRValue* ir_id = ctx->get_var(function->get_parameter(i)->get_name());
        IRValue* tmp0 = ctx->new_temporary();
        IR* ir_addr = ctx->new_unary(IR_ALLOCA, tmp0, ir_id);
        last_value = tmp0;
        ctx->set_alloca_value(ir_id->to_str(), last_value);

        ctx->new_unary(IR_STORE, p, tmp0);
    }

    build_compound_statement(function->get_statements());

    current_module->add_function(ir_func);

    if (strcmp(function->get_name(), "main") == 0) {
        current_module->set_main_function(ir_func);
    }
}

void IRBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

std::vector<IRModule *> IRBuilder::get_modules() const {
    return modules;
}

void IRBuilder::set_modules(const std::vector<IRModule *>& value) {
    modules = value;
}

void IRBuilder::build_statement(Statement* statement) {
    if (statement == nullptr) return;

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
        build_if((BranchStatement*) statement);
        break;

    case STMT_ELSE:
        build_else((BranchStatement*) statement);
        break;

    case STMT_RETURN:
        build_return_statement((JumpStatement*) statement);
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

void IRBuilder::build_compound_statement(CompoundStatement* stmts) {
    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }
}

void IRBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void IRBuilder::build_jump_statement(JumpStatement* statement) {

}

void IRBuilder::build_while_statement(WhileStatement* statement) {

}

void IRBuilder::build_for_statement(ForStatement* statement) {

}

void IRBuilder::build_branch_statement(BranchStatement* statement) {

}

void IRBuilder::build_if(BranchStatement* statement) {
    IRValue* cond;
    IRLabel* fb = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* fb_label = ctx->new_label_value(fb->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    build_expression(statement->get_condition());
    cond = last_value;
    ctx->new_bin(IR_BZ, nullptr, cond, fb_label);

    build_statement(statement->get_true_statements());
    ctx->new_unary(IR_GOTO, nullptr, after_label);
    ctx->add_instruction(fb);
    build_statement(statement->get_false_statements());
    ctx->add_instruction(after);
}

void IRBuilder::build_else(BranchStatement* statement) {
    build_statement(statement->get_true_statements());
}

void IRBuilder::build_return_statement(JumpStatement* statement) {
    if (statement->get_expression()) {
        build_expression(statement->get_expression());
        ctx->new_unary(IR_RETURN, nullptr, last_value);
    } else {
        ctx->new_unary(IR_RETURN, nullptr, nullptr);
    }
}

void IRBuilder::build_expression(Expression* expression, bool lvalue) {
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
        build_identifier((Identifier*) expression, lvalue);
        break;

    case EXPR_CALL:
        build_call(bin);
        break;

    case EXPR_ASSIGN:
        build_assignment(bin, lvalue);
        break;

    case EXPR_EQ:
        build_equal(bin);
        break;

    case EXPR_NE:
        build_not_equal(bin);
        break;

    case EXPR_LT:
        build_less_than(bin);
        break;

    case EXPR_GT:
        build_greater_than(bin);
        break;

    case EXPR_LE:
        build_less_than_or_equal(bin);
        break;

    case EXPR_GE:
        build_greater_than_or_equal(bin);
        break;

    case EXPR_PLUS:
        build_plus(bin);
        break;

    case EXPR_MINUS:
        build_minus(bin);
        break;

    case EXPR_TIMES:
        build_times(bin);
        break;

    case EXPR_ADDRESS_OF:
        build_address_of(un);
        break;

    case EXPR_DEREFERENCE:
        build_dereference(un, lvalue);
        break;

    case EXPR_LITERAL_BOOL:
        build_literal(literal, IR_VALUE_LITERAL_BOOL);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal(literal, IR_VALUE_LITERAL_INTEGER);
        break;

    case EXPR_LITERAL_FLOAT:
        build_literal(literal, IR_VALUE_LITERAL_FLOAT);
        break;

    case EXPR_LITERAL_DOUBLE:
        build_literal(literal, IR_VALUE_LITERAL_DOUBLE);
        break;

    case EXPR_LITERAL_CHAR:
        build_literal(literal, IR_VALUE_LITERAL_CHAR);
        break;

    case EXPR_LITERAL_STRING:
        build_literal(literal, IR_VALUE_LITERAL_STRING);
        break;

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, IR_VALUE_LITERAL_SYMBOL);
        break;
    }
}

void IRBuilder::build_identifier(Identifier* id, bool lvalue) {
    IR* ir_addr;
    IR* ir_load;
    IRValue* ir_id;
    IRValue* tmp0;
    IRValue* tmp1;
    Type* type;

    type = id->get_type();

    if (lvalue) {
        if (type->is_primitive() || type->get_kind() == TYPE_POINTER) {
            ir_id = ctx->get_var(id);

            if (ctx->has_alloca(ir_id->to_str())) {
                last_value = ctx->get_alloca_value(ir_id->to_str());
            } else {
                tmp0 = ctx->new_temporary();
                ir_addr = ctx->new_unary(IR_ALLOCA, tmp0, ir_id);
                last_value = tmp0;
                ctx->set_alloca_value(ir_id->to_str(), last_value);
            }
        }
    } else {
        if (type->is_primitive() || type->get_kind() == TYPE_POINTER) {
            //ir_id = new IRValue(IR_VALUE_VAR, id->get_lexeme());
            ir_id = ctx->get_var(id);

            if (ctx->has_alloca(ir_id->to_str())) {
                tmp0 = ctx->get_alloca_value(ir_id->to_str());
                tmp1 = ctx->new_temporary();
                ir_load = ctx->new_unary(IR_LOAD64, tmp1, tmp0);
                last_value = tmp1;
            } else {
                tmp0 = ctx->new_temporary();
                ir_addr = ctx->new_unary(IR_ALLOCA, tmp0, ir_id);
                tmp1 = ctx->new_temporary();
                ir_load = ctx->new_unary(IR_LOAD64, tmp1, tmp0);
                last_value = tmp1;
            }
        }
    }
}

void IRBuilder::build_call(BinOp* bin) {
    IRCall* call = new IRCall();
    build_call_arguments(call, (ExpressionList*) bin->get_right());

    if (bin->get_left()->get_kind() == EXPR_ID) {
        Identifier* id = (Identifier*) bin->get_left();
        Function* f = (Function*) id->get_symbol()->get_descriptor();
        std::string name = f->get_qualified_name();

        call->set_name(name);
        ctx->add_instruction(call);
    }

    if (bin->get_type()->get_kind() != TYPE_VOID) {
        call->set_dst(ctx->new_temporary());
        last_value = call->get_dst();
    }
}

void IRBuilder::build_call_arguments(IRCall* call, ExpressionList* args) {
    if (args == nullptr) {
        return;
    }

    if (args->expressions_count() > 0) {
        for (int i = 0; i < args->expressions_count(); ++i) {
            build_expression(args->get_expression(i));
            call->add_argument(last_value);
        }
    }
}

void IRBuilder::build_assignment(BinOp* bin, bool lvalue) {
    IR* ir;
    IRValue* left;
    IRValue* right;
    IRValue* dst;

    build_expression(bin->get_right());
    right = last_value;

    build_expression(bin->get_left(), true);
    left = last_value;

    // FIXME
    // on complex types, should call memcpy instead of a simple store
    ir = ctx->new_unary(IR_STORE, right, left);
    last_value = left;
}

void IRBuilder::build_equal(BinOp* bin) {
    build_binop(bin, IR_EQ);
}

void IRBuilder::build_not_equal(BinOp* bin) {
    build_binop(bin, IR_NE);
}

void IRBuilder::build_less_than(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LT);
    } else {
        build_binop(bin, IR_ULT);
    }
}

void IRBuilder::build_greater_than(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GT);
    } else {
        build_binop(bin, IR_UGT);
    }
}

void IRBuilder::build_less_than_or_equal(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LE);
    } else {
        build_binop(bin, IR_ULE);
    }
}

void IRBuilder::build_greater_than_or_equal(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GE);
    } else {
        build_binop(bin, IR_UGE);
    }
}

void IRBuilder::build_plus(BinOp* bin) {
    build_binop(bin, IR_ADD);
}

void IRBuilder::build_minus(BinOp* bin) {
    build_binop(bin, IR_SUB);
}

void IRBuilder::build_times(BinOp* bin) {
    build_binop(bin, IR_MUL);
}

void IRBuilder::build_division(BinOp* bin) {
    build_binop(bin, IR_DIV);
}

void IRBuilder::build_address_of(UnOp* op) {
    build_expression(op->get_expression(), true);
}

void IRBuilder::build_dereference(UnOp* op, bool lvalue) {
    build_expression(op->get_expression());

    if (!lvalue) {
        IRValue* tmp = ctx->new_temporary();
        IR* ir_load = ctx->new_unary(IR_LOAD64, tmp, last_value);
        last_value = tmp;
    }
}

void IRBuilder::build_binop(BinOp* bin, int kind) {
    IR* ir;
    IRValue* left;
    IRValue* right;
    IRValue* dst;

    build_expression(bin->get_left());
    left = last_value;

    build_expression(bin->get_right());
    right = last_value;

    dst = ctx->new_temporary();
    ir = ctx->new_bin(kind, dst, left, right);
    last_value = dst;
}

void IRBuilder::build_literal(Literal* literal, int kind) {
    IR* ir;
    IRValue* ir_literal;
    IRValue* tmp;

    ir_literal = ctx->get_literal(kind, literal->get_lexeme());
    tmp = ctx->new_temporary();
    ir = ctx->new_unary(IR_LI, tmp, ir_literal);
    last_value = tmp;

    last_value = tmp;
}

void IRBuilder::build_literal_integer(Literal* literal) {
    IR* ir;
    IRValue* ir_literal;
    IRValue* tmp;

    ir_literal = ctx->get_literal(IR_VALUE_LITERAL_INTEGER, literal->get_lexeme());
    tmp = ctx->new_temporary();
    ir = ctx->new_unary(IR_LI, tmp, ir_literal);
    last_value = tmp;

    last_value = tmp;
}

