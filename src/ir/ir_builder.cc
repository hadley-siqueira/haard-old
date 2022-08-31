#include <iostream>
#include "ir/ir_builder.h"

using namespace haard;

IRBuilder::IRBuilder() {
    logger = nullptr;
    tmp_counter = 0;
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
    tmp_counter = 0;
    build_compound_statement(function->get_statements());
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

}

void IRBuilder::build_expression_statement(ExpressionStatement* statement) {

}

void IRBuilder::build_jump_statement(JumpStatement* statement) {

}

void IRBuilder::build_while_statement(WhileStatement* statement) {

}

void IRBuilder::build_for_statement(ForStatement* statement) {

}

void IRBuilder::build_branch_statement(BranchStatement* statement) {

}

void IRBuilder::build_expression(Expression* expression) {
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

    case EXPR_ASSIGN:
        build_assignment(bin);
        break;

    case EXPR_PLUS:
        build_plus(bin);
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

void IRBuilder::build_identifier(Identifier* id) {

}

void IRBuilder::build_assignment(BinOp* bin) {

}

void IRBuilder::build_plus(BinOp* bin) {
    IR* ir;
    IRValue* left;
    IRValue* right;
    IRValue* dst;

    build_expression(bin->get_left());
    left = last_value;

    build_expression(bin->get_right());
    right = last_value;

    dst = new IRValue(IR_VALUE_TEMP, tmp_counter++);
    ir = new IRBin(IR_ADD, dst, left, right);
    last_value = dst;
}

void IRBuilder::build_literal(Literal* literal, int kind) {
    IR* ir;
    IRValue* ir_literal;
    IRValue* tmp;

    ir_literal = new IRValue(kind, literal->get_lexeme());
    tmp = new IRValue(IR_VALUE_TEMP, tmp_counter++);
    ir = new IRUnary(IR_LI, tmp, ir_literal);
    //ret = new IRConst(tmp, value);
}
