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

void Printer::print_source(Source* source) {
    if (source->import_count() > 0) {
        for (int i = 0; i < source->import_count(); ++i) {
            print_import(source->get_import(i));
            out << '\n';
        }

        out << '\n';
    }

    for (int i = 0; i < source->function_count(); ++i) {
        print_function(source->get_function(i));
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
            
void Printer::print_function(Function* function) {
    print_indentation();

    out << "def " << function->get_name() << " : ";
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
    int kind = type->get_kind();

    switch (kind) {
    case TYPE_U8:
        out << "u8";
        break;

    case TYPE_U16:
        out << "u16";
        break;

    case TYPE_U32:
        out << "u32";
        break;

    case TYPE_U64:
        out << "u64";
        break;

    case TYPE_I8:
        out << "i8";
        break;

    case TYPE_I16:
        out << "i16";
        break;

    case TYPE_I32:
        out << "i32";
        break;

    case TYPE_I64:
        out << "i64";
        break;

    case TYPE_VOID:
        out << "void";
        break;

    case TYPE_SYMBOL:
        out << "sym";
        break;

    case TYPE_BOOL:
        out << "bool";
        break;

    case TYPE_UCHAR:
        out << "uchar";
        break;

    case TYPE_CHAR:
        out << "char";
        break;

    case TYPE_USHORT:
        out << "ushort";
        break;

    case TYPE_SHORT:
        out << "short";
        break;

    case TYPE_UINT:
        out << "uint";
        break;

    case TYPE_INT:
        out << "int";
        break;

    case TYPE_ULONG:
        out << "ulong";
        break;

    case TYPE_LONG:
        out << "long";
        break;

    case TYPE_FLOAT:
        out << "float";
        break;

    case TYPE_DOUBLE:
        out << "double";
        break;

    case TYPE_STR:
        out << "str";
        break;

    case TYPE_NAMED:
        out << "named";
        break;

    case TYPE_POINTER:
        out << "*";
        break;

    case TYPE_REFERENCE:
        out << "&";
        break;

    case TYPE_ARRAY:
        out << "[]";
        break;

    case TYPE_HASH:
        out << "{}";
        break;

    case TYPE_FUNCTION:
        out << "->";
        break;

    default:
        break;
    }
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

    case STMT_EXPRESSION:
        print_expression_statement((ExpressionStatement*) statement);
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

void Printer::print_expression(Expression* expression) {
    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;

    switch (kind) {
    case EXPR_ID:
        print_identifier((Identifier*) expression);
        break;

    case EXPR_ASSIGN:
        print_binop("=", bin);
        break;

    case EXPR_PLUS:
        print_binop("+", bin);
        break;

    case EXPR_MINUS:
        print_binop("-", bin);
        break;
    }
}

void Printer::print_binop(std::string oper, BinOp* bin) {
    print_expression(bin->get_left());
    out << " " << oper << " ";
    print_expression(bin->get_right());
}

void Printer::print_identifier(Identifier* id) {
    out << id->get_lexeme();
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
