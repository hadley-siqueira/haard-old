#include <iostream>
#include "ir/ir_builder.h"

using namespace haard;

IRBuilder::IRBuilder() {
    logger = nullptr;
    ctx = new IRContext();
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
    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }
}

void IRBuilder::build_class(Class* klass) {

}

void IRBuilder::build_function(Function* function) {
    IRFunction* ir_func = new IRFunction();

    ir_func->set_name(function->get_name());

    functions.push_back(ir_func);
    ctx = ir_func->get_context();
    build_compound_statement(function->get_statements());

    std::cout << ir_func->to_str();
}

void IRBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

void IRBuilder::build_statement(Statement* statement) {
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

    case EXPR_ASSIGN:
        build_assignment(bin, lvalue);
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
            ir_id = new IRValue(IR_VALUE_VAR, id->get_lexeme());
            //ir_id = ctx->get_var(id->get_lexeme());

            if (alloca_map.count(ir_id->to_str()) > 0) {
                last_value = alloca_map[ir_id->to_str()];
            } else {
                tmp0 = ctx->new_temporary();
                ir_addr = ctx->new_unary(IR_FRAME, tmp0, ir_id);
                last_value = tmp0;
                alloca_map[ir_id->to_str()] = last_value;
            }
        }
    } else {
        if (type->is_primitive() || type->get_kind() == TYPE_POINTER) {
            ir_id = new IRValue(IR_VALUE_VAR, id->get_lexeme());

            if (alloca_map.count(ir_id->to_str()) > 0) {
                tmp0 = alloca_map[ir_id->to_str()];
                tmp1 = ctx->new_temporary();
                ir_load = ctx->new_unary(IR_LOAD, tmp1, tmp0);
                last_value = tmp1;
            } else {
                tmp0 = ctx->new_temporary();
                ir_addr = ctx->new_unary(IR_FRAME, tmp0, ir_id);
                tmp1 = ctx->new_temporary();
                ir_load = ctx->new_unary(IR_LOAD, tmp1, tmp0);
                last_value = tmp1;
            }
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
    ir = ctx->new_unary(IR_STORE, left, right);
    last_value = left;
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
        IR* ir_load = ctx->new_unary(IR_LOAD, tmp, last_value);
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

