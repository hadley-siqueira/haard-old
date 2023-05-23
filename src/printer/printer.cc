#include <iostream>
#include "printer/printer.h"
#include "defs.h"

using namespace haard;

Printer::Printer() {
    indent_c = 0;
}
 
std::string Printer::to_str() {
    return out.str();
}

void Printer::print_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        print_module(modules->get_module(i));
    }
}

void Printer::print_module(Module* module) {
    if (module->import_count() > 0) {
        for (int i = 0; i < module->import_count(); ++i) {
            print_import(module->get_import(i));
            out << '\n';
        }

        out << '\n';
    }

    for (int i = 0; i < module->functions_count(); ++i) {
        print_function(module->get_function(i));
        out << '\n';
    }

    for (int i = 0;i < module->classes_count(); ++i) {
        print_class(module->get_class(i));
        out << '\n';
    }
}
            
void Printer::print_import(Import* import) {
    int i;

    out << "import ";

    if (import->path_count() > 0) {
        for (i = 0; i < import->path_count() - 1; ++i) {
            out << import->get_path(i) << '.';
        }

        out << import->get_path(i);
    }

    if (import->has_alias()) {
        out << " as " << import->get_alias();
    }
}

void Printer::print_class(Class* klass) {
    Variable* var;

    print_indentation();

    out << "class " << klass->get_name() << ":\n";
    indent();

    if (klass->variables_count() > 0) {
        for (int i = 0; i < klass->variables_count(); ++i) {
            print_indentation();
            var = klass->get_variable(i);
            out << var->get_name() << " : ";
            print_type(var->get_type());
            out << '\n';        
        }

        out << '\n';
    }

    if (klass->methods_count() > 0) {
        for (int i = 0; i < klass->methods_count(); ++i) {
            print_function(klass->get_method(i));
            out << '\n';
        }

        out << '\n';
    }

    dedent();
}

void Printer::print_function(Function* function) {
    print_indentation();

    out << "def " << function->get_name();

    if (function->get_template_header()) {
        out << function->get_template_header()->to_str();
    }

    out << " : ";
    print_type(function->get_return_type());
    out << '\n';
    indent();

    print_parameters(function);
    print_compound_statement(function->get_statements());

    dedent();
}

void Printer::print_parameters(Function* function) {
    Variable* param = nullptr;

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            param = function->get_parameter(i);

            print_indentation();
            out << "@" << param->get_name() << " : ";
            print_type(param->get_type());
            out << "\n";
        }

        out << '\n';
    }
}

void Printer::print_type(Type* type) {
    int i;
    int kind = type->get_kind();
    IndirectionType* pr = (IndirectionType*) type;
    ArrayListType* al = (ArrayListType*) type;
    HashType* hs = (HashType*) type;
    TypeList* tl = (TypeList*) type;
    NamedType* named = (NamedType*) type;

    switch (kind) {
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_VOID:
    case TYPE_SYMBOL:
    case TYPE_BOOL:
    case TYPE_UCHAR:
    case TYPE_CHAR:
    case TYPE_USHORT:
    case TYPE_SHORT:
    case TYPE_UINT:
    case TYPE_INT:
    case TYPE_ULONG:
    case TYPE_LONG:
    case TYPE_FLOAT:
    case TYPE_DOUBLE:
    case TYPE_STR:
    case TYPE_NAMED:
        out << type->to_str();
        break;

    case TYPE_ARRAY:
        print_type(al->get_subtype());

        out << "[";

        if (al->get_expression() != nullptr) {
            print_expression(al->get_expression());
        }

        out << "]";
        break;

    case TYPE_LIST:
        out << "[";
        print_type(al->get_subtype());
        out << "]";
        break;

    case TYPE_HASH:
        out << "{";
        print_type(hs->get_key_type());
        out << " : ";
        print_type(hs->get_value_type());
        out << "}";
        break;

    case TYPE_FUNCTION:
        print_type_list(" -> ", tl);
        break;

    case TYPE_TUPLE:
        print_type_list(", ", tl);
        break;

    case TYPE_POINTER:
        print_type(pr->get_subtype());
        out << "*";
        break;

    case TYPE_REFERENCE:
        print_type(pr->get_subtype());
        out << "&";
        break;

    case TYPE_PARENTHESIS:
        out << "(";
        print_type(pr->get_subtype());
        out << ")";

    default:
        break;
    }
}

void Printer::print_type_list(std::string oper, TypeList* tlist) {
    int i;

    for (i = 0; i < tlist->types_count() - 1; ++i) {
        print_type(tlist->get_type(i));
        out << oper;
    }

    print_type(tlist->get_type(i));
}

void Printer::print_statement(Statement* statement) {
    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        print_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        print_while_statement((WhileStatement*) statement);
        break;

    case STMT_FOR:
    case STMT_FOREACH:
        print_for_statement((ForStatement*) statement);
        break;

    case STMT_EXPRESSION:
        print_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
    case STMT_ELIF:
    case STMT_ELSE:
        print_branch_statement((BranchStatement*) statement);
        break;

    case STMT_RETURN:
        print_jump_statement("return", (JumpStatement*) statement);
        break;

    case STMT_GOTO:
        print_jump_statement("goto", (JumpStatement*) statement);
        break;

    case STMT_YIELD:
        print_jump_statement("yield", (JumpStatement*) statement);
        break;

    case STMT_CONTINUE:
        print_jump_statement("continue", (JumpStatement*) statement);
        break;

    case STMT_BREAK:
        print_jump_statement("break", (JumpStatement*) statement);
        break;

    case STMT_VAR_DECL:
        print_variable_declaration((VarDeclaration*) statement);
        break;
    }
}

void Printer::print_while_statement(WhileStatement* statement) {
    print_indentation();
    out << "while ";
    print_expression(statement->get_condition());
    out << ":\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    out << '\n';
}

void Printer::print_for_statement(ForStatement* statement) {
    print_indentation();
    out << "for ";

    if (statement->get_kind() == STMT_FOR) {
        print_expression(statement->get_initialization());
        out << "; ";
        print_expression(statement->get_condition());
        out << "; ";
        print_expression(statement->get_increment());
    } else {
        print_expression(statement->get_condition());
    }

    out << ":\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    out << '\n';
}

void Printer::print_branch_statement(BranchStatement* statement) {
    int kind;

    kind = statement->get_kind();
    print_indentation();

    switch (kind) {
    case STMT_IF:
        out << "if ";
        print_expression(statement->get_condition());
        out << ":\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();

        if (statement->get_false_statements() != nullptr) {
            print_statement(statement->get_false_statements());
        }

        break;

    case STMT_ELIF:
        out << "elif ";
        print_expression(statement->get_condition());
        out << ":\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();

        if (statement->get_false_statements() != nullptr) {
            print_statement(statement->get_false_statements());
        }

        break;

    case STMT_ELSE:
        out << "else:\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();

        break;

    }
}

void Printer::print_jump_statement(std::string op, JumpStatement* statement) {
    print_indentation();
    out << op;

    if (statement->get_expression() != nullptr) {
        out << ' ';
        print_expression(statement->get_expression());
    }

    out << '\n';
}

void Printer::print_compound_statement(CompoundStatement* statement) {
    for (int i = 0; i < statement->statements_count(); ++i) {
        print_statement(statement->get_statement(i));
    }
}

void Printer::print_expression_statement(ExpressionStatement* statement) {
    print_indentation();
    print_expression(statement->get_expression());
    out << "\n";
}

void Printer::print_variable_declaration(VarDeclaration* decl) {
    Variable* var;

    var = decl->get_variable();
    print_indentation();
    out << "var ";
    out << var->get_name();

    if (var->get_type() != nullptr) {
        out << " : ";
        print_type(var->get_type());
    }

    if (decl->get_expression() != nullptr) {
        out << " = ";
        print_expression(decl->get_expression());
    }

    out << '\n';
}

void Printer::print_expression(Expression* expression) {
    if (expression == nullptr) return;

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        print_identifier((Identifier*) expression);
        break;

    case EXPR_ASSIGN:
        print_binop("=", bin);
        break;

    case EXPR_SPECIAL_ASSIGNMENT:
        print_binop(":=", bin);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT:
        print_binop(">>>=", bin);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT:
        print_binop(">>=", bin);
        break;

    case EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT:
        print_binop("<<=", bin);
        break;

    case EXPR_TIMES_ASSIGNMENT:
        print_binop("*=", bin);
        break;

    case EXPR_PLUS_ASSIGNMENT:
        print_binop("+=", bin);
        break;

    case EXPR_MODULO_ASSIGNMENT:
        print_binop("%=", bin);
        break;

    case EXPR_MINUS_ASSIGNMENT:
        print_binop("-=", bin);
        break;

    case EXPR_INTEGER_DIVISION_ASSIGNMENT:
        print_binop("//=", bin);
        break;

    case EXPR_DIVISION_ASSIGNMENT:
        print_binop("/=", bin);
        break;

    case EXPR_BITWISE_NOT_ASSIGNMENT:
        print_binop("~=", bin);
        break;

    case EXPR_BITWISE_OR_ASSIGNMENT:
        print_binop("|=", bin);
        break;

    case EXPR_BITWISE_XOR_ASSIGNMENT:
        print_binop("^=", bin);
        break;

    case EXPR_BITWISE_AND_ASSIGNMENT:
        print_binop("&=", bin);
        break;

    case EXPR_PLUS:
        print_binop("+", bin);
        break;

    case EXPR_MINUS:
        print_binop("-", bin);
        break;

    case EXPR_TIMES:
        print_binop("*", bin);
        break;

    case EXPR_DIVISION:
        print_binop("/", bin);
        break;

    case EXPR_INTEGER_DIVISION:
        print_binop("//", bin);
        break;

    case EXPR_MODULO:
        print_binop("%", bin);
        break;

    case EXPR_POWER:
        print_binop("**", bin);
        break;

    case EXPR_BITWISE_OR:
        print_binop("|", bin);
        break;

    case EXPR_BITWISE_XOR:
        print_binop("^", bin);
        break;

    case EXPR_BITWISE_AND:
        print_binop("&", bin);
        break;

    case EXPR_SHIFT_LEFT_LOGICAL:
        print_binop("<<", bin);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL:
        print_binop(">>>", bin);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC:
        print_binop(">>", bin);
        break;

    case EXPR_INCLUSIVE_RANGE:
        print_binop("..", bin);
        break;

    case EXPR_EXCLUSIVE_RANGE:
        print_binop("...", bin);
        break;

    case EXPR_LOGICAL_OR:
        print_binop("or", bin);
        break;

    case EXPR_LOGICAL_OR_OPER:
        print_binop("||", bin);
        break;

    case EXPR_LOGICAL_AND:
        print_binop("and", bin);
        break;

    case EXPR_LOGICAL_AND_OPER:
        print_binop("&&", bin);
        break;

    case EXPR_EQUAL:
        print_binop("==", bin);
        break;

    case EXPR_NOT_EQUAL:
        print_binop("!=", bin);
        break;

    case EXPR_LESS_THAN:
        print_binop("<", bin);
        break;

    case EXPR_GREATER_THAN:
        print_binop(">", bin);
        break;

    case EXPR_LESS_THAN_OR_EQUAL:
        print_binop("<=", bin);
        break;

    case EXPR_GREATER_THAN_OR_EQUAL:
        print_binop(">=", bin);
        break;

    case EXPR_IN:
        print_binop("in", bin);
        break;

    case EXPR_NOT_IN:
        print_binop("not in", bin);
        break;

    case EXPR_LOGICAL_NOT_OPER:
        print_unop("!", un);
        break;

    case EXPR_LOGICAL_NOT:
        print_unop("not ", un);
        break;

    case EXPR_ADDRESS_OF:
        print_unop("&", un);
        break;

    case EXPR_DEREFERENCE:
        print_unop("*", un);
        break;

    case EXPR_BITWISE_NOT:
        print_unop("~", un);
        break;

    case EXPR_UNARY_MINUS:
        print_unop("-", un);
        break;

    case EXPR_UNARY_PLUS:
        print_unop("+", un);
        break;

    case EXPR_PRE_INC:
        print_unop("++", un);
        break;

    case EXPR_PRE_DEC:
        print_unop("--", un);
        break;

    case EXPR_POS_INC:
        print_unop("++", un, false);
        break;

    case EXPR_POS_DEC:
        print_unop("--", un, false);
        break;

    case EXPR_SIZEOF:
        print_unop("!", un);
        break;

    case EXPR_PARENTHESIS:
        out << "(";
        print_expression(un->get_expression());
        out << ")";
        break;

    case EXPR_CALL:
        print_expression(bin->get_left());
        print_expression(bin->get_right());
        break;

    case EXPR_INDEX:
        print_expression(bin->get_left());
        out << "[";
        print_expression(bin->get_right());
        out << "]";
        break;

    case EXPR_ARROW:
        print_expression(bin->get_left());
        out << "->";
        print_expression(bin->get_right());
        break;

    case EXPR_DOT:
        print_expression(bin->get_left());
        out << ".";
        print_expression(bin->get_right());
        break;

    case EXPR_LITERAL_BOOL:
    case EXPR_LITERAL_INTEGER:
    case EXPR_LITERAL_FLOAT:
    case EXPR_LITERAL_DOUBLE:
    case EXPR_LITERAL_CHAR:
    case EXPR_LITERAL_STRING:
    case EXPR_LITERAL_SYMBOL:
        print_literal(literal);
        break;

    case EXPR_LITERAL_NULL:
        out << "null";
        break;

    case EXPR_TUPLE:
        print_expression_list("(", ")", exprlist);
        break;

    case EXPR_LIST:
        print_expression_list("[", "]", exprlist);
        break;

    case EXPR_ARRAY:
        print_expression_list("{", "}", exprlist);
        break;

    case EXPR_ARGS:
        print_expression_list("(", ")", exprlist);
        break;

    case EXPR_FOR_INIT:
    case EXPR_FOR_INC:
        print_expression_list("", "", exprlist);
        break;

    case EXPR_HASH:
    case EXPR_HASH_RAW:
        print_hash(exprlist);
        break;

    case EXPR_FUNCTION:
        print_function_expression((FunctionExpression*) expression);
        break;

    case EXPR_NEW:
        print_new_expression((NewExpression*) expression);
        break;
    }
}

void Printer::print_binop(std::string oper, BinOp* bin) {
    print_expression(bin->get_left());
    out << " " << oper << " ";
    print_expression(bin->get_right());
}

void Printer::print_unop(std::string oper, UnOp* un, bool before) {
    if (before) {
        out << oper;
    }

    print_expression(un->get_expression());

    if (!before) {
        out << oper;
    }
}

void Printer::print_identifier(Identifier* id) {
    if (id->has_scope()) {
        out << id->get_alias();

        if (id->get_alias() != "::") {
            out << "::";
        }
    }

    out << id->get_name();
}

void Printer::print_literal(Literal* literal) {
    out << literal->get_lexeme();
}

void Printer::print_expression_list(std::string begin, std::string end, ExpressionList* tuple) {
    int i;
    out << begin;

    for (i = 0; i < tuple->expressions_count() - 1; ++i) {
        print_expression(tuple->get_expression(i));
        out << ", ";
    }
        
    print_expression(tuple->get_expression(i));
    out << end;
}

void Printer::print_hash(ExpressionList* hash) {
    BinOp* pair;
    int i;

    if (hash->get_kind() == EXPR_HASH) {
        out << "{";
    }

    for (i = 0; i < hash->expressions_count() - 1; ++i) {
        pair = (BinOp*) hash->get_expression(i);
        print_expression(pair->get_left());
        out << ": ";
        print_expression(pair->get_right());
        out << ", ";
    }

    pair = (BinOp*) hash->get_expression(i);
    print_expression(pair->get_left());
    out << ": ";
    print_expression(pair->get_right());

    if (hash->get_kind() == EXPR_HASH) {
        out << "}";
    }
}

void Printer::print_function_expression(FunctionExpression* function) {
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
    print_compound_statement(f->get_statements());
    dedent();

    print_indentation();
    out << "}";
}

void Printer::print_new_expression(NewExpression* expr) {
    out << "new ";
    print_type(expr->get_new_type());

    if (expr->has_arguments()) {
        print_expression_list("(", ")", expr->get_arguments());
    }
}

void Printer::indent() {
    indent_c++;
}

void Printer::dedent() {
    indent_c--;
}

void Printer::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        out << "    ";
    }
}
