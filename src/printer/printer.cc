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

void Printer::print_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        print_source(sources->get_source(i));
    }
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

    for (int i = 0;i < source->classes_count(); ++i) {
        print_class(source->get_class(i));
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
    int i;
    int kind = type->get_kind();
    IndirectionType* pr = (IndirectionType*) type;
    ArrayListType* al = (ArrayListType*) type;
    HashType* hs = (HashType*) type;
    TypeList* tl = (TypeList*) type;

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

    case EXPR_SCOPE:
        print_expression(bin->get_left());
        out << "::";
        print_expression(bin->get_right());
        break;

    case EXPR_GLOBAL_SCOPE:
        out << "::";
        print_expression(un->get_expression());
        break;

    case EXPR_ASSIGN:
        print_binop("=", bin);
        break;

    case EXPR_SPECIAL_ASSIGN:
        print_binop(":=", bin);
        break;

    case EXPR_SRL_ASSIGN:
        print_binop(">>>=", bin);
        break;

    case EXPR_SRA_ASSIGN:
        print_binop(">>=", bin);
        break;

    case EXPR_SLL_ASSIGN:
        print_binop("<<=", bin);
        break;

    case EXPR_TIMES_ASSIGN:
        print_binop("*=", bin);
        break;

    case EXPR_PLUS_ASSIGN:
        print_binop("+=", bin);
        break;

    case EXPR_MODULO_ASSIGN:
        print_binop("%=", bin);
        break;

    case EXPR_MINUS_ASSIGN:
        print_binop("-=", bin);
        break;

    case EXPR_INTEGER_DIVISIO_ASSIGN:
        print_binop("//=", bin);
        break;

    case EXPR_DIVISION_ASSIGN:
        print_binop("/=", bin);
        break;

    case EXPR_BITWISE_NOT_ASSIGN:
        print_binop("~=", bin);
        break;

    case EXPR_BITWISE_OR_ASSIGN:
        print_binop("|=", bin);
        break;

    case EXPR_BITWISE_XOR_ASSIGN:
        print_binop("^=", bin);
        break;

    case EXPR_BITWISE_AND_ASSIGN:
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

    case EXPR_SLL:
        print_binop("<<", bin);
        break;

    case EXPR_SRL:
        print_binop(">>>", bin);
        break;

    case EXPR_SRA:
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

    case EXPR_EQ:
        print_binop("==", bin);
        break;

    case EXPR_NE:
        print_binop("!=", bin);
        break;

    case EXPR_LT:
        print_binop("<", bin);
        break;

    case EXPR_GT:
        print_binop(">", bin);
        break;

    case EXPR_LE:
        print_binop("<=", bin);
        break;

    case EXPR_GE:
        print_binop(">=", bin);
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

    case EXPR_HASH:
    case EXPR_HASH_RAW:
        print_hash(exprlist);
        break;

    case EXPR_FUNCTION:
        print_function_expression((FunctionExpression*) expression);
        break;
    }
}

void Printer::print_binop(std::string oper, BinOp* bin) {
    print_expression(bin->get_left());
    out << " " << oper << " ";
    print_expression(bin->get_right());
}

void Printer::print_unop(std::string oper, UnOp* un) {
    out << oper;
    print_expression(un->get_expression());
}

void Printer::print_identifier(Identifier* id) {
    out << id->get_lexeme();
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
